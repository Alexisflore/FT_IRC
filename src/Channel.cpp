/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 09:48:25 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/13 13:15:11 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"

Channel::Channel(std::string channelName) : name(channelName)
{
}

Channel::Channel() : name(NULL)
{
}

Channel::~Channel()
{
}

// Channel(const Channel &other)
// {

// }

// Channel &operator=(const Channel &other)
// {

// }

std::string     Channel::getName() const
{
    return this->name;
}

bool    Channel::isClientInChannel(int clientFd)
{
    bool res = std::find(this->clients.begin(), this->clients.end(), clientFd) != this->clients.end();
    return (res);
}

void Channel::joinChannel(int clientFd)
{
    this->clients.push_back(clientFd);
    std::cout << "Client " << clientFd << " join the channel #" << this->name << std::endl;
}

void Channel::leaveChannel(int clientFd)
{
    for (std::vector<int>::iterator it = clients.begin(); it != clients.end(); it++)
    {
        if (*it == clientFd)
        {
            clients.erase(it);
            std::cout << "Client " << clientFd << " has left channel #" << this->name << std::endl;
            return ;
        }
    }
}

void Channel::sendMessage(const std::string message)
{
    for (std::vector<int>::iterator it = clients.begin(); it != clients.end(); it++)
    {
        if (send(*it, message.c_str(), message.size(), 0) == -1)
            throw SendException();
        std::cout << "Message " << message << " send to client " << *it << " in channel #" << this->name << std::endl;
    }
}

std::vector<int>    Channel::getClients() const
{
	return this->clients;
}