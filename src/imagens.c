/* imagens.c - carregamento das imagens do jogo.
 * Se um arquivo não existir, o sprite fica zerado e o jogo desenha formas simples.
 * Personagens podem ter uma spritesheet de caminhada (quadros lado a lado). */

#include <stddef.h>

#include "imagens.h"

/* Carrega a imagem se o arquivo existir; senão devolve um sprite vazio.
 * "quadros" é quantos quadros a imagem tem lado a lado (1 para imagem parada). */
static Sprite carregar_sprite(const char *caminho, int quadros)
{
    Sprite sprite = { 0 };
    Image imagem;

    if (!FileExists(caminho)) {
        return sprite;
    }
    imagem = LoadImage(caminho);
    /* Retângulo que contém só os pixels visíveis; usamos a altura dele para dimensionar
     * o desenho sem depender da margem transparente do PNG. */
    sprite.altura_conteudo = GetImageAlphaBorder(imagem, 0.1f).height;
    sprite.textura = LoadTextureFromImage(imagem);
    UnloadImage(imagem);

    SetTextureFilter(sprite.textura, TEXTURE_FILTER_POINT);   /* mantém o pixel art nítido */
    sprite.quadros = quadros;
    if (sprite.altura_conteudo <= 0.0f) {
        sprite.altura_conteudo = (float)sprite.textura.height;
    }
    return sprite;
}

/* Tenta a spritesheet de caminhada; se ela não existir, usa a imagem parada. */
static Sprite carregar_animado(const char *arquivo_parado, const char *arquivo_caminhada,
                               int quadros_caminhada)
{
    Sprite sprite = { 0 };

    if (arquivo_caminhada != NULL) {
        sprite = carregar_sprite(arquivo_caminhada, quadros_caminhada);
    }
    if (sprite.textura.id == 0) {
        sprite = carregar_sprite(arquivo_parado, 1);
    }
    return sprite;
}

void imagens_carregar(Imagens *imagens)
{
    int indice;

    for (indice = 0; indice < dados_total_personagens(); indice++) {
        const Personagem *dados = dados_personagem(indice);

        imagens->personagens[indice] = carregar_animado(dados->arquivo_imagem,
            dados->arquivo_caminhada, dados->quadros_caminhada);
    }
    for (indice = 0; indice < TOTAL_TIPOS_INIMIGO; indice++) {
        const TipoInimigo *dados = dados_tipo_inimigo(indice);

        imagens->inimigos[indice] = carregar_animado(dados->arquivo_imagem,
            dados->arquivo_caminhada, dados->quadros_caminhada);
    }
    for (indice = 0; indice < dados_total_mundos(); indice++) {
        const Mundo *mundo = dados_mundo(indice);
        int objeto;

        imagens->chao[indice] = carregar_sprite(mundo->arquivo_chao, 1);
        for (objeto = 0; objeto < mundo->qtd_objetos; objeto++) {
            imagens->cenario[indice][objeto] = carregar_sprite(
                mundo->objetos[objeto].arquivo_imagem, 1);
        }
    }
    imagens->projetil_heroi = carregar_sprite("assets/sprites/projetil_arcano.png", 1);
    imagens->projetil_inimigo = carregar_sprite("assets/sprites/projetil_inimigo.png", 1);
    imagens->portal = carregar_sprite("assets/sprites/portal.png", 1);
}

/* Descarrega a textura só se ela foi carregada. */
static void descarregar(Sprite sprite)
{
    if (sprite.textura.id > 0) {
        UnloadTexture(sprite.textura);
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
        int objeto;

        descarregar(imagens->chao[indice]);
        for (objeto = 0; objeto < dados_mundo(indice)->qtd_objetos; objeto++) {
            descarregar(imagens->cenario[indice][objeto]);
        }
    }
    descarregar(imagens->projetil_heroi);
    descarregar(imagens->projetil_inimigo);
    descarregar(imagens->portal);
}
