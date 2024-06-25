#include "../../includes/Server.hpp"

void Server::processPing(int fd, std::string string)
{
	if (getClient(fd)->isLogged() == false)
	{
		std::string msg = ERR_NOTREGISTERED(getClient(fd)->getNickname(), "PING").c_str();
		send(fd, msg.c_str(), msg.length(), 0);
		return ;
	}
	string.replace(0, 4, "PONG");
	send(fd, string.c_str(), string.length(), 0);
	std::cout << string << " sent to " << fd << std::endl;
}
