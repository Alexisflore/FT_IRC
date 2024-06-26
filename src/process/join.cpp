/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 14:42:52 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/26 11:06:18 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include <string>

void	Server::processJoin(int fd, std::string arg)
{
	if (getClient(fd)->isLogged() == false)
	{
		std::string msg = ERR_NOTREGISTERED(getClient(fd)->getNickname(), "JOIN").c_str();
		send(fd, msg.c_str(), msg.length(), 0);
		return ;
	}
	arg.erase(0, 5);
	std::string ChannelName;
	std::string channelusers;
	std::vector<std::string> args = split_args(arg, " ");
	if (args.size() == 0 || args[0].empty() || args.size() > 2)
	{
		std::string msg = ERR_NEEDMOREPARAMS(getClient(fd)->getNickname(), "JOIN").c_str();
		send(fd, msg.c_str(), strlen(msg.c_str()), 0);
		return ;
	}
	std::vector<std::string> channelNames;
	std::vector<std::string> passwords;
	std::string topic;
	if (args.size() == 2)
		passwords = split_args(args[1], ",");
	Channel newChannel;
	channelNames = split_args(args[0], ",");
	std::string pass;
	std::vector<std::pair<std::string, std::string > > channels;
	for (unsigned int i = 0; i < channelNames.size(); i++)
	{
		pass.clear();
		if (i < passwords.size())
			pass = passwords[i];
		channels.push_back(std::make_pair(channelNames[i], pass));
	}
	// selectChannels(channelNames, &password, fd);
	if (channelNames.size() == 0)
		return ;
	for (std::vector< std::pair<std::string, std::string > >::iterator it = channels.begin(); it != channels.end(); it++)
	{
		// joinChannel(findChannel(*it), &password, fd);
		std::vector<Channel>::iterator it2 = std::find_if(
		this->_channels.begin(),
		this->_channels.end(),
		ChannelNameComparator((*it).first));
		if (it2 != this->_channels.end())
		{
			Channel& channel = *it2;
			std::cout << "channe is invite only: " << channel.isInviteOnly() << std::endl;
			if (channel.isClientInChannel(fd) && channel.isClientInvited(fd) == false)
				return ;
			if ((channel.isPasswordProtected() && (*it).second.size() == 0) || (channel.isPasswordProtected() && (*it).second.size() > 0 && channel.getPassword() != (*it).second))
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
				if (channel.isClientInChannel(fd) == true)
					channel.setClientasNormal(fd);
				else
					channel.joinChannel(*getClient(fd));
				std::cout << "Client " << channel.getClient(fd).getNickname() << " joined the channel " << channel.getName() << std::endl;
				topic = channel.getTopic();
				ChannelName = channel.getName();
				channelusers = channel.getUsers();
			}
		}
		else
		{
			std::cout << "Channel " << (*it).first << " does not exist" << std::endl;
			Channel newChannel((*it).first);
			ChannelName = newChannel.getName();
			this->_channels.push_back(newChannel);
			for (std::vector<Channel>::iterator it2 = this->_channels.begin(); it2 != this->_channels.end(); it2++)
			{
				if (it2->getName() == (*it).first)
				{
					Channel& channel = *it2;
					channel.joinChannel(*getClient(fd));
					channel.setClientasOperator(fd);
					break;
				}
			}
			topic = newChannel.getTopic();
			channelusers = newChannel.getUsers();
		}
		std::string userid = USER_ID(getClient(fd)->getNickname(), getClient(fd)->getUsername());
		std::string msg = "JOIN " + newChannel.getName() + "\n";
		newChannel.sendMessage(userid + " " + msg);
		std::cout << "Client " << fd << " has joined the channel " << (*it).first << std::endl;
		if (topic.empty() == false)
			msg = RPL_TOPIC(getClient(fd)->getNickname(), ChannelName, topic).c_str();
		else
			msg = RPL_NOTOPIC(getClient(fd)->getNickname(), ChannelName).c_str();
		send(fd, msg.c_str(), strlen(msg.c_str()), 0);
		std::cout << "joining with this message :" << msg << std::endl;
		msg = RPL_NAMREPLY(getClient(fd)->getNickname(), "=", ChannelName, channelusers).c_str();
		send(fd, msg.c_str(), strlen(msg.c_str()), 0);
		msg = RPL_ENDOFNAMES(getClient(fd)->getNickname(), ChannelName).c_str();
		send(fd, msg.c_str(), strlen(msg.c_str()), 0);
	}
}

// void Server::addClientToChannel(int fd, Channel *channel)
// {
// 	channel->joinChannel(getClient(fd));
// }

