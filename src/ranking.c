/* ranking.c - top 10 de pontuações. A lista é mantida em ordem com insertion sort
 * (o mesmo algoritmo que a lista de inimigos usa) e gravada num arquivo de texto. */

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "ranking.h"

/* Compara dois nomes sem diferenciar maiúsculas. Devolve 1 se forem iguais. */
static int nomes_iguais(const char *primeiro, const char *segundo)
{
    while (*primeiro != '\0' && *segundo != '\0') {
        if (tolower((unsigned char)*primeiro) != tolower((unsigned char)*segundo)) {
            return 0;
        }
        primeiro++;
        segundo++;
    }
    return *primeiro == *segundo;
}

/* Insertion sort do maior para o menor. Cada entrada anda para a esquerda enquanto tiver
 * mais pontos que a vizinha. Como o ranking já está quase ordenado, é rápido: O(n) no
 * melhor caso e O(n^2) no pior (n é no máximo 10). */
static void ordenar(Ranking *ranking)
{
    int indice;

    for (indice = 1; indice < ranking->quantidade; indice++) {
        EntradaRanking atual = ranking->entradas[indice];
        int posicao = indice - 1;

        while (posicao >= 0 && ranking->entradas[posicao].pontos < atual.pontos) {
            ranking->entradas[posicao + 1] = ranking->entradas[posicao];
            posicao--;
        }
        ranking->entradas[posicao + 1] = atual;
    }
}

void ranking_iniciar(Ranking *ranking)
{
    ranking->quantidade = 0;
}

int ranking_carregar(Ranking *ranking, const char *arquivo)
{
    FILE *entrada = fopen(arquivo, "r");
    char linha[64];

    ranking_iniciar(ranking);
    if (entrada == NULL) {
        return 0;
    }
    while (ranking->quantidade < RANKING_TAMANHO && fgets(linha, sizeof(linha), entrada)) {
        EntradaRanking *nova = &ranking->entradas[ranking->quantidade];

        /* %12[^\n] lê o nome até o fim da linha (pode ter espaço), com no máximo 12 letras. */
        if (sscanf(linha, "%d %12[^\n]", &nova->pontos, nova->nome) == 2) {
            ranking->quantidade++;
        }
    }
    fclose(entrada);
    ordenar(ranking);       /* garante a ordem mesmo se alguém editou o arquivo */
    return 1;
}

int ranking_salvar(const Ranking *ranking, const char *arquivo)
{
    FILE *saida = fopen(arquivo, "w");
    int indice;

    if (saida == NULL) {
        return 0;
    }
    for (indice = 0; indice < ranking->quantidade; indice++) {
        fprintf(saida, "%d %s\n", ranking->entradas[indice].pontos,
                ranking->entradas[indice].nome);
    }
    fclose(saida);
    return 1;
}

/* Procura um nome no ranking. Devolve o índice ou -1 se não estiver. O(n) */
static int buscar_nome(const Ranking *ranking, const char *nome)
{
    int indice;

    for (indice = 0; indice < ranking->quantidade; indice++) {
        if (nomes_iguais(ranking->entradas[indice].nome, nome)) {
            return indice;
        }
    }
    return -1;
}

int ranking_registrar(Ranking *ranking, const char *nome, int pontos)
{
    int indice = buscar_nome(ranking, nome);

    if (indice >= 0) {
        /* Nome repetido: só atualiza se fez mais pontos que da última vez. */
        if (pontos <= ranking->entradas[indice].pontos) {
            return 0;
        }
    } else if (ranking->quantidade < RANKING_TAMANHO) {
        indice = ranking->quantidade;                       /* ainda há vaga */
        ranking->quantidade++;
        snprintf(ranking->entradas[indice].nome, TAMANHO_NOME, "%s", nome);
    } else if (pontos > ranking->entradas[RANKING_TAMANHO - 1].pontos) {
        indice = RANKING_TAMANHO - 1;                       /* toma o lugar do último */
        snprintf(ranking->entradas[indice].nome, TAMANHO_NOME, "%s", nome);
    } else {
        return 0;
    }
    ranking->entradas[indice].pontos = pontos;
    ordenar(ranking);
    return buscar_nome(ranking, nome) + 1;
}
