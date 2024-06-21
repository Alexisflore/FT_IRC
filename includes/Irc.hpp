/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 09:48:17 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/20 18:16:58 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

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

#include "mode.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Utils.hpp"
#include "define.hpp"
#include "Client.hpp"

/* +++ Global +++ */
extern bool _state;

