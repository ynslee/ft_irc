/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoonseonlee <yoonseonlee@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 16:28:14 by jpelaez-          #+#    #+#             */
/*   Updated: 2024/02/08 00:34:01 by yoonseonlee      ###   ########.fr       */
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
#include <csignal>
#include <poll.h>

#define MAXCLIENTS 32

class Server
{
    private:
    std::string port;
    std::string password;
    std::vector<struct pollfd> pfds;
    int pollfd_count;
    std::string message;
    int client_id;
        
    public:
    Server(std::string port, std::string password);
    Server();
    ~Server();
    const int getClientId();
    void    setClientId(const int id);
    void    setMessage(const char* msg);
    int serverSetup(std::string prt, std::string password);
    int acceptPendingConnections();
    int recieve_msg(int new_fd);
    int send_msg(int new_fd);
    int poll_loop();
};

#endif