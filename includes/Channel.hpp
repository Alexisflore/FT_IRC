/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 09:48:10 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/21 11:36:47 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Client.hpp"
#include "mode.hpp"

class MODE;
class Client;

class Channel
{
private:
	MODE						_modes;
    std::string 				_name;
    std::vector<int> 			_clients;
	std::map<int, std::string>	_nicks;
	std::vector<int> 			_banned;
	std::vector<int> 			_invited;
	std::vector<int> 			_operators;
	std::string 				_topic;
	// std::map<std::string, bool> _modes;
	// std::string					_key;
	// unsigned long				_limit;
public:
    Channel();
    Channel(std::string channelName);
    ~Channel();
    Channel(const Channel &other);
	// Channel &operator=(const Channel &other);

	/*--------------Getters--------------*/
    std::string 				getName() const;
	Client*						getClient(int fd);
	std::vector<int>			getClients() const;
	std::string 				getTopic();
	std::map<std::string, bool> getModesAsString();
	bool						getMode(char mode);
	std::string					getParams(char mode);
	int							getFdFromNick(std::string nick);
	bool 						isClientOperator(int clientFd);
	bool 						isClientBanned(int clientFd);
	bool 						isClientInvited(int clientFd);
	bool 						isClientInChannel(int clientFd);
	bool 						isChannelFull();
	bool 						isInviteOnly();
	bool 						isPasswordProtected();
	bool 						isTopicProtected();
	std::string 				getPassword();
	void						displayMode(int _clientFd, std::string nickname);

	/*--------------Setters--------------*/
	void						setName(std::string name);
	void						setTopic(std::string topic);
	void						setMode(t_mode *mode);
	void						setModeByType(char mode, char value, bool needParams, std::string params, std::string nickname);
	void						clearTopic();
	void						setClientasOperator(int clientFd);
	void						setClientasBanned(int clientFd);
	void						setClientasInvited(int clientFd);
	void						setClientasNotOperator(int clientFd);

	/*--------------Methods--------------*/
    void    					leaveChannel(int clientFd);
    void 						sendMessage(const std::string message);
    void    					joinChannel(int clientFd, std::string nickname);
	bool						canClientSetTopic(int clientFd);
	void						removeClientfromList(int clientFd, std::vector<int> &_clients);
	void						processMode(int fd, t_mode mode, int size_of_cmd);
	// void						createCmd(t_mode* mode);
	/*--------------Exceptions--------------*/
    class SendException : public std::exception
    {
        public :
            virtual const char* what() const throw() {return "failed to send message";}
    };
};


