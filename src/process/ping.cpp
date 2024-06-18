#include "../../includes/Server.hpp"

void Server::processPing(int fd, std::string string)
{
	std::vector<std::string> strings = split_args(string, " ");
	std::string		msg = "PONG ";
	for (size_t i = 1; i < strings.size(); i++)
	{
		msg += strings[i];
		if (i + 1 < strings.size())
			msg += " ";
	}
	msg += "\n";
	send(fd, msg.c_str(), msg.length(), 0);
}
