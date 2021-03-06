#include "headers/jogador.h"

int velocidade;

int prepara_jogadores_client()
{
	im_jog = al_load_bitmap("images/super_shade.png");
	for(int i = 0; i < 4; i++)
	{
		pos[i] = false;
		mir[i] = false;
	}

	//limpa jogadores
	for(int i = 0; i < 6; i++)
	{
		jogadores[i].id = 0;
		jogadores[i].score = 0;
		jogadores[i].vida = 0;
		jogadores[i].angulo = 2;
		jogadores[i].x = 0;
		jogadores[i].y = 0;
		jogadores[i].ammo = 100;
		jogadores[i].passo = 0;
		jogadores[i].bouncer = NULL;
	}

	for(int i = 0; i < num_jogadores; i++)
		pthread_mutex_init(&jogadores[i].mtx, NULL);

	int gap = 20;
	//preenche somente os que vao jogar
	//jogador do cliente
	jogadores[num_jogadores - 1].id = id_jogador;
	jogadores[num_jogadores - 1].score = 0;
	jogadores[num_jogadores - 1].vida = 100;
	jogadores[num_jogadores - 1].angulo = 2;
	jogadores[num_jogadores - 1].x = 50*id_jogador;
	jogadores[num_jogadores - 1].y = 150;
	jogadores[num_jogadores - 1].ammo = 100;
	jogadores[num_jogadores - 1].passo = 0;
	jogadores[num_jogadores - 1].bouncer = al_create_sub_bitmap(im_jog, 0, 0, BOUNCER_SIZE, BOUNCER_SIZE);

	al_set_target_bitmap(al_get_backbuffer(display));

	//outros jogadores
	for(int i = 0; i < num_jogadores - 1; i++)
	{
		jogadores[i].id = id_jogadores[i];
		jogadores[i].score = 0;
		jogadores[i].vida = 100;
		jogadores[i].angulo = 2;
		jogadores[i].x = 50 + gap*(i+1);
		jogadores[i].y = 150;
		jogadores[i].ammo = 100;
		jogadores[i].passo = 0;
		jogadores[i].bouncer = al_create_sub_bitmap(im_jog, 0, 0, BOUNCER_SIZE, BOUNCER_SIZE);

	}
	//count_balas = 0;
	velocidade = 2;
	movimento = false;

	return 0;
}

void finaliza_jogadores()
{
	for(int i = 0; i < num_jogadores; i++)
	{
		al_destroy_bitmap(jogadores[i].bouncer);
		pthread_mutex_destroy(&jogadores[i].mtx);
	}
	al_destroy_bitmap(im_jog);
}

void atualiza_jogadores(struct Buffer buffer)
{
	//recebe info dos jogadores do servidor e coloca no array
	//printf("recebeu %d\n", buffer.id);
	for(int i = 0; i < num_jogadores; i++)
	{
		pthread_mutex_lock(&jogadores[i].mtx);
		if(jogadores[i].id == buffer.id)
		{
			//printf("%d", jogadores[i].id);
			jogadores[i].vida = buffer.vida;
			jogadores[i].x = buffer.x;
			jogadores[i].y = buffer.y;
			jogadores[i].angulo = buffer.angulo;
			//printf("atualizou!\n");
			pthread_mutex_unlock(&jogadores[i].mtx);
			break;
		}
		pthread_mutex_unlock(&jogadores[i].mtx);
	}
	refresh = true;

}

void atualiza_jogador()
{
	//envia informacoes do jogador para o servidor
	struct Buffer buffer;
	buffer.tipo = 0;
	buffer.id = id_jogador;
	buffer.x = jogadores[num_jogadores-1].x;
	buffer.y = jogadores[num_jogadores-1].y;
	buffer.angulo = jogadores[num_jogadores-1].angulo;
	buffer.vida = jogadores[num_jogadores-1].vida;
	
	write(sock, &buffer, sizeof(struct Buffer));

}

