#include "../../includes/Server.hpp"

void Server::processCap(int fd, std::string string)
{
	std::vector<std::string> strings = split_args(string);
	if (strings[1] == "LS")
	{
		std::string capabilities = "";

		std::string response = "CAP * LS :" + capabilities + "\r\n";
		send(fd, response.c_str(), response.size(), 0);
	}
}
