#include "delta-v.h"

void	usage(int return_value)
{
  printf("Usage: delta-v [ -i IP ] [ -p PORT ] [ FILES ]\n\n"
	 "\t-h --help\tDisplay this message\n"
	 "\t-i --ip\t\tIP to connect\n"
	 "\t-p --port\tPort to use (1 to 65535)\n");
  exit(return_value);
}

int		main(int ac, char **av)
{
  int		socketfd;
  t_args	*args;

  args = get_args(ac, av);
  if (args->ip == NULL)
    {
      display_ip_address();
      socketfd = wait_connection(args->port);
    }
  else
    {
      socketfd = connect_to_ip(args->ip, args->port);
    }
  if (check_transfer_parameters(socketfd, args))
    {
      if (args->files)
	{
	  delta_send(socketfd, args);
	}
      else
	{
	  delta_receive(socketfd, args);
	}
    }
  if (args->ip == NULL)
    usleep(10000);
  shutdown(socketfd, 2);
  close(socketfd);
  if (args->files)
    free(args->files);
  free(args);
  return (EVERYTHING_OK);
}
