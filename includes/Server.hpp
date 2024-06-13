/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 09:48:20 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/13 16:39:34 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Irc.hpp"
#include "Channel.hpp"

class Client;

class Channel;

struct ChannelNameComparator
{
	std::string channelName;

	ChannelNameComparator(const std::string& name) : channelName(name) {}

	bool operator()(const Channel& channel) const;
};

class Server
{
	public:
		static bool	Signal;//variable static

		Server();
		// Server(const Server &other);
		~Server();

		// Server &operator=(const Server &other);
		// void			Run();

		void			securArg(const char *port, const char *pass);
		void			creatSocket();
		void			initServer(char *port, char *pass);
		void			newClient();
		void			newDataClient(int fd);
		static void		signalHandler(int signum);
		void			treatData(std::vector<std::string>, int fd);
		void			closeFds();
		void			clearClient(int fd);
		Client*			getClient(int fd);
		void			processJoin(int fd, std::vector<std::string>);
		void			processQuit(int fd, std::vector<std::string>);
		void			processNames(int fd, std::vector<std::string>);
		void			processPass(int fd, std::vector<std::string>);
		void			processBong(int fd, std::vector<std::string>);
		void			processKick(int fd, std::vector<std::string>);
		void			processTopic(int fd, std::vector<std::string>);
		void			processPrivmsg(int fd, std::vector<std::string>);
		void			processInvite(int fd, std::vector<std::string>);
		void			processMode(int fd, std::vector<std::string>);
		void			processNick(int fd, std::vector<std::string>);
		void			processUser(int fd, std::vector<std::string>);
		int				isNameInChannel(const std::string& channelName, const std::string& clientName);
		std::vector<std::string>	getArgs(char* buffer);

	private:
		int							_port;//
		int							_socket_fd;//
		static bool					_signal;//
		std::vector<Client>			_clients;//
		std::vector<Channel>		_channels;
		std::vector<struct pollfd>	_fds;//
		std::string					_pass;
		struct sockaddr_in			_socket_add;
};

#endif