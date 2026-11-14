/* jogador.h - criação e movimento do herói. */

#ifndef JOGADOR_H
#define JOGADOR_H

#include "tipos.h"

/* Cria o herói no centro do mundo, com os valores iniciais do config.h. */
Jogador jogador_criar(void);

/* Lê o teclado (WASD ou setas) e move o herói. "dt" é o tempo do quadro em segundos. */
void jogador_atualizar(Jogador *jogador, float dt);

#endif
