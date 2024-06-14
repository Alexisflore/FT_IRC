/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 11:39:17 by alfloren          #+#    #+#             */
/*   Updated: 2024/06/14 12:35:03 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

/* ++ NUMERIC REPLY ++ */
#define RPL_WELCOME 			"001"
#define RPL YOURHOST 			"002"
#define ERR_NICKNAMEINUSE 		"433"
#define ERR_NONICKNAMEGIVEN 	"431"
#define ERR_NEEDMOREPARAMS	 	"461"
#define ERR_NOTONCHANNEL 		"442"
#define RPL_NOTOPIC 			"331"
#define RPL_TOPIC 				"332"
#define ERR_CHANOPRIVSNEEDED 	"482"