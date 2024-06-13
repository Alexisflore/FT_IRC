/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 14:42:47 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/13 17:03:31 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void	Server::processQuit(int fd, std::vector<std::string> string)
{
	(void)string;
	std::string		msg = "PART\n";

	send(fd, msg.c_str(), msg.length(), 0);
}