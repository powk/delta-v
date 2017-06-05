#include "delta-v.h"

char		**add_to_tab(char **array, char *str)
{
  int		i;

  i = 0;
  if (array)
    {
      while (array[i])
	++i;
    }
  array = realloc(array, sizeof(char *) * (i + 2));
  if (!array)
    {
      dprintf(2, "%s Realloc failed: not enough memory\n", RED_ERROR);
      exit(MALLOC_FAILED);
    }
  array[i] = str;
  array[i + 1] = NULL;
  return (array);
}

t_args		*get_args(int ac, char **av)
{
  int		i;
  t_args	*args;

  args = xmalloc(sizeof(t_args));
  args->port = DEFAULT_PORT;
  args->ip = NULL;
  args->files = NULL;
  i = 1;
  while (i < ac)
    {
      if ((!strcmp(av[i], "-i") || !strcmp(av[i], "--ip")) && i < ac - 1)
	{
	  ++i;
	  if (args->ip != NULL)
	    {
	      dprintf(2, "%s Only one IP can be set as parameter\n", RED_ERROR);
	      exit(BAD_PARAMETER);
	    }
	  args->ip = av[i];
	}
      else if ((!strcmp(av[i], "-p") || !strcmp(av[i], "--port")) && i < ac - 1)
	{
	  ++i;
	  if (args->port != DEFAULT_PORT)
	    {
	      dprintf(2, "%s Only one port can be set as parameter\n", RED_ERROR);
	      exit(BAD_PARAMETER);
	    }
	  args->port = atoi(av[i]);
	  if (args->port < 1 || args->port > 65535)
	    {
	      dprintf(2, "%s Bad port number\n", RED_ERROR);
	      exit(BAD_PARAMETER);
	    }
	}
      else if (!strcmp(av[i], "-h") || !strcmp(av[i], "--help"))
	{
	  usage(EVERYTHING_OK);
	}
      else
	{
	  if (strlen(av[i]) > 0)
	    args->files = add_to_tab(args->files, av[i]);
	}
      ++i;
    }
  return (args);
}
