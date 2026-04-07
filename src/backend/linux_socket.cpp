#include "betterSocket/backend/linux_socket.h"

namespace bs::linux
{
	int linuxSocketService::AddSocket()
	{
		int new_socket = socket(AF_INET,SOCK_STREAM,0);

		int opt = 1;
    	setsockopt(new_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

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

	std::unordered_map<std::string, std::string> httpParser(std::string msg) {
	    std::unordered_map<std::string, std::string> parsed;
	    std::stringstream stream(msg);
	    std::string line;
	
	    while (std::getline(stream, line)) {

	        if (!line.empty() && line.back() == '\r') {
	            line.pop_back();
	        }
	
	        size_t pos = line.find("GET ");
	        if (pos != std::string::npos) {
	            std::string method, path;
	            std::stringstream firstLine(line);
	            firstLine >> method >> path;
	            
	            parsed["path"] = path;
	            parsed["method"] = method;
	            
	            std::cout << path << "\n";
	        }
	        
	        size_t delim = line.find(": ");
	        if (delim != std::string::npos) {
	            std::string key = line.substr(0, delim);
	            std::string value = line.substr(delim + 2);
	            parsed[key] = value;
	        }
	    }
	
	    return parsed;
	}
	void linuxSocketService::StopSocket(int socketID)
	{
		m_socketThreads[socketID] = false;
	}

	void linuxSocketService::RunSocket(int socketID)
	{
		m_socketThreads[socketID] = true;
		while (m_socketThreads[socketID])
		{
			int client_socket = accept(m_sockets.at(socketID),nullptr,nullptr);

			char buffer[4096];

			recv(client_socket,buffer,sizeof(buffer),0);

			auto parsed = httpParser(std::string(buffer));

			for (auto listener : m_socketFunctions[socketID])
			{
				if (parsed["path"] == listener.path)
				{
					ListenBlock block;
					block.clientSocket = client_socket;
					block.parsed = parsed;
					listener.func(block);
				}
			}
			
		}
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

	void linuxSocketService::Bind(int socketID, int port)
	{
		sockaddr_in address;
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = INADDR_ANY;
		address.sin_port = htons(port);

		bind(m_sockets.at(socketID), (sockaddr*)&address, sizeof(address));
	}

	void linuxSocketService::Listen(int socketID)
	{
		listen(m_sockets.at(socketID),3);
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

	void linuxSocketService::AddListener(int socketID,std::string path ,int method,std::function<void(ListenBlock)> func)
	{
		m_socketFunctions[socketID].emplace_back(path,func);
	}

	void linuxSocketService::Send(int clientSocket,std::string msg)
	{
		send(clientSocket,msg.c_str(),msg.length() * sizeof(char),0);
	}
}