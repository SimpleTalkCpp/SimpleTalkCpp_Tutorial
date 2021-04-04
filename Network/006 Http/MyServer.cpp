#include "MyServer.h"

bool MyServer::s_quit = false;

MyServer::MyServer()
{
	_clients.reserve(kClientCountMax);
}

void MyServer::run()
{
	MyUtil::getCurrentDir(_documentRoot);
	_documentRoot.append("/www");

	_listenSock.createTCP();

	MySocketAddr addr;
	addr.setIPv4(0, 0, 0, 0);
	addr.setPort(8011);

	_listenSock.bind(addr);
	_listenSock.listen();

	while (!s_quit) {
		fd_set readfds;
		fd_set writefds;
		FD_ZERO(&readfds);
		FD_ZERO(&writefds);
		FD_SET(_listenSock.sock(), &readfds);
		int n = 1;
		auto max_fd = _listenSock.sock();

		removeClosedClients();

		for (auto& client : _clients) {
			if (!client || !client->sock.isValid())
				continue;

			if (n >= FD_SETSIZE) {
				throw MyError("n >= FD_SETSIZE");
				break;
			}

			auto cs = client->sock.sock();

			if (cs > max_fd)
				max_fd = cs;

			if (client->needRecv())
				FD_SET(cs, &readfds);

			if (client->needSend())
				FD_SET(cs, &writefds);
			n++;
		}

		struct timeval tv;
		tv.tv_sec = 1;
		tv.tv_usec = 0;

		// select( nfds <--
		//            Linux - nfds should be set to the highest-numbered file descriptor in any of the three sets, plus 1
		//            Windows - Ignored. The nfds parameter is included only for compatibility with Berkeley sockets
		int ret = ::select(max_fd + 1, &readfds, &writefds, nullptr, &tv);
		if (ret < 0) {
			throw MyError("select");
		}
		if (ret == 0) {
			continue;
		}

		if (FD_ISSET(_listenSock.sock(), &readfds)) {
			if (_clients.size() < kClientCountMax) {
				_clients.emplace_back(new MyClient());
				auto& client = _clients.back();
				client->server = this;

				try {
					_listenSock.accept(client->sock);
					client->sock.setNonBlocking(true); // !!! Set Non-Blocking Mode
					client->onConnected();
				}
				catch (...) {
					client->close();
				}
			}
		}

		for (auto& client : _clients) {
			if (!client || !client->sock.isValid())
				continue;

			auto cs = client->sock.sock();
			try {
				if (FD_ISSET(cs, &readfds))
					client->onRecv();

				if (FD_ISSET(cs, &writefds))
					client->onSend();

			} catch (...) {
				client->close();
			}
		}
	}
}

void MyServer::removeClosedClients()
{

	for (size_t i = 0; i < _clients.size();) {
		auto & c = _clients[i];
		if (c && c->sock.isValid()) {
			i++;
			continue;
		}

		try {
			if (_clients.size() > 1)
				std::swap(c, _clients.back());
			_clients.resize(_clients.size() - 1);
		}
		catch (...) {
			printf("exception in removeClosedClients");
		}
	}
}

