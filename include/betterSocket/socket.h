#ifdef __linux__
#include "backend/linux_socket.h"
#endif

#pragma once
#include <vector>
#include <string>
#include <utility>

namespace bs
{
	enum RequestType
	{
		GET,
		POST
	};
	struct RequestBlock
	{
		std::string host;
		RequestType rtype;
		bool isHttps = false;
	};
	int AddSocket();
	std::string GenerateRequest(RequestBlock requestBlock);

	std::string Connect(int socketID,std::string domain, std::string port, bool ipv4);
	void CloseSocket(int socketID);
	void Request(int socketID, std::string request);
}