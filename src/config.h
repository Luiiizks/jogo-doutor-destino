/* config.h - constantes do jogo.
 * Todo número que pode ser ajustado fica aqui, para ninguém precisar caçar
 * "número mágico" espalhado pelo código. */

#ifndef CONFIG_H
#define CONFIG_H

#define TITULO_JOGO     "Doomsday Survivors"
#define LARGURA_JANELA  1280
#define ALTURA_JANELA   720
#define FPS_ALVO        60
#define DT_MAXIMO       0.05f     /* evita "pulos" se a janela travar um instante */

/* Chão desenhado como grade, enquanto não há imagem de chão */
#define TAMANHO_QUADRADO_CHAO  64

/* Inimigos */
#define LIMITE_INIMIGOS  250      /* máximo de inimigos em cena (o spawn para aqui) */
#define RAIO_SPAWN       700.0f   /* distância do herói onde os inimigos nascem */

/* Hordas e portal */
#define INTERVALO_HORDA          30.0f   /* segundos entre hordas grandes */
#define HORDA_TAMANHO_BASE       10      /* inimigos da primeira horda */
#define HORDA_TAMANHO_CRESCIMENTO 5      /* inimigos a mais a cada horda seguinte */
#define HORDA_ABERTURA_GRAUS     30      /* a horda vem de um lado, espalhada nesse ângulo */
#define DURACAO_AVISO_HORDA      3.0f
#define DISTANCIA_PORTAL         500.0f  /* onde o portal abre, longe do herói */
#define RAIO_PORTAL              40.0f

/* Tiros dos inimigos à distância */
#define MAX_TIROS_INIMIGOS   100
#define VELOCIDADE_TIRO_INIMIGO 240.0f
#define TEMPO_VIDA_TIRO_INIMIGO 3.0f
#define RAIO_TIRO_INIMIGO    6.0f
#define MARGEM_DISTANCIA_TIRO 30.0f   /* tolerância para considerar que já chegou à distância */

/* Imagens */
#define MAX_PERSONAGENS      8
#define MAX_MUNDOS           4

/* Herói e arma arcana */
#define TEMPO_INVULNERAVEL   0.5f   /* segundos sem levar dano depois de ser atingido */
#define MAX_ALVOS            8      /* máximo de projéteis por disparo */
#define MAX_PROJETEIS        200
#define VELOCIDADE_PROJETIL  420.0f
#define TEMPO_VIDA_PROJETIL  2.0f
#define RAIO_PROJETIL        5.0f

/* Nível e upgrades */
#define XP_BASE_NIVEL        20
#define XP_POR_NIVEL         15
#define OPCOES_LEVEL_UP      3
#define NIVEL_MAX_UPGRADE    5
#define BONUS_DANO           4.0f
#define FATOR_CADENCIA       0.85f  /* cada nível multiplica o tempo entre disparos */
#define BONUS_VELOCIDADE     20.0f
#define BONUS_VIDA           20.0f

/* Pulso de energia (upgrade) */
#define INTERVALO_PULSO      3.0f
#define RAIO_PULSO_BASE      90.0f
#define RAIO_PULSO_POR_NIVEL 25.0f
#define DANO_PULSO_BASE      15.0f
#define DANO_PULSO_POR_NIVEL 8.0f
#define DURACAO_EFEITO_PULSO 0.25f

#endif
