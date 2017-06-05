#include "delta-v.h"

void	usage(int return_value)
{
  printf("Usage: delta-w option [IP / file]\n"
	 "Options:\n"
	 "\t-h --help\tDisplay this message\n"
	 "\t-r --receive\tReceive\n"
	 "\t-s --send\tSend\n");
  exit(return_value);
}

int	main(int ac, char **av)
{
  int	socketfd;
  int	port;
  char	next;

  port = DEFAULT_PORT;
  if (ac == 1)
    usage(BAD_PARAMETER);
  if (ac >= 2 && (!strcmp(av[1], "-h") || !strcmp(av[1], "--help")))
    usage(EVERYTHING_OK);
  if (!strcmp(av[1], "-r") || !strcmp(av[1], "--receive"))
    {
      if (ac < 3)
	{
	  dprintf(2, "You must specify an IP to connect to.\n");
	  return (BAD_PARAMETER);
	}
      socketfd = connect_to_ip(av[2], port);
      receive_tree(socketfd);

      usleep(10000); // The receiver must close the connection before the receiver
      shutdown(socketfd, 2);
      close(socketfd);
    }
  else if (!strcmp(av[1], "-s") || !strcmp(av[1], "--send"))
    {
      if (ac < 3)
	{
	  dprintf(2, "You must specify at least one file or directory to transfer\n");
	  return (BAD_PARAMETER);
	}
      display_ip_address();
      socketfd = wait_connection(port);
      send_recursively(socketfd, av[2], av[2]);

      next = END_NEXT;	// Say to the the receiver that the transmission is finished
      xwrite(socketfd, &next, sizeof(next));

      shutdown(socketfd, 2);
      close(socketfd);
    }
  else
    {
      dprintf(2, "Unknow option: %s\n", av[1]);
      exit(BAD_PARAMETER);
    }
  return (EVERYTHING_OK);
}
