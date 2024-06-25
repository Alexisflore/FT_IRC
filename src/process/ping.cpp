#include "../../includes/Server.hpp"

void Server::processPing(int fd, std::string string)
{
	std::vector<std::string> strings = split_args(string, " ");
	if (getClient(fd)->isLogged() == false)
	{
		std::string msg = ERR_NOTREGISTERED(getClient(fd)->getNickname(), "PING").c_str();
		send(fd, msg.c_str(), msg.length(), 0);
		return ;
	}
	std::string msg;
	msg = "PONG " + strings[1] + "\n";
	send(fd, msg.c_str(), msg.length(), 0);
	std::cout << msg << "sent to" << fd << std::endl;
}
