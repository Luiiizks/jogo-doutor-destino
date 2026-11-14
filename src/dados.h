/* dados.h - tabelas de conteúdo do jogo (personagens, inimigos, mundos, upgrades).
 * Para adicionar um personagem, inimigo ou mundo basta incluir uma linha na tabela
 * em dados.c; a lógica do jogo não muda. */

#ifndef DADOS_H
#define DADOS_H

#include "raylib.h"
#include "tipos.h"

#define MAX_TIPOS_POR_MUNDO 4

typedef enum {
    TIPO_DOOMBOT,
    TIPO_DOOMBOT_VELOZ,
    TIPO_MAGO,
    TOTAL_TIPOS_INIMIGO
} IndiceTipoInimigo;

typedef struct {
    const char *nome;
    float vida;
    float velocidade;
    float dano_contato;
    float raio;
    int xp;
    Color cor;                    /* usada quando a imagem não existe */
    const char *arquivo_imagem;
    float distancia_ataque;       /* 0 = só corpo a corpo */
    float intervalo_tiro;
    float dano_tiro;
} TipoInimigo;

typedef struct {
    const char *nome;
    float vida;
    float velocidade;
    float dano;
    float cadencia;
    Color cor;
    const char *arquivo_imagem;
} Personagem;

typedef struct {
    const char *nome;
    const char *descricao;
} DadosUpgrade;

typedef struct {
    const char *nome;
    float duracao;                          /* segundos */
    float intervalo_inicial;                /* tempo entre spawns no começo do mundo */
    float intervalo_final;                  /* ...e no fim (menor = mais inimigos) */
    int tipos[MAX_TIPOS_POR_MUNDO];         /* inimigos sorteados no spawn (repetir = mais comum) */
    int qtd_tipos;
    Color cor_linhas_chao;
    const char *arquivo_chao;
} Mundo;

const TipoInimigo *dados_tipo_inimigo(int tipo);
const Personagem *dados_personagem(int indice);
int dados_total_personagens(void);
const DadosUpgrade *dados_upgrade(TipoUpgrade upgrade);
const Mundo *dados_mundo(int indice);
int dados_total_mundos(void);

/* Monta um inimigo novo do tipo pedido, já com os atributos da tabela. */
Inimigo dados_criar_inimigo(int tipo, Vetor2 posicao);

#endif
