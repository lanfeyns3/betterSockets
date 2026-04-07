#ifdef __linux__
#include "backend/linux_socket.h"
#endif

#pragma once
#include <vector>
#include <string>
#include <utility>
#include <unordered_map>

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

	void Send(int clientSocket, std::string response,std::string contentType,int statusCode,std::string statusMsg);
	void Bind(int socketID);
	void Listen(int socketID);

	std::thread RunSocket(int socketID);
	void StopSocket(int socketID);

	int AddSocket();
	void AddListener(int socketID,std::string path ,int method,std::function<void(ListenBlock)> func);

	std::string GenerateRequest(RequestBlock requestBlock);

	std::string Connect(int socketID,std::string domain, std::string port, bool ipv4);
	void CloseSocket(int socketID);
	void Request(int socketID, std::string request);
}