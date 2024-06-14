/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 09:48:17 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/14 13:05:43 by alfloren         ###   ########.fr       */
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

#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Utils.hpp"
#include "define.hpp"

/* +++ Global +++ */
extern bool _state;

#endif