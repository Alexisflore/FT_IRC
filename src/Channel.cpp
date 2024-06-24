/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 09:48:25 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/25 01:28:06 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"


Channel::Channel() {
	_modes = MODE();
}

Channel::Channel(std::string channelName) : _name(channelName) {
	_modes = MODE();
}
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
int							Channel::getFdFromNick(std::string nick)
{
	for (std::vector<std::pair<Client, char> >::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->first.getNickname() == nick)
			return (it->first.getFd());
	}
	return (-1);
}
Client						Channel::getClientByNick(std::string nick)
{
	for (std::vector<std::pair<Client, char> >::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->first.getNickname() == nick)
			return (it->first);
	}
	throw std::invalid_argument("Client not found");
}

/*--------------Setters--------------*/
void						Channel::setName(std::string name) {this->_name = name;}
void						Channel::setTopic(std::string topic) {this->_topic = topic;}
void						Channel::clearTopic() {this->_topic.clear();}
void						Channel::setClientasBanned(int clientFd) {
	for (std::vector<std::pair<Client, char> >::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->first.getFd() == clientFd)
		{
			it->second = 'b';
			break ;
		}
	}
}

void						Channel::setClientasInvited(int clientFd) {
	for (std::vector<std::pair<Client, char> >::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->first.getFd() == clientFd)
		{
			it->second = 'i';
			break ;
		}
	}
}

void						Channel::setClientasOperator(int clientFd) {
	for (std::vector<std::pair<Client, char> >::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->first.getFd() == clientFd)
		{
			it->second = 'o';
			break ;
		}
	}
}
void						Channel::setClientasNormal(int clientFd) {
	for (std::vector<std::pair<Client, char> >::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->first.getFd() == clientFd)
		{
			it->second = 'n';
			break ;
		}
	}
}
std::vector<int>			Channel::getClientsFd()
{
	std::vector<int> clientsFd;
	for (std::vector<std::pair<Client, char> >::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		//show the adress of the client
		// std::cout << "Client address: " << it->first << std::endl;
		std::cout << "HERE"	<< std::endl;
		clientsFd.push_back(it->first.getFd());
	}
	return clientsFd;
}

/*--------------Methods--------------*/
bool    Channel::isClientInChannel(int clientFd)
{
	// for (std::map<Client, char>::iterator it = _clients.begin(); it != _clients.end(); it++)
	// {
	// 	if (it->first.getFd() == clientFd)
	// 		return true;
	// }
	for (std::vector<std::pair<Client, char> >::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if ((it)->first.getFd() == clientFd)
		{
			std::cout << "Client " << clientFd << " is in the channel " << this->_name << std::endl;
			return true;
		}
	}
	return false;
}
void Channel::joinChannel(Client client)
{
	_clients.push_back(std::make_pair(client, 'n'));
	this->_client.push_back(client);
}

void Channel::removeClient(Client client)
{
	int fd = client.getFd();
	for (std::vector<std::pair<Client, char> >::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->first.getFd() == fd)
		{
			_clients.erase(it);
			break ;
		}
	}
}

void Channel::leaveChannel(int clientFd)
{
	for (std::vector<std::pair<Client, char> >::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->first.getFd() == clientFd)
		{
			_clients.erase(it);
			break ;
		}
	}
}

void Channel::sendMessage(const std::string message)
{
	for (std::vector<std::pair<Client, char> >::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		send(it->first.getFd(), message.c_str(), message.length(), 0);
	}
}

bool Channel::isClientBanned(int clientFd)
{
	for (std::vector<std::pair<Client, char> >::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->first.getFd() == clientFd && it->second == 'b')
			return true;
	}
	return false;
}

bool Channel::isClientInvited(int clientFd)
{
	for (std::vector<std::pair<Client, char> >::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->first.getFd() == clientFd && it->second == 'i')
			return true;
		return true;
		
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

bool Client::operator<(const Client& other) const {
        // Compare the clients based on their nicknames, for example
        return this->getNickname() < other.getNickname();
    }