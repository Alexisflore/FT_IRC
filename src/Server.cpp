/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kali <kali@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 09:47:59 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/28 05:10:15 by kali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include <algorithm>

bool Server::Signal = false;

bool ChannelNameComparator::operator()(const Channel& channel) const
{
	return channel.getName() == channelName;
}

bool NicknameComparator::operator()(const Client& client) const
{
	return client.getNickname() == this->nickname;
}

Server::Server()
{
	_socket_fd = -1;
	std::string datetime;
	time_t now = time(0);
	tm *ltm = localtime(&now);
	std::stringstream ss;
	ss << ltm->tm_mday << "/" << (ltm->tm_mon + 1) << "/" << (ltm->tm_year + 1900) << " " << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec;
	datetime = ss.str();
	_creationTime = datetime;
	// std::cout << VERT << "Default constructor are called" << REINIT << std::endl;
}

Server::~Server()
{
	// _state = false;
	std::string msg;
	msg = "ERROR :Server is shutting down\n";
	for (size_t i = 0; i < _clients.size(); i++)
		send(_clients[i].getFd(), msg.c_str(), strlen(msg.c_str()), 0);
	closeFds();
	std::cout << ROUGE << "ircserv off" << REINIT << std::endl;
}

std::vector<std::string>	Server::split_args(std::string s, std::string delimiter)
{

	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	std::string token;
	std::vector<std::string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
		token = s.substr (pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back (token);
	}

	res.push_back (s.substr (pos_start));
    return res;
}

void Server::securArg(const char *port, const char *pass)
{
	int			i = 0;
	std::string	sPort = port;
	std::string	sPass = pass;

	if (sPort.empty() || sPass.empty())
		throw std::invalid_argument("The arguments are empty");
	while (port[i])
	{
		if (!isdigit(port[i]))
			throw std::invalid_argument("The connection port needs just digit");
		i++;
	}

	long int	tmp = atol(sPort.c_str());
	if (tmp > INT_MAX)
		throw std::overflow_error("Alert: Int overflow");
	else if (tmp < 0)
			throw std::invalid_argument("The connection port can't take a negative value");
	else if (tmp == 0)
		throw std::invalid_argument("The connection port can't take zero for value");
	this->_port = tmp;
	this->_pass = sPass;
}
// Server::Server(const Server &other)
// {

// }

void	Server::clearClient(int fd)
{
	//supprimer le client du vecteur des fds
	for (size_t i = 0; i < _fds.size(); i++)
	{
		if (_fds[i].fd == fd)
		{
			_fds.erase(_fds.begin() + i);
			break ;
		}
	}
	//supprimer le client du vecteur des clients
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i].getFd() == fd)
		{
			_clients.erase(_clients.begin() + i);
			break ;
		}
	}
}

Client	&Server::getClientbyRef(int fd)
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i].getFd() == fd)
			return (_clients[i]);
	}
	throw std::runtime_error("The client doesn't exist.");
}

Client *Server::getClient(int fd)
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i].getFd() == fd)
			return (&this->_clients[i]);
	}
	return (NULL);
}

Client Channel::getClient(int fd)
{
	for (std::vector<std::pair<Client, char> >::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->first.getFd() == fd)
			return (it->first);
	}
	// return nothing if the client is not found
	return (Client());
}

Channel&	Server::getChannelbyName(std::string name, int fd)
{
	std::vector<Channel>::iterator channelIt = std::find_if(this->_channels.begin(), this->_channels.end(), ChannelNameComparator(name));
	if (channelIt == this->_channels.end())
	{
		std::string msg = ERR_NOSUCHCHANNEL(getClient(fd)->getNickname(), name).c_str();
		send(fd, msg.c_str(), strlen(msg.c_str()), 0);
		std::cout << "Client " << fd << " is trying to join a channel that doesn't exist" << std::endl;
		throw std::runtime_error("The channel doesn't exist.");
		return *channelIt;
	}
	return *channelIt;
}

void		Server::signalHandler(int signum)
{
	(void)signum;
	std::cout << std::endl << "Signal received !" << std::endl;
	Server::Signal = true;
}

