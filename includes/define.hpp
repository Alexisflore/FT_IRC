/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 11:39:17 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/27 18:30:45 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <csignal>
#include <cstring>
#include <string>
#include <vector>
#include <netinet/in.h>
#include <sys/socket.h>
#include <exception>
#include <arpa/inet.h>
#include <limits.h>
#include <sstream>
#include <unistd.h>
#include <cstdlib> //pour atoi()
#include <poll.h>
#include <fcntl.h>//pour fnctl()
#include <algorithm>
#include <map>
#include <sstream>

typedef struct s_mode
{
	std::string	mode;
	std::vector<std::string>	params;
	int							type;
	int							client_fd;
	std::string					clientNick;
	std::string					name;
	int							index;
}				t_mode;

/* +++ COLORS +++ */
#define REINIT "\033[0m"
#define NOIR "\033[30m"
#define ROUGE "\033[31m"
#define VERT "\033[32m"
#define JAUNE "\033[33m"
#define BLEU "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define BLANC "\033[37m"

#define PENDING_QUEUE_MAXLENGTH 1 // a changer => nombre de client qui peuvent se connecte
#define BUFFER_SIZE 1024
#define MAX_INT 2147483647

/* ++ NUMERIC REPLY ++ */

# define USER_ID(nickname, username) (":" + nickname + "!" + username + "@localhost")
# define RPL_WELCOME(nickname, user_id) (":localhost 001 " + nickname + " :Welcome to the Internet Relay Network " + user_id + "\r\n")
# define RPL_YOURHOST(client, servername, version) (":localhost 002 " + client + " :Your host is " + servername + " (localhost), running version " + version + "\r\n")
# define RPL_CREATED(client, datetime) (":localhost 003 " + client + " :This server was created " + datetime + "\r\n")
# define RPL_MYINFO(client, servername, version, user_modes, chan_modes, chan_param_modes) (":localhost 004 " + client + " " + servername + " " + version + " " + user_modes + " " + chan_modes + " " + chan_param_modes + "\r\n")
# define RPL_ISUPPORT(client, tokens) (":localhost 005 " + client + " " + tokens " :are supported by this server\r\n")

# define ERR_UNKNOWNCOMMAND(client, command) (":localhost 421 " + client + " " + command + " :Unknown command\r\n")
# define ERR_NOTREGISTERED(client, command) (":localhost 451 " + client + " " + command + " :You have not registered\r\n")
// INVITE
# define ERR_NEEDMOREPARAMS(client, command) (":localhost 461 " + client + " " + command + " :Not enough parameters.\r\n")
# define ERR_NOSUCHCHANNEL(client, channel) (":localhost 403 " + client + " " + channel + " :No such channel\r\n")
# define ERR_NOTONCHANNEL(client, channel) (":localhost 442 " + client + " " + channel + " :The user is not on this channel.\r\n")
# define ERR_USERONCHANNEL(client, nick, channel) (":localhost 443 " + client + " " + nick + " " + channel + " :Is already on channel\r\n")
# define RPL_INVITING(user_id, client, nick, channel) (user_id + " 341 " + client + " " + nick + " " + channel + "\r\n")
# define RPL_INVITE(user_id, invited, channel) (user_id + " INVITE " + invited + " " + channel + "\r\n")

// JOIN
# define RPL_JOIN(user_id, channel) (user_id + " JOIN :" +  channel + "\r\n")
# define ERR_BANNEDFROMCHAN(client, channel) (":localhost 474 " + client + " " + channel + " :Cannot join channel (+b)\r\n")
# define ERR_BADCHANNELKEY(client, channel) (":localhost 475 " + client + " " + channel + " :Cannot join channel (+k)\r\n")
# define ERR_TOOMANYCHANNELS(client, channel) (":localhost 405 " + client + " " + channel + " :You have joined too many channels\r\n")
# define ERR_INVITEONLYCHAN(client, channel) (":localhost 473 " + client + " " + channel + " :Cannot join channel (+i)\r\n")
# define ERR_CHANNELISFULL(client, channel) (":localhost 471 " + client + " " + channel + " :Cannot join channel (+l)\r\n")

// KICK
# define ERR_USERNOTINCHANNEL(client, nickname, channel) (":localhost 441 " + client + " " + nickname + " " + channel + " :They aren't on that channel\r\n")
// # define ERR_CHANOPRIVSNEEDED(client, channel) ("482 " + client + " #" +  channel + " :You're not channel operator\r\n")
# define RPL_KICK(user_id, channel, kicked, reason) (user_id + " KICK " + channel + " " + kicked + " " + reason + "\r\n")

// KILL
# define ERR_NOPRIVILEGES(client) (":localhost 481 " + client + " :Permission Denied- You're not an IRC operator\r\n")
# define RPL_KILL(user_id, killed, comment) (user_id + " KILL " + killed + " " + comment + "\r\n")

