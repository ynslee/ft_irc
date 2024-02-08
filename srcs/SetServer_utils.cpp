#include "../includes/Server.hpp"


void Server::shut_down_server(int i, int fd)
{
    close(fd);
    this->pfds[i] = this->pfds[this->pollfd_count - 1];
    // we have to remove from the client when we have it 
    this->pollfd_count--;
}