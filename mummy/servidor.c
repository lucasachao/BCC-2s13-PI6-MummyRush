#include "headers/servidor.h"

struct Buffer buffer;
int sock, novo_sock, conectados, prontos, ok;
socklen_t tamanho;
fd_set conjunto, novo_conjunto;
struct sockaddr_in endereco, novo_endereco;
struct hostent *host;
char *server;
int id_jog[6];
pthread_t thr_clientes[6], thr_inimigos;
clock_t t1;

/*THREADS*/
void *gerencia_inimigos(void *arg)
{
	int* jogando = arg;
	//printf("%d jogando\n", jogando[0]);
	while(jogando[0] != 0)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(queue_inimigos, &ev);

		if(ev.type == ALLEGRO_EVENT_TIMER)
		{
			move_inimigos(conectados, id_jog);
		}
	}

	return NULL;
}

void *gerencia_clientes(void *arg)
{
	//buffer que recebe info dos clientes
	struct Buffer buffer1;

	//arg = id_cliente que a thread esta tratando
	int *aux = (int*)arg;
	int id_cliente = *aux;
	int rodando = 1;

	//printf("teste %d\n", id_cliente);
	while(rodando == 1)
	{
		if(read(id_cliente, &buffer1, sizeof(struct Buffer)) == 0)
			{
				FD_CLR(id_cliente,&conjunto);	  
				printf("socket %d desconectado!\n",id_cliente);
				rodando = 0;
				conectados --;
			}
			else
			{
				//printf("Recebeu %d de socket %d\n", buffer1.tipo, id_cliente);	
				//coloca info recebida nos arrays
				switch(buffer1.tipo)
				{
					case 0://atualizacao de personagem
					for(int i = 0; i < conectados; i++)
						if(jogadores_s[i].id == id_cliente)
						{
							pthread_mutex_lock(&jogadores_s[0].mtx);
							jogadores_s[i].x = buffer1.x;
							jogadores_s[i].y = buffer1.y;
							jogadores_s[i].vida = buffer1.vida;
							jogadores_s[i].angulo = buffer1.angulo;
							pthread_mutex_unlock(&jogadores_s[0].mtx);
//printf("jogador atualizado: x%f y%f\n", jogadores_s[i].x, jogadores_s[i].y);
//printf("atualizou pos de %d\n", id_cliente);
							break;
						}
					break;

					case 1://atualizacao de tiro
					break;

					case 2://atualizacao de inimigo
					break;
				}

				//replica info para outros clientes
				for(int i = 0; i < FD_SETSIZE; i++)
					if(FD_ISSET(i, &conjunto))
						if(i != sock && i != id_cliente)
							write(i, &buffer1, sizeof(struct Buffer));
			}
	}

	return NULL;
}



/*PREPARA E FINALIZA*/
int prepara_rede()
{
	conectados = 0;
	prontos = 0;

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1)
	{
		fprintf(stderr, "Erro ao tentar abrir socket!\n");
		return -1;
	}
	printf("Socket aberto!\n");

	printf("Digite o endereço do servidor\n");
	server = malloc(18*sizeof(int));
	scanf("%s", server);
	host = gethostbyname(server);
	free(server);

	if(host == NULL)
	{
		fprintf(stderr, "Erro ao tentar obter host!\n");
		return -1;
	}
	printf("Host obtido!\n");

	endereco.sin_family = AF_INET;
	endereco.sin_addr = *((struct in_addr *) host->h_addr_list[0]);
	endereco.sin_port = htons(PORT);

	if(bind(sock, (struct sockaddr *) &endereco, sizeof(struct sockaddr_in)))
	{
		fprintf(stderr, "Erro ao tentar associar endereco!\n");
		return -1;
	}
	printf("Endereco associado!\n");

	if(listen(sock, TAM_FILA) == -1)
	{
		fprintf(stderr, "Erro ao tentar habilitar recepcao!\n");
		return -1;
	}
	printf("Recepcao habilitada!\n");
	
	return 0;
}

