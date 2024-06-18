/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:09:06 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/18 11:47:24 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void Server::processPart(int fd, std::string arg)
{
	std::vector<std::string> args = split_args(arg, ' ');
	if (args.size() > 2)
		std::cout << "Usage : \"QUIT nameofthechannel\"" << std::endl; // a envoyer au client a la place de l ecrire cote serveur
	else if (args.size() == 2)
	{
		std::cout << "Client " << fd << " is trying to quit channel #" << args[1] << std::endl;
		std::string channelName = args[1];

		std::vector<Channel>::iterator it = std::find_if(
			this->_channels.begin(),
			this->_channels.end(),
			ChannelNameComparator(channelName));
		if (it != this->_channels.end()) // le canal existe
		{
			Channel& channel = *it;
			if (channel.isClientInChannel(fd))
				channel.leaveChannel(fd);
			else
				std::cout << "Client " << fd << " isn't in the channel " << channelName << std::endl;
		}
		else
			std::cout << "Channel " << channelName << " doesnt exist yet!" << std::endl;
	}
	else
		std::cout << "need to specify the channel name" << std::endl;
}
