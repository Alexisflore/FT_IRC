/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 09:47:59 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/14 11:44:44 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include <algorithm>

bool Server::Signal = false;

bool ChannelNameComparator::operator()(const Channel& channel) const
{
	return channel.getName() == channelName;
}

Server::Server()
{
	_socket_fd = -1;
	// std::cout << VERT << "Default constructor are called" << REINIT << std::endl;
}

Server::~Server()
{
	// _state = false;
	std::cout << ROUGE << "ircserv off" << REINIT << std::endl;
}

std::vector<std::string>	Server::split_args(std::string str)
{
	std::vector<std::string> args;
	std::istringstream iss(str);
	std::string arg;

	while (iss >> arg)
		args.push_back(arg);
	return (args);
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

Client* Server::getClient(int fd)
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i].getFd() == fd)
			return (&_clients[i]);
	}
	return (NULL);
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
	if (listen(this->_socket_fd, PENDING_QUEUE_MAXLENGTH) == -1)
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
	Client *client = getClient(fd);
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
		// buffer[recevBytes] = '\0';
		client->setBuffer(buffer);//set le buffer du client
		if (client->getBuffer().find("\n") == std::string::npos)
			return ;
		std::cout << "Client <" << fd << "> Data: " << client->getBuffer() << std::endl;
		args = getArgs(client->getBuffer().c_str());
		// for (size_t i = 0; i < args.size(); i++)
		// 	std::cout << "Args[" << i << "]: " << args[i] << std::endl;
		for (size_t i = 0; i < args.size(); i++)
			treatData(args[i], fd);
		if (getClient(fd) == NULL)
			return ;
		client->clearBuffer();
	}
}

std::vector<std::string>	Server::getArgs(std::string buffer)
{
	std::vector<std::string> args;
	std::istringstream iss(buffer);
	std::string str;

	while (std::getline(iss, str))
	{
		size_t pos = str.find("\n");
		if (pos != std::string::npos)
			str = str.substr(0, pos);
		args.push_back(str);
	}
	return (args);
}

// std::vector<std::string>	Server::getArgs(char* buffer)
// {
// 	std::vector<std::string> args;
// 	std::istringstream iss(buffer);
// 	std::string str;

// 	while (iss >> str)
// 	{
// 		size_t pos = str.find("\n");
// 		if (pos != std::string::npos)
// 			str = str.substr(0, pos);
// 		args.push_back(str);
// 	}
	
// 	return (args);
// }


void	Server::treatData(std::string arg, int fd)
{
	std::string command[] = {
		"JOIN", "QUIT", "NAMES", "BONG", "PART", "KICK", "TOPIC", "PRIVMSG",
		"INVITE", "MODE", "NICK", "USER"};

	void	(Server::*commandFunc[12])(int, std::vector<std::string>) = {
		&Server::processJoin, &Server::processQuit, &Server::processNames, &Server::processBong,
		&Server::processPass, &Server::processKick, &Server::processTopic, &Server::processPrivmsg,
		&Server::processInvite, &Server::processMode, &Server::processNick, &Server::processUser};

	std::vector <std::string> args = split_args(arg);
	for (size_t i = 0; i < 12; i++)
	{
		if (strcmp(args[0].c_str(), command[i].c_str()) == 0)
		{
			(this->*commandFunc[i])(fd, args);
			return;
		}
	}
	std::cout << "Command " << args[0] << " not found." << std::endl;
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
				if (_fds[i].fd ==  _socket_fd)
					newClient();//accepter un client
				else
					newDataClient(_fds[i].fd);//recevoir une data du client
			}
		}
	}
	closeFds();//close tout les fds a l'arret du server
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