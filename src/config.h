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

/* Modo rápido (para demonstração): tudo acontece mais depressa */
#define FATOR_TEMPO_RAPIDO       0.33f  /* duração do mundo e intervalo das hordas viram 1/3 */
#define FATOR_XP_RAPIDO          0.7f   /* precisa de 70% do XP para subir de nível */
#define FATOR_VIDA_CHEFE_RAPIDO  0.6f   /* o chefe tem 60% da vida */

/* Troca de mundo */
#define CURA_TROCA_MUNDO       0.5f   /* fração da vida máxima recuperada ao mudar de mundo */

/* Chefe e arena (mundo 4) */
#define ARENA_METADE_LARGURA     760.0f  /* a arena é um retângulo centrado em (0, 0) */
#define ARENA_METADE_ALTURA      460.0f
#define DISTANCIA_INICIAL_CHEFE  400.0f  /* o chefe nasce acima do herói, a esta distância */
#define QTD_REFORCOS_CHEFE       6
#define RAIO_REFORCOS_CHEFE      90.0f   /* os reforços nascem num círculo ao redor do chefe */
#define MAX_INIMIGOS_ARENA       30      /* o chefe só invoca se houver menos que isto em cena */
#define INTERVALO_REFORCOS       8.0f
#define INTERVALO_REFORCOS_FASE2 5.0f
#define INTERVALO_INTANGIVEL     14.0f   /* de quanto em quanto tempo o chefe fica intangível */
#define DURACAO_INTANGIVEL       3.0f
#define FASE2_FRACAO_VIDA        0.5f    /* abaixo desta fração da vida o chefe entra na fase 2 */
#define FASE2_FATOR_VELOCIDADE   1.5f

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
#define TEMPO_VIDA_TIRO_INIMIGO 2.0f
#define RAIO_TIRO_INIMIGO    6.0f
#define MARGEM_DISTANCIA_TIRO 30.0f   /* tolerância para considerar que já chegou à distância */

/* Objetos de cenário */
#define TAMANHO_CELULA_CENARIO 256    /* o mapa é dividido em células deste tamanho */
#define CHANCE_OBJETO_CENARIO  40     /* % de células que têm um objeto */
#define RAIO_LIVRE_INICIO      120.0f /* sem objetos perto de onde o herói nasce */
#define MAX_OBJETOS_POR_MUNDO  6

/* Drops: gemas de XP e itens (o XP só vale quando o herói pega no chão) */
#define MAX_DROPS              400
#define LIMITE_MORTES          300     /* mortes guardadas pela lista até o jogo soltar os drops */
#define RAIO_COLETA            14.0f   /* distância extra (além do raio do herói) para pegar */
#define RAIO_IMA_BASE          70.0f   /* gemas dentro deste raio são atraídas para o herói */
#define BONUS_RAIO_IMA         50.0f
#define VELOCIDADE_ATRACAO     420.0f
#define CHANCE_DROP_COMUM      3       /* % de um inimigo comum soltar um item */
#define CHANCE_DROP_ELITE      50      /* % de um elite soltar um item */
#define CURA_CORACAO           0.25f   /* fração da vida máxima que o coração cura */

/* Elites: versão dourada, maior e mais forte de um inimigo comum */
#define ELITE_FATOR_VIDA       3.0f
#define ELITE_FATOR_XP         3
#define ELITE_FATOR_DANO       1.5f
#define ELITE_FATOR_TAMANHO    1.25f
#define ELITES_POR_HORDA_MIN   1
#define ELITES_POR_HORDA_MAX   2

/* Inimigos ficam mais fortes: durante o mundo e a cada mundo novo */
#define CRESCIMENTO_VIDA_NO_MUNDO   0.4f   /* +40% de vida do começo ao fim do mundo */
#define CRESCIMENTO_VIDA_POR_MUNDO  0.2f   /* +20% de vida a cada mundo seguinte */

/* Efeitos visuais */
#define DURACAO_FLASH_DANO     0.12f  /* inimigo fica avermelhado ao levar dano */
#define MAX_NUMEROS_DANO       40
#define DURACAO_NUMERO_DANO    0.7f
#define VELOCIDADE_NUMERO_DANO 45.0f  /* pixels por segundo, subindo */
#define DURACAO_TREMOR         0.25f  /* tremor de tela quando o herói leva dano */
#define FORCA_TREMOR           5.0f   /* deslocamento máximo da câmera, em pixels */

/* Imagens e animação */
#define FPS_ANIMACAO         7.0f   /* quadros da animação de caminhada por segundo */
#define MAX_PERSONAGENS      8
#define MAX_MUNDOS           4

/* Herói e arma arcana */
#define TEMPO_INVULNERAVEL   0.5f   /* segundos sem levar dano depois de ser atingido */
#define MAX_ALVOS            8      /* máximo de projéteis por disparo */
#define MAX_PROJETEIS        200
#define VELOCIDADE_PROJETIL  420.0f
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
#define BONUS_ALCANCE        60.0f  /* upgrade Alcance: distância máxima dos tiros */
#define BONUS_RAIO_ALCANCE   15.0f  /* ...e raio da onda de impacto */
#define BONUS_DANO_PULSO     4.0f   /* "Mais dano" também reforça o pulso/onda */
#define BONUS_RAIO_PULSO     8.0f
#define NIVEL_NOVA_ARMA      3      /* a partir deste nível a carta "Nova arma" é oferecida */
#define INTERVALO_ENTRE_GOLPES 0.22f /* segundos entre os golpes seguidos da onda */

/* Pulso de energia (upgrade) */
#define INTERVALO_PULSO      3.0f
#define RAIO_PULSO_BASE      90.0f
#define RAIO_PULSO_POR_NIVEL 25.0f
#define DANO_PULSO_BASE      15.0f
#define DANO_PULSO_POR_NIVEL 8.0f
#define DURACAO_EFEITO_PULSO 0.25f

#endif