void			Server::closeFds()
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		std::cout << "Client <" << _clients[i].getFd() << "> Disconnected" << std::endl;
		close(_clients[i].getFd());
	}
	if (_socket_fd != -1)
	{
		std::cout << "Server <" << _socket_fd << "> Disconnected" << std::endl;
		close(_socket_fd);
	}
}

void			Server::creatSocket()
{
	struct pollfd newPoll;

	this->_socket_add.sin_family = AF_INET;//definir la famille d'adresse ipv4
	this->_socket_add.sin_port = htons(this->_port);//convertit un u_short(port) de l’hôte en ordre d’octet réseau TCP/IP
	this->_socket_add.sin_addr.s_addr = INADDR_ANY;
	// this->_socket_add.sin_addr.s_addr = inet_addr("127.0.0.1");

	this->_socket_fd = socket(AF_INET, SOCK_STREAM, 0);//creation du socket server

	if (this->_socket_fd == -1)
		throw std::runtime_error("(SERVER) failed to create socket");

	int en = 1;// la valeur de SO_REUSEADDR de setsocket()
	if (setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1)
		throw std::runtime_error("(SERVER) failed to set option on socket");
	if (fcntl(_socket_fd, F_SETFL, O_NONBLOCK) == -1)// definir l'option NonBlaquant pour les sockets
		throw std::runtime_error("(SERVER) failed to set option (O_NONBLOCK) on socket");

	// configuration et liaisons
	int	socketAdresslenght = sizeof(this->_socket_add);
	int	bind_adress = bind(this->_socket_fd, (struct sockaddr*) &this->_socket_add, socketAdresslenght);

	if (bind_adress == -1)
		throw std::runtime_error("(SERVER) failed to bind socket");
	if (listen(this->_socket_fd, SOMAXCONN) == -1)
		throw std::runtime_error("(SERVER) failed to listen");

	newPoll.fd = _socket_fd;// ajouter le socket du server au pollfd
	newPoll.events = POLLIN;//evenements attendus (entree)
	newPoll.revents = 0;//evenements detectes (sortie)

	_fds.push_back(newPoll);// ajouter le socket du server au pollfd
}

void	Server::newClient()
{
	Client				client;
	struct pollfd		newPoll;
	struct sockaddr_in	cliaddr;
	socklen_t			sock_len = sizeof(cliaddr);

	int	connectSockFD = accept(_socket_fd, (struct sockaddr*) &cliaddr,
	(socklen_t *) &sock_len);

	if (connectSockFD == -1)
	{
		std::cout << "(SERVEUR) accept() failed" << std::endl;
		return ;
	}
	if (fcntl(connectSockFD, F_SETFL, O_NONBLOCK) == -1)// definir l'option NonBlaquant pour le socket
	{
		std::cout << ("(SERVER) fcntl()failed") << std::endl;
		return ;
	}
	newPoll.fd = connectSockFD;// ajouter le socket du client au pollfd
	newPoll.events = POLLIN;//evenements attendus (entree / lecture)
	newPoll.revents = 0;//evenements detectes (sortie)

	client.setFd(connectSockFD);//set le fd au client
	client.setIpAdd(inet_ntoa(cliaddr.sin_addr));// convertire l'addr ip en string
	_clients.push_back(client);// ajouter le client au vecteur des clients
	_fds.push_back(newPoll);// ajouter le socket du client au pollfd

	std::cout << VERT << "Client Connected" << REINIT << std::endl;
}

void	Server::newDataClient(int fd)
{
	char buffer[BUFFER_SIZE];//buffer pour recevoir la data
	Client &client = getClientbyRef(fd);
	std::vector<std::string> args;
	memset(buffer, 0, BUFFER_SIZE);
	ssize_t	recevBytes = recv(fd, buffer, (BUFFER_SIZE - 1), 0);//recevoir la data
	if (recevBytes <= 0)// si le client est deconnecte ou y a une erreur
	{
		std::cout << "(SERVEUR) Failure to receive message from the client" << std::endl;
		clearClient(fd);//supprimer le client
		close(fd);//close le fd du client
	}
	else
	{
		std::string stdbuffer = buffer;
		client.setBuffer(stdbuffer);//set le buffer du client
		if (client.getBuffer().find_first_of("\r\n") == std::string::npos)
			return ;
		args = getArgs(client.getBuffer().c_str());
		for (size_t i = 0; i < args.size(); i++)
		{
			std::cout << "CMD[" << i << "]: " << args[i] << std::endl;
			treatData(args[i], fd);
		}
		if (getClient(fd) == NULL)
			return ;
		client.clearBuffer();
	}
}

