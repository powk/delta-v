#include "delta-v.h"

void			xread(int fd, void *buffer, size_t size)
{
  size_t		already_read;
  ssize_t		currently_read;
  int			finished;

  already_read = 0;
  finished = 0;
  while (!finished)
    {
      currently_read = read(fd, buffer + already_read, size - already_read);
      if (currently_read < 0)
	{
	  dprintf(2, "\n%s Connection closed\n", RED_ERROR);
	  exit(CONNECTION_CLOSED);
	}
      already_read += currently_read;
      if (already_read == size)
	finished = 1;
    }
}
