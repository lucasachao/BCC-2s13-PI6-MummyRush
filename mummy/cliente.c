#include "headers/cliente.h"

fd_set servidor;

int prepara_socket()
{
	sock = socket(PF_INET, SOCK_STREAM, 0);

	if(sock == -1)
	{
		fprintf(stderr, "Erro ao tentar abrir socket!\n");
		return -1;
	}
	else
	{
		printf("Socket aberto!\n");
		return 0;
	}
}

int prepara_server(char *server)
{
	host = gethostbyname(server);

	if(host == NULL)
	{
		fprintf(stderr, "Erro ao tentar obter host!\n");
		return -1;
	}
	else
		printf("Host obtido!\n");

	endereco.sin_family = AF_INET;
	endereco.sin_addr = *((struct in_addr *) host->h_addr_list[0]);
	endereco.sin_port = htons(PORT);

	if(connect(sock, (struct sockaddr *) &endereco, sizeof(struct sockaddr_in)) == -1)
	{
		fprintf(stderr, "Erro ao tentar conectar!\n");
		return -1;
	}

	printf("Conectado!\n");
	FD_ZERO(&servidor);
	FD_SET(sock, &servidor);	

	return 0;
}

void envia_buffer(struct Buffer buffer)
{
	write(sock, &buffer, sizeof(struct Buffer));
}

struct Buffer recebe_buffer()
{
	int teste = select(FD_SETSIZE, &servidor, NULL, NULL, NULL);
	struct Buffer buffer;
	buffer.tipo = 0;

	if(teste == -1)
	{
		printf("Erro ao tentar selecionar socket!\n");
		return buffer;
	}
	else if(teste > 0)
	{
		read(sock, &buffer, sizeof(struct Buffer));
		return buffer;
	}
	else return buffer;
}

void trata_buffer(struct Buffer buffer);

void finaliza_rede()
{
	if(close(sock) == -1)
		fprintf(stderr, "Erro ao tentar fechar socket!\n");
	else
		printf("Socket fechado!\n");
}
