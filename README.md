# Better Sockets
A (soon to be)cross platform sockets library intending to make C++ talk to the world.



# Supported OS'
![Static Badge](https://img.shields.io/badge/linux-supported-green) ![Static Badge](https://img.shields.io/badge/windows-not_supported-red) ![Static Badge](https://img.shields.io/badge/mac_os-not_support-red)

# Requirements
|CMake| C++ |
|--|--|
| CMake 3.10(minimum) | C++17 |

# Installation
**Linux:**

	git clone https://github.com/lanfeyns3/betterSockets.git
	cd betterSockets
	mkdir build
	cd build
	cmake ..
	make
# Examples

### Visual Get Request
```mermaid
stateDiagram-v2
    [*] --> main
    main --> AddSocket()
    AddSocket() --> Connect(socketID,domain,port,ipv4)
    Connect(socketID,domain,port,ipv4) --> RequestBlock
    RequestBlock --> Request(socketID,GenerateRequest(requestBlock))
    Request(socketID,GenerateRequest(requestBlock)) --> CloseSocket(socketID)
 ``` 
### Code Get Request

```cpp
#include <betterSocket/socket.h>

int main()
{
    int socketID = bs::AddSocket();

    std::string hostname = bs::Connect(socketID, "http://www.example.com/", "80", true);

    bs::RequestBlock block;
    block.host = hostname;
    block.rtype = bs::RequestType::GET;

    bs::Request(socketID, bs::GenerateRequest(block));
    bs::CloseSocket(socketID);

    return 0;
}
```

# FAQ
## Will BetterSockets support Mac OS in the future?

No.  No and there is no plans on adding Mac OS support for BetterSockets.

## Does BetterSockets support HTTPS?

Currently no.  In the future HTTPS support will be added but currently it is not supported.
