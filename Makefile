# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kali <kali@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/13 09:48:39 by alfloren          #+#    #+#              #
#    Updated: 2024/06/28 05:11:38 by kali             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =				ircserv

BUILD_DIR :=		./build
INCLUDE_DIR	:= 		./includes
SRC_DIR :=			./src

CC =				c++

SRC_FILES :=		process/mode.cpp \
					Channel.cpp \
					Client.cpp \
					main.cpp \
					Server.cpp \
					Utils.cpp \
					process/bong.cpp \
					process/ping.cpp \
					process/invite.cpp \
					process/join.cpp \
					process/kick.cpp \
					process/names.cpp \
					process/nick.cpp \
					process/part.cpp \
					process/privmsg.cpp \
					process/quit.cpp \
					process/topic.cpp \
					process/user.cpp \
					process/pass.cpp \
					process/cap.cpp \
					process/who.cpp \
					process/oper.cpp \

OBJ_FILES	:= $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))
DEP_FILES	:= $(OBJ_FILES:.o=.d)

#Coompiler flags
CFLAGS =			-Iincludes -std=c++98 -Wall -Wextra -Werror -g3
CPPFLAGS	:= -I$(INCLUDE_DIR)

all:				$(NAME)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

-include $(DEP_FILES)

$(NAME): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) -o $(NAME)

clean:
	@rm -rf $(BUILD_DIR)

fclean:				clean
	@rm -f $(NAME)

re:					fclean all

.PHONY:				all clean fclean re
