/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:09:03 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/26 17:20:37 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void Server::processNick(int fd, std::string string)
{
	std::string msg;
	if (getClient(fd)->isRegistered() == false)
	{
		std::string msg = ERR_NOTREGISTERED(getClient(fd)->getNickname(), "NICK").c_str();
		send(fd, msg.c_str(), msg.length(), 0);
		return ;
	}
	std::vector<std::string> strings = split_args(string, " ");
	if (strings.size() != 2)
	{
		std::cout << "Usage : NICK <nickname>" << std::endl;
		msg = ERR_NEEDMOREPARAMS(getClient(fd)->getNickname(), "NICK").c_str();
		send(fd, msg.c_str(), msg.length(), 0);
		return ;
	}
	std::string nickname = strings[1];
	if (nickname.size() > 9)
	{
		std::cout << "Nickname is too long" << std::endl;
		msg = ERR_ERRONEUSNICKNAME(getClient(fd)->getNickname(), nickname).c_str();
		send(fd, msg.c_str(), msg.length(), 0);
		return ;
	}
	if (isNicknameUsed(nickname))
	{
		std::cout << "Nickname is already used" << std::endl;
		msg = ERR_NICKNAMEINUSE(getClient(fd)->getNickname(), nickname).c_str();
		send(fd, msg.c_str(), msg.length(), 0);
		return ;
	}
	getClientbyRef(fd).setNickname(nickname);
	msg = RPL_NICK(getClient(fd)->getNickname(), nickname).c_str();
	send(fd, msg.c_str(), msg.length(), 0);
	std::cout << "Client " << fd << " changed nickname to " << nickname << std::endl;
	getClient(fd)->welcomeMessage(_creationTime);
}

bool Server::isNicknameUsed(std::string nickname)
{
	std::vector<Client>::iterator it = std::find_if(
		this->_clients.begin(),
		this->_clients.end(),
		NicknameComparator(nickname));
	if (it != this->_clients.end())
		return true;
	return false;
}
