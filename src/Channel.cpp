/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 01:34:39 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/27 12:08:25 by alfloren         ###   ########.fr       */
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
std::vector<std::pair<Client, char> >	Channel::getClients() {return this->_clients;}
// std::map<std::string, bool> Channel::getModesAsString() {return this->_modes;}
bool						Channel::getMode(char mode) {return _modes.getModeValue(mode);}
// std::string					Channel::getParams(char mode) {return _modes.getParams(mode);}
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
	return (Client());
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
			std::cout << "Client " << clientFd << " is now operator in the channel " << this->_name << std::endl;
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

std::string					Channel::getUsers()
{
	std::string users;
	for (std::vector<std::pair<Client, char> >::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->second == 'o')
			users += '@';
		else if (it->second == 'i' || it->second == 'b')
			continue;
		users += it->first.getNickname() + " ";
	}
	return users;
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
		if (it->second != 'b' && it->second != 'i')
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
	}
	return false;
}

bool Channel::isClientOperator(int clientFd)
{
	for (std::vector<std::pair<Client, char> >::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->first.getFd() == clientFd && it->second == 'o')
			return true;
	}
	return false;
}

bool Channel::isChannelFull()
{
	if (_modes.getModeValue('l') == true && _clients.size() >= static_cast<unsigned long>(_modes.getLimit()))
		return true;
	return false;
}

bool Channel::isInviteOnly() {return _modes.getModeValue('i');}
bool Channel::isPasswordProtected(){return _modes.getModeValue('k');}
bool Channel::isTopicProtected(){return _modes.getModeValue('t');}
std::string Channel::getPassword() {return _modes.getPassword();}
bool Client::operator<(const Client& other) const {return this->getNickname() < other.getNickname();}

std::string displaymodewithparams(MODE mode)
{
	std::string modewithparams;
	modewithparams = "+";
	if (mode.getModeValue('i'))
		modewithparams += "i";
	if (mode.getModeValue('t'))
		modewithparams += "t";
	if (mode.getModeValue('l'))
	{
		std::stringstream ss;
		ss << mode.getLimit();
		std::string st = ss.str();
		modewithparams += "l " + st;
	}
	if (mode.getModeValue('k'))
	{
		std::string pass = mode.getPassword();
		if (mode.getModeValue('l'))
			modewithparams += " +k " + pass;
		else
			modewithparams += "k " + pass;
	}
	return modewithparams;
}
void	Channel::displayMode(int fd, std::string nick)
{
	std::string mode = displaymodewithparams(_modes);
	std::string msg = RPL_CHANNELMODEIS(nick, _name, mode).c_str();
	send(fd, msg.c_str(), msg.length(), 0);
}

void Channel::sendNotification(const std::string message, int fd)
{
	for (std::vector<std::pair<Client, char> >::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->first.getFd() != fd)
			send(fd, message.c_str(), message.length(), 0);
	}
}

void Channel::sendprivmsg(int fd, std::string message)
{
	for (std::vector<std::pair<Client, char> >::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->first.getFd() != fd && it->second != 'b' && it->second != 'i')
			send(it->first.getFd(), message.c_str(), message.length(), 0);
	}
}
