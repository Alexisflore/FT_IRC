/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 09:48:17 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/20 15:56:09 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
#define IRC_HPP

#include <iostream>
#include <csignal>
#include <cstring>
#include <string>
#include <vector>
#include <netinet/in.h>
#include <sys/socket.h>
#include <exception>
#include <arpa/inet.h>
#include <limits.h>
#include <sstream>
#include <unistd.h>
#include <cstdlib> //pour atoi()
#include <poll.h>
#include <fcntl.h>//pour fnctl()
#include <algorithm>
#include <map>

typedef struct s_mode
{
	std::string					mode;
	std::vector<std::string>	params;
	int							type;
	int							client_fd;
	std::string					clientNick;
	std::string					name;
}				t_mode;


#include "Channel.hpp"
#include "Server.hpp"
#include "Utils.hpp"
#include "define.hpp"
#include "Client.hpp"
#include "mode.hpp"
/* +++ Global +++ */
extern bool _state;

#endif
