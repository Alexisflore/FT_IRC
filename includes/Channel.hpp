/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 09:48:10 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/17 19:04:41 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Irc.hpp"

class Client;

class Channel
{
private:
    std::string 				_name;
    std::vector<int> 			_clients;
	std::string 				_topic;
	std::map<std::string, bool> _modes;
	std::vector<int> 			_banned;
	std::vector<int> 			_invited;
	std::vector<int> 			_operators;
	std::string					_key;
	long long					_limit;
public:
    Channel();
    Channel(std::string channelName);
    ~Channel();
    // Channel(const Channel &other);
	// Channel &operator=(const Channel &other);

	/*--------------Getters--------------*/
    std::string 				getName() const;
	Client*						getClient(int fd);
	std::vector<int>			getClients() const;
	std::string 				getTopic();
	std::map<std::string, bool> getModes();
	int							getFdFromNick(std::string nick);

	/*--------------Setters--------------*/
	void						setName(std::string name);
	void						setTopic(std::string topic);
	void						clearTopic();
	void						setMode(std::string mode, bool value, std::string params);
	void						setClientasOperator(int clientFd);
	void						setClientasBanned(int clientFd);
	void						setClientasInvited(int clientFd);

	/*--------------Methods--------------*/
    void    					leaveChannel(int clientFd);
    bool    					isClientInChannel(int clientFd);
	bool						isClientBanned(int clientFd);
	bool						isClientInvited(int clientFd);
	bool						isClientOperator(int clientFd);
    void 						sendMessage(const std::string message);
    void    					joinChannel(int clientFd);
	bool						canClientSetTopic(int clientFd);
	bool						canClientSetMode(int clientFd);
	void						removeClientfromList(int clientFd, std::vector<int> &_clients);
	void						topicMode(bool value, std::string topic);
	void						inviteMode(bool value, std::string clientName);
	void						keyMode(bool value, std::string key);
	void						limitMode(bool value, std::string limit);
	void						operatorMode(bool value, std::string clientName);

	/*--------------Exceptions--------------*/
    class SendException : public std::exception
    {
        public :
            virtual const char* what() const throw() {return "failed to send message";}
    };
};

#endif

