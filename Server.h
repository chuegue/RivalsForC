#ifndef __SERVER__
#define __SERVER__
#include <stdint.h>

//Returns the socket file descriptor for listening
int server_init_listen_socket(uint16_t port);
void server_main_accept_loop(int server_fd, uint16_t port);
#endif // __SERVER__
