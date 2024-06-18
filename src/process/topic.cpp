/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:09:16 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/18 11:20:33 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include "../../includes/Channel.hpp"
#include <string>

void Server::processTopic(int fd, std::string string)
{
	std::string channelName = findChannel(string, "TOPIC");
	Channel &channel = getChannelbyName(channelName, getClient(fd)->getNickname());
	if (string.find(":") == std::string::npos)
			displayTopic(fd, channel);
	else if (channel.canClientSetTopic(fd) == true)
	{
		std::string topic = string.substr(string.find(":") + 1, string.length());
		if (topic.length() > 50)
		{
			send(fd, "The topic is too long.\n", 23, 0);
			return ;
		}
		else if (topic.empty())
		{
			channel.clearTopic();
			topic = "No topic is set\n";
		}
		else
			channel.setTopic(topic);
		std::string msg = "TOPIC " + channel.getName() + " :" + topic + "\n";
		channel.sendMessage(msg);
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

std::string Server::findChannel(std::string string, std::string cmd)
{
	int size = cmd.length();
	std::string channelName;

	string.erase(0, size + 1);
	if (string[0] == '#')
		string.erase(0, 1);
	std::cout << string << std::endl;
	string.find(":") == std::string::npos ? channelName = string.substr(0, string.length()) : channelName = string.substr(0, string.find(":") - 2);
	return channelName;
}