void desenha_jogadores()
{
	for(int i = 0; i < num_jogadores; i++)
	{
		pthread_mutex_lock(&jogadores[i].mtx);
		if(jogadores[i].vida != 0)
		{
			al_destroy_bitmap(jogadores[i].bouncer);
			//escolhe sprite adequado de bouncer
			switch(jogadores[i].angulo)
			{
				case 0:
				jogadores[i].bouncer = al_create_sub_bitmap(im_jog, 0, 3*BOUNCER_SIZE, BOUNCER_SIZE, BOUNCER_SIZE);
				break;

				case 1:
				jogadores[i].bouncer = al_create_sub_bitmap(im_jog, 0, 3*BOUNCER_SIZE, BOUNCER_SIZE, BOUNCER_SIZE);
				break;

				case 2:
				jogadores[i].bouncer = al_create_sub_bitmap(im_jog, 0, 2*BOUNCER_SIZE, BOUNCER_SIZE, BOUNCER_SIZE);
				break;

				case 3:
				jogadores[i].bouncer = al_create_sub_bitmap(im_jog, 0, 0, BOUNCER_SIZE, BOUNCER_SIZE);
				break;

				case 4:
				jogadores[i].bouncer = al_create_sub_bitmap(im_jog, 0, 0, BOUNCER_SIZE, BOUNCER_SIZE);
				break;

				case 5:
				jogadores[i].bouncer = al_create_sub_bitmap(im_jog, 0, 0, BOUNCER_SIZE, BOUNCER_SIZE);
				break;

				case 6:
				jogadores[i].bouncer = al_create_sub_bitmap(im_jog, 0, 1*BOUNCER_SIZE, BOUNCER_SIZE, BOUNCER_SIZE);
				break;

				case 7:
				jogadores[i].bouncer = al_create_sub_bitmap(im_jog, 0, 3*BOUNCER_SIZE, BOUNCER_SIZE, BOUNCER_SIZE);
				break;
			}
			al_draw_bitmap(jogadores[i].bouncer,jogadores[i].x, jogadores[i].y, 0);
			pthread_mutex_unlock(&jogadores[i].mtx);
		}
		else pthread_mutex_unlock(&jogadores[i].mtx);
	}
	
}

void move_personagem()
{
	if(pos[KEY_UP] && verifica_colisao(jogadores[num_jogadores-1].x,jogadores[num_jogadores-1].y, BOUNCER_SIZE, 0, velocidade))
	{
		if(pos[KEY_LEFT] || pos[KEY_RIGHT])
			jogadores[num_jogadores-1].y -= velocidade*0.7;
		else
			jogadores[num_jogadores-1].y -= velocidade;
		refresh = true;
		movimento = true;
	}
 
	if(pos[KEY_DOWN] && verifica_colisao(jogadores[num_jogadores-1].x,jogadores[num_jogadores-1].y, BOUNCER_SIZE, 1, velocidade))
	{
		if(pos[KEY_LEFT] || pos[KEY_RIGHT])
			jogadores[num_jogadores-1].y += velocidade*0.7;
		else
			jogadores[num_jogadores-1].y += velocidade;
		refresh = true;
		movimento = true;
	}
 
       	if(pos[KEY_LEFT] && verifica_colisao(jogadores[num_jogadores-1].x,jogadores[num_jogadores-1].y, BOUNCER_SIZE, 2, velocidade))
	{
		if(pos[KEY_UP] || pos[KEY_DOWN])
			jogadores[num_jogadores-1].x -= velocidade*0.7;
		else
			jogadores[num_jogadores-1].x -= velocidade;
		refresh = true;
		movimento = true;
	}
 
	if(pos[KEY_RIGHT] && verifica_colisao(jogadores[num_jogadores-1].x,jogadores[num_jogadores-1].y, BOUNCER_SIZE, 3, velocidade))
	{
		if(pos[KEY_UP] || pos[KEY_DOWN])
			jogadores[num_jogadores-1].x += velocidade*0.7;
		else
			jogadores[num_jogadores-1].x += velocidade;
		refresh = true;
		movimento = true;
	}
	if(movimento == true)
	{
		atualiza_jogador();
		movimento = false;
	}
}

void move_mira()
{
	if(mir[KEY_UP] || mir[KEY_DOWN] || mir[KEY_LEFT] || mir[KEY_RIGHT])
	{
		int mira_x = 0;
		int mira_y = 0;

		if(mir[KEY_UP])
			mira_y -= 20;
		if(mir[KEY_DOWN])
			mira_y += 20;
		if(mir[KEY_LEFT])
			mira_x -= 20;
		if(mir[KEY_RIGHT])
			mira_x += 20;

		if(mira_y == -20 && mira_x == 0)//cima
			jogadores[num_jogadores-1].angulo = 0;

		else if(mira_y == -20 && mira_x == 20)//cima direita
			jogadores[num_jogadores-1].angulo = 1;

		else if(mira_y == 0 && mira_x == 20)//direita
			jogadores[num_jogadores-1].angulo = 2;

		else if(mira_y == 20 && mira_x == 20)//baixo direita
			jogadores[num_jogadores-1].angulo = 3;

		else if(mira_y == 20 && mira_x == 0)//baixo
			jogadores[num_jogadores-1].angulo = 4;

		else if(mira_y == 20 && mira_x == -20)//baixo esquerda
			jogadores[num_jogadores-1].angulo = 5;

		else if(mira_y == 0 && mira_x == -20)//esquerda
			jogadores[num_jogadores-1].angulo = 6;

		else if(mira_y == -20 && mira_x == -20)//cima esquerda
			jogadores[num_jogadores-1].angulo = 7;
		else
			jogadores[num_jogadores-1].angulo = 2;//apertando teclas opostas - default direita

		atualiza_jogador();//envia jogador com novo angulo para server

		refresh = true;

	}
}
