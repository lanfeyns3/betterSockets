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
}