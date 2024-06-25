/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:09:11 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/25 14:42:57 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void Server::processPrivmsg(int fd, std::string string)
{
	std::string msg;
	if (getClient(fd)->isLogged() == false)
	{
		msg = ERR_NOTREGISTERED(getClient(fd)->getNickname(), "PRIVMSG").c_str();
		send(fd, msg.c_str(), msg.length(), 0);
		return ;
	}
	std::vector<std::string> args = split_args(string, " ");
	if (args.size() < 3)
	{
		msg = ERR_NEEDMOREPARAMS(getClient(fd)->getNickname(), "PRIVMSG").c_str();
		send(fd, msg.c_str(), msg.length(), 0);
		std::cout << "Client " << fd << " needs to specify the client name and the message" << std::endl;
		return ;
	}
	std::string user;
	std::string messageToSend;
	user += USER_ID(getClient(fd)->getNickname(), getClient(fd)->getUsername());
	user += " PRIVMSG ";
	if (args[2][0] == ':')
		messageToSend += args[2].erase(0, 1);
	else
		messageToSend += args[2];
	for (size_t i = 3; i < args.size(); i++)
		messageToSend += " " + args[i];
	std::vector<std::string> wheretoSend = split_args(args[1], ",");
	for (std::vector<std::string>::iterator it = wheretoSend.begin(); it != wheretoSend.end(); it++)
	{
		if (it->empty())
			continue ;
		if (it->at(0) != '#' && it->at(0) != '&')
			sendToClient(fd, *it, user + it->c_str() + " :" + messageToSend);
		else
			sendToChannel(fd, *it, user + it->c_str() + " :" + messageToSend);
	}
}

void Server::sendToChannel(int fd, std::string channelName, std::string message)
{
	Channel &channel = getChannelbyName(channelName, getClient(fd)->getNickname());
	channel.sendMessage(message);
	std::cout << "Client " << fd << " has sent " << message << " to channel " << channelName << std::endl;
}

void Server::sendToClient(int fd, std::string clientName, std::string message)
{
	Client *client = getClientbyNickname(clientName);
	if (client == NULL)
	{
		std::string msg = ERR_NOSUCHNICK(getClient(fd)->getNickname(), clientName).c_str();
		send(fd, msg.c_str(), msg.length(), 0);
		return ;
	}
	send(client->getFd(), message.c_str(), message.length(), 0);
		std::cout << "Client " << fd << " has sent " << message << " to " << clientName << std::endl;
}
