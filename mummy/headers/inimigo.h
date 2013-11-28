#ifndef INIMIGO_
#define INIMIGO_

#include "variaveis.h"
#include "colisao.h"

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

int num_inimigos;
struct Inimigo *inimigos;

//para servidor
struct Jogador_s{
	int id;
	float x;
	float y;
	int angulo;
	int vida;
	pthread_mutex_t mtx;
};
struct Jogador_s *jogadores_s;
ALLEGRO_EVENT_QUEUE *queue_inimigos;
ALLEGRO_TIMER *tempo;

void prepara_inimigos_server(int qtd_inimigos, int qtd_jogadores, int *id);
void prepara_inimigos_client(int qtd_inimigos);
void finaliza_inimigos_server();
void finaliza_inimigos_client();
void move_inimigos(int qtd_jogadores, int id_jog[]);
void desenha_inimigos();
void atualiza_inimigo_client(struct Buffer buffer);

#endif
