/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 14:42:47 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/26 12:53:37 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void	Server::processQuit(int fd, std::string arg)
{
	std::vector<std::string> args = split_args(arg, " ");
	std::string quitMessage;
	std::string userid = USER_ID(getClient(fd)->getNickname(), getClient(fd)->getUsername());
	std::string msg;
	if (args.size() > 1)
	{
		quitMessage += args[1];
		for (size_t i = 2; i < args.size(); i++)
			quitMessage += " " + args[i];
		quitMessage += "\n";
	}
	else
		quitMessage = ":Quit\n";
	msg = RPL_QUIT(userid, quitMessage).c_str();
	for (size_t i = 0; i < _clients.size(); i++)
		send(fd, msg.c_str(), strlen(msg.c_str()), 0);
	closeConnection(fd);
}

void	Server::closeConnection(int fd)
{
	std::stringstream ss;
	ss << fd;
	std::string fdStr = ss.str();
	for (size_t i = 0; i < _channels.size(); i++)
		_channels[i].leaveChannel(fd);
	close(fd);
	clearClient(fd);
	std::cout << "Client " << fd << " has quit " << std::endl;
}
