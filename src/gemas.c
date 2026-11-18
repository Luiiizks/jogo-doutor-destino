/* gemas.c - drops no chão: gemas de XP, ímã e coração.
 * O XP só entra quando o herói passa por cima da gema. */

#include <math.h>

#include "raylib.h"
#include "gemas.h"

/* Coloca um drop no vetor. Se o vetor estiver cheio, a gema de XP vai direto para o herói
 * (assim nenhum XP se perde). */
static void adicionar_drop(Partida *partida, Vetor2 posicao, int tipo, int valor)
{
    Drop *novo;

    if (partida->qtd_drops >= MAX_DROPS) {
        if (tipo == DROP_XP) {
            partida->jogador.xp += valor;
        }
        return;
    }
    novo = &partida->drops[partida->qtd_drops];
    novo->posicao = posicao;
    novo->tipo = tipo;
    novo->valor = valor;
    novo->atraida = 0;
    partida->qtd_drops++;
}

void gemas_criar_das_mortes(Partida *partida)
{
    Morte mortes[LIMITE_MORTES];
    int qtd_mortes = lista_retirar_mortes(partida->inimigos, mortes, LIMITE_MORTES);
    int indice;

    for (indice = 0; indice < qtd_mortes; indice++) {
        int chance = mortes[indice].elite ? CHANCE_DROP_ELITE : CHANCE_DROP_COMUM;

        partida->pontos_abates += mortes[indice].xp * PONTOS_POR_XP;   /* pontuação do abate */
        if (mortes[indice].xp > 0) {
            adicionar_drop(partida, mortes[indice].posicao, DROP_XP, mortes[indice].xp);
        }
        if (GetRandomValue(1, 100) <= chance) {
            Vetor2 posicao = mortes[indice].posicao;
            int tipo = (GetRandomValue(0, 1) == 0) ? DROP_IMA : DROP_CORACAO;

            posicao.x += 16.0f;                     /* ao lado da gema, para não ficar em cima */
            adicionar_drop(partida, posicao, tipo, 0);
        }
    }
}

/* Efeito de pegar um drop. */
static void aplicar_drop(Partida *partida, const Drop *drop)
{
    Jogador *jogador = &partida->jogador;
    int indice;

    if (drop->tipo == DROP_XP) {
        jogador->xp += drop->valor;
    } else if (drop->tipo == DROP_CORACAO) {
        jogador->vida += jogador->vida_maxima * CURA_CORACAO;
        if (jogador->vida > jogador->vida_maxima) {
            jogador->vida = jogador->vida_maxima;
        }
    } else {                                        /* ímã: todas as gemas voam até o herói */
        for (indice = 0; indice < partida->qtd_drops; indice++) {
            partida->drops[indice].atraida = 1;
        }
    }
}

void gemas_atualizar(Partida *partida, float dt)
{
    const Jogador *jogador = &partida->jogador;
    int indice = 0;

    while (indice < partida->qtd_drops) {
        Drop *drop = &partida->drops[indice];
        float diferenca_x = jogador->posicao.x - drop->posicao.x;
        float diferenca_y = jogador->posicao.y - drop->posicao.y;
        float distancia = sqrtf(diferenca_x * diferenca_x + diferenca_y * diferenca_y);

        if (distancia <= jogador->raio + RAIO_COLETA) {
            aplicar_drop(partida, drop);
            /* Troca pelo último do vetor (a ordem dos drops não importa). */
            partida->qtd_drops--;
            partida->drops[indice] = partida->drops[partida->qtd_drops];
        } else {
            /* Só a gema de XP é puxada (pelo ímã ou por estar perto); itens precisam de toque. */
            if (drop->tipo == DROP_XP && (drop->atraida || distancia <= jogador->raio_ima)) {
                drop->posicao.x += diferenca_x / distancia * VELOCIDADE_ATRACAO * dt;
                drop->posicao.y += diferenca_y / distancia * VELOCIDADE_ATRACAO * dt;
            }
            indice++;
        }
    }
}
