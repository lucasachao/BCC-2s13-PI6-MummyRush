#ifndef TIRO_
#define TIRO_

#include "colisao.h"
#include "variaveis.h"
#include "cliente.h"

struct Tiro{
	bool ativa;
	int angulo;
	float x;
	float y;
};

int velocidade_tiro;
struct Tiro tiro[400];

void prepara_tiro();
void adiciona_tiro(int angulo, float x, float y, int aux);
void move_tiro();
void desenha_tiro();

#endif
