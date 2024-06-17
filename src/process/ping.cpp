#include "../../includes/Server.hpp"

void Server::processPing(int fd, std::vector <std::string> string)
{
	std::string		msg = "PONG ";
	for (size_t i = 1; i < string.size(); i++)
	{
		msg += string[i];
		if (i + 1 < string.size())
			msg += " ";
	}
	msg += "\n";
	send(fd, msg.c_str(), msg.length(), 0);
}
