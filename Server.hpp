#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <exception>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <signal.h>
#include <errno.h>

#define MAXCLIENTS 32

class Server
{
    private:
    uint16_t port;
    std::string password;
        
    public:
    Server(char **argv);
    int serverSetup(char **argv);
    int acceptPendingConnections(int socketfd, struct sockaddr_storage their_addr);
};

#endif