#include "delta-v.h"

void			xwrite(int fd, const void *buf, size_t size)
{
  size_t		already_write;
  ssize_t		currently_write;
  int			finished;

  already_write = 0;
  finished = 0;
  while (!finished)
    {
      currently_write = write(fd, buf + already_write, size - already_write);
      if (currently_write < 0)
	{
	  dprintf(2, "\n%s Connection closed\n", RED_ERROR);
	  exit(CONNECTION_CLOSED);
	}
      already_write += currently_write;
      if (already_write == size)
	finished = 1;
    }
}
