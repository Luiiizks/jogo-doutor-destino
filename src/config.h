/* config.h - constantes do jogo.
 * Todo número que pode ser ajustado fica aqui, para ninguém precisar caçar
 * "número mágico" espalhado pelo código. */

#ifndef CONFIG_H
#define CONFIG_H

#define TITULO_JOGO     "Doomsday Survivors"
#define LARGURA_JANELA  1280
#define ALTURA_JANELA   720
#define FPS_ALVO        60

/* Herói (valores iniciais; depois vêm da tabela de personagens) */
#define VIDA_HEROI_INICIAL   100.0f
#define VELOCIDADE_HEROI     180.0f
#define RAIO_HEROI           16.0f

/* Chão desenhado como grade, enquanto não há imagem de chão */
#define TAMANHO_QUADRADO_CHAO  64

#endif
