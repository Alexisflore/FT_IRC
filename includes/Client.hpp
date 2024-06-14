/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 09:48:14 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/14 12:20:48 by alfloren         ###   ########.fr       */
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
		Client &operator=(const Client &other);
		
		//---------Getters------------
		int			getFd();
		std::string	getNickname();
		std::string	getPassword();
		bool		getIsOperator();
		std::string	getIpAdd();
		std::string	getUsername();
		std::string	getBuffer();
		bool		isLogged();
		bool		isRegistered();

		//---------Setters------------
		void		setFd(int fd);
		void		setBuffer(std::string buffer);
		void		setNickname(std::string nickname);
		void		setPassword(std::string password);
		void		setIpAdd(std::string ip_add);
		void		setUsername(std::string username);

		//---------Methods------------
		void		sendMessage(const std::string message);
		void		addInvolvedChannel(std::string channelName);
		void		removeInvolvedChannel(std::string channelName);
		void		clearBuffer();
	private:
		int							_fd;
		std::string					_username;
		std::string					_nickname;
		std::string					_password;
		std::string					_ip_add;
		bool						_registered;
		bool						_logged;
		std::string					_buffer;
		std::vector<std::string>	_channelInvolved;
		// struct sockaddr_in	_cliaddr;
};

#endif