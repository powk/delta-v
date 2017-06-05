#include "delta-v.h"

void			*get_buffer(ssize_t size)
{
  static void		*buffer = NULL;

  if (!buffer)
    buffer = xmalloc(size);
  if (size == -1)
    {
      free(buffer);
      buffer = NULL;
    }
  return (buffer);
}

void			send_file(int socketfd, char *origin, char *filename)
{
  int			len;
  int			filefd;
  char			ready;
  struct stat		file_info;
  t_file_data		file_data;
  ssize_t		size;
  size_t		total_size;
  char			finished;
  char			*buffer;
  struct timeval	time_before;
  struct timeval	time_after;
  char			*name;
  char			*path;

  if ((stat(filename, &file_info)) < 0)
    {
      dprintf(2, "%s Can't stat file: %s\n", RED_ERROR, filename);
      return;
    }
  printf("%s (", filename);
  display_bytes(file_info.st_size, 0);
  printf(")\n");
  if ((filefd = open(filename, O_RDONLY)) < 0)
    {
      dprintf(2, "%s Can't open file: %s\n", RED_ERROR, filename);
      return;
    }

  origin = realpath(origin, NULL);
  name = realpath(filename, NULL);
  if (!origin || !name)
    {
      dprintf(2, "%s Failed to resolve file: %s", RED_ERROR, filename);
      return;
    }
  path = get_real_name(origin, name);

  len = strlen(path);
  xwrite(socketfd, &len, sizeof(len));	// Send the lenght of the filename
  xwrite(socketfd, path, len);	// Send the filename
  free(origin);
  free(name);

  file_data.size = file_info.st_size;
  file_data.mode = file_info.st_mode;
  xwrite(socketfd, &file_data, sizeof(file_data)); // Send the file informations
  xread(socketfd, &ready, sizeof(ready));   // Check if the receiver is OK
  buffer = get_buffer(BUFFER_SIZE);
  total_size = 0;
  finished = 0;

  while (!finished)
    {
      gettimeofday(&time_before, NULL);
      size = read(filefd, buffer, BUFFER_SIZE);	// Read the file
      xwrite(socketfd, &size, sizeof(size));	// Send the buffer size
      if (size < 0)
	{
	  dprintf(2, "%s Can't read %s\n", RED_ERROR, filename);
	  return;
	}
      if (size == 0)
	{
	  finished = 1;
	  continue;
	}
      xwrite(socketfd, buffer, size);	// Send the file content
      total_size += size;
      progress_bar(total_size, file_data.size);

      printf("  ");
      display_bytes(total_size, 1);
      printf(" / ");
      display_bytes(file_data.size, 0);

      gettimeofday(&time_after, NULL);
      display_speed(time_before, time_after, size);
      fflush(stdout);
    }
  close(filefd);
  printf("\r\e[K%s\n", GREEN_OK); // Clear the line
}

void			receive_file(int socketfd)
{
  char			*filename;
  int			len;
  char			ready;
  char			*buffer;
  t_file_data		file_data;
  ssize_t		size;
  char			finished;
  int			filefd;
  size_t		total_size;
  struct timeval	time_before;
  struct timeval	time_after;

  xread(socketfd, &len, sizeof(len));	// Receive the filename's lenght
  if (len <= 0 || len > PATH_MAX)	// Check the lenght of the filename
    {
      dprintf(2, "%s Bad size for a file name: %d\n", RED_ERROR, len);
      exit(SOMETHING_BAD_HAPPENED);
    }
  filename = xmalloc(sizeof(char) * (len + 1));	// Allocate space for the filename
  xread(socketfd, filename, len);	// Read the filename
  filename[len] = '\0';
  xread(socketfd, &file_data, sizeof(file_data));  // Receive file informations
  printf("%s (", filename);
  display_bytes(file_data.size, 0);
  printf(")\n");
  ready = 1;
  xwrite(socketfd, &ready, sizeof(ready));	// I'm ready

  if ((filefd = creat(filename, file_data.mode)) < 0)	// Create the file
    {
      dprintf(2, "%s Failed to create file: %s", RED_ERROR, filename);
      exit(-1);
    }
  total_size = 0;
  finished = 0;
  buffer = get_buffer(BUFFER_SIZE);

  while (!finished)
    {
      gettimeofday(&time_before, NULL);
      xread(socketfd, &size, sizeof(size));	// Receive the buffer size
      if (size < 0)
	{
	  dprintf(2, "%s Sender failed to read the file\n", RED_ERROR);
	  return;
	}
      if (size == 0)
	{
	  finished = 1;
	  continue;
	}
      if (size > BUFFER_SIZE)
	{
	  dprintf(2, "%s Buffer size of the sender too large\n", RED_ERROR);
	  dprintf(2, "Your buffer size: %d\n", BUFFER_SIZE);
	  dprintf(2, "The sender's buffer size: %ld", size);
	  close(socketfd);
	  exit(BUFFER_TOO_LARGE);
	}
      xread(socketfd, buffer, size);	// Receive the file content
      xwrite(filefd, buffer, size);	// Write to the ouptut file
      fsync(filefd);

      total_size += size;
      progress_bar(total_size, file_data.size);	// Display the progress bar

      printf("  ");
      display_bytes(total_size, 1);
      printf(" / ");
      display_bytes(file_data.size, 0);

      gettimeofday(&time_after, NULL);
      display_speed(time_before, time_after, size);

      fflush(stdout);
    }
  close(filefd);
  free(filename);
  printf("\r\e[K%s\n", GREEN_OK); // Clear the line
}
