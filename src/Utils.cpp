/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 09:48:35 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/18 09:38:31 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Utils.hpp"
#include "../includes/Channel.hpp"
#include "../includes/Server.hpp"

bool    compareChannelName(Channel channel, std::string channelName)
{
    return (channelName == channel.getName());
}

std::string deleteSpaces(std::string str)
{
	std::string::iterator it = str.begin();
	while (*it == ' ')
	{
		str.erase(it);
		it++;
	}
	it = str.end();
	while (*it == ' ')
	{
		str.erase(it);
		it--;
	}
	return str;
}
