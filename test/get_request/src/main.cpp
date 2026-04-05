#include <betterSocket/socket.h>
#include <string>

int main()
{
    int socketID = bs::AddSocket();

    std::string hostname = bs::Connect(socketID, "http://www.google.com/", "80", true);

    bs::RequestBlock block;
    block.host = hostname;
    block.rtype = bs::RequestType::GET;

    bs::Request(socketID, bs::GenerateRequest(block));
    bs::CloseSocket(socketID);
}