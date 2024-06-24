/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:08:56 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/24 15:07:36 by alfloren         ###   ########.fr       */
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

std::string				MODE::getParams(char mode) {
	for (std::vector<std::pair<char, std::string> >::iterator it = _params.begin(); it != _params.end(); it++)
	{
		if (it->first == mode)
			return it->second;
	}
	return "";
}

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
	if (mode->mode[mode->index].first[0] != '+' && mode->mode[mode->index].first[0] != '-')
	{
		msg = ERR_UNKNOWNMODE(mode->clientNick, mode->mode[mode->index].first + " ").c_str();
		send(mode->client_fd, msg.c_str(), strlen(msg.c_str()), 0);
		throw std::invalid_argument("The mode must start with + or -");
	}
	lastOperator = mode->mode[mode->index].first[0];
	for (unsigned long i = 1; i < mode->mode[mode->index].first.size(); i++)
	{
		if (mode->mode[mode->index].first[i] == '+' || mode->mode[mode->index].first[i] == '-')
			lastOperator = mode->mode[mode->index].first[i];
		else if (_modes.getAuthorizedMode(CHANNEL_MODE).find(mode->mode[mode->index].first[i]) == std::string::npos)
		{
			msg = ERR_UNKNOWNMODE(mode->clientNick, mode->mode[mode->index].first + " ").c_str();
			send(mode->client_fd, msg.c_str(), strlen(msg.c_str()), 0);
			throw std::invalid_argument("The mode doesn't exist.");
		}
		else if (_modes.getParamsNeeded(CHANNEL_MODE).find(mode->mode[mode->index].first[i]) != std::string::npos)
			{
				if (mode->mode[mode->index].second.empty())
				{
					msg = ERR_NEEDMOREPARAMS(mode->clientNick, "MODE").c_str();
					send(mode->client_fd, msg.c_str(), strlen(msg.c_str()), 0);
					throw std::invalid_argument("The mode needs parameters.");
				}
				else
					setModeByType(mode->client_fd, mode->mode[mode->index].first[i], lastOperator, mode->mode[mode->index].second);
				return ;
			}
		else
		{
			std::cout << "mode: " << mode->mode[mode->index].first[i] << " value: " << lastOperator << std::endl;
			_modes.setModeByType(mode->mode[mode->index].first[i], lastOperator);
		}
	}
}

MODE 	Client::getMode() {return _mode;}
void	Client::setMode(t_mode* mode)
{
	char lastOperator;
	std::string msg;
	if (mode->mode[mode->index].first[0] != '+' && mode->mode[mode->index].first[0] != '-')
	{
		msg = ERR_UNKNOWNMODE(mode->clientNick, mode->mode[mode->index].first + " ").c_str();
		send(mode->client_fd, msg.c_str(), strlen(msg.c_str()), 0);
		throw std::invalid_argument("The mode must start with + or -");
	}
	lastOperator = mode->mode[mode->index].first[0];
	for (unsigned long i = 1; i < mode->mode.size(); i++)
	{
		if (mode->mode[mode->index].first[i] == '+' || mode->mode[mode->index].first[i] == '-')
			lastOperator = mode->mode[mode->index].first[i];
		else
			_mode.isModeAuthorized(mode->mode[mode->index].first[i], mode);
		setModeByType(mode->client_fd, mode->mode[mode->index].first[i], lastOperator);
	}
}

void MODE::setModeByType(char mode, char value)
{
	for (std::vector<std::pair<char, bool> >::iterator it = _mode.begin(); it != _mode.end(); it++)
	{
		if (it->first == mode)
		{
			std::cout << "mode: " << mode << " value: " << value << std::endl;
			it->second = (value == '+') ? true : false;
			return ;
		}
	}
}

void Client::setModeByType(int fd, char mode, char value)
{
	if (value == '-')
		_mode.setModeByType(mode, value);
	else if (value == '+')
	{
		if (_mode.getModeValue('o') == true)
			_mode.setModeByType(mode, value);
		else
		{
			std::string msg = ERR_USERSDONTMATCH(getNickname()).c_str();
			send(fd, msg.c_str(), strlen(msg.c_str()), 0);
			throw std::invalid_argument("The client doesn't match.");
		}
	}
}

void Channel::setModeByType(int fd, char mode, char value, std::vector<std::string> params)
{
	std::string param;
	Client client;
	param = params[0];
	if (mode == 'o')
	{
		for (unsigned long i = 0; i < params.size(); i++)
		{
			client = getClientByNick(params[i]);
			if (client.getFd() == -1)
			{
				std::string msg = ERR_NOSUCHNICK(getClient(fd).getNickname(), params[i]).c_str();
				send(fd, msg.c_str(), strlen(msg.c_str()), 0);
				throw std::invalid_argument("The client doesn't exist.");
			}
			if (value == '+')
				setClientasOperator(client.getFd());
			else
				setClientasNormal(client.getFd());
		}
		return ;
	}
	else if (mode == 'k')
	{
		if (value == '+')
		{
			if (_modes.getPassword().empty() == false)
			{
				std::string msg = ERR_KEYSET(getClient(fd).getNickname(), getName()).c_str();
				send(fd, msg.c_str(), strlen(msg.c_str()), 0);
				throw std::invalid_argument("The password is already set.");
			}
			for (unsigned long i = 0; i < params.size(); i++)
				param += " " + params[i];
			_modes.setPassword(param);
		}
		else
			_modes.clearPassword();
	}
	else if (mode == 'l')
	{
		if (value == '+')
		{
			for (unsigned long i = 0; i < params.size(); i++)
				param += " " + params[i];
			if (_modes.setLimit(param) == false)
			{
				std::string msg = ERR_UNKNOWNMODE(getClient(fd).getNickname(), mode + " ").c_str();
				send(fd, msg.c_str(), strlen(msg.c_str()), 0);
				throw std::invalid_argument("The limit is invalid.");
			}
		}
		else
			_modes.clearLimit();
	}
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
	for (unsigned long i = 0; i < mode.mode.size(); i++)
	{
		std::cout << "mode: " << mode.mode[i].first << std::endl;
		mode.index = i;
		if ( mode.type == CHANNEL_MODE)
		{
			Channel &channel = getChannelbyName(mode.name, getClient(fd)->getNickname());
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
	unsigned long i = 2;
	unsigned long j;
	std::vector<std::string> params;
	if (size > 2) {
		while (i < size)
		{
			if (splitcmd[i][0] == '+' || splitcmd[i][0] == '-')
			{
					j = i + 1;
					params.clear();
					while (j < size && splitcmd[j][0] != '+' && splitcmd[j][0] != '-')
					{
						params.push_back(splitcmd[j]);
						j++;
					}
					mode.mode.push_back(std::make_pair(splitcmd[i], params));
					i = j;
			}
			else
			{
				std::string msg = ERR_UNKNOWNMODE(mode.clientNick, splitcmd[i]).c_str();
				send(fd, msg.c_str(), strlen(msg.c_str()), 0);
				throw std::invalid_argument("The mode doesn't exist.");
			}
		}
		if (mode.mode.empty())
		{
			std::string msg = ERR_NEEDMOREPARAMS(mode.clientNick, "MODE").c_str();
			send(fd, msg.c_str(), strlen(msg.c_str()), 0);
			throw std::invalid_argument("The mode is empty.");
			return (0);
		}
	}
	mode.client_fd = fd;
	return (size);
}


