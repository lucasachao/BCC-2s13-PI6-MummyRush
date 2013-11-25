#include "headers/jogo.h"
#include "headers/menu.h"

void *recebe_info(void *arg)
{
	bool *at = arg;
	struct Buffer buffer;
	buffer.tipo = 0;

	while(buffer.tipo != 9)
	{
		read(sock, &buffer, sizeof(struct Buffer));
		//printf("recebeu! %d\n", buffer.tipo);
		switch(buffer.tipo)
		{
			case 0:
				atualiza_jogadores(buffer);
			break;

			case 1:
				adiciona_tiro(buffer.angulo, buffer.x, buffer.y, 1);
			break;

			case 2:
			break;
		}
		*at = true;
	}

	return NULL;
}


int main()
{
	//splashscreen / inicializa componetes do allegro
	if(splash() == -1)
		return -1;

	//executa menu inicial e conecta com servidor
	num_jogadores = loop_menu();
	
	//le id_jogador
	read(sock, &id_jogador, 1);
	//printf("id jogador = %d\n", id_jogador);

	//le id dos outros jogadores
	id_jogadores = malloc((num_jogadores-1)*sizeof(int));
	int aux = 0;
	for(int i = 0; i < num_jogadores -1; i++)
	{
		read(sock, &aux, 1);
		//printf("aux %d\n", aux);
		id_jogadores[i] = aux;
		//printf("jogador %d\n", id_jogadores[i]);
	}

	//load de imagens/inicializacao de variaveis/prepara colisao
	if(prepara_jogo() == -1)
		finaliza_allegro();

	//inicializa mutex
	for(int i = 0; i < num_jogadores; i++)
		pthread_mutex_init(&jogadores[i].mtx, NULL);
	for(int i = 0; i < 300; i++)
		pthread_mutex_init(&tiro[i].mtx, NULL);


	//começou o jogo
	doexit = false;
	refresh = false;
	al_flip_display();

	//inicia thread para receber info do server
	pthread_t thr_server;
	if(pthread_create(&thr_server, NULL, recebe_info, &refresh) != 0)
		printf("Erro ao tentar criar thread do server!\n");
	else
		printf("thread do server criada com sucesso!\n");


	while(!doexit)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
 
		if(ev.type == ALLEGRO_EVENT_TIMER)
		{
			atualiza();
		}
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			doexit = true;
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			tecla_down(ev);
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			tecla_up(ev);
		}
		if(refresh && al_is_event_queue_empty(event_queue))
		{
			desenha();
			refresh = false;
			al_flip_display();
		} 
	}
	
	//fim de jogo
	if(pthread_join(thr_server, NULL) != 0)
		printf("Erro ao tentar juntar thread do server!\n");
	else
		printf("thread do server encerrada com sucesso!\n");

	for(int i = 0; i < num_jogadores; i++)
		pthread_mutex_destroy(&jogadores[i].mtx);
	for(int i = 0; i < 300; i++)
		pthread_mutex_destroy(&tiro[i].mtx);

	finaliza_rede();
	finaliza_jogo();//destroi imagens
	finaliza_allegro();//finaliza componentes allegro
	free(id_jogadores);
}
