/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:09:16 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/17 15:16:04 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include "../../includes/Channel.hpp"
#include <string>

void Server::processTopic(int fd, std::vector <std::string> string)
{

	try {
		std::string channelName = string[2].substr(1, string[0].length());
		if (string.size() != 4 && string.size() != 3)
			throw std::invalid_argument("Usage : TOPIC <channel> [<topic>].");
		std::cout << channelName << std::endl;
		Channel &channel = getChannel(channelName);
		if (string.size() == 2)
			displayTopic(fd, channel);
		else
			changeTopic(fd, channel, string[3]);
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
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

void Server::displayTopic(int fd, Channel& channel)
{
		std::string topic = channel.getTopic();
		std::string msg = "TOPIC " + channel.getName() + " :";
		if (topic.empty())
			msg += "No topic is set\n";
		else
			msg += topic + "\n";
		send(fd, msg.c_str(), msg.length(), 0);
}

void Server::changeTopic(int fd, Channel& channel, std::string topic)
{
	if (topic.length() > 50)
	{
		std::cout << "The topic is too long." << std::endl;
		return ;
	}
	if (channel.canClientSetTopic(fd) == true)
	{
		channel.setTopic(topic);
		std::string msg = "TOPIC " + channel.getName() + " :" + topic + "\n";
		send(fd, msg.c_str(), msg.length(), 0);
	}
	else
		std::cout << "The client can't set the topic." << std::endl;
}
