#include "../includes/Server.hpp"
#include "Server.hpp"


void Server::close_client(int i, int fd)
{
	close(fd);
	this->pfds[i] = this->pfds[this->pollfd_count - 1];
	// we have to remove from the client when we have it
	this->pollfd_count--;
	_clients.erase(fd);
}
