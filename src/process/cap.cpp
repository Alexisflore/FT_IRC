#include "../../includes/Server.hpp"

void Server::processCap(int fd, std::vector <std::string> string)
{
	if (string[1] == "LS")
	{
		std::string capabilities = "";

		std::string response = "CAP * LS :" + capabilities + "\r\n";
		send(fd, response.c_str(), response.size(), 0);
	}
}
