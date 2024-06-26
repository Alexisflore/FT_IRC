/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:08:47 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/26 17:10:20 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void Server::processInvite(int fd, std::string string)
{
	if (getClient(fd)->isLogged() == false)
	{
		std::string msg = ERR_NOTREGISTERED(getClient(fd)->getNickname(), "INVITE").c_str();
		send(fd, msg.c_str(), msg.length(), 0);
		return ;
	}
	std::vector<std::string> args = split_args(string, " ");
	if (args.size() != 3)
	{
		std::string msg = ERR_NEEDMOREPARAMS(getClient(fd)->getNickname(), "INVITE").c_str();
		send(fd, msg.c_str(), strlen(msg.c_str()), 0);
		std::cout << "Client " << fd << " needs to specify the channel name and the client name" << std::endl;
		return ;
	}
	std::string channelName = args[2];
	std::string clientName = args[1];
	Channel &channel = getChannelbyName(channelName, getClient(fd)->getNickname());
	if (!channel.isClientInChannel(fd))
	{
		std::string msg = ERR_NOTONCHANNEL(getClient(fd)->getNickname(), channel.getName()).c_str();
		std::cout << "Client " << fd << " isn t in the channel " << channel.getName() << std::endl;
		send(fd, msg.c_str(), strlen(msg.c_str()), 0);
		return ;
	}
	if (channel.isInviteOnly() && channel.isClientOperator(fd) == false)
	{
		std::string msg = ERR_CHANOPRIVSNEEDED(getClient(fd)->getNickname(), channel.getName()).c_str();
		std::cout << "Client " << fd << " isn t operator in the channel " << channel.getName() << std::endl;
		send(fd, msg.c_str(), strlen(msg.c_str()), 0);
		return ;

	}
	Client *client = getClientbyNickname(clientName);
	if (client == NULL)
	{
		std::string msg = ERR_NOSUCHNICK(getClient(fd)->getNickname(), clientName).c_str();
		std::cout << "Client " << fd << " isn t in the channel " << channel.getName() << std::endl;
		send(fd, msg.c_str(), strlen(msg.c_str()), 0);
		return ;
	}
	if (!channel.isClientOperator(fd))
	{
		std::string msg = ERR_CHANOPRIVSNEEDED(getClient(fd)->getNickname(), channel.getName()).c_str();
		std::cout << "Client " << fd << " isn t operator in the channel " << channel.getName() << std::endl;
		send(fd, msg.c_str(), strlen(msg.c_str()), 0);
		return ;
	}
	if (channel.isClientInChannel(client->getFd()) && channel.isClientInvited(client->getFd()) == false)
	{
		std::string msg = ERR_USERONCHANNEL(getClient(fd)->getNickname(), client->getNickname(), channel.getName()).c_str();
		std::cout << "Client " << fd << " is already in the channel " << channel.getName() << std::endl;
		send(fd, msg.c_str(), strlen(msg.c_str()), 0);
		return ;
	}
	if (channel.isClientInvited(client->getFd()))
	{
		return ;
	}
	channel.joinChannel(*client);
	channel.setClientasInvited(client->getFd());
	std::string localhost = "localhost";
	std::string userid = USER_ID(getClient(fd)->getNickname(), getClient(fd)->getUsername());
	std::string msg = RPL_INVITING(localhost, getClient(fd)->getNickname(), client->getNickname(), channel.getName()).c_str();
	send(fd, msg.c_str(), strlen(msg.c_str()), 0);
	msg = userid + " INVITE " + client->getNickname() + " :" + channel.getName() + "\n";
	send(client->getFd(), msg.c_str(), strlen(msg.c_str()), 0);
	std::cout << "Client " << fd << " has invited " << client->getNickname() << " to the channel " << channel.getName() << std::endl;
}

void Server::setClientasInvited(int fd, Channel *channel)
{
	channel->setClientasInvited(fd);
}
