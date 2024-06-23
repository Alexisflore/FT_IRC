/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 14:42:47 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/18 18:05:36 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void	Server::processQuit(int fd, std::string arg)
{
	std::vector<std::string> args = split_args(arg, " ");
	std::string quitMessage;

	if (args.size() > 1)
	{
		quitMessage = args[1];
		for (size_t i = 2; i < args.size(); i++)
		{
			quitMessage += " " + args[i];
		}
	}
	else 
	{
		for (size_t i = 0; i < this->_clients.size(); i++)
		{
			if (_clients[i].getFd() == fd)
			{
				quitMessage = _clients[i].getNickname();
			}
			break;
		}
	}

	std::cout << "Client " << fd << " has quit: " << quitMessage << std::endl;

	closeConnection(fd, quitMessage);
}

void	Server::closeConnection(int fd, std::string quitMessage)
{
	std::stringstream ss;
	ss << fd;
	std::string fdStr = ss.str();

	std::string disconectMessage = "Client " + fdStr + " has quit: " + quitMessage;
	for (size_t i = 0; i < this->_clients.size(); i++)
	{
		if (_clients[i].getFd() != fd)
			send(_clients[i].getFd(), disconectMessage.c_str(), disconectMessage.size(), 0);
	}
	
	for (size_t i = 0; i < _channels.size(); i++)
	{
		_channels[i].leaveChannel(fd);
	}

	close(fd);

	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i].getFd() == fd)
		{
			_clients.erase(_clients.begin() + i);
			break;
		}
	}
	
	for (size_t i = 0; i < _fds.size(); i++)
	{
		if (_fds[i].fd == fd)
		{
			_fds.erase(_fds.begin() + i);
			break;
		}
	}
	
}
