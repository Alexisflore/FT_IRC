/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:09:16 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/14 13:21:25 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include "../../includes/Channel.hpp"

void Server::processTopic(int fd, std::vector <std::string> string)
{
	if (string.size() < 2 || string.size() > 3)
	{
		std::cout << "Usage : TOPIC <channel> [<topic>]." << std::endl;
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
		std::string topic = channel.getTopic();
		std::string msg = "TOPIC " + channelName + " :";
		if (topic.empty())
			msg += "No topic is set\n";
		else
			msg += topic + "\n";
		send(fd, msg.c_str(), msg.length(), 0);
	}
	else
	{
		if (string[2].length() > 50)
		{
			std::cout << "The topic is too long." << std::endl;
			return ;
		}
		std::string channelName = string[1];
		std::string topic = string[2];
		std::vector<Channel>::iterator channelIt = std::find_if(this->_channels.begin(), this->_channels.end(), ChannelNameComparator(channelName));
		if (channelIt == this->_channels.end()) // le canal n existe pas
		{
			std::cout << "the channel " << channelName << " doesn t exist." << std::endl;
			return ;
		}
		Channel &channel = *channelIt; // le canal existe bien
		if (channel.canClientSetTopic(fd) == true)
		{
			channel.setTopic(topic);
			std::string msg = "TOPIC " + channelName + " :" + topic + "\n";
			send(fd, msg.c_str(), msg.length(), 0);
		}
		else
			return ;
	}
}

bool Channel::canClientSetTopic(int clientFd)
{
	//check if the client is in the channel
	if (!isClientInChannel(clientFd))
	{
		std::cout << "Client " << clientFd << " isn t in the channel #" << this->_name << std::endl;
		return false;
	}
	//check if the client is the channel operator
	if (isClientOperator(clientFd))
		return true;
	if (this->_modes["t"] == true)
	{
		std::cout << "Client " << clientFd << " isn t the channel operator in the channel #" << this->_name << std::endl;
		return false;
	}
	return true;
}