#include "delta-v.h"

int		check_transfer_parameters(int socketfd, t_args *args)
{
  char		next;

  if (args->ip)
    {
      if (args->files)
	next = 1;
      else
	next = 0;
      xwrite(socketfd, &next, sizeof(next));
      xread(socketfd, &next, sizeof(next));
      if (next == 0)
	{
	  if (args->files)
	    dprintf(2, "%s You can't both send files\n", RED_ERROR);
	  else
	    dprintf(2, "%s Someone may add some files as parameter\n", RED_ERROR);
	}
    }
  else
    {
      xread(socketfd, &next, sizeof(next));
      if (args->files)
	{
	  if (next == 1) // Both have files as parameters: ERROR
	    {
	      dprintf(2, "%s You can't both send files\n", RED_ERROR);
	      next = 0;
	    }
	  else
	    next = 1;
	}
      else
	{
	  if (next == 0) // None have file as parameters: ERROR
	    {
	      dprintf(2, "%s Someone may add some files as parameter\n", RED_ERROR);
	      next = 0;
	    }
	  else
	    next = 1;
	}
      xwrite(socketfd, &next, sizeof(next));
    }
  return (next);
}
