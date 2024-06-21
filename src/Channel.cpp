/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 09:48:25 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/21 17:20:27 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"


Channel::Channel() {
	_modes = MODE();
}

Channel::Channel(std::string channelName) : _name(channelName) {}
Channel::~Channel() {}
Channel::Channel(const Channel &other) {
	_name = other._name;
	_clients = other._clients;
	_topic = other._topic;
	_modes = other._modes;
}
// Channel &operator=(const Channel &other) {}

/*--------------Getters--------------*/
std::string     			Channel::getName() const {return this->_name;}
std::string					Channel::getTopic() {return this->_topic;}
// std::map<std::string, bool> Channel::getModesAsString() {return this->_modes;}
bool						Channel::getMode(char mode) {return _modes.getModeValue(mode);}
std::string					Channel::getParams(char mode) {return _modes.getParams(mode);}
int							Channel::getFdFromNick(std::string nick)
{
	for (std::map<Client*, char>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->first->getNickname() == nick)
			return (it->first->getFd());
	}
	return (-1);
}
Client*						Channel::getClientByNick(std::string nick)
{
	for (std::map<Client*, char>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->first->getNickname() == nick)
			return (it->first);
	}
	return (NULL);
}

/*--------------Setters--------------*/
void						Channel::setName(std::string name) {this->_name = name;}
void						Channel::setTopic(std::string topic) {this->_topic = topic;}
void						Channel::clearTopic() {this->_topic.clear();}
void						Channel::setClientasBanned(int clientFd) {_clients[getClient(clientFd)] = 'b';}
void						Channel::setClientasInvited(int clientFd) {_clients[getClient(clientFd)] = 'i';}
void						Channel::setClientasOperator(int clientFd) {_clients[getClient(clientFd)] = 'o';}
void						Channel::setClientasNormal(int clientFd) {_clients[getClient(clientFd)] = 'n';}
std::vector<int>			Channel::getClientsFd()
{
	std::vector<int> clientsFd;
	for (std::map<Client*, char>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		clientsFd.push_back(it->first->getFd());
	}
	return clientsFd;
}

/*--------------Methods--------------*/
bool    Channel::isClientInChannel(int clientFd)
{
	Client *client = getClient(clientFd);
	if (client == NULL)
		return false;
	if (_clients.find(client) != _clients.end())
		return true;
	return false;
}

void Channel::joinChannel(Client *client)
{
	this->_clients[client] = 'n';
}

void Channel::removeClient(Client *client)
{
	_clients.erase(client);
}

void Channel::leaveChannel(int clientFd)
{
	_clients.erase(getClient(clientFd));
}

void Channel::sendMessage(const std::string message)
{
	for (std::map<Client*, char>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		send(it->first->getFd(), message.c_str(), strlen(message.c_str()), 0);
	}
}

bool Channel::isClientBanned(int clientFd)
{
	if (_clients[getClient(clientFd)] == 'b')
		return true;
	return false;
}

bool Channel::isClientInvited(int clientFd)
{
	if (_clients[getClient(clientFd)] == 'i')
		return true;
	return false;
}

bool Channel::isClientOperator(int clientFd)
{
	if (_clients[getClient(clientFd)] == 'o')
		return true;
	return false;
}

bool Channel::isChannelFull()
{
	if (_modes.getModeValue('l') == true && _clients.size() >= static_cast<unsigned long>(_modes.getLimit()))
		return true;
	return false;
}

bool Channel::isInviteOnly()
{
	if (_modes.getModeValue('i') == false)
		return false;
	return true;
}

bool Channel::isPasswordProtected()
{
	if (_modes.getModeValue('k') == false)
		return false;
	return true;
}

bool Channel::isTopicProtected()
{
	if (_modes.getModeValue('t') == false)
		return false;
	return true;
}

std::string Channel::getPassword()
{
	return _modes.getParams('k');
}
