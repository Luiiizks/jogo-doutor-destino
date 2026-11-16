/* partida.h - tudo que existe numa partida: herói, lista de inimigos, projéteis, tempo. */

#ifndef PARTIDA_H
#define PARTIDA_H

#include "config.h"
#include "lista_inimigos.h"
#include "tipos.h"

/* Modo normal ou modo rápido (demonstração, mundos mais curtos). */
typedef enum {
    MODO_NORMAL,
    MODO_RAPIDO
} ModoJogo;

typedef struct {
    Jogador jogador;
    ListaInimigos *inimigos;
    Projetil projeteis[MAX_PROJETEIS];
    int qtd_projeteis;
    NumeroDano numeros_dano[MAX_NUMEROS_DANO];
    Drop drops[MAX_DROPS];
    int qtd_drops;
    int qtd_numeros_dano;
    float tempo_tremor;        /* segundos restantes do tremor de tela */
    Projetil tiros_inimigos[MAX_TIROS_INIMIGOS];
    int qtd_tiros_inimigos;
    ModoJogo modo;
    float escala_tempo;        /* 1 no modo normal; menor que 1 no modo rápido */
    float vida_chefe_maxima;
    int mundo;                 /* índice na tabela de mundos */
    float tempo_mundo;         /* segundos desde que o mundo começou */
    float tempo_ate_spawn;
    float tempo_ate_horda;
    int hordas_ocorridas;
    float aviso_horda;         /* segundos restantes do aviso "HORDA!" na tela */
    int portal_aberto;         /* 1 depois que o tempo do mundo acaba */
    Vetor2 posicao_portal;
    int chefe_criado;          /* mundo do chefe: 1 depois que ele nasceu */
    int chefe_fase2;
    float tempo_ate_reforcos;
    float tempo_ate_intangivel;
} Partida;

typedef enum {
    PARTIDA_EM_ANDAMENTO,
    PARTIDA_DERROTA,
    PARTIDA_MUNDO_CONCLUIDO
} ResultadoPartida;

/* Começa uma partida nova no modo pedido. Devolve 0 se faltar memória, 1 se deu certo. */
int partida_iniciar(Partida *partida, int indice_personagem, int indice_mundo, ModoJogo modo);

/* Devolve 1 se existe um mundo depois do atual na tabela de mundos. */
int partida_tem_proximo_mundo(const Partida *partida);

/* Passa para o próximo mundo mantendo o herói (nível, melhorias) e curando parte da vida.
 * Devolve 0 se faltar memória. Chame só se partida_tem_proximo_mundo for 1. */
int partida_proximo_mundo(Partida *partida);

/* Devolve 1 se o mundo atual é a arena do chefe. */
int partida_eh_arena(const Partida *partida);

/* Libera a lista de inimigos (fim de mundo ou de partida). Pode chamar mais de uma vez. */
void partida_encerrar(Partida *partida);

/* Avança um quadro: move, spawna, ordena a lista, atira e aplica dano. */
ResultadoPartida partida_atualizar(Partida *partida, float dt);

#endif
