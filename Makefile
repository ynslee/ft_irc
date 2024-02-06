# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jpelaez- <jpelaez-@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/05 16:26:39 by jpelaez-          #+#    #+#              #
#    Updated: 2024/02/05 17:07:58 by jpelaez-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_irc

SRC = main.cpp SetServer.cpp

OBJECT = $(SRC:.cpp=.o)
HEADER = Server.hpp\

FLAGS = -Wall -Wextra -Werror 
CC = c++
INCLUDES = -Iincludes -I

all: $(NAME)

$(NAME): $(OBJECT) $(HEADER)
		$(CC) $(INCLUDES) $(FLAGS) $(OBJECT) -o $(NAME)

clean:
	rm -f $(OBJECT) 

fclean: clean
	   rm -f $(NAME)

re: fclean all

.PHONY: all re fclean clean