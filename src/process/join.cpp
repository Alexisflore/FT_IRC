/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 14:42:52 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/17 16:35:36 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include <string>

void	Server::processJoin(int fd, std::vector<std::string> args)
{
	if (args.size() > 2)
		std::cout << "Usage : \"JOIN nameofthechannel\"" << std::endl; // a envoyer au client a la place de l ecrire cote serveur
	else if (args.size() == 2)
	{
		std::cout << "Client " << fd << " is trying to join channel " << args[1] << std::endl;
		std::string channelName = args[1].substr(1, args[1].length());
		std::vector<Channel>::iterator it = std::find_if(
			this->_channels.begin(),
			this->_channels.end(),
			ChannelNameComparator(channelName));
		if (it != this->_channels.end()) // le canal existe
		{
			Channel& channel = *it;
			if (channel.isClientInChannel(fd))
				std::cout << "Client " << fd << " is already in the channel " << channel.getName() << std::endl;
			else
				channel.joinChannel(fd);
		}
		else // creation du canal
		{
			std::cout << "Channel " << channelName << " doesnt exist yet!" << std::endl;
			Channel newChannel(channelName);
			this->_channels.push_back(newChannel);
			std::vector<Channel>::iterator newIt = std::find_if(this->_channels.begin(), this->_channels.end(), ChannelNameComparator(channelName));
			if (newIt != this->_channels.end())
			{
				std::cout << "new channel " << channelName << " created" << std::endl;
				newIt->joinChannel(fd);
			}
			else
				std::cout << "failed to create channel " << channelName << std::endl;
		}
	}
	else
		std::cout << "need to specify the channel name" << std::endl;
}
