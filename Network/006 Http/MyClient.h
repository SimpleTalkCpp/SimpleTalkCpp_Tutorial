#pragma once

#include "MySocket.h"
#include "MyUtil.h"
#include "MyFileStream.h"

class MyServer;
class MyClient : public MyNonCopyable {
public:
	static const size_t kHeaderSizeLimit = 16 * 1024;
	static const size_t kFileBufSize = 4 * 1024;

	void onConnected();

	void onRecv();
	void onRecv_WaitRequest();
	void onRecv_ProcessHeader();

	bool needSend() const { return sendBufOffset < sendBuf.size(); }
	bool needRecv() const { return !needSend(); }

	void onSend();

	void close();

	void send(const char* s, size_t n);

	template<size_t N>
	void send(const char (&sz)[N]) { if (N) send(sz, N-1); }
	void send(const std::string& s) { send(s.c_str(), s.size()); }

	enum class Action {
		WaitRequest,
		SendContent,
	};

	enum class HttpMethod {
		Unknown,
		Get,
		Head,
		Post,
	};

	class Request {
	public:
		Request() { reset(); }

		std::string header;

		HttpMethod  method;
		std::string	host;

		std::string	url;
		std::string urlPath;
		std::string urlQuery;

		std::string localPath;

		void reset();
		void setUrl(const char* sz);
	};

	class Response {
	public:
		Response() { reset(); }

		int status;
		std::string	header;

		std::vector<char> content;
		size_t            contentSent;

		MyFileStream		fileContent;
		uint64_t			fileContentSent;

		void reset();

		void begin(int status, const char* reason = nullptr);
		void addHeaderField(const char* name, const char* value);

		template<size_t N>
		void setContent(const char (&sz)[N]) { if (N) content.assign(sz, sz+N-1); }
		void send(MyClient* client);

		void sendContent(MyClient* client);
	};

	MySocket sock;
	MyServer* server = nullptr;
	Action action = Action::WaitRequest;

	std::string recvBuf;
	std::string lineBuf;
	std::string token;

	std::string sendBuf;
	size_t sendBufOffset = 0;

	Request		request;
	Response	response;
};
