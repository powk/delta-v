#include "delta-v.h"

void	display_ip_address()
{
  printf("IP address:\n");
  system("ifconfig | grep 'inet ' | grep -v '127.0.0.1' | grep -o -E '[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}' | grep -v 255");
  //system("ifconfig | grep 'inet ' | grep -v '127.0.0.1' | cut -d ' ' -f 10");
}
