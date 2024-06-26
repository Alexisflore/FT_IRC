/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:09:16 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/26 20:05:33 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include "../../includes/Channel.hpp"
#include <string>

void Server::processTopic(int fd, std::string string)
{
	if (getClient(fd)->isLogged() == false)
	{
		std::cout << "Client " << fd << " isn t logged." << std::endl;
		std::string msg = ERR_NOTREGISTERED(getClient(fd)->getNickname(), "TOPIC").c_str();
		send(fd, msg.c_str(), msg.length(), 0);
		return ;
	}
	std::string channelName = findChannel(string, "TOPIC");
	Channel &channel = getChannelbyName(channelName, fd);
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
			topic = RPL_NOTOPIC(getClient(fd)->getNickname(), channel.getName()).c_str();
		}
		else
			channel.setTopic(topic);
		std::string userid = USER_ID(getClient(fd)->getNickname(), getClient(fd)->getUsername());
		std::string msg = "TOPIC " + channel.getName() + " :" + topic + "\n";
		channel.sendMessage(userid + " " + msg);
	}
}

bool Channel::canClientSetTopic(int clientFd)
{
	//check if the client is in the channel
	if (!isClientInChannel(clientFd))
	{
		std::cout << "Client " << clientFd << " isn t in the channel " << this->_name << std::endl;
		std::string msg = ERR_NOTONCHANNEL(getClient(clientFd).getNickname(), this->_name).c_str();
		send(clientFd, msg.c_str(), msg.length(), 0);
		return false;
	}
	//check if the client is the channel operator
	if (isClientOperator(clientFd))
		return true;
	if (_modes.getModeValue('t') && !isClientOperator(clientFd))
	{
		std::cout << "Client " << clientFd << " isn t the channel operator in the channel #" << this->_name << std::endl;
		std::string msg = ERR_CHANOPRIVSNEEDED(getClient(clientFd).getNickname(), this->_name).c_str();
		send(clientFd, msg.c_str(), msg.length(), 0);
		return false;
	}
	return true;
}

void Server::displayTopic(int fd, Channel& channel)
{
	std::string topic = channel.getTopic();
	std::string msg;
	if (topic.empty())
		msg = RPL_NOTOPIC(getClient(fd)->getNickname(), channel.getName()).c_str();
	else
		msg = RPL_TOPIC(getClient(fd)->getNickname(), channel.getName(), topic).c_str();
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
