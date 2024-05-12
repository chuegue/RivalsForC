#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Server.h"
#include "Client.h"

#define GRAPHICAL 0

void print_usage(){
		printf("Wrong number of arguments.\n");
		printf("Usage: ./game <server|client>\n\n");
}

void server(){
		printf("Starting server!\n");
		int server_fd = server_init_listen_socket(6969);
		printf("server_fd = %d\n", server_fd);
		server_main_accept_loop(server_fd, 6969);
}

void client(){
	printf("Starting client side!\n");
#if GRAPHICAL
	struct SDLContext sdl_context = client_SDL_context_init(800,600);	
	TTF_Init();
	TTF_Font *super_creamy = TTF_OpenFont("super_creamy.ttf", 50);
	SDL_Color white = {255, 255, 255, 255};
	SDL_Surface *surface_text = TTF_RenderText_Solid(super_creamy, "yooo textooououo!", white);
	printf("%s", SDL_GetError());
	SDL_Texture *texture_text = SDL_CreateTextureFromSurface(sdl_context.renderer, surface_text);
	printf("%s", SDL_GetError());
	SDL_Rect rect_text = {0, 0, surface_text->w, surface_text->h};
	while(1){
		SDL_Event event;
		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_QUIT:
					exit(0);
					break;
				default:
					break;
			}
		}
		SDL_SetRenderDrawColor(sdl_context.renderer, COLOR_FMT(0xFF0000FF));
		printf("%s", SDL_GetError());
		SDL_RenderClear(sdl_context.renderer);
		printf("%s", SDL_GetError());
		SDL_RenderCopy(sdl_context.renderer, texture_text, NULL, &rect_text);
		printf("%s", SDL_GetError());
		SDL_RenderPresent(sdl_context.renderer);
		printf("%s", SDL_GetError());
		SDL_Delay(100);
		printf("%s", SDL_GetError());
	}
#endif
}

/*==============IMPORTANT! NAMING CONVENTION FOR PROJECT!!!===============*/
/*  local_variable
    struct StructName{ type fieldName;};
    type function_name(type ArgName){...}
    type _intermidieate_function_naem(type ArgName){...}
 */
int main(int argc, char *argv[]){
		if(argc != 2){
				print_usage();
				exit(1);
		}
		if(strcmp(argv[1], "server") == 0){
				server();
		} else if(strcmp(argv[1], "client") == 0){
				client();
		} else {
				print_usage();
				exit(1);
		}
		return 0;
}
