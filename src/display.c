#include "delta-v.h"

void		display_bytes(size_t bytes, char left_padding)
{
  int		padding;

  if (left_padding)
    padding = 3;
  else
    padding = 0;
  if (bytes < 1000)
    {
      printf("%*lu B", padding, bytes);
    }
  if (bytes >= 1000 && bytes < 1000000)
    {
      printf("%*lu.%02lu KB", padding, bytes / 1000, (bytes % 1000) / 10);
    }
  if (bytes >= 1000000 && bytes < 1000000000)
    {
      printf("%*lu.%02lu MB", padding, bytes / 1000000, (bytes % 1000000) / 10000);
    }
  if (bytes >= 1000000000 && bytes < 1000000000000)
    {
      printf("%*lu.%02lu GB", padding, bytes / 1000000000,
	     (bytes % 1000000000) / 10000000);
    }
  if (bytes >= 1000000000000)
    {
      printf("%*lu.%02lu TB", padding, bytes / 1000000000000,
	     (bytes % 1000000000000) / 10000000000);
    }
}

void		display_speed(struct timeval time_before,
			      struct timeval time_after,
			      ssize_t size)
{
  uint64_t	computed_time;

  computed_time = (time_after.tv_sec - time_before.tv_sec) * 1000000 +
    (time_after.tv_usec - time_before.tv_usec);
  if (computed_time > 0)
    {
      printf("   ");
      display_bytes((size_t)(1000000.0 / (double)computed_time * (double)size)
		    , 1);
      printf("/s");
    }
}

void		progress_bar(size_t done, size_t total)
{
  int		i;
  int		remain;

  i = (done + 1) * 50 / total;
  remain = 50 - i;
  printf("\r\e[K|");
  while (i > 0)
    {
      printf("#");
      i--;
    }
  while (remain > 0)
    {
      printf(" ");
      remain--;
    }
  printf("|");
}
