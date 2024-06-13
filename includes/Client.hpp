/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 09:48:14 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/13 15:22:27 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Irc.hpp"

class Client
{
	public:
		Client();
		Client(int fd, std::string nickname, std::string password, std::string ipadd);
		// Client(const Client &other);
		~Client();
		// Client &operator=(const Client &other);

		int			getFd();
		void		setFd(int fd);
		void		setIpAdd(std::string ipadd);

	private:
		int					_fd;
		std::string			_ip_add;
		bool			_is_admin;
		std::string		_nickname;
		std::string		_password;
		// struct sockaddr_in	_cliaddr;
};

#endif