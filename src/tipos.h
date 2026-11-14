/* tipos.h - structs simples usadas pelo jogo.
 * Não depende da raylib, para a lista de inimigos poder ser testada no terminal. */

#ifndef TIPOS_H
#define TIPOS_H

/* Posição ou direção no plano 2D. */
typedef struct {
    float x;
    float y;
} Vetor2;

/* Dados de um inimigo. Fica dentro de um nó da lista. */
typedef struct {
    int id;                    /* número único, dado pela lista ao inserir */
    int tipo;                  /* índice na tabela de tipos de inimigo */
    Vetor2 posicao;
    float vida;
    float velocidade;          /* pixels por segundo */
    float dano_contato;        /* dano causado ao herói ao encostar */
    float raio;                /* tamanho do círculo de colisão */
    int xp;                    /* experiência dada ao morrer */
    float distancia_quadrada;  /* distância² até o herói (calculada na ordenação) */
} Inimigo;

/* Disparo do herói. */
typedef struct {
    Vetor2 posicao;
    Vetor2 velocidade;         /* pixels por segundo, nos dois eixos */
    float dano;
    float raio;
    float tempo_restante;      /* segundos até sumir sozinho */
} Projetil;

/* O herói controlado pelo jogador. */
typedef struct {
    Vetor2 posicao;
    float vida;
    float vida_maxima;
    float velocidade;          /* pixels por segundo */
    float raio;
} Jogador;

#endif
