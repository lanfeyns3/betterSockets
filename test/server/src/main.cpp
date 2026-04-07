#include <betterSocket/socket.h>
#include <fstream>

int main()
{
    int socketID = bs::AddSocket();

    bs::Bind(socketID);
    bs::Listen(socketID);


    std::string html;
    std::fstream file("./index.html");

    std::string line;

    while(std::getline(file,line))
    {
        html += line;
    }

    bs::AddListener(socketID,"/banking",bs::RequestType::GET,[&](bs::ListenBlock block) {
        bs::Send(block.clientSocket,html,"text/html",200,"OK");
    });

    bs::AddListener(socketID,"/",bs::RequestType::GET,[](bs::ListenBlock block) {
        bs::Send(block.clientSocket,"Hello, World!","text/text",200,"OK");
    });

    bs::testRun(socketID);
    
    bs::CloseSocket(socketID);
}
