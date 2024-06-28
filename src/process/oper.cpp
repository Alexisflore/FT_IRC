#include "../../includes/Server.hpp"

void Server::processOper(int fd, std::string string)
{	
	std::string OPER_LOG = "admin";
	std::string OPER_PASS = "42irc";
    Client *client = getClient(fd);
	if (!client)
	{
		std::cout << "Unauthorized command (not yet registered)\n" << std::endl;
		return ;
	}
	if (client->getMode().getModeValue('o'))
	{
		std::cout << "Permission denied you're already an operator\n" << std::endl;
		return;
	}
	if (string.size() < 2)
	{
		std::cout << "OPER :Not enough parameters\n" << std::endl;
		return;
	}
	std::vector<std::string> args = split_args(string, " ");
	std::vector<std::string> username = split_args(args[1], ",");
	std::vector<std::string> password = split_args(args[2], ",");
	if (username[0] == OPER_LOG && password[0] == OPER_PASS)
	{
		client->getMode().setModeByType('o', '+');
		std::cout << "You are now an IRC operator\n" << std::endl;
	}
	else
	{
		std::cout << "Invalid username or password\n" << std::endl;
	}
}