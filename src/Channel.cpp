/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 09:48:25 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/21 11:10:11 by alfloren         ###   ########.fr       */
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
	_banned = other._banned;
	_invited = other._invited;
	_operators = other._operators;
}
// Channel &operator=(const Channel &other) {}

/*--------------Getters--------------*/
std::string     			Channel::getName() const {return this->_name;}
std::vector<int>    		Channel::getClients() const {return this->_clients;}
std::string					Channel::getTopic() {return this->_topic;}
// std::map<std::string, bool> Channel::getModesAsString() {return this->_modes;}
bool						Channel::getMode(char mode) {return _modes.getModeValue(mode);}
std::string					Channel::getParams(char mode) {return _modes.getParams(mode);}
int							Channel::getFdFromNick(std::string nick)
{
	for (std::map<int, std::string>::iterator it = _nicks.begin(); it != _nicks.end(); it++)
	{
		if (it->second == nick)
			return (it->first);
	}
	return (-1);
}
// Client*						Server::getClient(int fd)
// {
// 	for (std::vector<int>::iterator it = _clients.begin(); it != _clients.end(); it++)
// 	{
// 		if (*it == fd)
// 			return (getClientbyFd(fd));
// 	}
// 	return (NULL);
// }

/*--------------Setters--------------*/
void						Channel::setName(std::string name) {this->_name = name;}
void						Channel::setTopic(std::string topic) {this->_topic = topic;}
void						Channel::clearTopic() {this->_topic.clear();}
void						Channel::setClientasBanned(int clientFd) {this->_banned.push_back(clientFd);}
void						Channel::setClientasInvited(int clientFd) {this->_invited.push_back(clientFd);}
void						Channel::setClientasOperator(int clientFd) {this->_operators.push_back(clientFd);}
void 						Channel::setClientasNotOperator(int clientFd) {removeClientfromList(clientFd, _operators);}

/*--------------Methods--------------*/
bool    Channel::isClientInChannel(int clientFd)
{
	if (_clients.empty() == true)
	{
		std::cout << "No clients in channel" << this->_name << std::endl;
		return false;
	}
	bool res = std::find(this->_clients.begin(), this->_clients.end(), clientFd) != this->_clients.end();
    return (res);
}

void Channel::joinChannel(int clientFd, std::string nick)
{
    this->_clients.push_back(clientFd);
	_nicks[clientFd] = nick;
}

void Channel::leaveChannel(int clientFd)
{
    for (std::vector<int>::iterator it = _clients.begin(); it != _clients.end(); it++)
    {
        if (*it == clientFd)
        {
            _clients.erase(it);
            std::cout << "Client " << clientFd << " has left channel #" << this->_name << std::endl;
            return ;
        }
    }
}

void Channel::sendMessage(const std::string message)
{
    for (std::vector<int>::iterator it = _clients.begin(); it != _clients.end(); it++)
    {
        if (send(*it, message.c_str(), message.size(), 0) == -1)
            throw SendException();
        std::cout << "Message " << message << " send to client " << *it << " in channel #" << this->_name << std::endl;
    }
}

bool Channel::isClientBanned(int clientFd)
{
	bool res = std::find(this->_banned.begin(), this->_banned.end(), clientFd) != this->_banned.end();
	return (res);
}

bool Channel::isClientInvited(int clientFd)
{
	bool res = std::find(this->_invited.begin(), this->_invited.end(), clientFd) != this->_invited.end();
	return (res);
}

bool Channel::isClientOperator(int clientFd)
{
	bool res = std::find(this->_operators.begin(), this->_operators.end(), clientFd) != this->_operators.end();
	return (res);
}

void Channel::removeClientfromList(int clientFd, std::vector<int> &_clients)
{
	for (std::vector<int>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (*it == clientFd)
		{
			_clients.erase(it);
			return ;
		}
	}
}
