/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:08:56 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/14 14:10:52 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include "../../includes/Channel.hpp"

void Server::processMode(int fd, std::vector <std::string> string)
{
	if (string.size() < 2 || string.size() > 3)
	{
		std::cout << "Usage : MODE <channel> [<mode>]." << std::endl;
		return ;
	}
	else if (string.size() == 2)
	{
		std::string channelName = string[1];
		std::vector<Channel>::iterator channelIt = std::find_if(this->_channels.begin(), this->_channels.end(), ChannelNameComparator(channelName));
		if (channelIt == this->_channels.end()) // le canal n existe pas
		{
			std::cout << "the channel " << channelName << " doesn t exist." << std::endl;
			return ;
		}
		Channel &channel = *channelIt; // le canal existe bien
		std::map<std::string, bool> modes = channel.getModes();
		std::string msg = "MODE " + channelName + " :";
		for (std::map<std::string, bool>::iterator it = modes.begin(); it != modes.end(); it++)
		{
			if (it->second == true)
				msg += "+" + it->first + " ";
			else
				msg += "-" + it->first + " ";
		}
	}
	else
	{
		if (string[2].length() > 50)
		{
			std::cout << "The mode is too long." << std::endl;
			return ;
		}
		std::string channelName = string[1];
		std::string mode = string[2];
		std::vector<Channel>::iterator channelIt = std::find_if(this->_channels.begin(), this->_channels.end(), ChannelNameComparator(channelName));
		if (channelIt == this->_channels.end()) // le canal n existe pas
		{
			std::cout << "the channel " << channelName << " doesn t exist." << std::endl;
			return ;
		}
		Channel &channel = *channelIt; // le canal existe bien
		if (channel.canClientSetMode(fd, mode) == true)
		{
			channel.setMode(mode, true);
			std::string msg = "MODE " + channelName + " :" + mode + "\n";
			send(fd, msg.c_str(), msg.length(), 0);
		}
	}
}

bool Channel::canClientSetMode(int clientFd, std::string mode)
{
	if (mode == "o" || mode == "l")
	{
		if (isClientInChannel(clientFd) == true)
			return (true);
		else
			return (false);
	}
	else if (mode == "t" || mode == "i" || mode == "k")
	{
		if (isClientOperator(clientFd) == true)
			return (true);
		else
			return (false);
	}
	else
	{
		std::cout << "The mode " << mode << " doesn't exist." << std::endl;
		return (false);
	}
}

void	Channel::setMode(std::string mode, bool value)
{
	this->_modes[mode] = value;
}