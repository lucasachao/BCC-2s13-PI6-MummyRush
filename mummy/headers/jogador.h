#ifndef JOGADOR_
#define JOGADOR_

#include "colisao.h"
#include "variaveis.h"
#include "tiro.h"

#define BOUNCER_SIZE 16

struct Jogador{
	int id;
	int score;
	int vida;
	int angulo;
	float x;
	float y;
	int ammo;
	int arma;
	int passo;
	ALLEGRO_BITMAP *bouncer;
};

enum teclas_movimento {
   KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};

//VARIAVEIS
struct Jogador jogadores[6];
int num_jogadores, id_jogador;
int *id_jogadores;
bool movimento;

//teclas de mira de movimento
bool pos[4];
bool mir[4];

int prepara_jogadores();
void finaliza_jogadores();
void atualiza_jogadores(struct Buffer buffer);//pega info do server
void atualiza_jogador();//manda info pro server
void move_personagem();
void move_mira();
void desenha_jogadores();

#endif