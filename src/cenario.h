/* cenario.h - sorteio da posição dos objetos de cenário (poste, barril, pedra...).
 * Não usa a raylib, então dá para testar no terminal. */

#ifndef CENARIO_H
#define CENARIO_H

#include "tipos.h"

/* Decide se a célula (celula_x, celula_y) do mapa tem um objeto e qual.
 * O mapa é dividido em células quadradas; o resultado depende só dos números da célula e
 * do mundo, então o objeto fica sempre no mesmo lugar, sem precisar guardar nada.
 * Devolve o índice do objeto (0 a qtd_tipos - 1) e a posição da base dele em "posicao",
 * ou -1 se a célula está vazia. */
int cenario_sortear_objeto(int mundo, int celula_x, int celula_y, int qtd_tipos,
                           Vetor2 *posicao);

#endif