std::vector<std::string>	Server::getArgs(std::string buffer)
{
	std::vector<std::string> args;
	std::istringstream iss(buffer);
	std::string str;

	while (std::getline(iss, str))
	{
		size_t pos = str.find("\r");

		if (pos != std::string::npos)
			str = str.substr(0, pos);
		// str += "\0";
		args.push_back(str);
	}
	return (args);
}

void	Server::treatData(std::string arg, int fd)
{
	std::string command[] = {
		"JOIN", "QUIT", "NAMES", "BONG", "PART", "KICK", "TOPIC", "PRIVMSG",
		"INVITE", "MODE", "NICK", "USER", "PASS", "CAP", "PING", "WHO", "OPER"};

	void	(Server::*commandFunc[17])(int, std::string) = {
		&Server::processJoin, &Server::processQuit, &Server::processNames, &Server::processBong,
		&Server::processPart, &Server::processKick, &Server::processTopic, &Server::processPrivmsg,
		&Server::processInvite, &Server::processMode, &Server::processNick, &Server::processUser,
		&Server::processPass, &Server::processCap, &Server::processPing, &Server::processWho, &Server::processOper};

	std::string cmd;
	arg.find(" ") != std::string::npos ? cmd = arg.substr(0, arg.find(" ")) : cmd = arg;
	for (size_t i = 0; i < 17; i++)
	{
		if (strcmp(cmd.c_str(), command[i].c_str()) == 0)
		{
			std::cout << "Command " << cmd << std::endl;
			try{(this->*commandFunc[i])(fd, arg);
			return;}
			catch (std::exception &e)
			{
				std::cout << e.what() << std::endl;
				return ;
			}
		}
	}
	std::cout << "Command " << cmd << " not found." << std::endl;
	std::string msg = ERR_UNKNOWNCOMMAND(getClient(fd)->getNickname(), cmd).c_str();
	send(fd, msg.c_str(), strlen(msg.c_str()), 0);
}

void	Server::initServer(char *port, char *pass)
{
	securArg(port, pass);//check error port and pass
	creatSocket();//creation du socket server

	std::cout << std::endl;
	std::cout << "--------------- Server IRC ---------------" << std::endl;
	std::cout << VERT << "Server <" << _socket_fd << "> connected" << REINIT << std::endl;
	std::cout << "Waiting to accept a connection..." << std::endl;

	while (Server::Signal == false)// le serveur est en marche si pas de signal
	{
		if ((poll(&_fds[0], _fds.size(), -1) == -1) && Server::Signal == false)// surveille si y a un fd qui est pret a etre lu
			throw std::runtime_error("(SERVER) poll() failed");

		for (size_t i = 0; i < _fds.size(); i++)//verifier tout les files descripteurs
		{
			if (_fds[i].revents & POLLIN)
			{
				if (_fds[i].fd == _socket_fd)
					newClient();//accepter un client
				else
				{
					// std::cout << "Client <" << _fds[i].fd << "> Data received" << std::endl;
					newDataClient(_fds[i].fd);//recevoir une data du client
				}
			}
		}
	}

	// closeFds();//close tout les fds a l'arret du server
}

int	Server::isNameInChannel(const std::string& channelName, const std::string& clientName)
{
	std::vector<Channel>::iterator channelIt = std::find_if(this->_channels.begin(), this->_channels.end(), ChannelNameComparator(channelName));
	if (channelIt != this->_channels.end())
	{
		for (size_t i = 0; i < this->_clients.size(); i++)
		{
			Client& client = this->_clients[i];
			if (client.getUsername() == clientName && channelIt->isClientInChannel(client.getFd()))
				return (client.getFd());
		}
	}
	return 0;
}

int		Server::findFdByName(std::string clientName)
{
	for (size_t i = 0; i < this->_clients.size(); i++)
	{
		if (this->_clients[i].getUsername() == clientName)
			return (this->_clients[i].getFd());
	}
	return (0);
}
