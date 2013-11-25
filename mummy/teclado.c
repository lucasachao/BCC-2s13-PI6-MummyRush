#include "headers/teclado.h"

void tecla_down(ALLEGRO_EVENT ev)
{
	switch(ev.keyboard.keycode)
	{
		case ALLEGRO_KEY_W:
		pos[KEY_UP] = true;
		break;
 
		case ALLEGRO_KEY_S:
		pos[KEY_DOWN] = true;
		break;
 
		case ALLEGRO_KEY_A: 
		pos[KEY_LEFT] = true;
		break;
 
		case ALLEGRO_KEY_D:
		pos[KEY_RIGHT] = true;
		break;

		case ALLEGRO_KEY_UP:
		mir[KEY_UP] = true;
		break;
 
		case ALLEGRO_KEY_DOWN:
		mir[KEY_DOWN] = true;
		break;

		case ALLEGRO_KEY_LEFT: 
		mir[KEY_LEFT] = true;
		break;
 
		case ALLEGRO_KEY_RIGHT:
		mir[KEY_RIGHT] = true;
		break;	
	}
}

void tecla_up(ALLEGRO_EVENT ev)
{
	switch(ev.keyboard.keycode)
	{
		case ALLEGRO_KEY_W:
		pos[KEY_UP] = false;
		break;
 
		case ALLEGRO_KEY_S:
		pos[KEY_DOWN] = false;
		break;

		case ALLEGRO_KEY_A: 
		pos[KEY_LEFT] = false;
		break;
 
		case ALLEGRO_KEY_D:
		pos[KEY_RIGHT] = false;
		break;
 
		case ALLEGRO_KEY_UP:
		mir[KEY_UP] = false;
		break;
 
		case ALLEGRO_KEY_DOWN:
		mir[KEY_DOWN] = false;
		break;
 
		case ALLEGRO_KEY_LEFT: 
		mir[KEY_LEFT] = false;
		break;
 
		case ALLEGRO_KEY_RIGHT:
		mir[KEY_RIGHT] = false;
		break;
	}
	//se tiver municao e for uma tecla de mira, adiciona tiro e diminui municao
	if(jogadores[num_jogadores-1].ammo > 0 && ((ev.keyboard.keycode == ALLEGRO_KEY_UP) || (ev.keyboard.keycode == ALLEGRO_KEY_DOWN) || (ev.keyboard.keycode == ALLEGRO_KEY_LEFT) || (ev.keyboard.keycode == ALLEGRO_KEY_RIGHT)))
	{
		adiciona_tiro(jogadores[num_jogadores-1].angulo, jogadores[num_jogadores-1].x+BOUNCER_SIZE/2, jogadores[num_jogadores-1].y+BOUNCER_SIZE/2, 0);
		jogadores[num_jogadores-1].ammo --;
	}
}
