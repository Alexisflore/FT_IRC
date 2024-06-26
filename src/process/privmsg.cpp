/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:09:11 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/26 11:32:22 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void Server::processPrivmsg(int fd, std::string string)
{
	std::string msg;
	if (getClient(fd)->isLogged() == false)
	{
		msg = ERR_NOTREGISTERED(getClient(fd)->getNickname(), "PRIVMSG").c_str();
		send(fd, msg.c_str(), msg.length(), 0);
		return ;
	}
	std::vector<std::string> args = split_args(string, " ");
	if (args.size() < 3)
	{
		msg = ERR_NEEDMOREPARAMS(getClient(fd)->getNickname(), "PRIVMSG").c_str();
		send(fd, msg.c_str(), msg.length(), 0);
		std::cout << "Client " << fd << " needs to specify the client name and the message" << std::endl;
		return ;
	}
	std::string user;
	std::string messageToSend;
	user += USER_ID(getClient(fd)->getNickname(), getClient(fd)->getUsername());
	user += " PRIVMSG ";
	if (args[2][0] == ':')
		messageToSend += args[2].erase(0, 1);
	else
		messageToSend += args[2];
	for (size_t i = 3; i < args.size(); i++)
		messageToSend += " " + args[i];
	messageToSend += "\n";
	std::vector<std::string> wheretoSend = split_args(args[1], ",");
	for (std::vector<std::string>::iterator it = wheretoSend.begin(); it != wheretoSend.end(); it++)
	{
		if (it->empty())
			continue ;
		if (it->at(0) != '#' && it->at(0) != '&')
			sendToClient(fd, *it, user + *it + " :" + messageToSend);
		else
			sendToChannel(fd, *it, user + *it + " :" + messageToSend);
	}
}

void Server::sendToChannel(int fd, std::string channelName, std::string message)
{
	Channel &channel = getChannelbyName(channelName, getClient(fd)->getNickname());
	if (!channel.isClientInChannel(fd))
	{
		std::string msg = ERR_NOTONCHANNEL(getClient(fd)->getNickname(), channelName).c_str();
		send(fd, msg.c_str(), msg.length(), 0);
		return ;
	}
	channel.sendprivmsg(fd, message);
	std::cout << "Client " << fd << " has sent " << message << "to " << channelName << std::endl;
}

void Server::sendToClient(int fd, std::string clientName, std::string message)
{
	Client *client = getClientbyNickname(clientName);
	if (client == NULL)
	{
		std::string msg = ERR_NOSUCHNICK(getClient(fd)->getNickname(), clientName).c_str();
		send(fd, msg.c_str(), msg.length(), 0);
		return ;
	}
	send(client->getFd(), message.c_str(), message.length(), 0);
		std::cout << "Client " << fd << " has sent " << message << "to " << clientName << std::endl;
}

// void	Server::processPrivmsg(int fd, std::string str)
// {
// 	std::vector<std::string> args = split_args(str, " ");

// 	if (args.size() < 3)
// 	{
// 		std::string error = ERR_NOTEXTTOSEND(getClient(fd)->getNickname());
// 		send(fd, error.c_str(), error.length(), 0);
// 		return ;
// 	}

// 	std::string target = args[1];
// 	std::string msg = args[2];
// 	for (size_t i = 2; i < args.size(); i++)
// 	{
// 		msg += " " + args[i];
// 	}
	
// 	std::vector<std::string> receivers;
// 	std::istringstream iss(target);
// 	std::string receiver;
// 	while (std::getline(iss, receiver, ','))
// 	{
// 		receivers.push_back(receiver);
// 	}

// 	for (size_t i = 0; i < receivers.size(); i++)
// 	{
// 		if (receivers[i][0] == '#')
// 		{
// 			std::string channelName = receivers[i];
// 			if (this->isClientInChannel(channelName, fd))
// 			{
// 				sendToChannel(fd, channelName, msg);
// 			}
// 			else
// 			{
// 				std::string error = ERR_CANNOTSENDTOCHAN(getClient(fd)->getNickname(), channelName);
// 				send(fd, error.c_str(), error.length(), 0);
// 			}
// 		}
// 		else
// 		{
// 			bool clientFound = false;
//             for (size_t j = 0; j < _clients.size(); ++j)
// 			{
//                 if (_clients[j].getNickname() == receivers[i])
// 				{
//                     sendMessageToClient(_clients[j].getFd(), msg);
//                     clientFound = true;
//                     break;
//                 }
//             }
//             if (!clientFound)
// 			{
//                 std::string error = ERR_NOSUCHNICK(getClient(fd)->getNickname(), receivers[i]);
//                 send(fd, error.c_str(), error.length(), 0);
//             }
// 		}
// 	}
	
// }

// void	Server::sendMessageToClient(int fd, const std::string& message)
// {
//     std::string privateMessage = "Private message: " + message + "\r\n";
//     send(fd, privateMessage.c_str(), privateMessage.length(), 0);
// }


// bool	Server::isClientInChannel(const std::string& channelName, int fd)
// {
//     for (size_t i = 0; i < _channels.size(); ++i)
// 	{
//         if (_channels[i].getName() == channelName)
// 		{
//             return _channels[i].isClientInChannel(fd);
//         }
//     }
//     return false;
// }