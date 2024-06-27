/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:08:56 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/27 12:06:45 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include "../../includes/Channel.hpp"
#include "../../includes/mode.hpp"
#include "../../includes/Client.hpp"

MODE::MODE()
{
	_mode.push_back(std::make_pair('i', false));
	_mode.push_back(std::make_pair('o', false));
	_mode.push_back(std::make_pair('s', false));
	_mode.push_back(std::make_pair('w', false));
	_mode.push_back(std::make_pair('k', false));
	_mode.push_back(std::make_pair('l', false));
	_mode.push_back(std::make_pair('t', false));
	_modesUser = "iosw";
	_modesChannel = "iotkl";
	_needParams = "klo";
}

MODE::~MODE() {}
MODE::MODE(const MODE &other) {*this = other;}
MODE 		&MODE::operator=(const MODE &other)
{
	_password = other._password;
	_limit = other._limit;
	_mode = other._mode;
	// _params = other._params;
	_needParams = other._needParams;
	_modesUser = other._modesUser;
	_modesChannel = other._modesChannel;
	return (*this);
}
void MODE::clearLimit() {_limit = -1;}
void MODE::clearPassword() {_password.clear();}

long long 				ft_atoll(const char *str)
{
	long long res = 0;
	int i = 0;
	if (str == NULL)
		return -1;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '-')
		return -1;
	else if (str[i] == '+')
		i++;
	for (int k = i; str[k] != '\0'; k++)
		if (isdigit(str[k]) == 0)
			return -1;
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + str[i] - '0';
		if (res > MAX_INT)
			return -1;
		i++;
	}
	return (res);
}

/*--------------Getters--------------*/
long long 				MODE::getLimit() {return _limit;}
bool					MODE::setLimit(std::string limit) {
	_limit = ft_atoll(limit.c_str());
	if (_limit == -1)
		return false;
	return true;
}
void					MODE::setPassword(std::string password) {_password = password;}
std::string				MODE::getPassword() {return _password;}
bool					MODE::getModeValue(char mode) {
	for (std::vector<std::pair<char, bool> >::iterator it = _mode.begin(); it != _mode.end(); it++)
	{
		if (it->first == mode)
			return it->second;
	}
	return false;
}

// std::string				MODE::getParams(char mode) {
// 	for (std::vector<std::pair<char, std::string> >::iterator it = _params.begin(); it != _params.end(); it++)
// 	{
// 		if (it->first == mode)
// 			return it->second;
// 	}
// 	return "";
// }

std::vector<std::pair<char, bool> >	MODE::getMode() {return _mode;}
std::string				MODE::getParamsNeeded(int Type) {return (Type == CHANNEL_MODE) ? _needParams : "";}
std::string				MODE::getAuthorizedMode(int type)
{
	if (type == CHANNEL_MODE)
		return _modesChannel;
	return _modesUser;
}

void	MODE::isModeAuthorized(char mode, t_mode* modes)
{
	std::string msg = ERR_UNKNOWNMODE(modes->clientNick, mode).c_str();
	if (getAuthorizedMode(modes->type).find(mode) == std::string::npos)
	{
		send(modes->client_fd, msg.c_str(), strlen(msg.c_str()), 0);
		throw std::invalid_argument("The mode doesn't exist.");
	}
}

// std::string MODE::getModesAsString()
// {
// 	std::string modes;
// 	for (std::vector<std::pair<char, bool> >::iterator it = _mode.begin(); it != _mode.end(); it++)
// 	{
// 		if (it->second == true)
// 			modes += it->first;
// 	}
// 	return modes;
// }

