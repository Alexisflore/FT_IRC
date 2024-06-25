/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:09:22 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/25 12:51:09 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void Server::processUser(int fd, std::string string)
{
	std::vector<std::string> strings = split_args(string, " ");
	if (getClient(fd)->isRegistered() == false)
	{
		std::string msg = ERR_NOTREGISTERED(getClient(fd)->getNickname(), "USER").c_str();
		send(fd, msg.c_str(), msg.length(), 0);
		return ;
	}
	if (strings.size() < 5 || strings.size() > 6)
	{
		std::string msg = ERR_NEEDMOREPARAMS(getClient(fd)->getNickname(), "USER").c_str();
		send(fd, msg.c_str(), msg.length(), 0);
		return ;
	}
	Client *client = getClient(fd);
	if (!client->isRegistered())
	{
		std::string cmd = "USER";
		std::string msg = ERR_NOTREGISTERED(getClient(fd)->getNickname(), cmd).c_str();
		send(fd, msg.c_str(), msg.length(), 0);
		return ;
	}
	client->setUsername(strings[1]);
	std::string realName;
	if (strings[4][0] == ':')
		realName += strings[4].erase(0, 1);
	else
		realName += strings[4];
	if (strings.size() > 5)
		realName += " " + strings[5];
	// std::cout << "Realname: " << realName << std::endl;
	client->setRealName(realName);
	client->welcomeMessage(_creationTime);
	std::cout << "Client " << fd << " has set his username to " << client->getUsername() << " and his realname to " << client->getRealName() << std::endl;
}

Client* Server::getClientbyNickname(std::string nickname)
{
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->getNickname() == nickname)
			return &(*it);
	}
	return NULL;
}

void	Client::setRealName(std::string realname)
{
	this->_realname = realname;
}

std::string	Client::getRealName()
{
	return this->_realname;
}
