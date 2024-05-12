#ifndef __SERVER__
#define __SERVER__
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/socket.h>
#include <unistd.h>
#include <asm/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include "Game_Backend.h"

//Returns the socket file descriptor for listening
int server_init_listen_socket(uint16_t port);
void server_main_accept_loop(int server_fd, uint16_t port);
#endif // __SERVER__
