/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:09:06 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/17 17:05:30 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void Server::processPart(int fd, std::string string)
{
		(void)string;
	std::string		msg = "PART\n";

	send(fd, msg.c_str(), msg.length(), 0);
}
