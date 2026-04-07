#ifndef BETTERSOCKERS_SHARE
#define BETTERSOCKERS_SHARE

#pragma once
#include <string>
#include <unordered_map>

namespace bs
{
	struct ListenBlock
	{
		int clientSocket;
		std::unordered_map<std::string,std::string> parsed;
	};
}
#endif