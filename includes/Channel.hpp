/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 09:48:10 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/24 17:18:01 by alfloren         ###   ########.fr       */
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
	MODE								_modes;
    std::string 						_name;
	std::vector<std::pair<Client, char> > _clients;
	std::vector<Client> 				_client;
	std::string 						_topic;
public:
    Channel();
    Channel(std::string channelName);
    ~Channel();
    Channel(const Channel &other);

	/*--------------Getters--------------*/
    std::string 				getName() const;
	Client						getClient(int fd);
	std::string					getUsers();
	std::vector<std::pair<Client, char> >	getClients();
	std::vector<int>			getClientsFd();
	// int							getClientbyFd(int fd);
	std::string 				getTopic();
	bool						getMode(char mode);
	// std::string					getParams(char mode);
	int							getFdFromNick(std::string nick);
	Client						getClientByNick(std::string nick);
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
	void						setModeByType(int fd, char mode, char value, std::vector<std::string> params);
	void						clearTopic();
	void						setClientasOperator(int clientFd);
	void						setClientasBanned(int clientFd);
	void						setClientasInvited(int clientFd);
	void						setClientasNormal(int clientFd);

	/*--------------Methods--------------*/
    void    					leaveChannel(int clientFd);
    void 						sendMessage(const std::string message);
	void						sendNotification(const std::string message, int fd);
    void    					joinChannel(Client client);
	void						removeClient(Client client);
	bool						canClientSetTopic(int clientFd);
	void						processMode(int fd, t_mode mode, int size_of_cmd);


	/*--------------Exceptions--------------*/
    class SendException : public std::exception
    {
        public :
            virtual const char* what() const throw() {return "failed to send message";}
    };
};


