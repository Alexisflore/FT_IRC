#include "../../includes/Server.hpp"

void Server::processPing(int fd, std::string string)
{
	string.replace(0, 4, "PONG ");
	send(fd, string.c_str(), string.length(), 0);
}
