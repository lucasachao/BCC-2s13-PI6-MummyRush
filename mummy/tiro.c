#include "headers/tiro.h"

void prepara_tiro()
{
	velocidade_tiro = 10;

	for(int i = 0; i < 300; i++)
		tiro[i].ativa = false;
}

void adiciona_tiro(int angulo, float x, float y, int aux)
{
	//aux - 0 se o cliente atirou, 1 se foi info recebida do server de outros clientes
	for(int i = 0; i < 300; i++)
	{
		pthread_mutex_lock(&tiro[i].mtx);
		if(!tiro[i].ativa)
		{
			tiro[i].ativa = true;
			tiro[i].x = x;
			tiro[i].y = y;
			tiro[i].angulo = angulo;

			struct Buffer buffer;
			buffer.tipo = 1;
			buffer.x = x;
			buffer.y = y;
			buffer.id = 0;
			buffer.angulo = angulo;
			buffer.vida = 1;

			if(aux == 0)
				write(sock, &buffer, sizeof(struct Buffer));

			pthread_mutex_unlock(&tiro[i].mtx);
			break;
		}
		pthread_mutex_unlock(&tiro[i].mtx);
	}
}

void move_tiro()
{
	for(int i = 0; i < 300; i++)
	{
		pthread_mutex_lock(&tiro[i].mtx);
		if(tiro[i].ativa)
		{
			if(!verifica_colisao(tiro[i].x,tiro[i].y, 2, 0, 0))
					tiro[i].ativa = false;
			else
			{
				switch(tiro[i].angulo)
				{
					case 0://cima
					tiro[i].y -= velocidade_tiro;
					break;

					case 1:
					tiro[i].y -= velocidade_tiro*0.7;
					tiro[i].x += velocidade_tiro*0.7;
					break;

					case 2://direita
					tiro[i].x += velocidade_tiro;
					break;

					case 3:
					tiro[i].x += velocidade_tiro*0.7;
					tiro[i].y += velocidade_tiro*0.7;
					break;

					case 4://baixo
					tiro[i].y += velocidade_tiro;
					break;

					case 5:
					tiro[i].x -= velocidade_tiro*0.7;
					tiro[i].y += velocidade_tiro*0.7;
					break;

					case 6://esquerda
					tiro[i].x -= velocidade_tiro;
					break;

					case 7:
					tiro[i].x -= velocidade_tiro*0.7;
					tiro[i].y -= velocidade_tiro*0.7;
					break;
				}
			}
			refresh = true;
		}
		pthread_mutex_unlock(&tiro[i].mtx);
	}
}

void desenha_tiro()
{
	for(int i = 0; i < 300; i++)
	{
		pthread_mutex_lock(&tiro[i].mtx);
		if(tiro[i].ativa)
			al_draw_filled_circle(tiro[i].x, tiro[i].y, 2, al_map_rgb(0,0,0));

		pthread_mutex_unlock(&tiro[i].mtx);
	}
}
