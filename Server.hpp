/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoonslee <yoonslee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 16:28:14 by jpelaez-          #+#    #+#             */
/*   Updated: 2024/02/06 15:35:26 by yoonslee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
    std::string port;
    std::string password;
        
    public:
    Server(std::string port, std::string password);
    Server();
    ~Server();
    int serverSetup(std::string prt, std::string password);
    int acceptPendingConnections(int socketfd, struct sockaddr_storage their_addr);
    int sendRecv(int new_fd, int socketfd);
};

#endif