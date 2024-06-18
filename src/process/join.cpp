/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 14:42:52 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/18 11:57:39 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include <string>

void	Server::processJoin(int fd, std::string arg)
{
	arg.erase(0, 5);
	std::vector<std::string> args = split_args(arg, ' ');
	if (args.size() == 0 || args[0].empty() || args.size() > 2)
	{
		std::string msg = ERR_NEEDMOREPARAMS(getClient(fd)->getNickname(), "JOIN").c_str();
		send(fd, msg.c_str(), strlen(msg.c_str()), 0);
		return ;
	}
	std::vector<std::string> channelNames;
	std::vector<Channel> channels;
	std::vector<std::string> password;
	if (args.size() == 2)
		password = split_args(args[1], ' ');
	Channel newChannel;
	channelNames = split_args(args[0], ',');
	channels = getChannels(channelNames);
	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		if ((it->isPasswordProtected() && password.size() == 0) || (it->isPasswordProtected() && password.size() > 0 && it->getPassword() != password[0]))
		{
			std::string msg = ERR_BADCHANNELKEY(getClient(fd)->getNickname(), it->getName()).c_str();
			send(fd, msg.c_str(), strlen(msg.c_str()), 0);
			std::cout << "Client " << fd << " needs to provide a password to join the channel " << it->getName() << std::endl;
		}
		else if (it->isInviteOnly() && !it->isClientInvited(fd))
		{
			std::string msg = ERR_INVITEONLYCHAN(getClient(fd)->getNickname(), it->getName()).c_str();
			send(fd, msg.c_str(), strlen(msg.c_str()), 0);
			std::cout << "Client " << fd << " needs to be invited to join the channel " << it->getName() << std::endl;
		}
		else if (it->isClientBanned(fd))
		{
			std::string msg = ERR_BANNEDFROMCHAN(getClient(fd)->getNickname(), it->getName()).c_str();
			send(fd, msg.c_str(), strlen(msg.c_str()), 0);
			std::cout << "Client " << fd << " is banned from the channel " << it->getName() << std::endl;
		}
		else if (it->isChannelFull())
		{
			std::string msg = ERR_CHANNELISFULL(getClient(fd)->getNickname(), it->getName()).c_str();
			send(fd, msg.c_str(), strlen(msg.c_str()), 0);
			std::cout << "Channel " << it->getName() << " is full" << std::endl;
		}
		else
		{
			it->joinChannel(fd);
			std::cout << "Client " << fd << " joined the channel " << it->getName() << std::endl;
			if (it->getTopic().empty())
				send(fd, RPL_NOTOPIC(getClient(fd)->getNickname(), it->getName()).c_str(), strlen(RPL_NOTOPIC(getClient(fd)->getNickname(), it->getName()).c_str()), 0);
			else
				send(fd, RPL_TOPIC(getClient(fd)->getNickname(), it->getName(), it->getTopic()).c_str(), strlen(RPL_TOPIC(getClient(fd)->getNickname(), it->getName(), it->getTopic()).c_str()), 0);
			if (password.size() > 0)
				password.erase(password.begin());
		}
	}
}

std::vector<Channel> Server::getChannels(std::vector< std::string > channelNames)
{
	std::vector< Channel > channels;
	for (std::vector< std::string >::iterator it = channelNames.begin(); it != channelNames.end(); it++)
	{
		std::vector< Channel >::iterator it2 = std::find_if(
			this->_channels.begin(),
			this->_channels.end(),
			ChannelNameComparator(*it));
		if (it2 != this->_channels.end())
			channels.push_back(*it2);
		else
		{
			Channel newChannel(*it);
			this->_channels.push_back(newChannel);
			channels.push_back(newChannel);
		}
	}
	return channels;
}

bool Channel::isChannelFull()
{
	if (this->_modes["l"] == true && this->_clients.size() >= this->_limit)
		return true;
	return false;
}

bool Channel::isPasswordProtected()
{
	if (this->_key.empty())
		return false;
	return true;
}

bool Channel::isInviteOnly()
{
	if (this->_modes["i"] == true)
		return true;
	return false;
}

std::string Channel::getPassword()
{
	return this->_key;
}
