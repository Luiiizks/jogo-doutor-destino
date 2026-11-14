/* armas.h - disparo arcano automático, projéteis e pulso de energia. */

#ifndef ARMAS_H
#define ARMAS_H

#include "partida.h"

/* Avança as armas um quadro: dispara quando a recarga acaba, move os projéteis,
 * aplica o dano nos inimigos e solta o pulso de energia (se desbloqueado).
 * Precisa da lista de inimigos já ordenada por distância. */
void armas_atualizar(Partida *partida, float dt);

#endif
