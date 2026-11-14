/* imagens.h - carrega as imagens da pasta assets/ (todas opcionais). */

#ifndef IMAGENS_H
#define IMAGENS_H

#include "raylib.h"
#include "config.h"
#include "dados.h"

/* Uma textura com id 0 significa "imagem não encontrada": o desenho usa uma forma
 * geométrica no lugar. Assim o jogo funciona mesmo sem nenhuma imagem. */
typedef struct {
    Texture2D personagens[MAX_PERSONAGENS];
    Texture2D inimigos[TOTAL_TIPOS_INIMIGO];
    Texture2D chao[MAX_MUNDOS];
    Texture2D projetil_heroi;
    Texture2D projetil_inimigo;
    Texture2D portal;
} Imagens;

/* Tenta carregar todas as imagens. Precisa ser chamada depois de abrir a janela. */
void imagens_carregar(Imagens *imagens);

/* Libera as texturas carregadas. */
void imagens_descarregar(Imagens *imagens);

#endif
