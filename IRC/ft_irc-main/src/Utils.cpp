#include "../includes/Utils.hpp"

bool    compareChannelName(Channel channel, std::string channelName)
{
    return (channelName == channel.getName());
}