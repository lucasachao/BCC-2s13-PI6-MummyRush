#ifndef INIMIGO_C
#define INIMIGO_C
#include "headers/inimigo.h"
#include <time.h>
int count;

void novo_round(int qtd_inimigos)
{
	num_inimigos = qtd_inimigos;
	num_inimigos_vivos = num_inimigos;
	inimigos = malloc(sizeof(struct Inimigo)*num_inimigos);
	int gap = 40;
	for(int i = 0; i < num_inimigos; i++)
	{
		inimigos[i].id = i;
		inimigos[i].vida = 2;
		inimigos[i].angulo = 2;
		inimigos[i].x = 50+gap*i;
		inimigos[i].dx = 0;
		inimigos[i].dy = 0;
		inimigos[i].y = 500;
		inimigos[i].passo = 0;
		inimigos[i].movimento = false;
		pthread_mutex_init(&inimigos[i].mtx, NULL);
	}
}

void prepara_inimigos_server(int qtd_inimigos, int qtd_jogadores, int id[])
{
//printf("comecou\n");
	al_init();
	al_init_image_addon();
	tempo = al_create_timer(1.0 / FPS);
	queue_inimigos = al_create_event_queue();
	al_register_event_source(queue_inimigos, al_get_timer_event_source(tempo));
	al_start_timer(tempo);

//printf("passou allegro\n");
	gera_matriz();
//printf("passou matriz\n");
	num_inimigos = qtd_inimigos;
	num_inimigos_vivos = num_inimigos;
	inimigos = malloc(sizeof(struct Inimigo)*num_inimigos);
	int gap = 40;
	for(int i = 0; i < num_inimigos; i++)
	{
		inimigos[i].id = i;
		inimigos[i].vida = 1;
		inimigos[i].angulo = 2;
		inimigos[i].x = 50+gap*i;
		inimigos[i].dx = 0;
		inimigos[i].dy = 0;
		inimigos[i].y = 500;
		inimigos[i].passo = 0;
		inimigos[i].movimento = false;
		pthread_mutex_init(&inimigos[i].mtx, NULL);
	}
//printf("passou inimigos\n");
	jogadores_s = malloc(sizeof(struct Jogador_s)*qtd_jogadores);
	for(int i = 0; i < qtd_jogadores; i++)
	{
		jogadores_s[i].id = id[i];
		jogadores_s[i].vida = 100;
		jogadores_s[i].x = 50;
		jogadores_s[i].y = 150;
		pthread_mutex_init(&jogadores_s[i].mtx, NULL);
//printf("passou jogador %d\n", id[i]);
	}
//printf("passou jogadores\n");
}

void prepara_inimigos_client(int qtd_inimigos)
{
	im_ini = al_load_bitmap("images/inimigos.png");
	count = 0;
	num_inimigos = qtd_inimigos;
	num_inimigos_vivos = num_inimigos;
	inimigos = malloc(sizeof(struct Inimigo)*num_inimigos);
	int gap = 40;
	for(int i = 0; i < num_inimigos; i++)
	{
		inimigos[i].id = i;
		inimigos[i].vida = 1;
		inimigos[i].angulo = 2;
		inimigos[i].x = 50+gap*i;
		inimigos[i].y = 500;
		inimigos[i].passo = 0;
		inimigos[i].movimento = false;
		pthread_mutex_init(&inimigos[i].mtx, NULL);
		inimigos[i].bouncer = NULL;

		inimigos[i].bouncer = al_create_sub_bitmap(im_ini, 1*BOUNCER_SIZE, 0, BOUNCER_SIZE, BOUNCER_SIZE);
//printf("passou load inimigo \n");
	}
//printf("passou prepara_inimigo \n");
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
	al_destroy_bitmap(im_ini);
}

bool verifica_colisao_inimigos(int id)
{
	bool verifica = true;
	for(int i = 0; i < num_inimigos; i++)
		if(i != id && inimigos[i].vida > 0)
			if((inimigos[id].x >= inimigos[i].x && inimigos[id].x <= inimigos[i].x + BOUNCER_SIZE) || (inimigos[id].x + BOUNCER_SIZE >= inimigos[i].x && inimigos[id].x + BOUNCER_SIZE <= inimigos[i].x + BOUNCER_SIZE))
				if((inimigos[id].y >= inimigos[i].y && inimigos[id].y <= inimigos[i].y + BOUNCER_SIZE) || (inimigos[id].y + BOUNCER_SIZE >= inimigos[i].y && inimigos[id].y + BOUNCER_SIZE <= inimigos[i].y + BOUNCER_SIZE))
					verifica = false;

//printf("%s", verifica ? "true" : "false");
	return verifica;
}

float getDist(int i, int j)
{
	pthread_mutex_lock(&inimigos[i].mtx);
	pthread_mutex_lock(&jogadores_s[j].mtx);

	float dist = sqrtf(powf((inimigos[i].x - jogadores_s[j].x),2) + powf((inimigos[i].y - jogadores_s[j].y),2));

	pthread_mutex_unlock(&inimigos[i].mtx);
	pthread_mutex_unlock(&jogadores_s[j].mtx);

	return dist;
}

