/* jogador.h - criação, movimento, vida, experiência e melhorias do herói. */

#ifndef JOGADOR_H
#define JOGADOR_H

#include "tipos.h"

/* Cria o herói no centro do mundo com os valores do personagem da tabela. */
Jogador jogador_criar(int indice_personagem);

/* Lê o teclado (WASD ou setas), move o herói e conta o tempo de invulnerabilidade.
 * "dt" é o tempo do quadro em segundos. */
void jogador_atualizar(Jogador *jogador, float dt);

/* Tira vida do herói, a não ser que ele ainda esteja invulnerável. */
void jogador_receber_dano(Jogador *jogador, float dano);

/* XP necessário para passar do nível atual para o próximo. */
int jogador_xp_para_proximo_nivel(const Jogador *jogador);

/* Devolve 1 se o herói já juntou XP para subir de nível. */
int jogador_pode_subir_nivel(const Jogador *jogador);

/* Gasta o XP e sobe um nível (as melhorias são aplicadas à parte). */
void jogador_subir_nivel(Jogador *jogador);

/* Sorteia até OPCOES_LEVEL_UP melhorias diferentes que ainda não estão no nível máximo.
 * A carta "Nova arma" aparece sempre a partir de NIVEL_NOVA_ARMA, até ser escolhida.
 * Devolve quantas foram sorteadas (pode ser 0 se tudo estiver no máximo). */
int jogador_sortear_upgrades(const Jogador *jogador, int escolhas[]);

/* Aplica a melhoria escolhida. */
void jogador_aplicar_upgrade(Jogador *jogador, TipoUpgrade upgrade);

#endif
