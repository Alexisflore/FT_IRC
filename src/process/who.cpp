/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 17:39:13 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/27 18:18:31 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include "../../includes/Channel.hpp"

void Server::processWho(int fd, std::string arg)
{
	if (getClient(fd)->isLogged() == false)
	{
		std::string msg = ERR_NOTREGISTERED(getClient(fd)->getNickname(), "WHO").c_str();
		send(fd, msg.c_str(), msg.length(), 0);
		return ;
	}
	std::vector<std::string> args = split_args(arg, " ");
	if (args.size() < 2)
	{
		std::string msg = ERR_NEEDMOREPARAMS(getClient(fd)->getNickname(), "WHO").c_str();
		send(fd, msg.c_str(), strlen(msg.c_str()), 0);
		std::cout << "Client " << fd << " needs to specify the channel name" << std::endl;
		return ;
	}
	if (args[1][0] == '#' || args[1][0] == '&')
	{
		Channel &channel = getChannelbyName(args[1], fd);
		if (!channel.isClientInChannel(fd))
		{
			std::string msg = ERR_NOTONCHANNEL(getClient(fd)->getNickname(), channel.getName()).c_str();
			std::cout << "Client " << fd << " isn t in the channel " << channel.getName() << std::endl;
			send(fd, msg.c_str(), strlen(msg.c_str()), 0);
			return ;
		}
		displayWho(fd, channel);
	}
	else
	{
		std::string msg = ERR_NOSUCHCHANNEL(getClient(fd)->getNickname(), args[1]).c_str();
		send(fd, msg.c_str(), strlen(msg.c_str()), 0);
	}
}

void Server::displayWho(int fd, Channel &channel)
{
	std::string msg;
	std::string user;
	std::vector<std::pair <Client, char> > clients = channel.getClients();
	#include "../../includes/Client.hpp" // Include the header file for the "Client" class

	for (std::vector<std::pair<Client, char> >::iterator it = clients.begin(); it != clients.end(); it++)
	{
		user.clear();
		user = RPL_WHOREPLY(getClient(fd)->getNickname(), channel.getName(), it->first.getNickname(), it->first.getUsername(), it->first.getIpAdd(), "H", "0", it->first.getRealName()).c_str();
		msg = user.c_str();
		send(fd, msg.c_str(), msg.length(), 0);
	}
	msg = RPL_ENDOFWHO(getClient(fd)->getNickname(), channel.getName()).c_str();
	send(fd, msg.c_str(), msg.length(), 0);
}