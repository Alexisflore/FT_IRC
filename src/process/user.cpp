/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:09:22 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/22 13:39:37 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void Server::processUser(int fd, std::string string)
{
	if (split_args(string, " ").size() < 5)
	{
		std::string msg = ERR_NEEDMOREPARAMS(getClient(fd)->getNickname(), "USER").c_str();
		send(fd, msg.c_str(), msg.length(), 0);
		return ;
	}
	Client *client = getClient(fd);
	string.erase(0, 5);
	std::vector<std::string> strings = split_args(string, " ");
	client->setUsername(strings[0]);
	std::string realName;
	realName += strings[3].erase(0, 1) + " " + strings[4];
	// std::cout << "Realname: " << realName << std::endl;
	client->setRealName(realName);
	client->welcomeMessage();
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
