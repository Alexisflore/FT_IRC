/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 09:48:28 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/13 15:23:30 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

Client::Client()
{
	_is_admin = false;
	// std::cout << VERT << "Default constructor are called" << REINIT << std::endl;
}

Client::Client(int fd, std::string nickname, std::string password, std::string ipadd) :
		_fd(fd),
		_nickname(nickname),
		_password(password),
		_ip_add(ipadd),
		_is_admin(false)
{
	// std::cout << VERT << "Constructor are called" << REINIT << std::endl;
};

Client::~Client()
{
}

// Client &Client::operator=(const Client &other)
// {
// }

int	Client::getFd()
{
	return (this->_fd);
}

void	Client::setFd(int fd)
{
	this->_fd = fd;
}

void	Client::setIpAdd(std::string ipadd)
{
	this->_ip_add = ipadd;
}

