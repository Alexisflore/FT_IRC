/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:09:06 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/25 11:42:35 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void Server::processPart(int fd, std::string arg)
{
	if (getClient(fd)->isLogged() == false)
	{
		std::string msg = ERR_NOTREGISTERED(getClient(fd)->getNickname(), "PASS").c_str();
		send(fd, msg.c_str(), msg.length(), 0);
		return ;
	}
	std::vector<std::string> args = split_args(arg, " ");
	std::string channelIn;
	if (args.size() < 2)
	{
		std::string msg = ERR_NEEDMOREPARAMS(getClient(fd)->getNickname(), "PART").c_str();
		send(fd, msg.c_str(), strlen(msg.c_str()), 0);
		std::cout << "Client " << fd << " needs to specify the channel name" << std::endl;
		return ;
	}

	std::vector<std::string> channelNames = split_args(args[1], ",");
	for (std::vector<std::string>::iterator it = channelNames.begin(); it != channelNames.end(); it++)
	{
		Channel& channel = getChannelbyName(*it, getClient(fd)->getNickname());
		if (!channel.isClientInChannel(fd))
		{
			std::string msg = ERR_NOTONCHANNEL(getClient(fd)->getNickname(), channel.getName()).c_str();
			std::cout << "Client " << fd << " isn t in the channel " << channel.getName() << std::endl;
			send(fd, msg.c_str(), strlen(msg.c_str()), 0);
			return ;
		}
		channel.removeClient(*getClient(fd));
		std::cout << "Client " << fd << " has left the channel " << channel.getName() << std::endl;
		if (channel.getClient(fd).getFd() == -1)
		{
			std::cout << "you are still in the channel" << std::endl;
		}
	}
	if (args[1][0] == '#' || args[1][0] == '&')
	{
		if (args[2][0] == ':')
			channelIn = args[2];
		else
			channelIn = args[1];
	}
}

// void Server::removeClientFromChannel(int fd, Channel *channel)
// {
// 	channel->removeClient(fd);
// }
