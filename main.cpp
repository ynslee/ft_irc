/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelaez- <jpelaez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 16:33:50 by jpelaez-          #+#    #+#             */
/*   Updated: 2024/02/06 16:53:52 by jpelaez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void sig_handler(int signal)
{
	if (signal == SIGINT)
	{
		std::cerr << "Server shut down" << std::endl;
		exit(0);
	}
}


int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return -1;
	}
	signal(SIGINT, sig_handler);
	Server New_server(argv[1], argv[2]);

	// Should we pulling here?? 
	return (0);
}
