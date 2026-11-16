/* lista_inimigos.h - lista duplamente encadeada de inimigos (estrutura de dados do jogo).
 * Cada inimigo em cena é um nó. Nascer = inserir no fim, morrer = remover o nó.
 * Este módulo não usa a raylib, por isso dá para testar no terminal (make test). */

#ifndef LISTA_INIMIGOS_H
#define LISTA_INIMIGOS_H

#include "config.h"
#include "tipos.h"

typedef struct NoInimigo {
    Inimigo dados;
    struct NoInimigo *anterior;
    struct NoInimigo *proximo;
} NoInimigo;

typedef struct {
    NoInimigo *inicio;
    NoInimigo *fim;
    int tamanho;
    int proximo_id;            /* contador para dar um id único a cada inimigo */
    Morte mortes[LIMITE_MORTES];   /* quem morreu desde a última retirada (para soltar drops) */
    int qtd_mortes;
} ListaInimigos;

/* Cria uma lista vazia. Devolve NULL se faltar memória. O(1) */
ListaInimigos *lista_criar(void);

/* Insere um inimigo no fim (spawn). Devolve o nó criado ou NULL se faltar memória.
 * Atenção: a ordenação troca os dados entre nós, então não guarde o ponteiro
 * por muito tempo (use o id do inimigo). O(1) */
NoInimigo *lista_inserir_fim(ListaInimigos *lista, Inimigo novo);

/* Remove um nó conhecido da lista e libera a memória (morte do inimigo). O(1) */
void lista_remover_no(ListaInimigos *lista, NoInimigo *no);

/* Insertion sort: deixa os inimigos do mais perto para o mais longe do herói.
 * Como a lista quase não muda de um quadro para o outro, é perto de O(n).
 * Pior caso (lista invertida): O(n²). */
void lista_ordenar_por_distancia(ListaInimigos *lista, Vetor2 posicao_jogador);

/* Copia em "alvos" os ponteiros dos primeiros "quantidade" nós que estiverem dentro do
 * "alcance" (em pixels). Como a lista está ordenada por distância (ver
 * lista_ordenar_por_distancia, chamada antes no mesmo quadro), a busca PARA no primeiro
 * inimigo fora do alcance: todos os seguintes estão ainda mais longe.
 * Devolve quantos achou. O(quantidade) */
int lista_obter_mais_proximos(const ListaInimigos *lista, int quantidade, float alcance,
                              NoInimigo *alvos[]);

/* Anda com todos os inimigos em direção ao herói; os atiradores param a certa
 * distância para atirar. O(n) */
void lista_mover_todos(ListaInimigos *lista, Vetor2 posicao_jogador, float dt);

/* Testa um projétil contra os inimigos (intangíveis são atravessados). Se acertar o primeiro
 * que encontrar, aplica o dano e remove o inimigo se morreu (registrando a morte).
 * Devolve 1 se acertou e 0 se não. O(n) */
int lista_colidir_com_projetil(ListaInimigos *lista, const Projetil *projetil);

/* Dano em área (onda de impacto, pulso): atinge todos dentro do raio. Remove os mortos
 * (registrando as mortes). Devolve quantos foram atingidos. O(n) */
int lista_dano_em_area(ListaInimigos *lista, Vetor2 centro, float raio, float dano);

/* Copia para "saida" as mortes registradas (até "maximo") e zera o registro. Quem chama usa
 * a posição e o XP delas para soltar os drops. Devolve quantas copiou. O(mortes) */
int lista_retirar_mortes(ListaInimigos *lista, Morte saida[], int maximo);

/* Maior dano de contato entre os inimigos que estão encostando no herói (0 se nenhum).
 * Usado para o herói levar dano da horda. O(n) */
float lista_dano_de_contato(const ListaInimigos *lista, Vetor2 posicao_jogador,
                            float raio_jogador);

/* Percorre os inimigos à distância: os que já chegaram perto e já recarregaram atiram
 * no herói. Os tiros criados vão para "tiros" (até "maximo"). Devolve quantos foram. O(n) */
int lista_coletar_tiros(ListaInimigos *lista, Vetor2 posicao_jogador, float dt,
                        Projetil tiros[], int maximo);

/* Quantidade de inimigos na lista. O(1) */
int lista_contar(const ListaInimigos *lista);

/* Devolve 1 se a lista não tem nenhum inimigo. O(1) */
int lista_esta_vazia(const ListaInimigos *lista);

/* Devolve o primeiro inimigo do tipo pedido (ou NULL se não houver). Usado para achar o
 * chefe. O ponteiro vale só até a próxima ordenação. O(n) */
NoInimigo *lista_buscar_por_tipo(const ListaInimigos *lista, int tipo);

/* Remove todos os inimigos, mas mantém a lista vazia e pronta para uso. O(n) */
void lista_limpar(ListaInimigos *lista);

/* Libera todos os nós e a própria lista. O(n) */
void lista_destruir(ListaInimigos *lista);

#endif
