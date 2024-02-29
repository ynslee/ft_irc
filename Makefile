
NAME = ircserv

SRC = main.cpp SetServer.cpp Client.cpp SetServer_utils.cpp Message.cpp Channel.cpp commands/Pass.cpp commands/Cap.cpp \
commands/User.cpp commands/Motd.cpp commands/Nick.cpp commands/Welcome.cpp commands/Quit.cpp commands/Oper.cpp \
commands/Join.cpp commands/Mode.cpp commands/Privmsg.cpp

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
