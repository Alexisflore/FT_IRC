/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:09:06 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/26 20:05:12 by alfloren         ###   ########.fr       */
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
	std::string userid = USER_ID(getClient(fd)->getNickname(), getClient(fd)->getUsername());
	std::vector<std::string> args = split_args(arg, " ");
	std::string channelIn;
	if (args.size() < 2 || args.size() > 3)
	{
		std::string msg = ERR_NEEDMOREPARAMS(getClient(fd)->getNickname(), "PART").c_str();
		send(fd, msg.c_str(), strlen(msg.c_str()), 0);
		std::cout << "Client " << fd << " needs to specify the channel name" << std::endl;
		return ;
	}
	std::vector<std::string> channelNames = split_args(args[1], ",");
	std::string reason;
	if (args.size() == 3)
		reason = args[2] + "\n";
	for (std::vector<std::string>::iterator it = channelNames.begin(); it != channelNames.end(); it++)
	{
		Channel& channel = getChannelbyName(*it, fd);
		if (!channel.isClientInChannel(fd))
		{
			std::string msg = ERR_NOTONCHANNEL(getClient(fd)->getNickname(), channel.getName()).c_str();
			std::cout << "Client " << fd << " isn t in the channel " << channel.getName() << std::endl;
			send(fd, msg.c_str(), strlen(msg.c_str()), 0);
			return ;
		}
		std::cout << "Client " << fd << " has left the channel " << channel.getName() << std::endl;
		channel.removeClient(*getClient(fd));
		if (channel.getClient(fd).getFd() == -1)
		{
			std::cout << "you are not in the channel" << std::endl;
		}
		std::string msg = RPL_PART(userid, channel.getName(), reason).c_str();
		channel.sendMessage(msg);
		send(fd, msg.c_str(), strlen(msg.c_str()), 0);
	}
}

// void Server::removeClientFromChannel(int fd, Channel *channel)
// {
// 	channel->removeClient(fd);
// }
