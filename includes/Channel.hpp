#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Irc.hpp"

class Channel
{
private:
    std::string name;
    std::vector<int> clients;

public:
    Channel();
    Channel(std::string channelName);
    ~Channel();
    // Channel(const Channel &other);
	// Channel &operator=(const Channel &other);

    void    joinChannel(int clientFd);
    std::string getName() const;
    void    leaveChannel(int clientFd);
    bool    isClientInChannel(int clientFd);
    void sendMessage(const std::string message);

    class SendException : public std::exception
    {
        public :
            virtual const char* what() const throw() {return "failed to send message";}
    };
};


#endif

