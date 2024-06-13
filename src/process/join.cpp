/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 14:42:52 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/13 17:03:41 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void	Server::processJoin(int fd, std::vector<std::string> args)
{
	if (args.size() > 2)
		std::cout << "Usage : \"JOIN nameofthechannel\"" << std::endl; // a envoyer au client a la place de l ecrire cote serveur
	else if (args.size() == 2)
	{
		std::cout << "Client " << fd << " is trying to join channel #" << args[1] << std::endl;
		std::string channelName = args[1];
		std::vector<Channel>::iterator it = std::find_if(
			this->_channels.begin(),
			this->_channels.end(),
			ChannelNameComparator(channelName));
		if (it != this->_channels.end()) // le canal existe
		{
			Channel& channel = *it;
			if (channel.isClientInChannel(fd))
				std::cout << "Client " << fd << " is already in the channel " << channel.getName();
			else
				channel.joinChannel(fd);
		}
		else // creation du canal
		{
			Channel newChannel(channelName);
			this->_channels.push_back(newChannel);
			std::cout << "new channel " << channelName << " created" << std::endl;
			newChannel.joinChannel(fd);
		}
	}
	else
		std::cout << "need to specify the channel name" << std::endl;
}