// MODE
/* user mode */
#define MODE_USERMSG(client, mode) (":" + client + " MODE " + client + " :" + mode + "\r\n")
#define ERR_UMODEUNKNOWNFLAG(client) (":localhost 501 " + client + " :Unknown MODE flag\r\n")
#define ERR_USERSDONTMATCH(client) (":localhost 502 " + client + " :Cant change mode for other users\r\n")
#define RPL_UMODEIS(userid, channel ,mode) (userid + " 221 " + channel + " :" + mode + "\r\n")
# define ERR_UNKNOWNMODE(client, mode) (":localhost 472 " + client + " " + mode + " :is unknown mode char to me\r\n")
# define ERR_KEYSET(client, channel) (":localhost 467 " + client + " " + channel + " :Channel key already set\r\n")
# define ERR_L
/* channel mode */
#define MODE_CHANNELMSG(channel, mode) (":localhost MODE " + channel + " " + mode + "\r\n")
#define MODE_CHANNELMSGWITHPARAM(channel, mode, param) (":localhost MODE " + channel + " " + mode + " " + param + "\r\n")
#define RPL_CHANNELMODEIS(client, channel, mode) (":localhost 324 " + client + " " + channel + " " + mode + "\r\n")
#define RPL_CHANNELMODEISWITHKEY(client, channel, mode, password) (":localhost 324 " + client + " " + channel + " " + mode + " " + password + "\r\n")
#define ERR_CANNOTSENDTOCHAN(client, channel) (":localhost 404 " + client + " " + channel + " :Cannot send to channel\r\n")
#define ERR_CHANNELISFULL(client, channel) (":localhost 471 " + client + " " + channel + " :Cannot join channel (+l)\r\n")
#define ERR_CHANOPRIVSNEEDED(client, channel) (":localhost 482 " + client + " " + channel + " :You're not channel operator\r\n")
#define ERR_INVALIDMODEPARAM(client, channel, mode, password) (":localhost 696 " + client + " " + channel + " " + mode + " " + password + " : password must only contained alphabetic character\r\n")
// RPL_ERR a broadcoast quand user pas +v ou operator veut parler
      // dans notre cas c'était tiff (client) qui voulait send a message
      // :lair.nl.eu.dal.net 404 tiff #pop :Cannot send to channel
#define RPL_ADDVOICE(nickname, username, channel, mode, param) (":" + nickname + "!" + username + "@localhost MODE #" + channel + " " + mode + " " + param + "\r\n")

// MOTD
#define ERR_NOSUCHSERVER(client, servername) (":localhost 402 " + client + " " + servername + " :No such server\r\n")
#define ERR_NOMOTD(client) (":localhost 422 " + client + " :MOTD File is missing\r\n")
#define RPL_MOTDSTART(client, servername) (":localhost 375 " + client + " :- " + servername + " Message of the day - \r\n")
#define RPL_MOTD(client, motd_line) (":localhost 372 " + client + " :" + motd_line + "\r\n")
#define RPL_ENDOFMOTD(client) (":localhost 376 " + client + " :End of /MOTD command.\r\n")


// NAMES
# define RPL_NAMREPLY(client, symbol, channel, list_of_nicks) (":localhost 353 " + client + " " + symbol + " " + channel + " :" + list_of_nicks + "\r\n")
# define RPL_ENDOFNAMES(client, channel) (":localhost 366 " + client + " " + channel + " :End of /NAMES list.\r\n")

// NICK
# define ERR_NONICKNAMEGIVEN(client) (":localhost 431 " + client + " :There is no nickname.\r\n")
# define ERR_ERRONEUSNICKNAME(client, nickname) (":localhost 432 " + client + " " + nickname + " :Erroneus nickname\r\n")
# define ERR_NICKNAMEINUSE(nickname, newnick) (":localhost 433 " + nickname + " " + newnick + " :Nickname is already in use.\r\n")
# define RPL_NICK(userid, client) (userid + " NICK " + client + "\r\n")

// NOTICE
# define RPL_NOTICE(nick, username, target, message) (":" + nick + "!" + username + "@localhost NOTICE " + target + " " + message + "\r\n")

// OPER
# define ERR_NOOPERHOST(client) (":localhost 491 " + client + " :No O-lines for your host\r\n")
# define RPL_YOUREOPER(client) (":localhost 381 " + client + " :You are now an IRC operator\r\n")

// PART
# define RPL_PART(user_id, channel, reason) (user_id + " PART " + channel + " " + (reason.empty() ? "." : reason ) + "\r\n")

// PASS
# define ERR_PASSWDMISMATCH(client) (":localhost 464 " + client + " :Password incorrect.\r\n")

// PING
# define RPL_PONG(user_id, token) (user_id + " PONG " + token + "\r\n")

// QUIT
# define RPL_QUIT(user_id, reason) (user_id + " QUIT " + reason + "\r\n")
# define RPL_ERROR(user_id, reason) (user_id + " ERROR :" + reason + "\r\n")

// PRIVMSG
# define ERR_NOSUCHNICK(client, target) (":localhost 401 " + client + " " + target + " :No such nick/channel\r\n")
# define ERR_NORECIPIENT(client) (":localhost 411 " + client + " :No recipient given PRIVMSG\r\n")
# define ERR_NOTEXTTOSEND(client) (":localhost 412 " + client + " :No text to send\r\n")
# define RPL_PRIVMSG(nick, username, target, message) (":" + nick + "!" + username + "@localhost PRIVMSG " + target + " " + message + "\r\n")

// TOPIC
# define RPL_TOPIC(client, channel, topic) (":localhost 332 " + client + " " + channel + " :" + topic + "\r\n")
# define RPL_NOTOPIC(client, channel) (":localhost 331 " + client + " " + channel + " :No topic is set\r\n")

// USER
# define ERR_ALREADYREGISTERED(client) (":localhost 462 " + client + " :You may not reregister.\r\n")

// WHO
# define RPL_WHOREPLY(clientnick, canal, username, host, nick, mode, hopcount, realname) (":localhost 352 " + clientnick + " " + canal + " " + username + " " + host + " localhost " + nick + " " + mode + " " + hopcount + " " + realname + "\r\n")
# define RPL_ENDOFWHO(client, name) (":localhost 315 " + client + " " + name + " :End of /WHO list.\r\n")

// BAN
# define RPL_ENDOFBANLIST(client, channel) (":localhost 368 " + client + " " + channel + " :End of channel ban list\r\n")