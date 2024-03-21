#ifndef COMMON_HPP
# define COMMON_HPP

#include <algorithm>
#include <arpa/inet.h>
#include <csignal>
#include <errno.h>
#include <exception>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <netdb.h>
#include <poll.h>
#include <signal.h>
#include <sstream>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <utility>
#include <vector>
#include <cstring>
#include <cstdio>
#include "Reply.hpp"

# define LIGHT BLACK  "\033[90m"
# define LIGHTRED "\033[91m"
# define GREEN "\033[32m"
# define LIGHTGREEN "\033[92m"
# define YELLOW "\033[33m"
# define LIGHTYELLOW "\033[93m"
# define BLUE "\033[34m"
# define LIGHTBLUE "\033[94m"
# define MAGENTA "\033[35m"
# define LIGHTMAGENTA "\033[95m"
# define CYAN "\033[36m"
# define LIGHTCYAN "\033[96m"
# define RESET "\033[0m"
# define BOLD "\033[1m"

#endif
