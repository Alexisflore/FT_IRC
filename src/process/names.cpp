#include "../includes/Server.hpp"

void Server::processNames(int fd, std::vector< std::string> strings)
{
	if (strings.size() > 2)
		std::cout << "Usage : \"NAMES nameofthechannel\"" << std::endl;
	else if (strings.size() == 2)
	{
		std::cout << "Client " << fd << " is trying to get the list of clients in channel #" << strings[1] << std::endl;
		std::string channelName = strings[1];

		std::vector<Channel>::iterator it = std::find_if(
			this->_channels.begin(),
			this->_channels.end(),
			ChannelNameComparator(channelName));
		if (it != this->_channels.end())
		{
			Channel& channel = *it;
			std::vector<int> clients = channel.getClients();
			for (std::vector<int>::iterator it = clients.begin(); it != clients.end(); it++)
			{
				std::cout << "Client " << *it << std::endl;
			}
		}
		else
			std::cout << "Channel " << channelName << " doesnt exist yet!" << std::endl;
	}
	else
		std::cout << "need to specify the channel name" << std::endl;
}