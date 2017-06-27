#include "delta-v.h"

char		*get_real_name(char *origin, char *name)
{
  int		i;

  i = strlen(origin);
  if (i > 0)
    i--;
  while (i > 0 && origin[i] != '/')
    i--;
  if (origin[i] == '/')
    return (&name[i + 1]);
  return (&name[i]);
}

void		send_directory(int socketfd, char *origin, char *name)
{
  int		len;
  char		*path;
  struct stat	info;

  origin = realpath(origin, NULL);
  name = realpath(name, NULL);
  if (!origin || !name)
    {
      dprintf(2, "%s Failed to resolve directory: %s\n", RED_ERROR, name);
      return;
    }
  if ((stat(name, &info)) < 0)
    {
      dprintf(2, "%s Failed to retrieve directory info: %s\n", RED_ERROR, name);
      return;
    }
  path = get_real_name(origin, name);
  len = strlen(path);
  xwrite(socketfd, &len, sizeof(len));	// Send the lenght of the dir name
  xwrite(socketfd, path, len);		// Send the dir name
  xwrite(socketfd, &info.st_mode, sizeof(info.st_mode));
  free(origin);
  free(name);
}

void		send_recursively(int socketfd, char *origin, char *name)
{
  char		*fullName;
  DIR		*dir;
  struct stat	info;
  struct dirent	*nextFile;
  int		nameLen;
  char		next;

  if (stat(name, &info) < 0)
    {
      dprintf(2, "%s %s: Failed to retrieve file informations\n", RED_ERROR, name);
      return;
    }
  if (S_ISREG(info.st_mode))
    {
      next = FILE_NEXT;	// Say to the the receiver that he will receive a file
      xwrite(socketfd, &next, sizeof(next));

      send_file(socketfd, origin, name);
    }
  else if (S_ISDIR(info.st_mode))
    {
      next = DIR_NEXT;	// Say to the the receiver that he will receive a directory
      xwrite(socketfd, &next, sizeof(next));

      send_directory(socketfd, origin, name);

      if (!(dir = opendir(name)))
	{
	  dprintf(2, "%s Failed to open directory: %s\n", RED_ERROR, name);
	  return;
	}
      nameLen = strlen(name);
      fullName = xmalloc(sizeof(char) * (nameLen + 257));
      strcpy(fullName, name);
      if (nameLen > 0 && fullName[nameLen - 1] != '/')
	{// if there is no / at the end, add one
	  strcat(fullName, "/");
	  nameLen += 1;
	}
      while ((nextFile = readdir(dir)))
	{
	  if (!strcmp(nextFile->d_name, ".") ||
	      !strcmp(nextFile->d_name, ".."))
	    continue;
	  strcpy(&fullName[nameLen], nextFile->d_name);
	  send_recursively(socketfd, origin, fullName);
	}
      closedir(dir);
      free(fullName);
    }
  else
    {
      dprintf(2, "%s %s: Not a file or directory\n", RED_ERROR, name);
      return;
    }
}

void		receive_dir(int socketfd)
{
  int		len;
  char		*name;
  mode_t	mode;

  xread(socketfd, &len, sizeof(len));
  if (len <= 0 || len > PATH_MAX)
    {
      dprintf(2, "%s Bad size for a directory name: %d\n", RED_ERROR, len);
      exit(SOMETHING_BAD_HAPPENED);
    }
  name = xmalloc(sizeof(char) * (len + 1));
  xread(socketfd, name, len);
  name[len] = '\0';
  xread(socketfd, &mode, sizeof(mode));
  if (!S_ISDIR(mode))
    {
      dprintf(2, "%s %s: Not a directory\n", RED_ERROR, name);
      free(name);
      exit(SOMETHING_BAD_HAPPENED);
    }
  if (check_path_validity(name) == 0)
    {
      dprintf(2, "%s Bad path: %s", RED_ERROR, name);
      exit(SOMETHING_BAD_HAPPENED);
    }
  if (mkdir(name, mode) < 0)
    {
      dprintf(2, "%s %s: Failed to create directory\n", RED_ERROR, name);
      free(name);
      return;
    }
  free(name);
}
