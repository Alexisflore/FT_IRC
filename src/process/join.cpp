/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 14:42:52 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/21 14:15:44 by alfloren         ###   ########.fr       */
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
			}
			else if (channel.isInviteOnly() && !channel.isClientInvited(fd))
			{
				std::string msg = ERR_INVITEONLYCHAN(getClient(fd)->getNickname(), channel.getName()).c_str();
				send(fd, msg.c_str(), strlen(msg.c_str()), 0);
				std::cout << "Client " << fd << " needs to be invchanneled to join the channel " << channel.getName() << std::endl;
			}
			else if (channel.isClientBanned(fd))
			{
				std::string msg = ERR_BANNEDFROMCHAN(getClient(fd)->getNickname(), channel.getName()).c_str();
				send(fd, msg.c_str(), strlen(msg.c_str()), 0);
				std::cout << "Client " << fd << " is banned from the channel " << channel.getName() << std::endl;
			}
			else if (channel.isChannelFull())
			{
				std::string msg = ERR_CHANNELISFULL(getClient(fd)->getNickname(), channel.getName()).c_str();
				send(fd, msg.c_str(), strlen(msg.c_str()), 0);
				std::cout << "Channel " << channel.getName() << " is full" << std::endl;
			}
			else
			{
				std::cout << "Client " << fd << " joined the channel " << channel.getName() << std::endl;
				addClientToChannel(fd, &channel);
				std::cout << "Client " << channel.getClient(fd)->getNickname() << " joined the channel " << channel.getName() << std::endl;
				if (channel.getTopic().empty())
					send(fd, RPL_NOTOPIC(getClient(fd)->getNickname(), channel.getName()).c_str(), strlen(RPL_NOTOPIC(getClient(fd)->getNickname(), channel.getName()).c_str()), 0);
			}
		}
		else
		{
			Channel newChannel(*it);
			addClientToChannel(fd, &newChannel);
			this->_channels.push_back(newChannel);
		}
	}
}

// // void Server::selectChannels(std::vector< std::string > channelNames, std::vector<std::string> *password, int fd)
// {
// 	if (channelNames.size() == 0)
// 		return ;
// 	std::vector< Channel> channels;
// 	for (std::vector< std::string >::iterator it = channelNames.begin(); it != channelNames.end(); it++)
// 	{
// 		joinChannel(findChannel(*it), &(*password), fd);
// 	}
// }

// void	Server::joinChannel(Channel& channel, std::vector<std::string> *password, int fd)
// {
// 	if (channel.isClientInChannel(fd))
// 		return ;
// 	if ((channel.isPasswordProtected() && (*password).size() == 0) || (channel.isPasswordProtected() && (*password).size() > 0 && channel.getPassword() != (*password)[0]))
// 	{
// 		std::string msg = ERR_BADCHANNELKEY(getClient(fd)->getNickname(), channel.getName()).c_str();
// 		send(fd, msg.c_str(), strlen(msg.c_str()), 0);
// 		std::cout << "Client " << fd << " needs to provide a password to join the channel " << channel.getName() << std::endl;
// 	}
// 	else if (channel.isInviteOnly() && !channel.isClientInvited(fd))
// 	{
// 		std::string msg = ERR_INVITEONLYCHAN(getClient(fd)->getNickname(), channel.getName()).c_str();
// 		send(fd, msg.c_str(), strlen(msg.c_str()), 0);
// 		std::cout << "Client " << fd << " needs to be invchanneled to join the channel " << channel.getName() << std::endl;
// 	}
// 	else if (channel.isClientBanned(fd))
// 	{
// 		std::string msg = ERR_BANNEDFROMCHAN(getClient(fd)->getNickname(), channel.getName()).c_str();
// 		send(fd, msg.c_str(), strlen(msg.c_str()), 0);
// 		std::cout << "Client " << fd << " is banned from the channel " << channel.getName() << std::endl;
// 	}
// 	else if (channel.isChannelFull())
// 	{
// 		std::string msg = ERR_CHANNELISFULL(getClient(fd)->getNickname(), channel.getName()).c_str();
// 		send(fd, msg.c_str(), strlen(msg.c_str()), 0);
// 		std::cout << "Channel " << channel.getName() << " is full" << std::endl;
// 	}
// 	else
// 	{
// 		std::cout << "Client " << fd << " joined the channel " << channel.getName() << std::endl;
// 		addClientToChannel(fd, &channel);
// 		std::cout << "Client " << channel.getClient(fd)->getNickname() << " joined the channel " << channel.getName() << std::endl;
// 		if (channel.getTopic().empty())
// 			send(fd, RPL_NOTOPIC(getClient(fd)->getNickname(), channel.getName()).c_str(), strlen(RPL_NOTOPIC(getClient(fd)->getNickname(), channel.getName()).c_str()), 0);
// 		else
// 			send(fd, RPL_TOPIC(getClient(fd)->getNickname(), channel.getName(), channel.getTopic()).c_str(), strlen(RPL_TOPIC(getClient(fd)->getNickname(), channel.getName(), channel.getTopic()).c_str()), 0);
// 		if ((*password).size() > 0)
// 			(*password).erase((*password).begin());
// 	}
// }

void Server::addClientToChannel(int fd, Channel *channel)
{
	channel->joinChannel(getClient(fd));
}

// Channel& Server::findChannel(std::string channelName)
// {
// 	//return a reference to the channel
// 	std::vector<Channel>::iterator it = std::find_if(
// 		this->_channels.begin(),
// 		this->_channels.end(),
// 		ChannelNameComparator(channelName));
// 	if (it != this->_channels.end())
// 	{
// 		return *it;
// 	}
// 	else
// 	{
// 		this->_channels.push_back(Channel(channelName));
// 		std::vector<Channel>::iterator it = std::find_if(
// 			this->_channels.begin(),
// 			this->_channels.end(),
// 			ChannelNameComparator(channelName));
// 		return *it;
// 	}
// }
