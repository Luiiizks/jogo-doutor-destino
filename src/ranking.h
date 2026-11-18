/* ranking.h - ranking dos melhores jogadores (top 10), salvo num arquivo de texto.
 * Este módulo não usa a raylib, por isso dá para testar no terminal (make test). */

#ifndef RANKING_H
#define RANKING_H

#include "config.h"

typedef struct {
    char nome[TAMANHO_NOME];
    int pontos;
} EntradaRanking;

/* As entradas ficam sempre em ordem: da maior pontuação para a menor. */
typedef struct {
    EntradaRanking entradas[RANKING_TAMANHO];
    int quantidade;
} Ranking;

/* Deixa o ranking vazio. O(1) */
void ranking_iniciar(Ranking *ranking);

/* Lê o ranking do arquivo (uma linha por jogador: "pontos nome"). Se o arquivo não existir,
 * o ranking fica vazio. Devolve 1 se leu o arquivo, 0 se não. O(n) */
int ranking_carregar(Ranking *ranking, const char *arquivo);

/* Grava o ranking no arquivo. Devolve 1 se deu certo, 0 se não conseguiu gravar. O(n) */
int ranking_salvar(const Ranking *ranking, const char *arquivo);

/* Registra uma pontuação. Se o nome já está no ranking, só vale se for maior que a anterior
 * (e aí o jogador sobe). Se é um nome novo, entra se houver vaga ou se superar o último.
 * O nome não diferencia maiúsculas de minúsculas.
 * Devolve a posição (1 = primeiro lugar) ou 0 se a pontuação não entrou. O(n) */
int ranking_registrar(Ranking *ranking, const char *nome, int pontos);

#endif
