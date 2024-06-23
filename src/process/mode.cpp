/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:08:56 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/23 19:43:31 by alfloren         ###   ########.fr       */
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
	_params = other._params;
	_needParams = other._needParams;
	_modesUser = other._modesUser;
	_modesChannel = other._modesChannel;
	return (*this);
}

/*--------------Getters--------------*/
long long 				MODE::getLimit() {return _limit;}
void					MODE::setLimit(std::string limit) {
	_limit = ft_atoll(limit.c_str());
	if (_limit == -1)
	{
		// std::string msg = ERR_KEYSET(
		// send(1, msg.c_str(), strlen(msg.c_str()), 0);
		throw std::invalid_argument("The limit is invalid.");
	}
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
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (isdigit(str[i]) == 0)
			return -1;
		res = res * 10 + str[i] - '0';
		if (res > MAX_INT)
			return -1;
		i++;
	}
	return (res);
}

std::string				MODE::getParams(char mode) {
	for (std::vector<std::pair<char, std::string> >::iterator it = _params.begin(); it != _params.end(); it++)
	{
		if (it->first == mode)
			return it->second;
	}
	return "";
}

std::vector<std::pair<char, bool> >	MODE::getMode() {return _mode;}

long long				MODE::getLimit()
{
	std::string limitStr = getParams('l');
	if (limitStr.empty() == true)
		return -1;
	long long limit = 0;
	if (limitStr[0] == '-')
		return -1;
	unsigned long i = 0;
	if (limitStr[0] == '+')
		i = 1;
	for (; i < limitStr.size(); i++)
	{
		if (isdigit(limitStr[i]))
		{
			limit = limit * 10 + (limitStr[i] - '0');
			if (limit > MAX_INT)
				return -1;
		}
		else
			return -1;
	}
	return limit;
}

std::string				MODE::getParamsNeeded(int Type)
{
	if (Type == USER_MODE)
		return "";
	else
		return _needParams;
}

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

std::string MODE::getModesAsString()
{
	std::string mode;
	for (std::vector<std::pair<char, bool> >::iterator it = _mode.begin(); it != _mode.end(); it++)
	{
		if (it->second == true)
			mode += "+" + std::string(1, it->first) + " ";
		else
			mode += "-" + std::string(1, it->first) + " ";
	}
	mode += "\n";
	return mode;
}

void Channel::setMode(t_mode* mode)
{
	char lastOperator;
	std::string msg;
	if (mode->mode[0] != '+' && mode->mode[0] != '-')
	{
		msg = ERR_UNKNOWNMODE(mode->clientNick, mode->mode + " ").c_str();
		send(mode->client_fd, msg.c_str(), strlen(msg.c_str()), 0);
		throw std::invalid_argument("The mode must start with + or -");
	}
	lastOperator = mode->mode[0];
	for (unsigned long i = 1; i < mode->mode.size(); i++)
	{
		_modes.isModeAuthorized(mode->mode[i], mode);
		if (mode->mode[i] == '+' || mode->mode[i] == '-')
			lastOperator = mode->mode[i];
		else
		{
			if (_modes.getParamsNeeded(CHANNEL_MODE).find(mode->mode[i]) != std::string::npos)
			{
				if (mode->params.empty() == true)
				{
					msg = ERR_NEEDMOREPARAMS(mode->clientNick, "MODE").c_str();
					send(mode->client_fd, msg.c_str(), strlen(msg.c_str()), 0);
					throw std::invalid_argument("The mode needs parameters.");
				}
				else
					setModeByType(mode->mode[i], lastOperator, true, mode->params[0], mode->clientNick);
				return ;
			}
			setModeByType(mode->mode[i], lastOperator, false, "", mode->clientNick);
		}
	}
}

/*--------------Setters--------------*/
void	Client::setMode(t_mode* mode)
{
	char lastOperator;
	std::string msg;
	if (mode->mode[0] != '+' && mode->mode[0] != '-')
	{
		msg = ERR_UNKNOWNMODE(mode->clientNick, mode->mode + " ").c_str();
		send(mode->client_fd, msg.c_str(), strlen(msg.c_str()), 0);
		throw std::invalid_argument("The mode must start with + or -");
	}
	lastOperator = mode->mode[0];
	for (unsigned long i = 1; i < mode->mode.size(); i++)
	{
		_mode.isModeAuthorized(mode->mode[i], mode);
		if (mode->mode[i] == '+' || mode->mode[i] == '-')
			lastOperator = mode->mode[i];
		setModeByType(mode->mode[i], lastOperator, false, "");
		return ;
	}
}