void Channel::setMode(t_mode* mode)
{
	char lastOperator;
	std::string msg;
	std::string userid = USER_ID(mode->clientNick, getClient(mode->client_fd).getUsername());
	if (mode->mode[0] != '+' && mode->mode[0] != '-')
	{
		msg = ERR_UNKNOWNMODE(mode->clientNick, mode->mode + " ").c_str();
		send(mode->client_fd, msg.c_str(), strlen(msg.c_str()), 0);
		throw std::invalid_argument("The mode must start with + or -");
	}
	lastOperator = mode->mode[0];
	for (unsigned long i = 1; i < mode->mode.size(); i++)
	{
		if (mode->mode[i] == '+' || mode->mode[i] == '-')
			lastOperator = mode->mode[i];
		else if (_modes.getAuthorizedMode(CHANNEL_MODE).find(mode->mode[i]) == std::string::npos)
		{
			msg = ERR_UNKNOWNMODE(mode->clientNick, mode->mode[i] + " ").c_str();
			send(mode->client_fd, msg.c_str(), strlen(msg.c_str()), 0);
			throw std::invalid_argument("The mode doesn't exist.");
		}
		else if ((lastOperator == '+'
			&& _modes.getParamsNeeded(CHANNEL_MODE).find(mode->mode[i]) != std::string::npos)
			|| (lastOperator == '-' && mode->mode[i] == 'o'))
			{
				if (mode->params.size() == 0)
				{
					msg = ERR_NEEDMOREPARAMS(mode->clientNick, "MODE").c_str();
					send(mode->client_fd, msg.c_str(), strlen(msg.c_str()), 0);
					return ;
				}
				else
				{
					setModeByType(mode->client_fd, mode->mode[i], lastOperator, mode->params[0]);
					mode->params.erase(mode->params.begin());
					std::cout << mode->params.size() << std::endl;
				}
			}
		else
		{
			msg = "MODE " + getName() + " " + std::string(1, lastOperator) + mode->mode[i] + "\n";
			if (_modes.getModeValue(mode->mode[i]) == (lastOperator == '+'))
				return ;
			sendMessage(userid + " " + msg);
			_modes.setModeByType(mode->mode[i], lastOperator);
		}
	}
}

void	Client::setMode(t_mode* mode)
{
	char lastOperator;
	std::string msg;
	if (mode->mode[0] != '+' && mode->mode[0] != '-')
	{
		msg = ERR_UNKNOWNMODE(mode->clientNick, mode->mode + " ").c_str();
		send(mode->client_fd, msg.c_str(), strlen(msg.c_str()), 0);
		return ;
	}
	lastOperator = mode->mode[0];
	for (unsigned long i = 1; i < mode->mode.size(); i++)
	{
		if (mode->mode[i] == '+' || mode->mode[i] == '-')
			lastOperator = mode->mode[i];
		else
			_mode.isModeAuthorized(mode->mode[i], mode);
		setModeByType(mode->client_fd, mode->mode[i], lastOperator);
	}
}

void MODE::setModeByType(char mode, char value)
{
	for (std::vector<std::pair<char, bool> >::iterator it = _mode.begin(); it != _mode.end(); it++)
	{
		if (it->first == mode)
		{
			it->second = (value == '+') ? true : false;
			return ;
		}
	}
}

void Client::setModeByType(int fd, char mode, char value)
{
	std::string msg = "MODE " + getNickname() + " " + std::string(1, value) + mode + "\n";
	std::string localhost = "localhost";
	if (_mode.getModeValue(mode) == (value == '+'))
		return ;
	if (value == '+' && _mode.getModeValue('o') == false && fd != getFd() && mode == 'o')
	{
			std::string msg = ERR_USERSDONTMATCH(getNickname()).c_str();
			send(fd, msg.c_str(), strlen(msg.c_str()), 0);
			std::cout << "The client doesn't match." << std::endl;
			return ;
	}
	_mode.setModeByType(mode, value);
	send(fd, msg.c_str(), strlen(msg.c_str()), 0);
}

void Channel::setModeByType(int fd, char mode, char value, std::string param)
{
	std::string userid = USER_ID(getClient(fd).getNickname(), getClient(fd).getUsername());
	std::string msg;
	Client client;
	msg = "MODE " + getName() + " " + std::string(1, value) + mode + " ";
	if (mode == 'o')
	{
		client = getClientByNick(param);
		if (client.getFd() == -1)
		{
			std::string msg = ERR_NOSUCHNICK(getClient(fd).getNickname(), param).c_str();
			send(fd, msg.c_str(), strlen(msg.c_str()), 0);
			std::cout << "The client doesn't exist." << std::endl;
			return;
		}
		if (value == '+')
		{
			if (isClientOperator(client.getFd()))
				return ;
			std::cout << "here" << std::endl;
			this->setClientasOperator(client.getFd());
		}
		else
		{
			if (isClientOperator(client.getFd()) == false)
				return ;
			setClientasNormal(client.getFd());
		}
		msg = "MODE " + getName() + " " + std::string(1, value) + mode + " " + client.getNickname() + "\n";
		sendMessage(userid + " " + msg);
		return ;
	}
	else if (mode == 'k')
	{
		if (value == '+')
		{
			if (_modes.getModeValue('k') == true)
			{
				msg = ERR_KEYSET(getClient(fd).getNickname(), getName()).c_str();
				send(fd, msg.c_str(), strlen(msg.c_str()), 0);
				return ;
			}
			_modes.setPassword(param);
			msg += param;
		}
		else
		{
			if (_modes.getModeValue('k') == false)
				return ;
			_modes.clearPassword();
		}
	}
	else if (mode == 'l')
	{
		if (value == '+')
		{
			if (ft_atoll(param.c_str()) == -1)
			{
				std::string msg = ERR_UNKNOWNMODE(getClient(fd).getNickname(), mode + " ").c_str();
				send(fd, msg.c_str(), strlen(msg.c_str()), 0);
				return ;
			}
			if (ft_atoll(param.c_str()) == _modes.getLimit())
				return ;
			_modes.setLimit(param);
			msg += param;
		}
		else
		{
			if (_modes.getModeValue('l') == false)
				return;
			_modes.clearLimit();
		}
	}
	msg += "\n";
	sendMessage(userid + " " + msg);
	_modes.setModeByType(mode, value);
}

