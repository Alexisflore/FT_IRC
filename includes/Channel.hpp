/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 09:48:10 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/14 14:50:04 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Irc.hpp"

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
	int							_limit;
	int							_key;
public:
    Channel();
    Channel(std::string channelName);
    ~Channel();
    // Channel(const Channel &other);
	// Channel &operator=(const Channel &other);

	/*--------------Getters--------------*/
    std::string 				getName() const;
	std::vector<int>			getClients() const;
	std::string 				getTopic();
	std::map<std::string, bool> getModes();

	/*--------------Setters--------------*/
	void						setName(std::string name);
	void						setTopic(std::string topic);
	void						setMode(std::string mode, bool value);
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
	bool						canClientSetMode(int clientFd, std::string mode);
	void						removeClientfromList(int clientFd, std::vector<int> &_clients);

	/*--------------Exceptions--------------*/
    class SendException : public std::exception
    {
        public :
            virtual const char* what() const throw() {return "failed to send message";}
    };
};

#endif

