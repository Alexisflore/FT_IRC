/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 09:48:28 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/18 16:19:51 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

Client::Client()
{
	// std::cout << VERT << "Default constructor are called" << REINIT << std::endl;
	_logged = false;
}

Client::~Client(){}

Client::Client(int fd, std::string nickname, std::string password, std::string ipadd) :
		_fd(fd),
		_nickname(nickname),
		_password(password),
		_ip_add(ipadd),
		_logged(false)
{
	// std::cout << VERT << "Constructor are called" << REINIT << std::endl;
};

Client::Client(const Client &other)
{
	_fd = other._fd;
	_nickname = other._nickname;
	_password = other._password;
	_ip_add = other._ip_add;
	_logged = other._logged;
}

Client::Client(int fd)
{
	_fd = fd;
}

Client 		&Client::operator=(const Client &other)
{
	if (this != &other)
	{
		this->_fd = other._fd;
		this->_nickname = other._nickname;
		this->_password = other._password;
		this->_ip_add = other._ip_add;
	}
	return (*this);
}

//----------------Getters------------
int			Client::getFd() {return (this->_fd);}
std::string	Client::getNickname() const {return (this->_nickname);}
std::string Client::getBuffer() {return (this->_buffer);}
std::string	Client::getIpAdd() {return (this->_ip_add);}
std::string	Client::getUsername() {return (this->_username);}
std::string	Client::getPassword() {return (this->_password);}
bool 		Client::isLogged() {return (this->_logged);}
bool 		Client::isRegistered() {return (this->_registered);}

//----------------Setters------------
void		Client::setFd(int fd) {this->_fd = fd;}
void		Client::setBuffer(std::string buffer) {this->_buffer += buffer;}
void		Client::setIpAdd(std::string ipadd) {this->_ip_add = ipadd;}
void		Client::setNickname(std::string nickname) {this->_nickname = nickname;}
void		Client::setPassword(std::string password) {this->_password = password;}
void		Client::setUsername(std::string username) {this->_username = username;}
void		Client::setLogged(bool logged) {this->_logged = logged;}

//----------------Methods------------
void		Client::clearBuffer() {_buffer.clear();}
void		Client::sendMessage(const std::string message)
{std::cout << "Message sent: " << message << std::endl;}

void		Client::addInvolvedChannel(std::string channelName)
{
	_channelInvolved.push_back(channelName);
}

void		Client::removeInvolvedChannel(std::string channelName)
{
	for (size_t i = 0; i < _channelInvolved.size(); i++)
	{
		if (_channelInvolved[i] == channelName)
		{
			_channelInvolved.erase(_channelInvolved.begin() + i);
			break ;
		}
	}
}
