/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:09:11 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/17 13:51:41 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void Server::processPass(int fd, std::vector <std::string> string)
{
	send(fd, "001 PASS :Welcome to the Internet Relay Network\n", 48, 0);
	if (string.size() != 2)
	{
		send(fd, "461 PASS :Not enough parameters\n", 31, 0);
		return ;
	}
	if (getClient(fd)->getNickname() != "")
	{
		send(fd, "462 PASS :You may not reregister\n", 32, 0);
		return ;
	}
	if (string[1] != this->_pass)
	{
		send(fd, "464 PASS :Password incorrect\n", 29, 0);
		return ;
	}
	if (string[1] == this->_pass)
	{
		send(fd, "001 PASS :Welcome to the Internet Relay Network\n", 48, 0);
		getClient(fd)->setNickname("Guest");
		getClient(fd)->setPassword(_pass);
	}
}
