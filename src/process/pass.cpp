/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:09:11 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/19 10:39:24 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

static bool passcmp(std::string pass, std::string string)
{
	if (pass.size() != string.size())
	{
		std::cout << "pass.size(): " << pass.size() << " string.size(): " << string.size() << std::endl;
		return false;
	}
	for (size_t i = 0; i < pass.size() - 1; i++)
	{
		if (pass[i] != string[i])
		{
			std::cout << "pass[i]: " << static_cast<int>(pass[i]) << " string[i]: " << static_cast<int>(string[i]) << std::endl;
			return false;
		}
	}
	return true;
}

void Server::processPass(int fd, std::string string)
{
	std::cout << string << std::endl;
	Client *client = getClient(fd);
	std::vector<std::string> strings = split_args(string, " ");
	std::string msg;
	if (strings.size() != 2)
	{
		msg = ERR_NEEDMOREPARAMS(getClient(fd)->getNickname(), "PASS").c_str();
		client->setLogged(false);
	}
	else if (client->isLogged())
	{
		msg = ERR_ALREADYREGISTERED(getClient(fd)->getNickname()).c_str();
	}
	else if (passcmp(_pass, strings[1]) == false)
	{
		msg = ERR_PASSWDMISMATCH(getClient(fd)->getNickname()).c_str();
		client->setLogged(false);
	}
	else
	{
		client->setLogged(true);
		msg = ":localhost 001 " + client->getNickname() + " :Welcome to the Internet Relay Network " + client->getNickname() + "!" + client->getUsername() + "@localhost\n";
	}
	if (!msg.empty())
		send(fd, msg.c_str(), strlen(msg.c_str()), 0);
}
