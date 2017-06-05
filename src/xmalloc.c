#include "delta-v.h"

void	*xmalloc(size_t size)
{
  void	*ptr;

  ptr = malloc(size);
  if (ptr == NULL)
    {
      dprintf(2, "%s Malloc failed\n", RED_ERROR);
      exit(MALLOC_FAILED);
    }
  return (ptr);
}
