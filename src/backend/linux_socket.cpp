#include "betterSocket/backend/linux_socket.h"

namespace bs::linux
{
	int linuxSocketService::AddSocket()
	{
		int new_socket = socket(AF_INET,SOCK_STREAM,0);
		m_sockets.emplace_back(new_socket);

		return m_sockets.size() - 1;
	}

	void* getSinAddr(struct addrinfo *addr) {
	    if (addr->ai_family == AF_INET) {
	        return &(((struct sockaddr_in*)addr->ai_addr)->sin_addr);
	    }
	    return &(((struct sockaddr_in6*)addr->ai_addr)->sin6_addr);
	}

	std::string getHostname(std::string url) {
    	std::string host = url;
    	size_t pos;

    	if ((pos = host.find("://")) != std::string::npos) {
    	    host.erase(0, pos + 3);
    	}

    	if ((pos = host.find("/")) != std::string::npos) {
    	    host.erase(pos);
    	}
	
    	return host;
	}



	std::pair<addrinfo*, std::string> compileAddress(const char* url, const char* port, bool ipv4)
	{
	    addrinfo hints = {}; // Renamed to 'hints' (standard convention)
	    hints.ai_flags = AI_CANONNAME;
	    hints.ai_family = (ipv4) ? AF_INET : AF_INET6;
	    hints.ai_socktype = SOCK_STREAM;
	    hints.ai_protocol = IPPROTO_TCP;
	
	    addrinfo *res = nullptr;
	    std::string stripped = getHostname(url);
	
	    int ret = getaddrinfo(stripped.c_str(), port, &hints, &res);
	    
	    if (ret != 0) {
	        std::cerr << "Get address failed: " << gai_strerror(ret) << std::endl;
	    }
	    
	    return {res, stripped};
	}

	std::string linuxSocketService::Connect(int socketID, const char* domain, const char* port, bool ipv4)
	{
		auto p = compileAddress(domain,port,ipv4);
		connect(m_sockets.at(socketID),std::get<0>(p)->ai_addr,std::get<0>(p)->ai_addrlen);
		freeaddrinfo(std::get<0>(p));
		return std::get<1>(p);
	}

	void linuxSocketService::Request(int socketID, std::string request)
	{
		char buffer[4096];
		std::string response;
		int bytes;
		send(m_sockets.at(socketID),request.c_str(),request.length(),0);

		while((bytes = recv(m_sockets.at(socketID),buffer,sizeof(buffer) - 1, 0)) > 0)
		{
			buffer[bytes] = '\0';
			response += buffer;
		}

		std::cout << response << "\n";
	}

	void linuxSocketService::CloseSocket(int socketID)
	{
		shutdown(m_sockets.at(socketID), SHUT_RDWR);
		close(m_sockets.at(socketID));
	}
}