
NAME = ircserv

SRC = main.cpp SetServer.cpp Client.cpp SetServer_utils.cpp Message.cpp

DIR_SRCS = srcs/
DIR_OBJS = objs/

OBJS = $(SRC:%.cpp=${DIR_OBJS}%.o)
HEADER = Server.hpp

FLAGS = -Wall -Wextra -Werror -MMD -MP -g3 -c #-fsanitize=address
CC = c++
INCLUDES = -I includes/

all: $(NAME)

$(NAME): $(OBJS)
		$(CC) $^ -o $@

${OBJS} : ${DIR_OBJS}%.o : ${DIR_SRCS}%.cpp
	mkdir -p ${DIR_OBJS}
	${CC} ${FLAGS} ${INCLUDES} $< -o $@

clean:
	rm -rf ${DIR_OBJS}

fclean: clean
	   rm -f $(NAME)

re: fclean all

.PHONY: all re fclean clean
