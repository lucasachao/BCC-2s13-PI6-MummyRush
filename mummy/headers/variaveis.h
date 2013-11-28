#ifndef VARIAVEIS_
#define VARIAVEIS_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <pthread.h>

#include "cliente.h"

#define SCREEN_W 1024
#define SCREEN_H 768
#define FPS 60.0
#define BOUNCER_SIZE 16

bool doexit;
bool refresh;
char* server;

//allegro stuff
ALLEGRO_DISPLAY *display;
ALLEGRO_EVENT_QUEUE *event_queue;
ALLEGRO_TIMER *timer;
ALLEGRO_FONT *text;
ALLEGRO_FONT *font;

int prepara_allegro();
void finaliza_allegro();

#endif
