#include "delta-v.h"

void	display_ip_address()
{
  printf("IP address:\n");
  system("/sbin/ifconfig | grep -oE \"inet [0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\" | grep -v \"127.0.0.1\" | cut -d \" \" -f 2");
}