void Channel::processMode(int fd, t_mode mode, int size_of_cmd)
{
	if (isClientInChannel(fd) == false)
	{
		std::string msg = ERR_NOTONCHANNEL(mode.clientNick, getName()).c_str();
		send(fd, msg.c_str(), strlen(msg.c_str()), 0);
		throw std::invalid_argument("The client isn't in the channel.");
	}
	if (size_of_cmd == 2)
		displayMode(fd, mode.clientNick);
	else if (isClientOperator(fd) == false && getClient(fd).getMode().getModeValue('o') == false)
	{
		std::string msg = ERR_CHANOPRIVSNEEDED(mode.clientNick, getName()).c_str();
		send(fd, msg.c_str(), strlen(msg.c_str()), 0);
		throw std::invalid_argument("The client isn't an operator.");
	}
	else
		setMode(&mode);
}

void	Client::processMode(int fd, t_mode mode, int size_of_cmd)
{
	if (size_of_cmd == 2)
		displayMode(fd, mode.clientNick);
	else if (getFd() != fd && _mode.getModeValue('o') == false)
	{
		std::cout << "Client " << fd << " != " << getFd() << " and " << _mode.getModeValue('o') << std::endl;
		std::string msg = ERR_USERSDONTMATCH(mode.clientNick).c_str();
		send(fd, msg.c_str(), strlen(msg.c_str()), 0);
		throw std::invalid_argument("The client doesn't match.");
	}
	else
		setMode(&mode);
}

void Server::processMode(int fd, std::string string)
{
	if (getClient(fd)->isLogged() == false)
	{
		std::cout << "Client " << fd << " isn't logged." << std::endl;
		std::string msg = ERR_NOTREGISTERED(getClient(fd)->getNickname(), "MODE").c_str();
		send(fd, msg.c_str(), msg.length(), 0);
		return ;
	}
	t_mode mode;
	int size_of_cmd = createModeAndParams(fd, string, mode);
	if ( mode.type == CHANNEL_MODE)
	{
		Channel &channel = getChannelbyName(mode.name, fd);
		channel.processMode(fd, mode, size_of_cmd);
	}
	else
	{
		Client *client = getClientbyNickname(mode.name);
		if (client == NULL)
		{
			std::string msg = ERR_NOSUCHNICK(getClient(fd)->getNickname(), mode.name).c_str();
			send(fd, msg.c_str(), strlen(msg.c_str()), 0);
			throw std::invalid_argument("The client doesn't exist.");
		}
		client->processMode(fd, mode, size_of_cmd);
	}
}

int	Server::createModeAndParams(int fd, std::string cmd, t_mode& mode)
{
	std::vector<std::string> splitcmd = split_args(cmd, " ");
	unsigned long size = splitcmd.size();
	mode.clientNick = getClient(fd)->getNickname();
	if (size < 2)
	{
		std::string msg = ERR_NEEDMOREPARAMS(mode.clientNick, "MODE").c_str();
		send(fd, msg.c_str(), strlen(msg.c_str()), 0);
		throw std::invalid_argument("Not enough parameters.");
		return (0);
	}
	if (size > 2)
		mode.mode = splitcmd[2];
	(splitcmd[1][0] == '#' || splitcmd[1][0] == '&') ? mode.type = CHANNEL_MODE : mode.type = USER_MODE;
	mode.name = splitcmd[1];
	for (unsigned long i = 3; i < size; i++)
		mode.params.push_back(splitcmd[i]);
	mode.client_fd = fd;
	return (size);
}
