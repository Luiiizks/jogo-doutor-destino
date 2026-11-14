/* partida.h - tudo que existe numa partida: herói, lista de inimigos, projéteis, tempo. */

#ifndef PARTIDA_H
#define PARTIDA_H

#include "config.h"
#include "lista_inimigos.h"
#include "tipos.h"

typedef struct {
    Jogador jogador;
    ListaInimigos *inimigos;
    Projetil projeteis[MAX_PROJETEIS];
    int qtd_projeteis;
    Projetil tiros_inimigos[MAX_TIROS_INIMIGOS];
    int qtd_tiros_inimigos;
    int mundo;                 /* índice na tabela de mundos */
    float tempo_mundo;         /* segundos desde que o mundo começou */
    float tempo_ate_spawn;
    float tempo_ate_horda;
    int hordas_ocorridas;
    float aviso_horda;         /* segundos restantes do aviso "HORDA!" na tela */
    int portal_aberto;         /* 1 depois que o tempo do mundo acaba */
    Vetor2 posicao_portal;
} Partida;

typedef enum {
    PARTIDA_EM_ANDAMENTO,
    PARTIDA_DERROTA,
    PARTIDA_MUNDO_CONCLUIDO
} ResultadoPartida;

/* Começa uma partida nova. Devolve 0 se faltar memória para a lista, 1 se deu certo. */
int partida_iniciar(Partida *partida, int indice_personagem, int indice_mundo);

/* Libera a lista de inimigos (fim de mundo ou de partida). Pode chamar mais de uma vez. */
void partida_encerrar(Partida *partida);

/* Avança um quadro: move, spawna, ordena a lista, atira e aplica dano. */
ResultadoPartida partida_atualizar(Partida *partida, float dt);

#endif
