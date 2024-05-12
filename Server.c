#include "Server.h"
#include "Game_Backend.h"
#include "DA.h"

int server_init_listen_socket(uint16_t port){
	int server_fd = socket(AF_INET, SOCK_STREAM, 0); 
	if(server_fd < 0){
		fprintf(stderr, "Socket creation failed. Exiting...\n\n");
		exit(EXIT_FAILURE);
	}
	printf("Server socket created.\n");
	int opt = 1;
	if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
		fprintf(stderr, "Setting socket options failed. Exiting...\n\n");
		exit(EXIT_FAILURE);
	}
	printf("Socket options set.\n");
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
	if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0){
		fprintf(stderr, "Bind to socket failed. Exiting...\n\n");
		exit(EXIT_FAILURE);
	}
	printf("Bind succesful.\n");
	if(listen(server_fd, 3) < 0){
		fprintf(stderr, "Listen on server socket failed. Exiting...\n\n");
		exit(EXIT_FAILURE);
	}
	printf("Set socket to listen successfully.\n");
	return server_fd;	
}

//DECLARE_DYNAMIC_ARRAY(int)

int _write_to_completion_server(int fd, const void *buffer, size_t count){
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

int _read_to_completion_server(int fd, char *buffer, size_t count){
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
	
void server_main_accept_loop(int server_fd, uint16_t port){
	printf("Entering listening loop.\n");
	fd_set readfds;
	int max_fd = 0;
	DynamicArray_int *clientfds = createDynamicArray_int();
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
	PlayerBoard player1_board, player2_board;
	while(1){
		FD_ZERO(&readfds);
		FD_SET(server_fd, &readfds);
		max_fd = server_fd;
		for(int i = 0; i < clientfds->size; i++){
			int thisfd = get_int(clientfds, i);
			if(thisfd > 0){
				FD_SET(thisfd, &readfds);
			}
			if(thisfd > max_fd){
				max_fd = thisfd;
			}
		}	
		int nfds = select(max_fd + 1, &readfds, NULL, NULL, NULL);	
		if(nfds < 0){
			fprintf(stderr, "Error in select loop. Exiting.\n\n");
			exit(EXIT_FAILURE);
		}
		if(FD_ISSET(server_fd, &readfds)){//incoming new connection
			FD_CLR(server_fd, &readfds);
			int new_fd;
			int addrlen = sizeof(address);
			if((new_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0){
				fprintf(stderr, "Error accepting new connection.\n\n");
				exit(EXIT_FAILURE);
			}
			char greet_message[] = "Welcome to Rivals for Catan!\n";
			if(_write_to_completion_server(new_fd, greet_message, sizeof(greet_message)) < 0){
				fprintf(stderr, "Error writing to new connection.\n\n");
				exit(EXIT_FAILURE);
			}
			append_int(clientfds, new_fd);
			printf("New connection , socket fd is %d , ip is : %s , port : %d\n", 
					new_fd , inet_ntoa(address.sin_addr), 
					ntohs(address.sin_port));
			if(clientfds->size == 2){
				game_backend_init_game(get_int(clientfds, 0), get_int(clientfds, 1), &player1_board, &player2_board);
			}
		}
		for(int i = clientfds->size - 1; i >= 0; i--){
			int fd = get_int(clientfds, i);
			if(FD_ISSET(fd, &readfds)){
				FD_CLR(fd, &readfds);
				char buffer[2048] = {0};
				int readen = read(fd, buffer, 2047);
				if(readen > 0){
					printf("Received %d bytes from fd %d:", readen, fd);
					printf(" %s", buffer);
					_write_to_completion_server(fd, buffer, strlen(buffer) + 1);
				} else if(readen == 0){ //client disconnected
					printf("Client with fd %d went bye bye.\n", fd);
					close(fd);
					removeAt_int(clientfds, i);
				} else {
					fprintf(stderr, "Error reading from client.\n\n");
					exit(EXIT_FAILURE);
				}
			}
		}
	}	
	freeDynamicArray_int(clientfds);
}

