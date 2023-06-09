#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "macros.hpp"

class Server
{
  public:
    fd_set master_fd_;
    fd_set read_fd_;
    struct sockaddr_storage client_addr_;

    // Constructors
    Server();
    Server(const Server &copy);

    // Destructor
    ~Server();

    // Operators
    Server &operator=(const Server &assign);

    // Getters / Setters
    int GetListenerFd() const;
    int GetNumberOfMaxConn() const;
    void SetNumberOfMaxConn(int max_conn);
    int GetNumberOfConnectedCli() const;
    void SetNumberOfConnectedCli(int nb_of_cli);
    fd_set GetMasterFd() const;
    fd_set GetReadFd() const;
    void SetReadFd(fd_set read_fd);
    int GetMaxFd() const;
    void SetMaxFd(int fd);
    struct sockaddr_storage GetClientAddr() const;

    void InitServer();
    void *GetInAddr();

  private:
    int listener_fd_;
    int number_of_max_conn_;
    int number_of_connected_cli_;
    int max_fd_;
};

#endif
