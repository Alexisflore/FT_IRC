/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:09:03 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/13 17:25:30 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void Server::processNick(int fd, std::vector <std::string> string)
{
		(void)string;
	std::string		msg = "NICK\n";

	send(fd, msg.c_str(), msg.length(), 0);
}