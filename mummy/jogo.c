#include "headers/jogo.h"

//background
ALLEGRO_BITMAP *bg;

//ASSINATURAS
void desenha_bg();


int prepara_jogo()
{
	//prepara variaveis de players
	if(prepara_jogadores() == -1)
		return -1;

	//prepara variaveis de bala
	prepara_tiro();

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
}

void atualiza()
{
	move_personagem();
	move_tiro();
	move_mira();
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
//	al_draw_textf(text, al_map_rgb(0,0,0), 200, 8,ALLEGRO_ALIGN_CENTRE, "%d",num_inimigos);

	al_draw_text(text, al_map_rgb(0,0,0), 760, 8,ALLEGRO_ALIGN_CENTRE, "ammo:");
	al_draw_textf(text, al_map_rgb(0,0,0), 850, 8,ALLEGRO_ALIGN_CENTRE, "%d",jogadores[num_jogadores-1].ammo);

}
	
void desenha()
{
	desenha_bg();
	desenha_jogadores();
	desenha_tiro();
	//desenha_inimigos();
}
