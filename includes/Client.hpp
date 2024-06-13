/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 09:48:14 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/13 09:48:48 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Irc.hpp"

class Client
{
	public:
		Client();
		// Client(const Client &other);
		~Client();
		// Client &operator=(const Client &other);

		int			getFd();
		void		setFd(int fd);
		void		setIpAdd(std::string ipadd);

	private:
		int					_fd;
		std::string			_ip_add;
		// struct sockaddr_in	_cliaddr;
};

#endif