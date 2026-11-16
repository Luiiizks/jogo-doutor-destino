/* gemas.h - gemas de XP e itens (ímã, coração) que ficam no chão depois que um inimigo morre. */

#ifndef GEMAS_H
#define GEMAS_H

#include "partida.h"

/* Pega as mortes registradas pela lista e solta o drop de cada uma: sempre uma gema de XP e,
 * com certa chance, um item (50% para elites, 3% para os comuns). */
void gemas_criar_das_mortes(Partida *partida);

/* Atrai as gemas que estão perto do herói e aplica o efeito de tudo que ele tocou:
 * XP da gema, cura do coração ou ímã (puxa todas as gemas do mapa). */
void gemas_atualizar(Partida *partida, float dt);

#endif
