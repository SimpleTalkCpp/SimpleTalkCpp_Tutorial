#include "MyServer.h"
#include "MyFileStream.h"

void MyClient::onConnected()
{
	printf("> client %p: connected\n", this);
}

void MyClient::onRecv()
{
	{
		size_t n = sock.availableBytesToRead();
		if (!n) {
			close(); // <------------ closed from the other side
			return;
		}

		// append into recvBuf
		auto oldSize = recvBuf.size();
		auto newSize = oldSize + n;

		// !!! limit the recv buf size, otherwise may attack by sending data without header end !!!
		if (newSize > kHeaderSizeLimit) {
			throw MyError("Excess header size limit");
		}

		recvBuf.resize(newSize);
		auto* p = &*(recvBuf.begin() + oldSize);
		sock.recv(p, n);
	}

	switch (action) {
		case Action::WaitRequest: onRecv_WaitRequest();	break;
		case Action::SendContent: return;
		default:
			throw MyError("Unhandled Action");
	}
}

void MyClient::onRecv_WaitRequest() {
	// find header
	auto* header = &(*recvBuf.begin());
	auto* headerEnd = strstr(header, "\r\n\r\n");
	if (!headerEnd)
		return; // need more header

	*headerEnd = 0;
	headerEnd += 4; // 4 bytes \r\n\r\n

	request.reset();
	response.reset();

	request.header = header;

	auto headerSize = headerEnd - header;
	auto remainSize = recvBuf.size() - headerSize;

	memmove(header, headerEnd, remainSize);
	recvBuf.resize(remainSize);

	//-------
	onRecv_ProcessHeader();
}

void MyClient::onRecv_ProcessHeader() {
	printf("> client %p: recv header\n%s\n\n", this, request.header.c_str());

	auto* nextLine = request.header.c_str();
	nextLine = MyUtil::getLine(lineBuf, nextLine);

	{ // Request line
		auto* input = lineBuf.c_str();
		input = MyUtil::getUpperStringToken(token, input, ' ');

		if (token == "GET") {
			request.method = HttpMethod::Get;
		} else if (token == "HEAD") {
			request.method = HttpMethod::Head;
		} else if (token == "POST") {
			request.method = HttpMethod::Post;
		} else {
			response.begin(405, "Method Not Allowed");
			response.addHeaderField("Allow", "GET, HEAD, POST");
			response.setContent("405 - Method Not Allowed");
			response.send(this);
			return;
		}

		//-- get url
		input = MyUtil::getStringToken(token, input, ' ');
		request.setUrl(token.c_str());

		// get local absolute file path
		std::string tmp = server->documentRoot();
		tmp.append(request.urlPath);
		MyUtil::getAbsPath(request.localPath, tmp.c_str());

		// !!! avoid access file out of document root !!!
		if (!MyUtil::stringStartsWith(request.localPath.c_str(), server->documentRoot().c_str())) {
			response.begin(401, "Unauthorized");
			response.setContent("401 - Unauthorized");
			response.send(this);
		}
	}

	// other header field
	for(;;) {
		nextLine = MyUtil::getLine(lineBuf, nextLine);
		if (!lineBuf.size())
			break;

		auto* p = MyUtil::getUpperStringToken(token, lineBuf.c_str(), ':');
		if (!p)
			continue;

		if (*p == ':')
			p++;

		// trim space and tab
		for (;*p; p++) {
			if (*p != ' ' && *p != '\t')
				break;
		}

		if (!token.size())
			return;

//		printf(">> [%s] = [%s]\n", token.c_str(), p);
		if (token == "HOST") {
			request.host = p;
		}
	}

	if (request.method == HttpMethod::Get || request.method == HttpMethod::Head) {
		try {
			action = Action::SendContent;

			response.begin(200, "OK");

			if (request.method == HttpMethod::Get)
				response.fileContent.openRead(request.localPath.c_str());

			response.send(this);

		} catch(...) {
			response.begin(404, "File Not Found");
			response.setContent("404 - File Not Found");
			response.send(this);
		}
		return;
	}

	{
		// error
		response.begin(500, "Internal Server Error");
		response.setContent("500 - Internal Server Error");
		response.send(this);
	}
}

void MyClient::onSend() {
	if (sendBufOffset < sendBuf.size()) {
		size_t n = sendBuf.size() - sendBufOffset;
		size_t ret = sock.send(sendBuf.data() + sendBufOffset, n);

		printf("> client %p: send to socket %u\n", this, (uint16_t)ret);
		sendBufOffset += ret;
	}

	if (sendBufOffset < sendBuf.size())
		return;

	sendBuf.clear();
	sendBufOffset = 0;

	if (action == Action::SendContent) {
		response.sendContent(this);
	}
}

void MyClient::close() {
	printf("> client %p: close\n", this);
	sock.close();
}

void MyClient::send(const char* s, size_t n) {
	printf("> client %p: send\n", this);
	fwrite(s, n, 1, stdout);
	printf("\n\n");

	sendBuf.append(s, n);
}

void MyClient::Response::reset() {
	status = 0;
	header.clear();

	content.clear();
	contentSent = 0;

	fileContent.close();
	fileContentSent = 0;
}

void MyClient::Response::begin(int status, const char* reason) {
	char tmp[256 + 1];
	snprintf(tmp, 256, "HTTP/1.1 %d ", status);
	tmp[256] = 0;

	header = tmp;
	if (reason)
		header.append(reason);
	header.append("\r\n");
}

void MyClient::Response::addHeaderField(const char* name, const char* value) {
	header.append(name);
	header.append(": ");
	header.append(value);
	header.append("\r\n");
}

void MyClient::Response::send(MyClient* client) {
	uint64_t contentLength = fileContent.isOpened() ? fileContent.fileSize() : content.size();

	char tmp[256 + 1];
	snprintf(tmp, 256, "Content-Length: %llu\r\n\r\n", (unsigned long long)contentLength);
	tmp[256] = 0;

	header.append(tmp);

	client->send(header.c_str(), header.size());
	sendContent(client);
}

void MyClient::Response::sendContent(MyClient* client) {
	if (fileContent.isOpened()) {
		// fill sendBuf from File

		auto fileSize = fileContent.fileSize();
		if (fileContentSent >= fileSize) {
			fileContent.close();
			client->action = Action::WaitRequest;
			return;
		}

		if (client->sendBuf.size())
			return;

		auto n = fileSize - fileContentSent;
		if (n > MyClient::kFileBufSize)
			n = MyClient::kFileBufSize;

		client->sendBuf.resize(static_cast<size_t>(n));
		fileContent.read(&*client->sendBuf.begin(), (size_t)n);
		fileContentSent += n;
		
	} else {
		client->sendBuf.append(content.data(), content.size());
	}
}

void MyClient::Request::reset() {
	method = HttpMethod::Unknown;
	host.clear();

	url.clear();
	urlPath.clear();
	urlQuery.clear();

	localPath.clear();
}

void MyClient::Request::setUrl(const char* sz) {
	MyUtil::decodeURI(url, sz); // !!! decode URI 

	auto* relativePath = url.c_str();
	auto* scheme = strstr(url.c_str(), "://");
	if (scheme) {
		// absolution path
		relativePath = strchr(scheme + 3, '/');
	}

	auto* query = strchr(relativePath, '?');
	if (query) {
		urlPath.assign(relativePath, query);
		urlQuery = query + 1;
	}else{
		urlPath = relativePath;
	}
}
