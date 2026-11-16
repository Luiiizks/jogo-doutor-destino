/* mundos.h - tempo do mundo e spawn de inimigos. */

#ifndef MUNDOS_H
#define MUNDOS_H

#include "partida.h"

/* Conta o tempo do mundo e faz inimigos nascerem (inserem na lista): um de cada vez,
 * num intervalo que diminui, e uma horda grande de tempos em tempos. Quando o tempo do
 * mundo acaba, abre o portal; os inimigos continuam nascendo até o herói entrar nele.
 * Na arena do chefe não há spawn comum: o chefe nasce, invoca reforços e fica intangível
 * de vez em quando. */
void mundos_atualizar(Partida *partida, float dt);

/* Duração do mundo atual em segundos, já levando em conta o modo (normal ou rápido). */
float mundos_duracao(const Partida *partida);

/* Devolve 1 se o mundo terminou: nos mundos normais, o herói encostou no portal aberto;
 * na arena, o chefe já nasceu e não está mais na lista (foi derrotado). */
int mundos_concluido(const Partida *partida);

#endif
