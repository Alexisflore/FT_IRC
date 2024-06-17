/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:09:11 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/17 18:14:47 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void Server::processPrivmsg(int fd, std::string string)
{
	std::vector<std::string> args = getArgs(string);

	if (args.size() < 3)
	{
		std::cout << "Usage : PRIVMSG <client> <message>" << std::endl;
		return ;
	}
	std::string clientName = args[1];
	std::string	msg = args[2];
	for (size_t i = 3; i < args.size(); i++)
	{
		msg += " " + args[i];
	}

	int clientFd = this->findFdByName(clientName);
	if (clientFd == 0)
	{
		std::cout << "Client " << clientName << " not found." << std::endl;
		return ;
	}
	std::stringstream ss;
	ss << fd;
	std::string fdStr = ss.str();
	msg = "Private message from fd " + fdStr + " : " + msg;
	send(clientFd, msg.c_str(), msg.size(), 0);
	std::cout << "private message sent to " << clientName << std::endl;
}
