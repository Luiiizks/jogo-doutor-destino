/* interface.h - menus e telas de texto (principal, ajuda, pausa, level-up, fim). */

#ifndef INTERFACE_H
#define INTERFACE_H

#include "imagens.h"
#include "ranking.h"
#include "tipos.h"

typedef enum {
    OPCAO_JOGAR,
    OPCAO_RANKING,
    OPCAO_COMO_JOGAR,
    OPCAO_SAIR,
    TOTAL_OPCOES_MENU
} OpcaoMenu;

typedef enum {
    OPCAO_PAUSA_CONTINUAR,
    OPCAO_PAUSA_MENU,
    TOTAL_OPCOES_PAUSA
} OpcaoPausa;

typedef enum {
    OPCAO_MODO_NORMAL,
    OPCAO_MODO_RAPIDO,
    TOTAL_OPCOES_MODO
} OpcaoModo;

typedef enum {
    OPCAO_FIM_JOGAR_DE_NOVO,
    OPCAO_FIM_MENU,
    TOTAL_OPCOES_FIM
} OpcaoFim;

/* Trata teclado e mouse numa lista vertical de botões. "selecionada" é o botão destacado
 * (atualizado aqui). Devolve o botão confirmado (ENTER ou clique) ou -1 se nenhum. */
int interface_atualizar_opcoes(int *selecionada, int total);

void interface_desenhar_menu_principal(const Imagens *imagens, int selecionada);
void interface_desenhar_como_jogar(const Imagens *imagens);

/* Seleção de herói (cartas lado a lado) e de modo de jogo (normal ou rápido). */
void interface_desenhar_selecao_heroi(const Imagens *imagens, int selecionada);
void interface_desenhar_selecao_modo(const Imagens *imagens, int selecionada);
void interface_desenhar_pausa(int selecionada);

/* Telas de fim de partida. Mostram a pontuação e a posição no ranking (0 = não entrou). */
void interface_desenhar_derrota(int selecionada, int pontos, int posicao);
void interface_desenhar_vitoria(int selecionada, int pontos, int posicao);

/* Tela onde o jogador digita o nome. Escreve em "nome" (tamanho TAMANHO_NOME). Devolve 1 quando
 * confirma com ENTER (nome não vazio), -1 se apertar ESC e 0 se ainda está digitando. */
int interface_atualizar_nome(char *nome);
void interface_desenhar_nome(const Imagens *imagens, const char *nome);

/* Tela do ranking. "destaque" é a posição (1 a 10) a realçar, ou 0 para nenhuma. */
void interface_desenhar_ranking(const Imagens *imagens, const Ranking *ranking, int destaque);

/* Tela entre um mundo e outro. Devolve 1 quando o jogador confirma (ENTER, ESPAÇO ou clique). */
int interface_atualizar_transicao(void);
void interface_desenhar_transicao(const char *mundo_concluido, const char *proximo_mundo);

/* Telas de cartas (level-up e seleção de herói): devolve o índice da carta escolhida (teclas
 * 1-3, setas + ENTER ou clique) ou -1 se ainda não escolheu. "para_heroi" = 1 usa as cartas
 * mais altas da seleção de herói. */
int interface_atualizar_cartas(int *selecionada, int quantidade, int para_heroi);
void interface_desenhar_level_up(const Jogador *jogador, const int escolhas[],
                                 int quantidade, int selecionada);

#endif