void move_inimigos(int qtd_jogadores, int id_jog[])
{
	//Define o destino de cada mumia
	for (int i = 0; i < num_inimigos; i++)
	{
		int minIndice = 0;
		float minG = 1000000;
		for (int j = 0; j < qtd_jogadores; j++)
		{
			float minAtual = getDist(i, j);
			if (minAtual < minG)
			{
				minG = minAtual;
				minIndice = j;
			}
		}
//printf("passou getDist\n");

		pthread_mutex_lock(&inimigos[i].mtx);
		pthread_mutex_lock(&jogadores_s[minIndice].mtx);

		inimigos[i].dx = jogadores_s[minIndice].x;
		inimigos[i].dy = jogadores_s[minIndice].y;
//printf("objetivo inimigo: x%f y%f\n", inimigos[i].dx, inimigos[i].dy);
//printf("jogador perseguido: x%f y%f\n", jogadores_s[minIndice].x, jogadores_s[minIndice].y);

		pthread_mutex_unlock(&inimigos[i].mtx);
		pthread_mutex_unlock(&jogadores_s[minIndice].mtx);
	}
	int velocidade = 1;

//printf("passou mutex\n");
	//Movimenta cada mumia 1 pixel na direcao definida
	for (int i = 0; i < num_inimigos; i++)
	{
		pthread_mutex_lock(&inimigos[i].mtx);
		//direita
		if (inimigos[i].x < inimigos[i].dx && verifica_colisao(inimigos[i].x,inimigos[i].y, BOUNCER_SIZE, 3, velocidade))
		{
			inimigos[i].x += velocidade;
			//verifica se inimigo colidiu com outro inimigo
			if(verifica_colisao_inimigos(i))
				inimigos[i].movimento = true;
			else
				inimigos[i].x -= velocidade;
		}
		//esquerda
		else if (inimigos[i].x > inimigos[i].dx && verifica_colisao(inimigos[i].x,inimigos[i].y, BOUNCER_SIZE, 2, velocidade))
		{
			inimigos[i].x -= velocidade;
			if(verifica_colisao_inimigos(i))
				inimigos[i].movimento = true;
			else
				inimigos[i].x += velocidade;
		}
		//baixo
		if (inimigos[i].y < inimigos[i].dy && verifica_colisao(inimigos[i].x,inimigos[i].y, BOUNCER_SIZE, 1, velocidade))
		{
			inimigos[i].y += velocidade;
			if(verifica_colisao_inimigos(i))
				inimigos[i].movimento = true;
			else
				inimigos[i].y -= velocidade;
		}
		//cima
		else if (inimigos[i].y > inimigos[i].dy && verifica_colisao(inimigos[i].x,inimigos[i].y, BOUNCER_SIZE, 0, velocidade))
		{
			inimigos[i].y -= velocidade;
			if(verifica_colisao_inimigos(i))
				inimigos[i].movimento = true;
			else
				inimigos[i].y += velocidade;
		}

//printf("%d ", inimigos[i].vida);
		//envia inimigo para clientes
		if(inimigos[i].movimento == true && inimigos[i].vida > 0)
		{
			//coloca info do inimigo que se mexeu no buffer
			struct Buffer buffer;
			buffer.tipo = 2;
			buffer.id = i;
			buffer.x = inimigos[i].x;
			buffer.y = inimigos[i].y;
			buffer.angulo = 2;//arrumar!!
			buffer.vida = inimigos[i].vida;

			//envia info do inimigo que se mexeu para os clients
			for(int j = 0; j < qtd_jogadores; j++)
				write(id_jog[j], &buffer, sizeof(struct Buffer));
	
			//printf("info enviada!!\n");
			inimigos[i].movimento = false;
		}
		pthread_mutex_unlock(&inimigos[i].mtx);

	}
//printf("\n");
}

void desenha_inimigos()
{
	int sprite = 0;
	for(int i = 0; i < num_inimigos; i++)
	{
		pthread_mutex_lock(&inimigos[i].mtx);
//printf("%d ", inimigos[i].vida);
		if(inimigos[i].vida > 0)
		{
			if(count > 10) sprite = 1;
			if(count > 20) sprite = 3;
			if(count > 30) sprite = 2;
			if(count > 40) count = 0;
			al_destroy_bitmap(inimigos[i].bouncer);
			inimigos[i].bouncer = al_create_sub_bitmap(im_ini, 1*BOUNCER_SIZE, sprite*BOUNCER_SIZE, BOUNCER_SIZE, BOUNCER_SIZE);
			al_draw_bitmap(inimigos[i].bouncer, inimigos[i].x, inimigos[i].y, 0);
			count ++;
		}
		pthread_mutex_unlock(&inimigos[i].mtx);
	}
//printf("\n");
}

//recebe info do inimigo do server e atualiza array para desenhar
void atualiza_inimigo_client(struct Buffer buffer)
{
	pthread_mutex_lock(&inimigos[buffer.id].mtx);

	inimigos[buffer.id].x = buffer.x;
	inimigos[buffer.id].y = buffer.y;
	inimigos[buffer.id].angulo = buffer.angulo;
	inimigos[buffer.id].vida = buffer.vida;

	pthread_mutex_unlock(&inimigos[buffer.id].mtx);

}


void verifica_inimigos_vivos()
{
	num_inimigos_vivos = 0;
	for(int i = 0; i < num_inimigos; i++)
	{
		pthread_mutex_lock(&inimigos[i].mtx);
		if(inimigos[i].vida > 0)
			num_inimigos_vivos++;
		pthread_mutex_unlock(&inimigos[i].mtx);
	}
}

bool verifica_fim()
{
	for(int i = 0; i < num_inimigos; i++)
		if(inimigos[i].vida > 0)
			return true;

	return false;
}
#endif
