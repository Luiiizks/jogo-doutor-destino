/* teste_ranking.c - testes de console do ranking (sem raylib). Roda com "make test". */

#include <stdio.h>
#include <string.h>

#include "../src/ranking.h"

static int total_conferencias = 0;
static int total_falhas = 0;

static void conferir(int condicao, const char *descricao)
{
    total_conferencias++;
    if (!condicao) {
        total_falhas++;
        printf("  FALHOU: %s\n", descricao);
    }
}

static void teste_ordem_e_posicao(void)
{
    Ranking ranking;

    printf("ordem e posicao\n");
    ranking_iniciar(&ranking);
    conferir(ranking_registrar(&ranking, "Ana", 500) == 1, "primeiro jogador fica em 1o");
    conferir(ranking_registrar(&ranking, "Bia", 900) == 1, "maior pontuacao vai para o 1o");
    conferir(ranking_registrar(&ranking, "Caio", 700) == 2, "700 fica em 2o");
    conferir(ranking.quantidade == 3, "tres jogadores");
    conferir(strcmp(ranking.entradas[0].nome, "Bia") == 0, "Bia lidera");
    conferir(strcmp(ranking.entradas[2].nome, "Ana") == 0, "Ana em ultimo");
}

static void teste_nome_repetido(void)
{
    Ranking ranking;

    printf("nome repetido\n");
    ranking_iniciar(&ranking);
    ranking_registrar(&ranking, "Ana", 500);
    ranking_registrar(&ranking, "Bia", 900);
    conferir(ranking_registrar(&ranking, "Ana", 300) == 0, "pontuacao menor nao entra");
    conferir(ranking.entradas[1].pontos == 500, "Ana continua com 500");
    conferir(ranking_registrar(&ranking, "ana", 500) == 0, "pontuacao igual nao entra");
    conferir(ranking_registrar(&ranking, "ANA", 1200) == 1, "maior pontuacao sobe para o 1o");
    conferir(ranking.quantidade == 2, "nome repetido nao cria entrada nova");
    conferir(strcmp(ranking.entradas[0].nome, "Ana") == 0, "mantem o nome original");
}

static void teste_ranking_cheio(void)
{
    Ranking ranking;
    char nome[TAMANHO_NOME];
    int indice;

    printf("ranking cheio\n");
    ranking_iniciar(&ranking);
    for (indice = 0; indice < RANKING_TAMANHO; indice++) {
        snprintf(nome, sizeof(nome), "J%d", indice);
        ranking_registrar(&ranking, nome, 1000 + indice * 100);
    }
    conferir(ranking.quantidade == RANKING_TAMANHO, "ranking lotado");
    conferir(ranking_registrar(&ranking, "Fraco", 900) == 0, "abaixo do ultimo nao entra");
    conferir(ranking.quantidade == RANKING_TAMANHO, "continua com 10");
    conferir(ranking_registrar(&ranking, "Forte", 1150) == 9, "entra em 9o e tira o ultimo");
    conferir(ranking.quantidade == RANKING_TAMANHO, "ainda 10 entradas");
    conferir(ranking.entradas[RANKING_TAMANHO - 1].pontos == 1100, "o de 1000 saiu");
    conferir(ranking_registrar(&ranking, "Rei", 99999) == 1, "novo recorde em 1o");
}

static void teste_arquivo(void)
{
    Ranking original;
    Ranking lido;
    const char *arquivo = "teste_ranking.tmp";

    printf("arquivo\n");
    ranking_iniciar(&original);
    ranking_registrar(&original, "Ana Maria", 500);
    ranking_registrar(&original, "Bia", 900);
    conferir(ranking_salvar(&original, arquivo) == 1, "salvou");
    conferir(ranking_carregar(&lido, arquivo) == 1, "carregou");
    conferir(lido.quantidade == 2, "2 entradas lidas");
    conferir(strcmp(lido.entradas[0].nome, "Bia") == 0 && lido.entradas[0].pontos == 900,
             "primeira entrada correta");
    conferir(strcmp(lido.entradas[1].nome, "Ana Maria") == 0, "nome com espaco preservado");
    remove(arquivo);
    conferir(ranking_carregar(&lido, arquivo) == 0 && lido.quantidade == 0,
             "arquivo inexistente deixa o ranking vazio");
}

int main(void)
{
    teste_ordem_e_posicao();
    teste_nome_repetido();
    teste_ranking_cheio();
    teste_arquivo();
    printf("\n%d conferencias, %d falhas\n", total_conferencias, total_falhas);
    return total_falhas == 0 ? 0 : 1;
}
