/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 09:48:14 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/24 11:10:44 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "mode.hpp"

class MODE;

class Client
{
	public:
		Client();
		Client(int fd);
		Client(int fd, std::string nickname, std::string password, std::string ipadd);
		// Client(const Client &other);
		~Client();
		Client(const Client &other);
		Client &operator=(const Client &other);
		bool operator<(const Client& other) const;

		//---------Getters------------
		int						getFd() const;
		std::string				getNickname() const;
		std::string				getRealName();
		std::string				getPassword();
		// bool					getIsOperator();
		std::string				getIpAdd();
		std::string				getUsername();
		std::string				getBuffer();
		bool					isLogged();
		void					setLogged(bool logged);
		bool					isRegistered();
		void					displayMode(int fd, std::string nickname);
		MODE					getMode();

		//---------Setters------------
		void		setFd(int fd);
		void		setBuffer(std::string buffer);
		void		setNickname(std::string nickname);
		void		setPassword(std::string password);
		void		setIpAdd(std::string ip_add);
		void		setUsername(std::string username);
		void		setRealName(std::string realname);
		void		setMode(t_mode* mode);
		void 		setModeByType(int fd, char mode, char value);

		//---------Methods------------
		void		sendMessage(const std::string message);
		void		addInvolvedChannel(std::string channelName);
		void		removeInvolvedChannel(std::string channelName);
		void		clearBuffer();
		void		welcomeMessage();
		void		processMode(int fd, t_mode mode, int size_of_cmd);
	private:
		int							_fd;
		std::string					_username;
		std::string					_realname;
		std::string					_nickname;
		std::string					_password;
		std::string					_ip_add;
		bool						_registered;
		bool						_logged;
		std::string					_buffer;
		std::vector<std::string>	_channelInvolved;
		MODE						_mode;
		// struct sockaddr_in	_cliaddr;
};

