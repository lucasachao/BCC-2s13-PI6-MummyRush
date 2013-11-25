#ifndef INIMIGO_C
#define INIMIGO_C
#include "headers/inimigo.h"
#include <time.h>

void prepara_inimigos(int qtd_inimigos)
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
		inimigos[i].x = 30+gap*1;
		inimigos[i].y = 700 + rand()%16;
		inimigos[i].passo = 0;
		pthread_mutex_init(&inimigos[i].mtx, NULL);
	}
}

void finaliza_inimigos()
{
	for(int i = 0; i < num_inimigos; i++)
	{
		al_destroy_bitmap(inimigos[i].bouncer);
		pthread_mutex_init(&inimigos[i].mtx, NULL);
	}
	free(inimigos);
}

//implentar inteligencia

int i1, i2;

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
			refresh = true;
			movimento = true;
		}
		else if (inimigos[i1].x > inimigos[i1].dx && verifica_colisao(inimigos[i1].x,inimigos[i1].y, 16, 6, velocidade))
		{
			inimigos[i1].x -= velocidade;
			refresh = true;
			movimento = true;
		}
		if (inimigos[i1].y < inimigos[i1].dy && verifica_colisao(inimigos[i1].x,inimigos[i1].y, 16, 4, velocidade))
		{
			inimigos[i1].y += velocidade;
			refresh = true;
			movimento = true;
		}
		else if (inimigos[i1].y > inimigos[i1].dy && verifica_colisao(inimigos[i1].x,inimigos[i1].y, 16, 0, velocidade))
		{
			inimigos[i1].y -= velocidade;
			refresh = true;
			movimento = true;
		}
	}
	pthread_mutex_unlock(&inimigos[i1].mtx);


	if(movimento == true)
	{
		atualiza_inimigos();
		movimento = false;
	}
}

void desenha_inimigos(struct Inimigo *inimigos)
{
	for(int i = 0; i < num_inimigos; i++)
	{
		pthread_mutex_lock(&inimigos[i].mtx);

		if(inimigos[i].vida > 0)
		{
			al_draw_bitmap(inimigos[i].bouncer, inimigos[i].x, inimigos[i].y, 0);
			refresh = true;
		}

		pthread_mutex_unlock(&inimigos[i].mtx);

	}
}

void* inicializadorMoveEAtualizaNoTempo()
{
	while (true)
		if ( ((clock() - t1 / CLOCKS_PER_SEC)*1000*1000) >= 1.0/FPS)
		{
			t1 = clock();
			move_inimigos();
			atualiza_inimigos();
		}
}

//envia informacoes para clientes
void atualiza_inimigos()
{
	int i;
	for (i = 0; i < num_inimigos; i++)
	{
		pthread_mutex_lock(&inimigos[i].mtx);

		struct Buffer buffer;
		buffer.tipo = 0;
		buffer.x = inimigos[i].x;
		buffer.y = inimigos[i].y;
//	buffer.angulo = inimigos[i].angulo;
		buffer.vida = inimigos[i].vida;
	
		pthread_mutex_unlock(&inimigos[i].mtx);

		write(sock, &buffer, sizeof(struct Buffer));
	}	
}
#endif
