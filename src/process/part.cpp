/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:09:06 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/21 16:21:01 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void Server::processPart(int fd, std::string arg)
{
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
		Channel channel = getChannelbyName(*it, getClient(fd)->getNickname());
		if (!channel.isClientInChannel(fd))
		{
			std::string msg = ERR_NOTONCHANNEL(getClient(fd)->getNickname(), channel.getName()).c_str();
			std::cout << "Client " << fd << " isn t in the channel " << channel.getName() << std::endl;
			send(fd, msg.c_str(), strlen(msg.c_str()), 0);
			return ;
		}
		removeClientFromChannel(fd, &channel);
		std::cout << "Client " << fd << " has left the channel " << channel.getName() << std::endl;
		if (channel.getClient(fd) != NULL)
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

void Server::removeClientFromChannel(int fd, Channel *channel)
{
	channel->removeClient(getClient(fd));
}
