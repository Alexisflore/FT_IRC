/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:08:54 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/13 17:25:23 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void Server::processKick(int fd, std::vector <std::string> string)
{
	(void)string;
	std::string		msg = "KICK\n";

	send(fd, msg.c_str(), msg.length(), 0);
}