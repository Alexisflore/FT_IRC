/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:08:56 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/18 18:05:55 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include "../../includes/Channel.hpp"

void Server::processMode(int fd, std::string strings)
{
	std::vector<std::string> string = split_args(strings, " ");
	if (string.size() < 2 || string.size() > 4)
	{
		std::string msg = ERR_NEEDMOREPARAMS(getClient(fd)->getNickname(), "MODE").c_str();
		send(fd, msg.c_str(), strlen(msg.c_str()), 0);
		return ;
	}
	std::string channelName = string[1];
	Channel &channel = getChannelbyName(channelName, getClient(fd)->getNickname());
	if (channel.canClientSetMode(fd) == false)
	{
		std::cout << "Client " << fd << " can't set mode " << string[2] << " in channel " << channelName << std::endl;
		return ;
	}
	if (string.size() == 2)
		displayMode(fd, channel);
	std::string mode = string[2];
	bool		value;
	std::string params;
	if (string.size() == 4)
		params = string[3];
	else
		params.clear();
	createValue(mode, value);
	channel.setMode(mode, value, params);
}

bool Channel::canClientSetMode(int clientFd)
{
	if (!isClientOperator(clientFd))
		return false;
	return true;
}

void	Channel::setMode(std::string mode, bool value, std::string params)
{
	this->_modes[mode] = value;
	std::string m[5] = {"t", "i", "k", "l", "o"};
	void	(Channel::*Mode[5])(bool, std::string) = {
		&Channel::topicMode,
		&Channel::inviteMode,
		&Channel::keyMode,
		&Channel::limitMode,
		&Channel::operatorMode
	};
	for (int i = 0; i < 5; i++)
	{
		if (mode == m[i])
		{
			try{(this->*Mode[i])(value, params);}
			catch (std::exception &e)
			{std::cout << e.what() << std::endl;}
			break;
		}
	}
}

void Server::displayMode(int fd, Channel& channel)
{
	try {
		std::map<std::string, bool> modes = channel.getModes();
		std::string msg = "MODE " + channel.getName() + " :";
		for (std::map<std::string, bool>::iterator it = modes.begin(); it != modes.end(); it++)
		{
			if (it->second == true)
				msg += "+" + it->first + " ";
			else
				msg += "-" + it->first + " ";
		}
		msg += "\n";
		send(fd, msg.c_str(), msg.length(), 0);
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		return ;
	}
}

void Server::createValue(std::string& mode, bool& value)
{
	if (mode[0] == '+')
		value = true;
	else if (mode[0] == '-')
		value = false;
	else
		throw std::invalid_argument("The mode must start with + or -");
	mode = mode.substr(1);
}

void		Channel::topicMode(bool value, std::string params)
{
	if  (params.empty() == false)
		throw std::invalid_argument("The topic mode doesn't need any parameters.");
	if (value == true)
		_modes["t"] = true;
	else
		_modes["t"] = false;
}

void		Channel::inviteMode(bool value, std::string params)
{
	if (params.empty() == false)
		throw std::invalid_argument("The invite mode doesn't need any parameters.");
	if (value == true)
		_modes["i"] = true;
	else
		_modes["i"] = false;
}

void		Channel::keyMode(bool value, std::string params)
{
	if (params.empty() == true)
		throw std::invalid_argument("The key mode needs a parameter.");
	if (value == true)
		this->_key = params;
	else
		this->_key = "";
}

void		Channel::limitMode(bool value, std::string params)
{
	if (params.empty() == true)
		throw std::invalid_argument("The limit mode needs a parameter.");
	if (value == true)
	{
		std::stringstream ss(params);
		ss >> this->_limit;
		if (ss.fail())
			throw std::invalid_argument("The limit parameter must be a number.");
	}
	else
		this->_limit = -1;
}

void		Channel::operatorMode(bool value, std::string params)
{
	if (params.empty() == true)
		throw std::invalid_argument("The operator mode needs a parameter.");
	int fd = getFdFromNick(params);
	if (value == true)
	{
		if (isClientInChannel(fd) == false)
			throw std::invalid_argument("The client isn't in the channel.");
		_operators.push_back(fd);
	}
	else
	{
		if (isClientOperator(fd) == false)
			throw std::invalid_argument("The client isn't an operator.");
		removeClientfromList(fd, _operators);
	}
}

int		Channel::getFdFromNick(std::string nick)
{
	for (std::vector<int>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (nick == getClient(*it)->getNickname())
			return (*it);
	}
	throw std::invalid_argument("The client isn't in the channel.");
}
