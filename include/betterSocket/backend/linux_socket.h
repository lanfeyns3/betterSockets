#ifndef LINUX_BACKEND_SOCKET
#define LINUX_BACKEND_SOCKET
#undef linux

#pragma once
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <unordered_map>
#include <functional>
#include <vector>
#include <string>
#include <utility>
#include <sstream>

#include <iostream>

#include "../share.h"

namespace bs::linux
{
    struct linuxListener
    {
        linuxListener(std::string Path, std::function<void(ListenBlock)> Func)
            : path(Path), func(Func)
        {

        }
        std::string path;
        std::function<void(ListenBlock)> func;
    };

    class linuxSocketService
    {
    public:

    	int AddSocket();

    public:

        void Bind(int socketID, int port);
        std::string Connect(int socketID, const char* domain, const char* port, bool ipv4);
        void Listen(int socketID);

        void test(int socketID);

        void AddListener(int socketID,std::string path ,int method,std::function<void(ListenBlock)> func);

        void Send(int clientSocket,std::string msg);

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
        std::unordered_map<int, std::vector<linuxListener>> m_socketFunctions;
    };
}

#endif