void MODE::setModeByType(char mode, char value, bool needParams, std::string params, std::string nick)
{
	if (mode == 'k' && value =='+')
	{
		setPassword(params);
	}
	else if (mode == 'l' && value == '+')
	{
		setLimit(params);
	}
	// else {
	// if (needParams == true)
	// {
	// 	for (std::vector<std::pair<char, std::string> >::iterator it = _params.begin(); it != _params.end(); it++)
	// 	{
	// 		if (it->first == mode)
	// 		{
	// 			it->second = params;
	// 			return ;
	// 		}
	// 	}
	// }
	// if (mode == 'l' && getLimit() == -1 && value =='+')
	// {
	// 	std::string msg = ERR_NEEDMOREPARAMS(nick, "MODE").c_str();
	// 	send(1, msg.c_str(), strlen(msg.c_str()), 0);
	// 	throw std::invalid_argument("The limit is invalid.");
	// }
	// else {
	for (std::vector<std::pair<char, bool> >::iterator it = _mode.begin(); it != _mode.end(); it++)
	{
		if (it->first == mode)
		{
			it->second = (value == '+') ? true : false;
			return ;
		}
	}
}

void Client::setModeByType(char mode, char value, bool needParams, std::string params)
{
	if (mode == 'o')
	{
		if (value == '-')
			_mode.setModeByType(mode, value, needParams, params, "");
	}
	else
		_mode.setModeByType(mode, value, needParams, params, "");
}

void Channel::setModeByType(char mode, char value, bool needParams, std::string params, std::string nick)
{
	if (mode == 'o')
	{
		if (value == '+')
		{
			for (unsigned long i = 0; i < params.size(); i++)
				setClientasOperator(getFdFromNick(params));
		}
		else
		{
			for (unsigned long i = 0; i < params.size(); i++)
				setClientasNormal(getFdFromNick(params));
		}
	}
	else
		_modes.setModeByType(mode, value, needParams, params, nick);
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
	else if (isClientOperator(fd) == false)
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
	else if (getFd() != fd)
	{
		std::string msg = ERR_USERSDONTMATCH(mode.clientNick).c_str();
		send(fd, msg.c_str(), strlen(msg.c_str()), 0);
		throw std::invalid_argument("The client doesn't match.");
	}
	else
		setMode(&mode);
}

void Server::processMode(int fd, std::string string)
{
	t_mode mode;
	int size_of_cmd = createModeAndParams(fd, string, mode);
	if ( mode.type == CHANNEL_MODE)
	{
		Channel &channel = getChannelbyName(mode.name, getClient(fd)->getNickname());
		std::vector<int> clients = channel.getClientsFd();
		std::cout << mode.name << std::endl;
		if (clients.empty() == true)
		{
			throw std::invalid_argument("NIQUE TA MERE");
		}
		for (std::vector<int>::iterator it = clients.begin(); it != clients.end(); it++)
		{
			std::cout << getClient(*it)->getNickname() << std::endl;
		}
		channel.processMode(fd, mode, size_of_cmd);
		channel.displayMode(fd, mode.clientNick);
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
	std::string msg = RPL_ENDOFNAMES(getClient(fd)->getNickname(), mode.name.c_str());
	send(fd, msg.c_str(), strlen(msg.c_str()), 0);
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
	(splitcmd[1][0] == '#' || splitcmd[1][0] == '&') ? mode.type = CHANNEL_MODE : mode.type = USER_MODE;
	mode.name = splitcmd[1];
	splitcmd.erase(splitcmd.begin(), splitcmd.begin() + 2);
	for (unsigned long i = 2; i < size; i++)
	{
		if (splitcmd[i][0] == '+' || splitcmd[i][0] == '-')
			mode.mode += splitcmd[i];
		else
			mode.params.push_back(splitcmd[i]);
	}
	if (mode.mode.empty())
	{
		std::string msg = ERR_NEEDMOREPARAMS(mode.clientNick, "MODE").c_str();
		send(fd, msg.c_str(), strlen(msg.c_str()), 0);
		throw std::invalid_argument("The mode must start with + or -");
		return (0);
	}
	mode.client_fd = fd;
	return (size);
}

void	Channel::displayMode(int fd, std::string nick)
{
	std::string msg = RPL_CHANNELMODEIS(nick, getName(), _modes.getModesAsString()).c_str();
	send(fd, msg.c_str(), msg.length(), 0);
}

void Client::displayMode(int fd, std::string nick)
{
	std::string msg = RPL_UMODEIS(nick, _mode.getModesAsString()).c_str();
	send(fd, msg.c_str(), msg.length(), 0);
}
