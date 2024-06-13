/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bong.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:19:00 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/13 17:25:07 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void			Server::processBong(int fd, std::vector<std::string>)
{
	std::string		msg = "PONG\n";

	send(fd, msg.c_str(), msg.length(), 0);
}
