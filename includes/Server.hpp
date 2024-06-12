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
		void			mewDataClient(int fd);
		static void		signalHandler(int signum);
		void			treatData(std::vector<std::string>, int fd);
		void			closeFds();
		void			clearClient(int fd);
		void			processJoin(int fd, std::vector<std::string>);
		void			processQuit(int fd, std::vector<std::string>);
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