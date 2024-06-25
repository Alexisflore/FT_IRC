/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bong.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:19:00 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/25 11:39:39 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void			Server::processBong(int fd, std::string)
{
	if (getClient(fd)->isLogged() == false)
	{
		std::string msg = ERR_NOTREGISTERED(getClient(fd)->getNickname(), "BONG").c_str();
		send(fd, msg.c_str(), msg.length(), 0);
		return ;
	}
	std::string		msg = "PONG\n";

	send(fd, msg.c_str(), msg.length(), 0);
}
