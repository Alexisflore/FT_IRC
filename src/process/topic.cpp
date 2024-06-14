/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:09:16 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/14 15:06:58 by alfloren         ###   ########.fr       */
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
		displayTopic(fd, string);
	else
		changeTopic(fd, string);
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

void Server::displayTopic(int fd, std::vector<std::string> string)
{
	try {
		std::string channelName = string[1];
		Channel &channel = getChannel(channelName);
		std::string topic = channel.getTopic();
		std::string msg = "TOPIC " + channelName + " :";
		if (topic.empty())
			msg += "No topic is set\n";
		else
			msg += topic + "\n";
		send(fd, msg.c_str(), msg.length(), 0);
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		return ;
	}
}

void Server::changeTopic(int fd, std::vector<std::string> string)
{
	if (string[2].length() > 50)
	{
		std::cout << "The topic is too long." << std::endl;
		return ;
	}
	std::string channelName = string[1];
	std::string topic = string[2];
	try {
		Channel &channel = getChannel(channelName);
		if (channel.canClientSetTopic(fd) == true)
		{
			channel.setTopic(topic);
			std::string msg = "TOPIC " + channelName + " :" + topic + "\n";
			send(fd, msg.c_str(), msg.length(), 0);
		}
		else
			return ;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		return ;
	}
}
