#ifndef INIMIGO_
#define INIMIGO_

//#include "variaveis.h"
#include "colisao.h"
//#include "cliente.h"
#include "jogador.h"



struct Inimigo{
	int id;
	int vida;
	int angulo;
	float x;
	float y;
	int passo;
	ALLEGRO_BITMAP *bouncer;
	float dx;
	float dy;
	bool movimento;
	pthread_mutex_t mtx;
};
clock_t t1;

int num_inimigos;
struct Inimigo *inimigos;

void prepara_inimigos(int qtd_inimigos);
void finaliza_inimigos();
void move_inimigos();
void desenha_inimigos();
void atualiza_inimigos();
void* inicializadorMoveEAtualizaNoTempo();

#endif
