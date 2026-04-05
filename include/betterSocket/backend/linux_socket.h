#ifndef LINUX_BACKEND_SOCKET
#define LINUX_BACKEND_SOCKET
#undef linux

#pragma once
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <vector>
#include <string>
#include <utility>

#include <iostream>

namespace bs::linux
{

    class linuxSocketService
    {
    public:

    	int AddSocket();

    public:

        std::string Connect(int socketID, const char* domain, const char* port, bool ipv4);
        void Request(int socketID, std::string request);
        void CloseSocket(int socketID);

    public:
        static linuxSocketService& GetInstance()
        {
            static linuxSocketService instance;
            return instance;
        }

        linuxSocketService(const linuxSocketService&) = delete;
        linuxSocketService& operator=(const linuxSocketService&) = delete;
        linuxSocketService(linuxSocketService&&) = delete;
        linuxSocketService& operator=(linuxSocketService&&) = delete;

    private:
        linuxSocketService() = default;
        ~linuxSocketService() = default;
    private:
    	std::vector<int> m_sockets;
    };
}

#endif