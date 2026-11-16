/* imagens.h - carrega as imagens da pasta assets/ (todas opcionais). */

#ifndef IMAGENS_H
#define IMAGENS_H

#include "raylib.h"
#include "config.h"
#include "dados.h"

/* Uma imagem do jogo. Se textura.id for 0, o arquivo não foi encontrado e o desenho
 * usa uma forma geométrica no lugar. Assim o jogo funciona mesmo sem nenhuma imagem. */
typedef struct {
    Texture2D textura;
    int quadros;               /* quadros lado a lado (1 = imagem parada) */
    float altura_conteudo;     /* altura visível de um quadro, sem a margem transparente */
} Sprite;

typedef struct {
    Sprite personagens[MAX_PERSONAGENS];
    Sprite inimigos[TOTAL_TIPOS_INIMIGO];
    Sprite chao[MAX_MUNDOS];
    Sprite cenario[MAX_MUNDOS][MAX_OBJETOS_POR_MUNDO];
    Sprite projetil_heroi;
    Sprite projetil_inimigo;
    Sprite portal;
} Imagens;

/* Tenta carregar todas as imagens. Precisa ser chamada depois de abrir a janela. */
void imagens_carregar(Imagens *imagens);

/* Libera as texturas carregadas. */
void imagens_descarregar(Imagens *imagens);

#endif
