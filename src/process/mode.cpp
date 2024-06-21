/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:08:56 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/21 11:44:33 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include "../../includes/Channel.hpp"
#include "../../includes/mode.hpp"
#include "../../includes/Client.hpp"

MODE::MODE()
{
	_mode['i'] = false;
	_mode['o'] = false;
	_mode['w'] = false;
	_mode['s'] = false;
	_mode['t'] = false;
	_mode['k'] = false;
	_mode['l'] = false;
	_modesUser = "iosw";
	_modesChannel = "iotkl";
	_needParams = "klo";
}

MODE::~MODE() {}
MODE::MODE(const MODE &other) {*this = other;}
MODE 		&MODE::operator=(const MODE &other)
{
	_mode = other._mode;
	_params = other._params;
	_needParams = other._needParams;
	_modesUser = other._modesUser;
	_modesChannel = other._modesChannel;
	return (*this);
}

/*--------------Getters--------------*/
bool					MODE::getModeValue(char mode) {return _mode[mode];}
std::string				MODE::getParams(char mode) {return _params[mode];}
std::map<char, bool>	MODE::getModes() {return _mode;}
long long				MODE::getLimit()
{
	if (_params.find('l') != _params.end())
	{
		std::string limitStr = _params['l'];
		long long limit = 0;
		for (unsigned long i = 0; i < limitStr.size(); i++)
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
	return -1;
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
	for (std::map<char, bool>::iterator it = _mode.begin(); it != _mode.end(); it++)
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
	if (needParams == true)
		_params[mode] = params;
	if (mode == 'l' && getLimit() == -1)
	{
		std::string msg = ERR_NEEDMOREPARAMS(nick, "MODE").c_str();
		send(1, msg.c_str(), strlen(msg.c_str()), 0);
		throw std::invalid_argument("The limit is invalid.");
	}
	_mode[mode] = (value == '+') ? true : false;
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
				setClientasNotOperator(getFdFromNick(params));
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
		std::vector<int> clients = channel.getClients();
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
