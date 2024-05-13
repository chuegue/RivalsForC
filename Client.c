#include "Client.h"

struct SDLContext client_SDL_context_init(unsigned int WindowWidth, unsigned int WindowHeight){
    struct SDLContext new;
    SDL_InitSubSystem(SDL_INIT_VIDEO);
    new.window = SDL_CreateWindow("game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WindowWidth, WindowHeight, SDL_WINDOW_RESIZABLE);
    new.renderer = SDL_CreateRenderer(new.window, -1, SDL_RENDERER_ACCELERATED);
    return new;
}

int client_init_server_connection(const char *ip, uint16_t port){
	int sock_fd;
	if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		fprintf(stderr, "Socket creation failed.\n");
		exit(EXIT_FAILURE);
	}
	struct sockaddr_in server_addr = {0};
	server_addr.sin_family =AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(ip);
	server_addr.sin_port = htons(port);
	if(connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0){
		fprintf(stderr, "Couldn't connect to server.\n\n");
		exit(EXIT_FAILURE);
	}
	printf("Connected to server!\n");
	return sock_fd;
}

int _write_to_completion_client(int fd, const void *buffer, size_t count){
	size_t written = 0;
	while(written != count){
	 	int tmp = write(fd, (char *)buffer + written, count - written);
		if(tmp == -1){
			return -1;
		}
		written += tmp;
	}
	return 0;
}

int _read_to_completion_client(int fd, char *buffer, size_t count){
	int readen = 0, tmp;
	while((tmp = read(fd, buffer + readen, count - readen)) > 0){
		readen += tmp;
	}
	if(readen > 0){
		return readen;
	} else {
		return tmp;
	}
}
	
int client_message_is_present(int fd){
	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(fd, &readfds);
	struct timeval tv = {.tv_sec=0, .tv_usec=0};
	return select(fd + 1, &readfds, NULL, NULL, &tv) == 1; 
}

void client_main_game_loop(int client_fd){
	char buffer[2048] = {0};
	while(1){
		if(client_message_is_present(client_fd)){
			if(read(client_fd, buffer, 2047) == -1){
				fprintf(stderr, "Error reading message from server.\n\n");
				exit(EXIT_FAILURE);
			}
			printf("Message from server:\n%s", buffer);
		}	
	}
}	
