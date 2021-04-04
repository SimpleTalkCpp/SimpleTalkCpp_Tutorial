#pragma once

#include "MyClient.h"

class MyServer : public MyNonCopyable {
public:
	const size_t kClientCountMax = 16;

	MyServer();

	void run();
	const std::string& documentRoot() const { return _documentRoot; }

	static bool s_quit;

private:
	void removeClosedClients();

	MySocket _listenSock;
	std::vector< std::unique_ptr<MyClient> > _clients;
	std::string _documentRoot;
};