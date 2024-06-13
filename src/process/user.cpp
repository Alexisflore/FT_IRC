/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:09:22 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/13 17:25:45 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void Server::processUser(int fd, std::vector <std::string> string)
{
		(void)string;
	std::string		msg = "USER\n";

	send(fd, msg.c_str(), msg.length(), 0);
}