#include "headers/menu.h"

struct Data
{
	int menuselection;
	int menuitemcount;
};

static const char* menuitems[] =
{
	"Jogar",
	"Sobre",
	"Sair"
};

typedef void (*funcptr)();

ALLEGRO_BITMAP *fundo;

struct Data data;

//assinaturas
int splash();
int loop_menu();
void jogar();
void sobre();
void sair();
int prepara_menu();
void finaliza_menu();

//metodos
void desenha_menu()
{
	al_draw_bitmap(fundo,0,0,0);
	al_draw_filled_rounded_rectangle(112,144,912,624, 40, 40,al_map_rgb(250,250,255));
	al_draw_rounded_rectangle(112,144,912,624, 40, 40,al_map_rgb(0,0,0),2);

}

int prepara_menu()
{
	//load de imagens
	fundo = al_load_bitmap("images/exemplo.jpg");
	if(!fundo)
	{
		fprintf(stderr, "failed to create fundo bitmap!\n");
			return -1;
	}
	
	//variaveis
	data.menuselection = 0;
	data.menuitemcount = sizeof(menuitems) / sizeof(const char*);

	doexit = false;
	refresh = false;
	al_set_target_bitmap(al_get_backbuffer(display));

	
	return 0;
}

void finaliza_menu()
{
	al_destroy_bitmap(fundo);
}

int splash()
{
	if(prepara_socket() == -1)
		return -1;

	if(prepara_allegro() == -1)
	{
		finaliza_rede();
		return -1;
	}

	if(prepara_menu() == -1)
	{
		finaliza_rede();
		finaliza_allegro();
		return -1;
	}

	desenha_menu();
	al_draw_text(font, al_map_rgb(0,0,0), (SCREEN_W/2),160 ,ALLEGRO_ALIGN_CENTRE, "Mummy Rush!");

	al_flip_display();

	return 0;
}

void conecta_server(int *cur_pos, ALLEGRO_USTR *input)
{
	//grava end. server
	server = malloc(*cur_pos+2);
	for(int i = 0; i < *cur_pos+1; i++)
		server[i] = al_ustr_get(input,i);
	server[*cur_pos+1] = '\0';
	if(prepara_server(server) == -1)//server invalido
	{
		//apaga string e atualiza valores
		for(int i = *cur_pos+1; i >-1; i--)
			al_ustr_remove_chr(input, i);
		*cur_pos = -1;
		doexit = false;
		free(server);

		//desenha "server invalido"
		desenha_menu();
		al_draw_text(font, al_map_rgb(0,0,0), (SCREEN_W/2),200 ,ALLEGRO_ALIGN_CENTRE, "Endereço do Servidor");
		al_draw_text(font, al_map_rgb(47,79,47), (SCREEN_W/2),400 ,ALLEGRO_ALIGN_CENTRE, "servidor invalido!");
		al_flip_display();
		al_rest(2);
	}
	else//server valido
	{
		//sai de todos os menus
		free(server);
		doexit = true;
	}

}

