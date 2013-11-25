#ifndef CLIENTE_
#define CLIENTE_

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/select.h>

#define PORT 10500

struct Buffer
{
	int tipo;
	int id;
	int x;
	int y;
	int angulo;
	int vida;
};
/*
tipo:
0 - jogador
1 - tiro
2 - inimigo
*/


char byte;
int sock;
struct sockaddr_in endereco;
struct hostent *host;
fd_set servidor;

int prepara_socket();
int prepara_server(char *server);
void envia_buffer(struct Buffer buffer);
struct Buffer recebe_buffer();
void trata_buffer(struct Buffer buffer);
void finaliza_rede();

#endif
