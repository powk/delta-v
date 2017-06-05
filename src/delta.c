#include "delta-v.h"

void	delta_send(int socketfd, t_args *args)
{
  int	i;
  char	next;

  i = 0;
  while (args->files[i])
    {
      send_recursively(socketfd, args->files[i], args->files[i]);
      ++i;
    }
  next = END_NEXT;	// Say to the the receiver that the transmission is finished
  xwrite(socketfd, &next, sizeof(next));
  get_buffer(-1);
}

void	delta_receive(int socketfd, t_args *args)
{
  char	next;

  (void)args;
  while (1)
    {
      xread(socketfd, &next, sizeof(next));
      if (next == END_NEXT)
	break;
      if (next == 1)
	receive_file(socketfd);
      if (next == 2)
	receive_dir(socketfd);
    }
  get_buffer(-1);
}
