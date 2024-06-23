/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 14:42:52 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/23 01:47:38 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include <string>

void	Server::processJoin(int fd, std::string arg)
{
	arg.erase(0, 5);
	std::vector<std::string> args = split_args(arg, " ");
	if (args.size() == 0 || args[0].empty() || args.size() > 2)
	{
		std::string msg = ERR_NEEDMOREPARAMS(getClient(fd)->getNickname(), "JOIN").c_str();
		send(fd, msg.c_str(), strlen(msg.c_str()), 0);
		return ;
	}
	std::vector<std::string> channelNames;
	std::vector<std::string> password;
	std::string topic;
	if (args.size() == 2)
		password = split_args(args[1], " ");
	Channel newChannel;
	channelNames = split_args(args[0], ",");
	// selectChannels(channelNames, &password, fd);
	if (channelNames.size() == 0)
		return ;
	std::vector< Channel> channels;
	for (std::vector< std::string >::iterator it = channelNames.begin(); it != channelNames.end(); it++)
	{
		// joinChannel(findChannel(*it), &password, fd);
		std::vector<Channel>::iterator it2 = std::find_if(
		this->_channels.begin(),
		this->_channels.end(),
		ChannelNameComparator(*it));
		if (it2 != this->_channels.end())
		{
			Channel& channel = *it2;
			if (channel.isClientInChannel(fd))
				return ;
			if ((channel.isPasswordProtected() && password.size() == 0) || (channel.isPasswordProtected() && password.size() > 0 && channel.getPassword() != password[0]))
			{
				std::string msg = ERR_BADCHANNELKEY(getClient(fd)->getNickname(), channel.getName()).c_str();
				send(fd, msg.c_str(), strlen(msg.c_str()), 0);
				std::cout << "Client " << fd << " needs to provide a password to join the channel " << channel.getName() << std::endl;
				return ;
			}
			else if (channel.isInviteOnly() && !channel.isClientInvited(fd))
			{
				std::string msg = ERR_INVITEONLYCHAN(getClient(fd)->getNickname(), channel.getName()).c_str();
				send(fd, msg.c_str(), strlen(msg.c_str()), 0);
				std::cout << "Client " << fd << " needs to be invchanneled to join the channel " << channel.getName() << std::endl;
				return ;
			}
			else if (channel.isClientBanned(fd))
			{
				std::string msg = ERR_BANNEDFROMCHAN(getClient(fd)->getNickname(), channel.getName()).c_str();
				send(fd, msg.c_str(), strlen(msg.c_str()), 0);
				std::cout << "Client " << fd << " is banned from the channel " << channel.getName() << std::endl;
				return ;
			}
			else if (channel.isChannelFull())
			{
				std::string msg = ERR_CHANNELISFULL(getClient(fd)->getNickname(), channel.getName()).c_str();
				send(fd, msg.c_str(), strlen(msg.c_str()), 0);
				std::cout << "Channel " << channel.getName() << " is full" << std::endl;
				return ;
			}
			else
			{
				std::cout << "Client " << fd << " joined the channel " << channel.getName() << std::endl;
				channel.joinChannel(*getClient(fd));
				topic = channel.getTopic();
			}
		}
		else 
		{
			Channel newChannel(*it);

			this->_channels.push_back(newChannel);
			for (std::vector<Channel>::iterator it2 = this->_channels.begin(); it2 != this->_channels.end(); it2++)
			{
				if (it2->getName() == *it)
				{
					Channel& channel = *it2;
					channel.joinChannel(*getClient(fd));
					break;
				}
			}
			topic = newChannel.getTopic();
		}
		std::string msg = RPL_JOIN(getClient(fd)->getNickname(), (*it)).c_str();
		send(fd, msg.c_str(), strlen(msg.c_str()), 0);
		std::cout << "Client " << fd << " has joined the channel " << *it << std::endl;
		if (topic.empty() == false)
		{
			std::string msg = RPL_TOPIC(getClient(fd)->getNickname(), (*it), topic).c_str();
			send(fd, msg.c_str(), strlen(msg.c_str()), 0);
		}
		else
		{
			std::string msg = RPL_NOTOPIC(getClient(fd)->getNickname(), (*it)).c_str();
			send(fd, msg.c_str(), strlen(msg.c_str()), 0);
		}
	}
}

// void Server::addClientToChannel(int fd, Channel *channel)
// {
// 	channel->joinChannel(getClient(fd));
// }