void jogar()//tela Endereco Servidor
{
	ALLEGRO_USTR *input = al_ustr_new("");

	//posicao na string - necessaria para apagar
  int cur_pos = -1;

	doexit = false;
	ALLEGRO_EVENT ev;

	while (!doexit)
	{
		al_wait_for_event(event_queue, &ev);
		if(ev.type == ALLEGRO_EVENT_TIMER)
			refresh = true;
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			sair();

		else if(ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch(ev.keyboard.keycode)
			{
				//palavra completa, tenta conexao com server
				case ALLEGRO_KEY_ENTER:
				conecta_server(&cur_pos, input);
				break;	

				case ALLEGRO_KEY_ESCAPE:
				doexit = true;
				break;	

				case ALLEGRO_KEY_BACKSPACE:
				if(cur_pos == -1)
					cur_pos = 0;
				// chop the string
				al_ustr_remove_chr(input, cur_pos); 
				cur_pos --;
				break;
			}
		}

		else if(ev.type == ALLEGRO_EVENT_KEY_CHAR)
		{
			int unichar = ev.keyboard.unichar;
			if (unichar >= 32)
			{
				al_ustr_append_chr(input, unichar);
				cur_pos++;
			}
		}
		if(refresh && al_is_event_queue_empty(event_queue))
		{
			desenha_menu();
			al_draw_text(font, al_map_rgb(0,0,0), (SCREEN_W/2),200 ,ALLEGRO_ALIGN_CENTRE, "Endereço do Servidor");

			al_draw_ustr(font, al_map_rgb(47,79,47), (SCREEN_W/2),400 ,ALLEGRO_ALIGN_CENTRE, input);

			al_flip_display();
		}
	}

	al_ustr_free(input);
	if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
	{
		//volta para menu
		doexit = false;
	}
}

void sobre()
{
	//desenha nomes
	desenha_menu();
	al_draw_text(font, al_map_rgb(0,0,0), (SCREEN_W/2),150 ,ALLEGRO_ALIGN_CENTRE, "Programaçao");
	al_draw_text(font, al_map_rgb(50,50,50), (SCREEN_W/2),230 ,ALLEGRO_ALIGN_CENTRE, "Lucas Chao");
	al_draw_text(font, al_map_rgb(0,0,0), (SCREEN_W/2),330 ,ALLEGRO_ALIGN_CENTRE, "Design");
	al_draw_text(font, al_map_rgb(50,50,50), (SCREEN_W/2),410 ,ALLEGRO_ALIGN_CENTRE, "Renan Valeiro");

	al_draw_text(font, al_map_rgb(47,79,47), (SCREEN_W/2),500 ,ALLEGRO_ALIGN_CENTRE, "Voltar");
	al_flip_display();

	//aguarda tecla enter
	while(!doexit)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			doexit = true;
		
		else if(ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch(ev.keyboard.keycode)
			{
				case ALLEGRO_KEY_ENTER:
				doexit = true;
				break;

				case ALLEGRO_KEY_BACKSPACE:
				doexit = true;
				break;

				case ALLEGRO_KEY_ESCAPE:
				doexit = true;
				break;
			}
		}
	}
	//volta para menu
	doexit = false;
}

void sair()
{
	finaliza_rede();
	finaliza_menu();
	finaliza_allegro();
}

void menu_item_selected()
{
	static funcptr actions[] =
	{
		&jogar,
		&sobre,
		&sair
	};

	actions[data.menuselection]();
}

void display_main_menu()
{
	const int menux = 512;
	const int menuy = 300;
	const int menugap = 100;

	ALLEGRO_COLOR selectedmenuitemcolor = al_map_rgb(47,79,47);
	ALLEGRO_COLOR unselectedmenuitemcolor = al_map_rgb(128, 128, 127);
	al_draw_text(font, al_map_rgb(0,0,0), (SCREEN_W/2),160 ,ALLEGRO_ALIGN_CENTRE, "Mummy Rush!");


	for (int index = 0; index < data.menuitemcount; index++)
	{
		if (index == data.menuselection)
		{
			// this item is selected, we draw its text with a highlight
			al_draw_text(font, selectedmenuitemcolor, menux, menuy + index * menugap ,ALLEGRO_ALIGN_CENTRE, menuitems[index]);
		}
		else
		{
			// the item is not selected, we just draw its text
			al_draw_text(font, unselectedmenuitemcolor, menux, menuy + index * menugap ,ALLEGRO_ALIGN_CENTRE, menuitems[index]);
		}
	}
	
}

