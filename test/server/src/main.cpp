#include <betterSocket/socket.h>
#include <fstream>
#include <vector>

int main()
{
    int socketID = bs::AddSocket();

    bs::Bind(socketID);
    bs::Listen(socketID);

    bs::AddListener(socketID,"/",bs::RequestType::GET,[](bs::ListenBlock block) {
        bs::Send(block.clientSocket,"Hello, World!","text/text",200,"OK");
    });

    std::vector<std::thread> threads;

    threads.emplace_back(bs::RunSocket(socketID));

    for (auto& thread : threads)
    {
        thread.join();
    }
    
    bs::CloseSocket(socketID);
}
