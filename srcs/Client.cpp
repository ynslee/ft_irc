
#include "Client.hpp"

//Nicknames are non-empty strings with the following restrictions:

// They MUST NOT contain any of the following characters: space (' ', 0x20), comma (',', 0x2C), asterisk ('*', 0x2A)
// question mark ('?', 0x3F), exclamation mark ('!', 0x21), at sign ('@', 0x40).
// They MUST NOT start with any of the following characters: dollar ('$', 0x24), colon (':', 0x3A).
// They MUST NOT start with a character listed as a channel type, channel membership prefix, or prefix listed in the IRCv3 multi-prefix Extension.
// They SHOULD NOT contain any dot character ('.', 0x2E).
// Servers MAY have additional implementation-specific nickname restrictions and SHOULD avoid the use of nicknames which are ambiguous 
// with commands or command parameters where this could lead to confusion or error.


/**
 * @brief Construct a new Client:: Client object
 * 
 * @param new_fd socket fd
 * @param _mode mode for USER, CHANNEL
 */
Client::Client(int new_fd): _client_fd(new_fd), _readbuf(NULL), _sendbuf(NULL){ 
		_connection_password = false;
		_registrationDone = false;
		_welcomeSent = false;
}