void update_main_menu(bool key[])
{
	static bool upkey = false;
	static bool downkey = false;
	static bool enterkey = false;

	if (key[UP])
	{
		if (!upkey)
		{
			upkey = true;
		}
	}
	else
	{
		if (upkey)
		{
			if (--data.menuselection < 0)
			{
				data.menuselection = data.menuitemcount - 1;
			}
			upkey = false;
		}
	}

	if (key[DOWN])
	{
		if (!downkey)
		{
			downkey = true;
		}
	}
	else
	{
		if (downkey)
		{
			if (++data.menuselection == data.menuitemcount)
			{
				data.menuselection = 0;
			}
			downkey = false;
		}
	}

	if (key[ENTER])
	{
		if (!enterkey)
		{
			enterkey = true;
		}
	}
	else
	{
		if (enterkey)
		{
			menu_item_selected();
			enterkey = false;
		}
	}
}

int loop_menu()
{
	int pronto = 0, teste, start = 0;//cliente pronto, server pronto para ler, server pronto - 0 nao /9 sim
	bool key[3] = { true, false, false };

	while(!doexit)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		if(ev.type == ALLEGRO_EVENT_TIMER)
		{
			update_main_menu(key);
			refresh = true;
		}
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			doexit = true;
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch(ev.keyboard.keycode)
			{
				case ALLEGRO_KEY_UP:
				key[UP] = true;
				break;

				case ALLEGRO_KEY_DOWN:
				key[DOWN] = true;
				break;

				case ALLEGRO_KEY_ENTER:
				key[ENTER] = true;
				break;
			}
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch(ev.keyboard.keycode)
			{
				case ALLEGRO_KEY_UP:
				key[UP] = false;
				break;

				case ALLEGRO_KEY_DOWN:
				key[DOWN] = false;
				break;

				case ALLEGRO_KEY_ENTER:
				key[ENTER] = false;
				break;
			}
		}
		if(refresh && al_is_event_queue_empty(event_queue))
		{
			desenha_menu();
			display_main_menu();
			al_flip_display();
		}
	}

	desenha_menu();

	//tela aguardando jogadores
	doexit = false;
	
	while(!doexit)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		if(ev.type == ALLEGRO_EVENT_TIMER)
		{
			refresh = true;
		}
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			sair();
		else if(ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch(ev.keyboard.keycode)
			{
				case ALLEGRO_KEY_ENTER:
				pronto = 9;
				write(sock, &pronto, 1);//envia confirmacao para server
				doexit = true;
				break;
			}
		}
		if(refresh && al_is_event_queue_empty(event_queue))
		{
			desenha_menu();

			al_draw_text(font, al_map_rgb(0,0,0), (SCREEN_W/2),200 ,ALLEGRO_ALIGN_CENTRE, "Aguardando Jogadores");
			al_draw_text(font, al_map_rgb(0,0,0), (SCREEN_W/2),400 ,ALLEGRO_ALIGN_CENTRE, "Pressione Enter");
			
			al_flip_display();
		}
	}

	if(pronto == 9)
	{
		desenha_menu();
		al_draw_text(font, al_map_rgb(0,0,0), (SCREEN_W/2),200 ,ALLEGRO_ALIGN_CENTRE, "Aguardando Jogadores");
		al_draw_text(font, al_map_rgb(0,0,0), (SCREEN_W/2),400 ,ALLEGRO_ALIGN_CENTRE, "Aguarde!");
		al_flip_display();
	}

	FD_ZERO(&servidor);
	FD_SET(sock, &servidor);

	while(start == 0)
	{
		teste = select(FD_SETSIZE, &servidor, NULL, NULL, NULL);
		if(teste == -1)
			printf("Erro ao tentar selecionar socket!\n");
		else if(teste > 0)
		{
			read(sock, &start, 1);
			printf("numero de jogadores: %d\n", start);
		}
	}

	desenha_menu();
	al_draw_text(font, al_map_rgb(0,0,0), (SCREEN_W/2),200 ,ALLEGRO_ALIGN_CENTRE, "Boa sorte!");
	al_flip_display();
	al_rest(3);
	
	finaliza_menu();
	return start;
}

