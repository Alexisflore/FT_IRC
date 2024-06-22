/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:09:16 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/22 12:26:14 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include "../../includes/Channel.hpp"
#include <string>

void Server::processTopic(int fd, std::string string)
{
	std::vector<std::string> args = split_args(string, " ");
	if (args.size() < 2)
	{
		std::string msg = ERR_NEEDMOREPARAMS(getClient(fd)->getNickname(), "TOPIC").c_str();
		send(fd, msg.c_str(), msg.length(), 0);
		std::cout << "Need more params" << std::endl;
		return ;
	}
	std::string channelName = args[1];
	Channel &channel = getChannelbyName(channelName, getClient(fd)->getNickname());
	if (args.size() == 2)
			displayTopic(fd, channel);
	else if (channel.canClientSetTopic(fd) == true)
	{
		if (args[2][0] != ':')
		{
			std::string msg = ERR_NEEDMOREPARAMS(getClient(fd)->getNickname(), "TOPIC").c_str();
			send(fd, msg.c_str(), msg.length(), 0);
			std::cout << "No : in the topic" << std::endl;
			return ;
		}
		if (args[2].length() == 1 || args[2][1] == '\n' || args[2][1] == '\r')
		{
			channel.clearTopic();
			std::string msg = RPL_NOTOPIC(getClient(fd)->getNickname(), channel.getName()).c_str();
			send(fd, msg.c_str(), msg.length(), 0);
			return ;
		}
		std::string topic = args[2].substr(1);
		for (size_t i = 3; i < args.size(); i++)
			topic += " " + args[i];
		if (topic.length() > 50)
		{
			send(fd, "The topic is too long.\n", 23, 0);
			return ;
		}
		// else if (topic.empty())
		// {
		// 	channel.clearTopic();
		// 	topic = RPL_NOTOPIC(getClient(fd)->getNickname(), channel.getName()).c_str();
		// }
		else
			channel.setTopic(topic);
		std::string msg = "TOPIC " + channel.getName() + " :" + channel.getTopic() + "\n";
		channel.sendMessage(msg);
		send(fd, msg.c_str(), msg.length(), 0);
	}
}

bool Channel::canClientSetTopic(int clientFd)
{
	//check if the client is in the channel
	if (!isClientInChannel(clientFd))
	{
		std::cout << "Client " << clientFd << " isn t in the channel " << this->_name << std::endl;
		return false;
	}
	//check if the client is the channel operator
	if (isClientOperator(clientFd))
		return true;
	if (_modes.getModeValue('t') && !isClientOperator(clientFd))
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
		std::string msg = RPL_TOPIC(getClient(fd)->getNickname(), channel.getName(), topic).c_str();
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
	std::cout << string << std::endl;
	string.find(":") == std::string::npos ? channelName = string.substr(0, string.length()) : channelName = string.substr(0, string.find(":") - 1);
	return channelName;
}
