/* desenho.h - desenha a partida: chão, inimigos, projéteis, herói e HUD. */

#ifndef DESENHO_H
#define DESENHO_H

#include "raylib.h"
#include "imagens.h"
#include "partida.h"

/* Desenha o primeiro quadro de um sprite centralizado em "centro", com a altura visual
 * pedida (usado nos retratos da seleção de herói). */
void desenho_retrato(const Sprite *sprite, Vetor2 centro, float altura);

/* Desenha tudo da partida (mundo visto pela câmera + HUD por cima). */
void desenho_partida(const Partida *partida, const Imagens *imagens, Camera2D camera);

#endif
