/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:08:54 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/17 17:00:26 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void Server::processKick(int fd, std::string arg)
{
	std::vector<std::string> args = split_args(arg);
	std::string		msg = "KICK\n";

	send(fd, msg.c_str(), msg.length(), 0);

	if (args.size() < 3 || args.size() > 4)
	{
		std::cout << "Usage : KICK <channel> <clientName> [<comment>]." << std::endl;
		return ;
	}
	std::string channelName = args[1];
	std::string clientName = args[2];
	// std::vector<Channel>::iterator channelIt = std::find_if(this->_channels.begin(), this->_channels.end(), ChannelNameComparator(channelName));
	// if (channelIt == this->_channels.end()) // le canal n existe pas
	// {
	// 	std::cout << "the channel " << channelName << " doesn t exist." << std::endl;
	// 	return ;
	// }
	try {
		Channel &channel = getChannel(channelName);

		if (!channel.isClientInChannel(fd))
		{
			std::cout << "Client requesting the kick isn t in the canal " << channelName << std::endl;
			return;
		}

		int fdToKick = isNameInChannel(channelName, clientName);
		if (fdToKick == 0)
		{
			std::cout << "Client isn t in the channel " << channelName << std::endl;
			return ;
		}
		else
		{
			channel.leaveChannel(fdToKick);
			std::string str = "You have been kicked from " + channelName;
			if (args.size() == 4)
				str = str + "\nComment : " + args[3];
			send(fdToKick, str.c_str(), str.size(), 0);
		}
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		return ;
	}
}
