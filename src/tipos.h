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
    int andando;               /* 1 se andou neste quadro (usado na animação) */
    int elite;                 /* 1 = versão elite (dourada, maior, dropa mais) */
    float tempo_flash;         /* segundos restantes do brilho de "levou dano" */
    float tempo_intangivel;    /* enquanto > 0 não leva dano nem causa dano (chefe) */
    float distancia_quadrada;  /* distância² até o herói (calculada na ordenação) */

    /* ataque à distância (distancia_ataque 0 = só corpo a corpo) */
    float distancia_ataque;    /* para de andar e atira a esta distância do herói */
    float intervalo_tiro;      /* segundos entre tiros */
    float tempo_ate_tiro;
    float dano_tiro;
} Inimigo;

/* Registro de um inimigo que morreu (a lista guarda até o jogo soltar o drop dele). */
typedef struct {
    Vetor2 posicao;
    int xp;
    int elite;
} Morte;

/* O que fica no chão: gema de XP ou item. */
typedef enum {
    DROP_XP,
    DROP_IMA,
    DROP_CORACAO
} TipoDrop;

typedef struct {
    Vetor2 posicao;
    int tipo;
    int valor;                 /* XP da gema */
    int atraida;               /* 1 = está voando até o herói (efeito do ímã) */
} Drop;

/* Disparo do herói. */
typedef struct {
    Vetor2 posicao;
    Vetor2 velocidade;         /* pixels por segundo, nos dois eixos */
    float dano;
    float raio;
    float tempo_restante;      /* segundos até sumir sozinho */
    int personagem;            /* de qual herói saiu (escolhe a imagem do tiro) */
} Projetil;

/* Número de dano que sobe e some depois de um acerto. */
typedef struct {
    Vetor2 posicao;
    int valor;
    float tempo_restante;
} NumeroDano;

/* Melhorias que o jogador escolhe ao subir de nível. */
typedef enum {
    UPGRADE_DANO,
    UPGRADE_CADENCIA,
    UPGRADE_ALCANCE,           /* tiros vão mais longe e a onda de impacto fica maior */
    UPGRADE_QUANTIDADE,        /* mais tiros, mais alvos ou mais golpes (depende do herói) */
    UPGRADE_VELOCIDADE,
    UPGRADE_VIDA,
    UPGRADE_IMA,               /* aumenta de onde as gemas de XP são atraídas */
    UPGRADE_NOVA_ARMA,         /* segunda arma: a que o herói ainda não tem */
    TOTAL_UPGRADES
} TipoUpgrade;

/* O herói controlado pelo jogador. */
typedef struct {
    Vetor2 posicao;
    int personagem;            /* índice na tabela de personagens */
    float vida;
    float vida_maxima;
    float velocidade;          /* pixels por segundo */
    float raio;
    int olhando_esquerda;      /* 1 se a última direção horizontal foi para a esquerda */
    int andando;               /* 1 se alguma tecla de movimento está apertada */
    float tempo_invulneravel;  /* segundos restantes sem levar dano */

    /* disparo arcano */
    float dano;
    float cadencia;            /* segundos entre disparos */
    float tempo_ate_disparo;
    int qtd_alvos;             /* quantos inimigos próximos recebem tiro por disparo */
    float alcance;             /* distância máxima, em pixels, em que os tiros atacam */

    /* pulso de energia (raio 0 = ainda não desbloqueado) */
    float raio_pulso;
    float dano_pulso;
    float intervalo_pulso;     /* segundos entre pulsos */
    int golpes_pulso;          /* quantas vezes a onda bate seguidas */
    int golpes_restantes;      /* golpes que ainda faltam neste pulso */
    float tempo_ate_golpe;
    float tempo_ate_pulso;
    float efeito_pulso;        /* segundos restantes do anel desenhado na tela */

    /* experiência e melhorias */
    int nivel;
    int xp;
    float raio_ima;            /* gemas de XP dentro deste raio voam até o herói */
    float fator_xp;            /* multiplica o XP necessário por nível (1 = normal) */
    int niveis_upgrade[TOTAL_UPGRADES];
} Jogador;

#endif
