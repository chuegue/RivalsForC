#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/socket.h>
#include <unistd.h>
#include <asm/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define COLOR_FMT(COL) (COL)>>24 & 0xFF,(COL)>>16 & 0xFF,(COL)>>8 & 0xFF,(COL)>>0 & 0xFF

struct SDLContext{
    SDL_Window *window;
    SDL_Renderer *renderer;
};

struct SDLContext client_SDL_context_init(unsigned int WindowWidth, unsigned int WindowHeight);
int client_init_server_connection(const char *ip, uint16_t port);
void client_main_game_loop(int client_fd);
