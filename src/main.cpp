/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 09:48:31 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/13 15:26:28 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Irc.hpp"

// bool _state = true;

// void handle_sigInt(int sig)
// {
// 	(void)sig;
// 	_state = false;
// }

int main(int ac, char **av)
{
	Server serv;
	// std::cout << " *-*-*- SERVER -*-*-* " << std::endl;
	try
	{
		if (ac != 3)
		{
			throw std::invalid_argument("The ircserv needs port and password for get ready");
			return (1);
		}
		signal(SIGINT, Server::signalHandler);//catch (Ctrl + C)
		signal(SIGQUIT, Server::signalHandler);//catch (Ctrl + \)
		serv.initServer(av[1], av[2]);
	}
	catch (const std::exception& e)
	{
		serv.closeFds();//close les files descripteurs
		std::cerr << ROUGE << "Error: " << e.what() << REINIT << std::endl;
	}
	std::cout << "The server Closed " << std::endl;
}