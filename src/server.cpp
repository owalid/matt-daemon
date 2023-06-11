#include "server.hpp"

// Constructors
Server::Server()
{
  max_fd_ = 0;
  number_of_connected_cli_ = 0;
  number_of_max_conn_ = 3;
}

Server::Server(const Server &copy)
{
  listener_fd_ = copy.GetListenerFd();
  number_of_connected_cli_ = copy.GetNumberOfConnectedCli();
  number_of_max_conn_ = copy.GetNumberOfMaxConn();
  master_fd_ = copy.GetMasterFd();
  read_fd_ = copy.GetReadFd();
  max_fd_ = copy.GetMaxFd();
  client_addr_ = copy.GetClientAddr();
}

// Destructor
Server::~Server()
{
}

// Operators
Server &Server::operator=(const Server &assign)
{
  listener_fd_ = assign.GetListenerFd();
  number_of_max_conn_ = assign.GetNumberOfMaxConn();
  number_of_connected_cli_ = assign.GetNumberOfConnectedCli();
  master_fd_ = assign.GetMasterFd();
  read_fd_ = assign.GetReadFd();
  max_fd_ = assign.GetMaxFd();
  client_addr_ = assign.GetClientAddr();
  return *this;
}

// Getters / Setters

int Server::GetListenerFd() const
{
  return this->listener_fd_;
}

int Server::GetNumberOfMaxConn() const
{
  return this->number_of_max_conn_;
}

void Server::SetNumberOfMaxConn(int max_conn)
{
  this->number_of_max_conn_ = max_conn;
}

int Server::GetNumberOfConnectedCli() const
{
  return this->number_of_connected_cli_;
}

void Server::SetNumberOfConnectedCli(int nb_of_cli)
{
  this->number_of_connected_cli_ = nb_of_cli;
}

fd_set Server::GetMasterFd() const
{
  return this->master_fd_;
}

fd_set Server::GetReadFd() const
{
  return read_fd_;
}

void Server::SetReadFd(fd_set read_fd)
{
  this->read_fd_ = read_fd;
}

int Server::GetMaxFd() const
{
  return max_fd_;
}

void Server::SetMaxFd(int fd)
{
  this->max_fd_ = fd;
}

struct sockaddr_storage Server::GetClientAddr() const
{
  return this->client_addr_;
}

void Server::InitServer()
{
  int err = 0, y = 1;
  struct addrinfo hints, *ai, *cpy;

  FD_ZERO(&this->master_fd_);// on clean le master fd ainsi que le fd de lecture.
  FD_ZERO(&this->read_fd_);
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC; //on accepte ip4 ou ip6
  hints.ai_socktype = SOCK_STREAM; //tcp
// AI_PASSIVE : permet d'écouter sur toutes les interface
// AI_ADDRCONFIG : permet d'écouter ip4 si serveur en ip4 et idem pour ip6.
  hints.ai_flags = AI_PASSIVE;
  err = getaddrinfo(NULL, PORT, &hints, &ai);//création d'une liste d'adresses de sockets.
  if (err != 0)
    throw std::runtime_error("getaddrinfo failed.");
  for (cpy = ai; cpy != nullptr; cpy = cpy->ai_next)
  {
    this->listener_fd_ = socket(cpy->ai_family, cpy->ai_socktype, cpy->ai_protocol);
    if (this->listener_fd_ < 0)
      continue;
    if (setsockopt(this->listener_fd_,SOL_SOCKET, SO_REUSEADDR, &y, sizeof(int)) < 0)
      throw std::runtime_error("setsockopt failed.");
    if (bind(this->listener_fd_, cpy->ai_addr, cpy->ai_addrlen) < 0)
    {
      close(this->listener_fd_);
      continue;
    }
    break;
  }
  if (cpy == nullptr)
    throw std::runtime_error("Unable to bind to any address.");
  freeaddrinfo(ai);
  if (listen(this->listener_fd_, SOMAXCONN) == -1)
    throw std::runtime_error("Unable to listen.");
  FD_SET(this->listener_fd_, &this->master_fd_);
  this->SetMaxFd(this->listener_fd_);
}

/**
 * @brief This function is used to return the correct addr format (ip4 or ip6)
 *
 * @param sa // the client_addr
 * @return void*
 */
void* Server::GetInAddr()
{
  struct sockaddr *sa = (struct sockaddr *)&this->client_addr_;
  if (sa->sa_family == AF_INET)
    return &(((struct sockaddr_in*)sa)->sin_addr);
  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
