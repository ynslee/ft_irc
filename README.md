<p align="center">
  <a href="" rel="noopener">
 <img width=200px height=200px src="https://cestoliv.com/projects/imgs/42/ft_irc.webp" alt="Project logo"></a>
</p>

<h3 align="center">Inter Relay Chat</h3>

<div align="center">

[![Status](https://img.shields.io/badge/status-active-success.svg)]()
[![GitHub Issues](https://img.shields.io/github/issues/kylelobo/The-Documentation-Compendium.svg)](https://github.com/kylelobo/The-Documentation-Compendium/issues)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](/LICENSE)

</div>

---

<p align="center"> Creating our own mini version of Inter Relay Chat server
    <br> 
</p>

## 📝 Table of Contents

- [About](#about)
- [Getting Started](#getting_started)
- [Running the server](#running_the_server)
- [Commands and its usage](#command)
- [External resources](#sources)

## 🧐 About <a name = "about"></a>

Exploring sockets, network programming and IRC protcols and develop our own Inter Relay Chat server from scratch using c++.

## Collaborators

[Yoonseon Lee](https://github.com/ynslee) , [Josefina Husso](https://github.com/hussojo) ,[
Juan Esteban Pelaez Hoyos](https://github.com/jestebanpelaez18)

## 🏁 Getting Started <a name = "getting_started"></a>

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

```
git clone the repository 
```

### Installing

```
$make
```
when the make file is compiled, run the executable as follow:
```
$./ircserv [port_number] [password]
```
##### 1\. `port_number` : 6667 as default
-  We highly recommend you to use port 6667 because IRC networks have defaulted to listening on TCP port 6667 for plain
   text connections. But you may use other ports that is under IRC assignment such as 6665-6669, for example.

##### 2\. `password` : PASS or any password of your wish


## 🤖 Running the server <a name = "running_the_server"></a>

### Open a new terminal and use Irssi or netcat to connect
-  You can use Irssi or netcat command to connect to server. We recommend you to use nc(netcat) command with the flag 'c' so that the communication between server and client is done properly with '\r\n' ending

##### 1\. Irssi
```
$irssi
$/connect localhost [port_number] [password]
```
##### 2\. Netcat
With netcat, you need to have 'pass', 'nick', 'user' command to connect to server. You need to type 'pass' command first to secure the connection and then you can use either 'nick' or 'user' command to register
```
$nc -c localhost [port_number]
pass [password]
nick [nickname]
user [username] * * :[real_name] (real_name is optional)
```
## Commands and its usage <a name = "command"></a>

| __Command__        | __Usage__   |
| -------------  |:-------------:|
| PASS              | PASS [password] |
| NICK       |       |
| USER  |     |
| MODE  |      |
| INVITE  |       |
| JOIN  |      |
| OPER  |      |
| PRIVMSG  |       |
| TOPIC  |      |
| KICK  |       |
| QUIT  |    |


 
## External resources <a name = "sources"></a>

+ [RFC 7194](https://datatracker.ietf.org/doc/rfc7194/)
+ [Modern IRC Client Protocol](https://modern.ircdocs.horse/)
+ [Inter Relay Chat Protocol](https://www.rfc-editor.org/rfc/rfc1459.html)
