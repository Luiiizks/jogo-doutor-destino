/* dados.h - tabelas de conteúdo do jogo (personagens, inimigos, mundos, upgrades).
 * Para adicionar um personagem, inimigo ou mundo basta incluir uma linha na tabela
 * em dados.c; a lógica do jogo não muda. */

#ifndef DADOS_H
#define DADOS_H

#include "raylib.h"
#include "config.h"
#include "tipos.h"

#define MAX_TIPOS_POR_MUNDO 5

typedef enum {
    TIPO_DOOMBOT,
    TIPO_DOOMBOT_VELOZ,
    TIPO_MAGO,
    TIPO_DOOMBOT_ELITE,
    TIPO_SENTINELA,
    TIPO_ESPECTRO,
    TIPO_CHEFE,
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
    const char *arquivo_imagem;   /* imagem parada */
    const char *arquivo_caminhada;/* spritesheet da caminhada (NULL = não tem) */
    int quadros_caminhada;        /* quantos quadros lado a lado na spritesheet */
    float altura_visual;          /* altura na tela, em pixels, da parte visível do desenho */
    float distancia_ataque;       /* 0 = só corpo a corpo */
    float intervalo_tiro;
    float dano_tiro;
} TipoInimigo;

/* Cada herói muda só os números da mesma lógica de armas: quantos inimigos ele ataca por
 * disparo (qtd_alvos, 0 = não atira) e se começa com o pulso em área (raio_pulso > 0). */
typedef struct {
    const char *nome;
    const char *arma;             /* nome da arma, mostrado na seleção */
    const char *descricao;        /* frase curta, mostrada na seleção */
    float vida;
    float velocidade;
    float dano;
    float cadencia;               /* segundos entre disparos */
    int qtd_alvos;
    float alcance;                /* distância máxima dos tiros, em pixels */
    float raio_pulso;             /* 0 = começa sem pulso */
    float dano_pulso;
    float intervalo_pulso;
    Color cor;
    Color cor_projetil;           /* usada quando a imagem do tiro não existe */
    const char *arquivo_imagem;
    const char *arquivo_caminhada;
    int quadros_caminhada;
    const char *arquivo_projetil;
    float altura_visual;
    const char *nome_quantidade;      /* texto da melhoria "mais golpes" deste herói */
    const char *desc_quantidade;
    const char *nome_segunda_arma;    /* texto da carta "Nova arma" deste herói */
    const char *desc_segunda_arma;
} Personagem;

typedef struct {
    const char *nome;
    const char *descricao;
} DadosUpgrade;

/* Objeto de cenário (poste, barril...). Só enfeite: o herói passa por cima. */
typedef struct {
    const char *arquivo_imagem;
    float altura_visual;          /* altura na tela, em pixels, da parte visível */
} ObjetoCenario;

typedef struct {
    const char *nome;
    float duracao;                          /* segundos */
    float intervalo_inicial;                /* tempo entre spawns no começo do mundo */
    float intervalo_final;                  /* ...e no fim (menor = mais inimigos) */
    int tipos[MAX_TIPOS_POR_MUNDO];         /* inimigos sorteados no spawn (repetir = mais comum) */
    int qtd_tipos;
    Color cor_linhas_chao;
    const char *arquivo_chao;
    ObjetoCenario objetos[MAX_OBJETOS_POR_MUNDO];
    int qtd_objetos;
    int arena_chefe;                        /* 1 = mundo do chefe (arena, sem portal) */
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
