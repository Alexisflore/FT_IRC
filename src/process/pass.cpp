/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:09:11 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/18 18:04:24 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void Server::processPass(int fd, std::string string)
{
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
	else if (strcmp(strings[1].c_str(), this->_pass.c_str()) != 0)
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
