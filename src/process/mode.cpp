/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:08:56 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/14 15:14:01 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include "../../includes/Channel.hpp"

void Server::processMode(int fd, std::vector <std::string> string)
{
	if (string.size() < 2 || string.size() > 3)
	{
		std::cout << "Usage : MODE <channel> [<mode>] {[+|-]|o|p|s|i|t|n|b|v} [<limit>] [<user>] [<ban mask>] [<key>]." << std::endl;
		return ;
	}
	std::string channelName = string[1];
	Channel &channel = getChannel(channelName);
	if (string.size() == 2)
		displayMode(fd, channel);
	else
	{
		std::string mode = string[2];
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

void Server::displayMode(int fd, Channel& channel)
{
	try {
		std::map<std::string, bool> modes = channel.getModes();
		std::string msg = "MODE " + channel.getName() + " :";
		for (std::map<std::string, bool>::iterator it = modes.begin(); it != modes.end(); it++)
		{
			if (it->second == true)
				msg += "+" + it->first + " ";
			else
				msg += "-" + it->first + " ";
		}
		msg += "\n";
		send(fd, msg.c_str(), msg.length(), 0);
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		return ;
	}
}