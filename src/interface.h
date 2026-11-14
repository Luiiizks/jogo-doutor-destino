/* interface.h - menu principal e telas de texto. */

#ifndef INTERFACE_H
#define INTERFACE_H

typedef enum {
    OPCAO_JOGAR,
    OPCAO_COMO_JOGAR,
    OPCAO_SAIR,
    TOTAL_OPCOES_MENU
} OpcaoMenu;

/* Trata teclado e mouse no menu. "selecionada" é a opção destacada (atualizada aqui).
 * Devolve a opção confirmada (ENTER ou clique) ou -1 se ninguém confirmou ainda. */
int interface_atualizar_menu(int *selecionada);

/* Desenha o menu com a opção selecionada destacada. */
void interface_desenhar_menu(int selecionada);

/* Desenha a tela "Como jogar". */
void interface_desenhar_como_jogar(void);

#endif
