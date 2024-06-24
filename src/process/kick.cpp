/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:08:54 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/24 15:04:50 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void Server::processKick(int fd, std::string arg)
{
	std::vector<std::string> args = split_args(arg, " ");
	if (args.size() < 3)
	{
		std::string msg = ERR_NEEDMOREPARAMS(getClient(fd)->getNickname(), "KICK").c_str();
		send(fd, msg.c_str(), strlen(msg.c_str()), 0);
		std::cout << "Client " << fd << " needs to specify the channel name and the client name" << std::endl;
		return ;
	}
	std::string channelName = args[1];
	std::string clientName = args[2];
	std::string comment;
	if (args.size() > 3)
	{
		std::string comment = args[3];
	}
	Channel &channel = getChannelbyName(channelName, getClient(fd)->getNickname());
	if (!channel.isClientInChannel(fd))
	{
		std::string msg = ERR_NOTONCHANNEL(getClient(fd)->getNickname(), channel.getName()).c_str();
		std::cout << "Client " << fd << " isn t in the channel " << channel.getName() << std::endl;
		send(fd, msg.c_str(), strlen(msg.c_str()), 0);
		return ;
	}
	Client client = channel.getClientByNick(clientName);
	if (client.getFd() == -1)
	{
		std::string msg = ERR_NOSUCHNICK(getClient(fd)->getNickname(), clientName).c_str();
		std::cout << "Client " << fd << " isn t in the channel " << channel.getName() << std::endl;
		send(fd, msg.c_str(), strlen(msg.c_str()), 0);
		return ;
	}
	if (!channel.isClientOperator(fd))
	{
		std::string msg = ERR_CHANOPRIVSNEEDED(getClient(fd)->getNickname(), channel.getName()).c_str();
		std::cout << "Client " << fd << " isn t operator in the channel " << channel.getName() << std::endl;
		send(fd, msg.c_str(), strlen(msg.c_str()), 0);
		return ;
	}
	if (args.size() > 4)
	{
		for (std::vector<std::string>::iterator it = args.begin() + 4; it != args.end(); it++)
			comment += " " + *it;
	}
	channel.removeClient(client);
	std::string msg = RPL_KICK(getClient(fd)->getNickname(), channel.getName(), client.getNickname(), comment).c_str();
	std::cout << "Client " << fd << " has kicked " << client.getNickname() << " from the channel " << channel.getName() << std::endl;
}
