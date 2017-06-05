#ifndef	DELTAV_H_
#define	DELTAV_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/tcp.h>
#include <dirent.h>
#include <sys/timeb.h>
#include <sys/time.h>
#include <limits.h>

#define	DEFAULT_PORT		4242

#define	BUFFER_SIZE		(4096 * 64) // Don't change it

#define	EVERYTHING_OK		0
#define	BAD_PARAMETER		1
#define	MALLOC_FAILED		2
#define	SOCKET_ERROR		3
#define	BIND_ERROR		4
#define	LISTEN_ERROR		5
#define	ACCEPT_ERROR		6
#define	CONNECT_ERROR		7
#define CONNECTION_CLOSED	8
#define WRITE_ERROR		9
#define	BUFFER_TOO_LARGE	10
#define SOMETHING_BAD_HAPPENED	11

#define	END_NEXT		0
#define	FILE_NEXT		1
#define	DIR_NEXT		2

#define	GREEN_OK		"[ \e[0;32mOK\e[0m ]"
#define	RED_ERROR		"[ \e[0;31mERROR\e[0m ]"

typedef struct	s_file_data
{
  uint64_t      size;
  mode_t	mode;
}		t_file_data;

typedef struct	s_args
{
  int		port;
  char		*ip;
  char		**files;
}		t_args;

int	wait_connection(int port);
int	connect_to_ip(char *ip, int port);
void	send_file(int socketfd, char *origin, char *filename);
void	receive_file(int socketfd);
void	network_read(int fd, void *buffer, size_t size);
void    xread(int fd, void *buffer, size_t size);
void	xwrite(int fd, const void *buf, size_t count);
void	*xmalloc(size_t size);
void	progress_bar(size_t done, size_t total);
void	send_recursively(int socketfd, char *origin, char *name);
char	*get_real_name(char *origin, char *name);
void	display_ip_address();
void	display_bytes(size_t bytes, char left_padding);
void	display_speed(struct timeval time_before, struct timeval time_after,
		      ssize_t size);
void	progress_bar(size_t done, size_t total);
t_args	*get_args(int ac, char **av);
char	**add_to_tab(char **array, char *str);
void	usage(int return_value);
int	check_transfer_parameters(int socketfd, t_args *args);
void	delta_send(int socketfd, t_args *args);
void	delta_receive(int socketfd, t_args *args);
void	receive_dir(int socketfd);
void	*get_buffer(ssize_t size);

#endif	/* !DELTAV_H_ */
