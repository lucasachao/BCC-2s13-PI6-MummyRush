#ifndef INIMIGO_C
#define INIMIGO_C
#include "headers/inimigo.h"
#include <time.h>

int i1, i2;

void prepara_inimigos_server(int qtd_inimigos)
{
	num_inimigos = qtd_inimigos;
	t1 = clock();
	inimigos = malloc(sizeof(struct Inimigo)*num_inimigos);
	int gap = 16;
	for(int i = 0; i < num_inimigos; i++)
	{
		inimigos[i].id = i;
		inimigos[i].vida = 2;
		inimigos[i].angulo = rand()%8;
		inimigos[i].x = 30+gap*i;
		inimigos[i].y = 500;
		inimigos[i].passo = 0;
		inimigos[i].movimento = false;
		pthread_mutex_init(&inimigos[i].mtx, NULL);
	}
}

void prepara_inimigos_client(int qtd_inimigos)
{
	num_inimigos = qtd_inimigos;
	t1 = clock();
	inimigos = malloc(sizeof(struct Inimigo)*num_inimigos);
	int gap = 16;
	for(int i = 0; i < num_inimigos; i++)
	{
		inimigos[i].id = i;
		inimigos[i].vida = 2;
		inimigos[i].angulo = rand()%8;
		inimigos[i].x = 30+gap*i;
		inimigos[i].y = 500;
		inimigos[i].passo = 0;
		inimigos[i].movimento = false;
		pthread_mutex_init(&inimigos[i].mtx, NULL);

		inimigos[i].bouncer = al_create_bitmap(BOUNCER_SIZE, BOUNCER_SIZE);
		al_set_target_bitmap(inimigos[i].bouncer);
		al_clear_to_color(al_map_rgb(250, 250, 250));
		al_set_target_bitmap(al_get_backbuffer(display));
	}
}

void finaliza_inimigos_server()
{
	for(int i = 0; i < num_inimigos; i++)
	{
		pthread_mutex_destroy(&inimigos[i].mtx);
	}
	free(inimigos);
}

void finaliza_inimigos_client()
{
	for(int i = 0; i < num_inimigos; i++)
	{
		al_destroy_bitmap(inimigos[i].bouncer);
		pthread_mutex_destroy(&inimigos[i].mtx);
	}
	free(inimigos);
}

float getDist()
{
	pthread_mutex_lock(&inimigos[i1].mtx);
	pthread_mutex_lock(&jogadores[i2].mtx);

	float dist = sqrtf(powf((inimigos[i1].x - jogadores[i2].x),2) + powf((inimigos[i1].y - jogadores[i2].y),2));

	pthread_mutex_unlock(&inimigos[i1].mtx);
	pthread_mutex_unlock(&jogadores[i2].mtx);
	return dist;
}

void move_inimigos()
{
	//Define o destino de cada mumia
	for (i1 = 0; i1 < num_inimigos; i1++)
	{
		int minIndice = -1;
		for (i2 = 0; i2 < num_jogadores; i2++)
		{
			float minG = 1000000;
			float minAtual = getDist();
			if (minAtual < minG)
			{
				minG = minAtual;
				minIndice = i2;
			}
		}
		pthread_mutex_lock(&inimigos[i1].mtx);
		pthread_mutex_lock(&jogadores[minIndice].mtx);

		inimigos[i1].dx = jogadores[minIndice].x;
		inimigos[i1].dy = jogadores[minIndice].y;

		pthread_mutex_unlock(&inimigos[i1].mtx);
		pthread_mutex_unlock(&jogadores[minIndice].mtx);
	}
	int velocidade = 4;
	
	//Movimenta cada mumia 1 pixel na direcao definida
	for (i1 = 0; i1 < num_inimigos; i1++)
	{
		pthread_mutex_lock(&inimigos[i1].mtx);

		if (inimigos[i1].x < inimigos[i1].dx && verifica_colisao(inimigos[i1].x,inimigos[i1].y, 16, 2, velocidade))
		{
			inimigos[i1].x += velocidade;
			inimigos[i1].movimento = true;
		}
		else if (inimigos[i1].x > inimigos[i1].dx && verifica_colisao(inimigos[i1].x,inimigos[i1].y, 16, 6, velocidade))
		{
			inimigos[i1].x -= velocidade;
			inimigos[i1].movimento = true;
		}
		if (inimigos[i1].y < inimigos[i1].dy && verifica_colisao(inimigos[i1].x,inimigos[i1].y, 16, 4, velocidade))
		{
			inimigos[i1].y += velocidade;
			inimigos[i1].movimento = true;
		}
		else if (inimigos[i1].y > inimigos[i1].dy && verifica_colisao(inimigos[i1].x,inimigos[i1].y, 16, 0, velocidade))
		{
			inimigos[i1].y -= velocidade;
			inimigos[i1].movimento = true;
		}
		pthread_mutex_unlock(&inimigos[i1].mtx);

		if(inimigos[i1].movimento == true)
		{
			atualiza_inimigo_server(i1);
			inimigos[i1].movimento = false;
		}
	}
}

void desenha_inimigos()
{
	for(int i = 0; i < num_inimigos; i++)
	{
		pthread_mutex_lock(&inimigos[i].mtx);

		if(inimigos[i].vida > 0)
			al_draw_bitmap(inimigos[i].bouncer, inimigos[i].x, inimigos[i].y, 0);

		pthread_mutex_unlock(&inimigos[i].mtx);
	}
}

void* atualiza_inimigos_tempo()
{
	while (true)
		if ( ((clock() - t1 / CLOCKS_PER_SEC)*1000*1000) >= 1.0/FPS)
		{
			t1 = clock();
			move_inimigos();
		}
}

//envia informacoes para clientes
void atualiza_inimigo_server(int id_inimigo)
{
	pthread_mutex_lock(&inimigos[id_inimigo].mtx);

	//coloca info do inimigo que se mexeu no buffer
	struct Buffer buffer;
	buffer.tipo = 2;
	buffer.x = inimigos[id_inimigo].x;
	buffer.y = inimigos[id_inimigo].y;
	buffer.angulo = 2;//arrumar!!
	buffer.vida = inimigos[id_inimigo].vida;

	//envia info do inimigo que se mexeu para os clients		
		write(sock, &buffer, sizeof(struct Buffer));
	
	pthread_mutex_unlock(&inimigos[id_inimigo].mtx);
}

void atualiza_inimigo_client(struct Buffer buffer)
{
	for(int i = 0; i < num_inimigos; i++)
	{
		pthread_mutex_lock(&inimigos[i].mtx);
		if(inimigos[i].id == buffer.id)
		{
			inimigos[i].x = buffer.x;
			inimigos[i].y = buffer.y;
			inimigos[i].angulo = buffer.angulo;
			inimigos[i].vida = buffer.vida;
			pthread_mutex_unlock(&inimigos[i].mtx);
			break;
		}
		pthread_mutex_unlock(&inimigos[i].mtx);
	}
}
#endif
