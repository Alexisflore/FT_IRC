/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:09:03 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/14 17:00:33 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void Server::processNick(int fd, std::vector <std::string> string)
{
	if (string.size() != 2)
	{
		std::cout << "Usage : NICK <nickname>" << std::endl;
		return ;
	}
	std::string nickname = string[1];
	if (nickname.size() > 9)
	{
		std::cout << "Nickname is too long" << std::endl;
		return ;
	}
	if (isNicknameUsed(nickname))
	{
		std::cout << "Nickname is already used" << std::endl;
		return ;
	}
	getClient(fd)->setNickname(nickname);
	std::cout << "Client " << fd << " changed nickname to " << nickname << std::endl;
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