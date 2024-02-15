# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yoonslee <yoonslee@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/05 16:26:39 by jpelaez-          #+#    #+#              #
#    Updated: 2024/02/15 09:59:12 by yoonslee         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

SRC = main.cpp SetServer.cpp Client.cpp SetServer_utils.cpp Message.cpp commands/Pass.cpp commands/Cap.cpp
# commands/Nick.cpp 

DIR_SRCS = srcs/
DIR_OBJS = objs/

OBJS = $(SRC:%.cpp=${DIR_OBJS}%.o)
HEADER = Server.hpp

FLAGS = -Wall -Wextra -Werror -MMD -MP -g3 -c -std=c++11 #-fsanitize=address
CC = c++
INCLUDES = -I includes/

all: $(NAME)

$(NAME): $(OBJS)
		$(CC) $^ -o $@

${OBJS} : ${DIR_OBJS}%.o : ${DIR_SRCS}%.cpp
	mkdir -p ${DIR_OBJS}/commands
	${CC} ${FLAGS} ${INCLUDES} $< -o $@

clean:
	rm -rf ${DIR_OBJS}

fclean: clean
	   rm -f $(NAME)

re: fclean all

.PHONY: all re fclean clean
