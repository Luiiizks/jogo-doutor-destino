/* mundos.h - tempo do mundo e spawn de inimigos. */

#ifndef MUNDOS_H
#define MUNDOS_H

#include "partida.h"

/* Conta o tempo do mundo e faz inimigos nascerem (inserem na lista): um de cada vez,
 * num intervalo que diminui, e uma horda grande de tempos em tempos. Quando o tempo do
 * mundo acaba, abre o portal; os inimigos continuam nascendo até o herói entrar nele. */
void mundos_atualizar(Partida *partida, float dt);

/* Devolve 1 se o portal já abriu e o herói está encostando nele. */
int mundos_jogador_no_portal(const Partida *partida);

#endif