int finaliza_server()
{
	if(close(sock) == -1)
	{
		fprintf(stderr,"Erro ao tentar fechar socket!\n");
		return -1;
	}
	printf("Socket fechado!\n");

	return 0;
}

//insere sock dos clientes no conjunto e cria uma thread para tratar cada cliente
void prepara_clientes()
{
	FD_ZERO(&conjunto);
	FD_SET(sock, &conjunto);

	while(1)
	{
		novo_conjunto = conjunto;

		if(select(FD_SETSIZE, &novo_conjunto, NULL, NULL, NULL) == -1)
			printf("Erro ao tentar selecionar socket!\n");
		else
		{
			printf("Socket selecionado!\n");

			for(int i = 0; i < FD_SETSIZE; i++)
				if(FD_ISSET(i, &novo_conjunto))
				{
					if(i == sock)
					{
						novo_sock = accept(i, (struct sockaddr *) &novo_endereco, &tamanho);

						if(novo_sock == -1)
							printf("Erro ao tentar aceitar conexao!\n");
						else
						{
							printf("Conexao aceita!\n");
							FD_SET(novo_sock, &conjunto);
							conectados ++;
						}
					}
					else
					{
						if(read(i, &buffer, sizeof(struct Buffer)) == 0)
						{
							FD_CLR(i,&conjunto);	  
							printf("socket %d desconectado!\n", i);
							conectados --;
						}
						else
						{
							//printf("Recebeu! %d\n", buffer.tipo);
							if(buffer.tipo == 9) prontos ++;
						}
					}
				}
		}
		printf("jogadores conectados: %d\n", conectados);
		printf("jogadores prontos: %d\n", prontos);
		//todos os jogadores estao prontos
		if(prontos == conectados)
		{
			ok = conectados;
			int aux = 0;
			for(int i = 0; i < FD_SETSIZE; i++)
				if(FD_ISSET(i, &conjunto))//para cada cliente
					if(i != sock)//tirando o sock de listen
					{
						write(i, &ok, 1);//envia numero de jogadores
						printf("comecou o jogo para sock %d!\n", i);
						write(i, &i, 1);//envia socket/id do jogador
						for(int j = 0; j < FD_SETSIZE; j++)
							//j nao eh nem o sock listen nem o cliente em questao
							if(FD_ISSET(j, &conjunto) && j != i && j != sock)
							{
								write(i, &j, 1);//envia id dos outros jogadores
								//printf("j = %d\n", j);
							}
						//uma thread para cada cliente
						id_jog[aux] = i;
						if(pthread_create(&thr_clientes[aux], NULL, gerencia_clientes, &id_jog[aux]) != 0)
							printf("Erro ao tentar criar thread dos clientes!\n");
						else
							printf("thread do cliente %d criada com sucesso!\n", id_jog[aux]);
						aux ++;
					}
			break;
		}
	}
}


/*MAIN*/
int main()
{
	if(prepara_rede() == -1)
		exit(EXIT_FAILURE);

	prepara_clientes();
	prepara_inimigos_server(1, conectados, id_jog);
	
	if(pthread_create(&thr_inimigos, NULL, gerencia_inimigos, &conectados) != 0)
		printf("Erro ao tentar criar thread dos inimigos!\n");
	else
		printf("thread dos inimigos criada com sucesso!\n");

	//encerra thread dos clientes
	for(int i = 0; i < conectados; i++)
		if(pthread_join(thr_clientes[i], NULL) != 0)
			printf("Erro ao tentar juntar thread dos clientes!\n");
		else
			printf("thread do cliente %d encerrada com sucesso!\n", id_jog[i]);

	//encerra thread dos inimigos
	if(pthread_join(thr_inimigos, NULL) != 0)
		printf("Erro ao tentar juntar thread dos inimigos!\n");
	else
	{
		printf("thread dos inimigos encerrada com sucesso!\n");
		finaliza_inimigos_server();
	}



	if(finaliza_server() == -1)
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}
