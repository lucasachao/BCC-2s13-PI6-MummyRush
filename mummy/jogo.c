#include "headers/jogo.h"

//background
ALLEGRO_BITMAP *bg;

//ASSINATURAS
void desenha_bg();


int prepara_jogo()
{
	//prepara variaveis de players
	if(prepara_jogadores_client() == -1)
		return -1;

	//prepara variaveis de bala
	prepara_tiro();

	//prepara variaveis de inimigos
	prepara_inimigos_client(5);

	//load de mapa
	bg = al_load_bitmap("images/exemplo.jpg");
	if(!bg)
	{
		fprintf(stderr, "Failed to load background!\n");
		return -1;
	}

	//gera matriz de colisao para mapa
	gera_matriz();

	//prepara mapa inicial
	desenha();

	return 0;
}

void finaliza_jogo()
{
	al_destroy_bitmap(bg);
	finaliza_jogadores();

	for(int i = 0; i < 300; i++)
		pthread_mutex_destroy(&tiro[i].mtx);
	finaliza_inimigos_client();
}

void verifica_colisao_tiro()
{
	for(int i = 0; i < 300; i++)
	{
		pthread_mutex_lock(&tiro[i].mtx);
		if(tiro[i].ativa)
		{
			for(int j = 0; j < num_inimigos; j++)
			{
				pthread_mutex_lock(&inimigos[j].mtx);
				if(inimigos[j].vida > 0)
					if(tiro[i].x >= inimigos[j].x && tiro[i].x <= inimigos[j].x + BOUNCER_SIZE)
						if(tiro[i].y >= inimigos[j].y && tiro[i].y <= inimigos[j].y + BOUNCER_SIZE)
						{
							inimigos[j].vida --;
							struct Buffer b;
							b.tipo = 2;
							b.id = j;
							b.vida = inimigos[j].vida;
							write(sock, &b, sizeof(struct Buffer));
							tiro[i].ativa = false;
						}
				pthread_mutex_unlock(&inimigos[j].mtx);
			}
		}
		pthread_mutex_unlock(&tiro[i].mtx);
	}
}

void verifica_colisao_jogador()
{
	int id = 0;
	//pega index do jogador no vetor
	for(int i = 0; i < num_jogadores; i++)
		if(jogadores[i].id == id_jogador)
			id = i;

	for(int i = 0; i < num_inimigos; i++)
	{
		pthread_mutex_lock(&inimigos[i].mtx);
		if(inimigos[i].vida > 0)
			if((inimigos[i].x >= jogadores[id].x && inimigos[i].x <= jogadores[id].x + BOUNCER_SIZE) || (inimigos[i].x + BOUNCER_SIZE >= jogadores[id].x && inimigos[i].x + BOUNCER_SIZE <= jogadores[id].x + BOUNCER_SIZE))
					if((inimigos[i].y >= jogadores[id].y && inimigos[i].y <= jogadores[id].y + BOUNCER_SIZE) || (inimigos[i].y + BOUNCER_SIZE >= jogadores[id].y && inimigos[i].y + BOUNCER_SIZE <= jogadores[id].y + BOUNCER_SIZE))
						jogadores[id].vida --;
		pthread_mutex_unlock(&inimigos[i].mtx);
	}
}

void atualiza()
{
	move_personagem();
	move_tiro();
	move_mira();
	verifica_colisao_tiro();
	verifica_colisao_jogador();
	verifica_inimigos_vivos();
}

void desenha_bg()
{
	//fundo
	al_draw_bitmap(bg,0,0,0);

	//informacoes
	al_draw_filled_rounded_rectangle(112,7,912,50, 10, 10,al_map_rgb(189,183,107));
	al_draw_rounded_rectangle(112,7,912,50, 10, 10,al_map_rgb(0,0,0),1);

	al_draw_text(text, al_map_rgb(0,0,0), 150, 8,ALLEGRO_ALIGN_CENTRE, "life:");

	al_draw_textf(text, al_map_rgb(0,0,0), 200, 8,ALLEGRO_ALIGN_CENTRE, "%d",jogadores[num_jogadores-1].vida);

	al_draw_text(text, al_map_rgb(0,0,0), 410, 8,ALLEGRO_ALIGN_CENTRE, "inimigos:");
	al_draw_textf(text, al_map_rgb(0,0,0), 500, 8,ALLEGRO_ALIGN_CENTRE, "%d",num_inimigos_vivos);

	al_draw_text(text, al_map_rgb(0,0,0), 760, 8,ALLEGRO_ALIGN_CENTRE, "ammo:");
	al_draw_textf(text, al_map_rgb(0,0,0), 850, 8,ALLEGRO_ALIGN_CENTRE, "%d",jogadores[num_jogadores-1].ammo);

}

void desenha_fim()
{
	desenha_bg();
	al_draw_text(font, al_map_rgb(0,0,0), 512, 300,ALLEGRO_ALIGN_CENTRE, "FIM DE JOGO!");
	al_flip_display();
	al_rest(3);
}

void desenha()
{
	desenha_bg();
	desenha_jogadores();
	desenha_tiro();
	desenha_inimigos();
}
