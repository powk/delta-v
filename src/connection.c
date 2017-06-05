#include "delta-v.h"

int			wait_connection(int port)
{
  struct sockaddr_in	addr;
  int			socketfd;

  if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
      dprintf(2, "%s Can't create socket\n", RED_ERROR);
      exit(SOCKET_ERROR);
    }
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  if (bind(socketfd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
      dprintf(2, "%s Can't bind socket to port %d\n", RED_ERROR, port);
      exit(BIND_ERROR);
    }
  if (listen(socketfd, 1) < 0)
    {
      dprintf(2, "%s Can't listen\n", RED_ERROR);
      exit(LISTEN_ERROR);
    }
  printf("Waiting for connection on port %d\n", port);
  if ((socketfd = accept(socketfd, (struct sockaddr*)NULL, NULL)) < 0)
    {
      dprintf(2, "%s Can't accept connection\n", RED_ERROR);
      exit(ACCEPT_ERROR);
    }
  printf("Connection succesfull\n");
  return (socketfd);
}

int			connect_to_ip(char *ip, int port)
{
  struct sockaddr_in	addr;
  int			socketfd;

  if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
      dprintf(2, "%s Can't create socket\n", RED_ERROR);
      exit(SOCKET_ERROR);
    }
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(ip);
  if (connect(socketfd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
      dprintf(2, "%s Can't connect to %s on port %d\n", RED_ERROR, ip, port);
      exit(CONNECT_ERROR);
    }
  printf("Connection succesfull\n");
  return (socketfd);
}
