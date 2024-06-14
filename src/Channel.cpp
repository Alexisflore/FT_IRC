/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 09:48:25 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/14 13:15:03 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"

Channel::Channel(std::string channelName) : _name(channelName) , _topic(NULL) {
	_modes["t"] = false;
	_modes["i"] = false;
	_modes["k"] = false;
	_modes["l"] = false;
	_modes["o"] = false;
}
Channel::Channel() : _name(NULL), _topic(NULL) {
	_modes["t"] = false;
	_modes["i"] = false;
	_modes["k"] = false;
	_modes["l"] = false;
	_modes["o"] = false;
}
Channel::~Channel() {}
// Channel(const Channel &other) {}
// Channel &operator=(const Channel &other) {}

/*--------------Getters--------------*/
std::string     			Channel::getName() const {return this->_name;}
std::vector<int>    		Channel::getClients() const {return this->_clients;}
std::string					Channel::getTopic() {return this->_topic;}
std::map<std::string, bool> Channel::getModes() {return this->_modes;}

/*--------------Setters--------------*/
void						Channel::setName(std::string name) {this->_name = name;}
void						Channel::setTopic(std::string topic) {this->_topic = topic;}
void						Channel::setClientasBanned(int clientFd) {this->_banned.push_back(clientFd);}
void						Channel::setClientasInvited(int clientFd) {this->_invited.push_back(clientFd);}
void						Channel::setClientasOperator(int clientFd) {this->_operators.push_back(clientFd);}

/*--------------Methods--------------*/
bool    Channel::isClientInChannel(int clientFd)
{
    bool res = std::find(this->_clients.begin(), this->_clients.end(), clientFd) != this->_clients.end();
    return (res);
}

void Channel::joinChannel(int clientFd)
{
    this->_clients.push_back(clientFd);
    std::cout << "Client " << clientFd << " join the channel #" << this->_name << std::endl;
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