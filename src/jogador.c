/* jogador.c - movimento do herói pelo teclado.
 * O herói só anda; o ataque é automático (fica em armas.c). */

#include <math.h>

#include "raylib.h"
#include "config.h"
#include "jogador.h"

Jogador jogador_criar(void)
{
    Jogador jogador;

    jogador.posicao.x = 0.0f;
    jogador.posicao.y = 0.0f;
    jogador.vida = VIDA_HEROI_INICIAL;
    jogador.vida_maxima = VIDA_HEROI_INICIAL;
    jogador.velocidade = VELOCIDADE_HEROI;
    jogador.raio = RAIO_HEROI;
    return jogador;
}

void jogador_atualizar(Jogador *jogador, float dt)
{
    float direcao_x = 0.0f;
    float direcao_y = 0.0f;
    float tamanho;

    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        direcao_x += 1.0f;
    }
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        direcao_x -= 1.0f;
    }
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
        direcao_y += 1.0f;
    }
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
        direcao_y -= 1.0f;
    }

    /* Na diagonal o vetor fica maior que 1; dividimos para não andar mais rápido. */
    tamanho = sqrtf(direcao_x * direcao_x + direcao_y * direcao_y);
    if (tamanho > 0.0f) {
        jogador->posicao.x += direcao_x / tamanho * jogador->velocidade * dt;
        jogador->posicao.y += direcao_y / tamanho * jogador->velocidade * dt;
    }
}
