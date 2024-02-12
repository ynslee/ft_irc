#include "Client.hpp"
#include "Commands.hpp"

Commands::Commands(std::string command): _command(command)
{
    // we can do here the switch case stuff
    // swich:
    // case NICK:
    //     execute_nick(commands)
    // case PASS:
    //      execute_pass(commands)
    // 
}

Commands::~Commands() {}