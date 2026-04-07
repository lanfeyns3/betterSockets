#include "betterSocket/socket.h"

namespace bs
{
	int AddSocket()
	{
		#ifdef __linux__
		int socketID = linux::linuxSocketService::GetInstance().AddSocket();
		return socketID;
		#endif
	}

	std::string Connect(int socketID, std::string domain, std::string port, bool ipv4)
	{
		#ifdef __linux__
		std::string hostName = linux::linuxSocketService::GetInstance().Connect(socketID,domain.c_str(),port.c_str(),ipv4);
		return hostName;
		#endif
	}

	std::string GenerateRequest(RequestBlock requestBlock)
	{
		std::string request = "";
		if (requestBlock.isHttps == false)
		{
			std::string requestType;
			switch (requestBlock.rtype) {
			case RequestType::GET:
				requestType = "GET";
				break;
			default:
				break;
			}

			request += requestType + " / HTTP/1.1\r\n";
			request += "Host: " + requestBlock.host + "\r\n";
			request += "User-Agent: betterSocket\r\n";
			request += "Connection: close\r\n";
			request += "\r\n";
		}

		return request;
	}

	void CloseSocket(int socketID)
	{
		#ifdef __linux__
		linux::linuxSocketService::GetInstance().CloseSocket(socketID);
		#endif
	}

	void Request(int socketID, std::string request)
	{
		#ifdef __linux__
		linux::linuxSocketService::GetInstance().Request(socketID,request);
		#endif
	}

	void Bind(int socketID)
	{
		#ifdef __linux__
		linux::linuxSocketService::GetInstance().Bind(socketID,8080);
		#endif
	}

	void Listen(int socketID)
	{
		#ifdef __linux__
		linux::linuxSocketService::GetInstance().Listen(socketID);
		#endif
	}

	std::thread RunSocket(int socketID)
	{
		#ifdef __linux__
		std::thread t(&linux::linuxSocketService::RunSocket, &linux::linuxSocketService::GetInstance(), socketID);
		return t;
		#endif
	}

	void StopSocket(int socketID)
	{
		#ifdef __linux__
		linux::linuxSocketService::GetInstance().StopSocket(socketID);
		#endif
	}

	void AddListener(int socketID,std::string path ,int method,std::function<void(ListenBlock)> func)
	{
		#ifdef __linux__
		linux::linuxSocketService::GetInstance().AddListener(socketID,path,method,func);
		#endif
	}

	void Send(int clientSocket, std::string response,std::string contentType,int statusCode,std::string statusMsg)
	{
		std::string msg = "HTTP/1.1 " + std::to_string(statusCode) + " " + statusMsg + "\r\n";
		
		msg += "Content-Type: " + contentType + "\r\n";
		msg += "Content-Length: " + std::to_string(response.length()) + "\r\n";
		msg += "Connection: close\r\n";
		msg += "\r\n";
		
		msg += response;
		
		#ifdef __linux__
		linux::linuxSocketService::GetInstance().Send(clientSocket, msg);
		#endif
	}
}