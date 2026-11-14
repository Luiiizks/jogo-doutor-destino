/* desenho.h - desenha a partida: chão, inimigos, projéteis, herói e HUD. */

#ifndef DESENHO_H
#define DESENHO_H

#include "raylib.h"
#include "imagens.h"
#include "partida.h"

/* Desenha tudo da partida (mundo visto pela câmera + HUD por cima). */
void desenho_partida(const Partida *partida, const Imagens *imagens, Camera2D camera);

#endif
