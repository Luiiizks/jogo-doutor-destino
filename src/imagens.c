/* imagens.c - carregamento das imagens do jogo.
 * Se um arquivo não existir, a textura fica zerada e o jogo desenha formas simples. */

#include "imagens.h"

/* Carrega a imagem se o arquivo existir; senão devolve uma textura vazia. */
static Texture2D carregar_se_existir(const char *caminho)
{
    Texture2D textura = { 0 };

    if (FileExists(caminho)) {
        textura = LoadTexture(caminho);
        SetTextureFilter(textura, TEXTURE_FILTER_POINT);   /* mantém o pixel art nítido */
    }
    return textura;
}

void imagens_carregar(Imagens *imagens)
{
    int indice;

    for (indice = 0; indice < dados_total_personagens(); indice++) {
        imagens->personagens[indice] = carregar_se_existir(
            dados_personagem(indice)->arquivo_imagem);
    }
    for (indice = 0; indice < TOTAL_TIPOS_INIMIGO; indice++) {
        imagens->inimigos[indice] = carregar_se_existir(
            dados_tipo_inimigo(indice)->arquivo_imagem);
    }
    for (indice = 0; indice < dados_total_mundos(); indice++) {
        imagens->chao[indice] = carregar_se_existir(dados_mundo(indice)->arquivo_chao);
    }
    imagens->projetil_heroi = carregar_se_existir("assets/sprites/projetil_arcano.png");
    imagens->projetil_inimigo = carregar_se_existir("assets/sprites/projetil_inimigo.png");
    imagens->portal = carregar_se_existir("assets/sprites/portal.png");
}

/* Descarrega uma textura só se ela foi carregada. */
static void descarregar(Texture2D textura)
{
    if (textura.id > 0) {
        UnloadTexture(textura);
    }
}

void imagens_descarregar(Imagens *imagens)
{
    int indice;

    for (indice = 0; indice < dados_total_personagens(); indice++) {
        descarregar(imagens->personagens[indice]);
    }
    for (indice = 0; indice < TOTAL_TIPOS_INIMIGO; indice++) {
        descarregar(imagens->inimigos[indice]);
    }
    for (indice = 0; indice < dados_total_mundos(); indice++) {
        descarregar(imagens->chao[indice]);
    }
    descarregar(imagens->projetil_heroi);
    descarregar(imagens->projetil_inimigo);
    descarregar(imagens->portal);
}
