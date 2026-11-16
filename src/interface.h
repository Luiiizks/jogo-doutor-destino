/* interface.h - menus e telas de texto (principal, ajuda, pausa, level-up, fim). */

#ifndef INTERFACE_H
#define INTERFACE_H

#include "tipos.h"

typedef enum {
    OPCAO_JOGAR,
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
    OPCAO_FIM_JOGAR_DE_NOVO,
    OPCAO_FIM_MENU,
    TOTAL_OPCOES_FIM
} OpcaoFim;

/* Trata teclado e mouse numa lista vertical de botões. "selecionada" é o botão destacado
 * (atualizado aqui). Devolve o botão confirmado (ENTER ou clique) ou -1 se nenhum. */
int interface_atualizar_opcoes(int *selecionada, int total);

void interface_desenhar_menu_principal(int selecionada);
void interface_desenhar_como_jogar(void);
void interface_desenhar_pausa(int selecionada);
void interface_desenhar_derrota(int selecionada);
void interface_desenhar_mundo_concluido(int selecionada);

/* Tela entre um mundo e outro. Devolve 1 quando o jogador confirma (ENTER, ESPAÇO ou clique). */
int interface_atualizar_transicao(void);
void interface_desenhar_transicao(const char *mundo_concluido, const char *proximo_mundo);

/* Level-up: devolve o índice da melhoria escolhida (teclas 1-3, setas + ENTER ou clique)
 * ou -1 se ainda não escolheu. */
int interface_atualizar_level_up(int *selecionada, int quantidade);
void interface_desenhar_level_up(const Jogador *jogador, const int escolhas[],
                                 int quantidade, int selecionada);

#endif
