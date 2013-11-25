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

void prepara_inimigos_server(int qtd_inimigos);
void prepara_inimigos_client(int qtd_inimigos);
void finaliza_inimigos_server();
void finaliza_inimigos_client();
void move_inimigos();
void desenha_inimigos();
void atualiza_inimigo_server(int id_inimigo);
void atualiza_inimigo_client(struct Buffer buffer);
void* atualiza_inimigos_tempo();

#endif
