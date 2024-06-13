# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/13 09:48:39 by alfloren          #+#    #+#              #
#    Updated: 2024/06/13 13:53:20 by alfloren         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =				ircserv

BUILD_DIR =			build

CXX =				c++

CXXFLAGS =			-Iincludes -std=c++98 -Wall -Wextra -Werror -g3

SRCS	=			src/main.cpp \
					src/Server.cpp \
					src/Client.cpp \
					src/Channel.cpp \
					src/Utils.cpp \
					src/join.cpp \
					src/names.cpp \
					src/quit.cpp \

OBJS =				$(SRCS:src/%.cpp=$(BUILD_DIR)/%.o) $(PROCESS:src/process/%.cpp=$(BUILD_DIR)/%.o)

all:				$(NAME)

$(NAME):			$(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: src/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@rm -rf $(BUILD_DIR)

fclean:				clean
	@rm -f $(NAME)

re:					fclean all

.PHONY:				all clean fclean re