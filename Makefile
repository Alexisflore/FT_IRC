# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/13 09:48:39 by alfloren          #+#    #+#              #
#    Updated: 2024/06/13 14:35:10 by alfloren         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =				ircserv

BUILD_DIR :=		./build
INCLUDE_DIR	:= 		./includes
SRC_DIR :=			./src

CC =				c++

SRC_FILES :=		Channel.cpp \
					Client.cpp \
					main.cpp \
					Server.cpp \
					Utils.cpp \
					process/join.cpp \
					process/quit.cpp \
					process/names.cpp \

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