/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoonslee <yoonslee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 16:33:50 by jpelaez-          #+#    #+#             */
/*   Updated: 2024/02/08 10:48:08 by yoonslee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

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
	try
	{
		if (New_server.poll_loop() < 0)
			throw std::runtime_error("Poll");
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}

	// Should we pulling here??
	return (0);
}
