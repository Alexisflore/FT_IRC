/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:08:56 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/13 17:25:26 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void Server::processMode(int fd, std::vector <std::string> string)
{
		(void)string;
	std::string		msg = "MODE\n";

	send(fd, msg.c_str(), msg.length(), 0);
}