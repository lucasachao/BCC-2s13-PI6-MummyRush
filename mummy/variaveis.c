#include "headers/variaveis.h"

int prepara_allegro()
{
	//inicializa componentes do allegro
	if(!al_init())
	{
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

	if(!al_init_image_addon())
		fprintf(stderr,"Failed to initialize image addon!\n");

	if(!al_init_primitives_addon())
		fprintf(stderr,"Failed to initialize primitives addon!\n");

	al_init_font_addon();
	al_init_ttf_addon();

	if(!al_install_keyboard())
	{
		fprintf(stderr, "failed to initialize the keyboard!\n");
	return -1;
	}

	//instancia variaveis do allegro 
	timer = al_create_timer(1.0 / FPS);
	if(!timer)
	{
		fprintf(stderr, "failed to create timer!\n");
	return -1;
	}

	display = al_create_display(SCREEN_W, SCREEN_H);
	if(!display)
	{
		fprintf(stderr, "failed to create display!\n");
		al_destroy_timer(timer);
		return -1;
	}

	event_queue = al_create_event_queue();
	if(!event_queue)
	{
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}

	text = al_load_ttf_font("fonts/BebasNeue.otf",40,0 );
	if (!text)
	{
		fprintf(stderr, "Could not load text.\n");
		al_destroy_display(display);
		al_destroy_timer(timer);
		al_destroy_event_queue(event_queue);
		return -1;
	}

	font = al_load_ttf_font("fonts/BebasNeue.otf",100,0 );
	if (!font)
	{
		fprintf(stderr, "Could not load font.\n");
		al_destroy_display(display);
		al_destroy_timer(timer);
		al_destroy_event_queue(event_queue);
		al_destroy_font(font);
		return -1;
	}

	//registra fonte de eventos
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_start_timer(timer);

	return 0;
}

void finaliza_allegro()
{
	al_destroy_font(font);
	al_destroy_font(text);
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	al_unregister_event_source(event_queue, al_get_keyboard_event_source());
	al_unregister_event_source(event_queue, al_get_display_event_source(display));
	al_unregister_event_source(event_queue, al_get_timer_event_source(timer));
	al_shutdown_font_addon();
	al_shutdown_primitives_addon();
	al_shutdown_image_addon();
	al_uninstall_keyboard();

	exit(0);
}
