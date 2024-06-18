/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 09:48:20 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/18 18:02:20 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Irc.hpp"
#include "Channel.hpp"
#include "Client.hpp"

class Client;

class Channel;

struct ChannelNameComparator
{
	std::string channelName;

	ChannelNameComparator(const std::string& name) : channelName(name) {}

	bool operator()(const Channel& channel) const;
};

struct NicknameComparator
{
	std::string nickname;

	NicknameComparator(const std::string& name) : nickname(name) {}

	bool operator()(const Client& client) const;
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
		/*--------------Getters--------------*/
		std::vector<std::string>	getArgs(std::string str);
		Client*						getClient(int fd);
		Channel&					getChannelbyName(std::string name, std::string clientName);
		std::vector<Channel>		getChannels(std::vector< std::string > channelNames);
		Client*						getClientbyNickname(std::string nickname);

		/*--------------Setters--------------*/


		/*--------------Methods--------------*/
		void						securArg(const char *port, const char *pass);
		void						creatSocket();
		void						initServer(char *port, char *pass);
		void						newClient();
		void						newDataClient(int fd);
		static void					signalHandler(int signum);
		void						treatData(std::string, int fd);
		void						closeFds();
		void						clearClient(int fd);
		int							isNameInChannel(const std::string& channelName, const std::string& clientName);
		std::vector<std::string>	split_args(std::string str, std::string delimiter);
		void 						displayTopic(int fd, Channel& channel);
		void 						changeTopic(int fd, Channel& channel, std::string topic);
		void 						displayMode(int fd, Channel& channel);
		void 						changeMode(int fd, Channel& channel);
		void 						createValue(std::string& mode, bool& value);
		bool						isNicknameUsed(std::string nickname);
		int							findFdByName(std::string clientName);
		std::string					findChannel(std::string string, std::string cmd);

		/*--------------PROCESS--------------*/
		void						processJoin(int fd, std::string);
		void						processQuit(int fd, std::string);
		void						processNames(int fd, std::string);
		void						processPass(int fd, std::string);
		void						processBong(int fd, std::string);
		void						processKick(int fd, std::string);
		void						processTopic(int fd, std::string);
		void						processPrivmsg(int fd, std::string);
		void						processInvite(int fd, std::string);
		void						processMode(int fd, std::string);
		void						processNick(int fd, std::string);
		void						processUser(int fd, std::string);
		void						processPart(int fd, std::string);
		void						processCap(int fd, std::string);
		void						processPing(int fd, std::string);
	private:
		int							_port;
		int							_socket_fd;
		static bool					_signal;
		std::vector<Client>			_clients;
		std::vector<Channel>		_channels;
		std::vector<struct pollfd>	_fds;
		std::string					_pass;
		struct sockaddr_in			_socket_add;
};

#endif
