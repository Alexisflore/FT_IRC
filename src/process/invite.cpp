/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:08:47 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/13 17:24:56 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void Server::processInvite(int fd, std::vector <std::string> string)
{
	(void)string;
	std::string		msg = "INVITE\n";

	send(fd, msg.c_str(), msg.length(), 0);
}