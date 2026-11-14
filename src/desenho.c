/* desenho.c - desenho da partida na tela.
 * Por enquanto tudo é forma geométrica; depois dá para trocar por imagens aqui. */

#include <math.h>
#include <stddef.h>

#include "dados.h"
#include "desenho.h"
#include "jogador.h"

#define LARGURA_BARRA_VIDA  260
#define ALTURA_BARRA        18
#define ALTURA_BARRA_XP     12

/* Desenha uma imagem centralizada em "centro", espelhada se "espelhar" for 1. */
static void desenhar_imagem(Texture2D textura, Vetor2 centro, int espelhar, Color tinta)
{
    float largura = (float)textura.width;
    float altura = (float)textura.height;
    Rectangle origem = { 0.0f, 0.0f, espelhar ? -largura : largura, altura };
    Rectangle destino = { centro.x, centro.y, largura, altura };
    Vector2 pivo = { largura / 2.0f, altura / 2.0f };

    DrawTexturePro(textura, origem, destino, pivo, 0.0f, tinta);
}

/* Chão com a imagem repetida (se existir) ou uma grade simples. */
static void desenhar_chao(Camera2D camera, const Mundo *mundo, Texture2D imagem_chao)
{
    int tamanho = (imagem_chao.id > 0) ? imagem_chao.width : TAMANHO_QUADRADO_CHAO;
    int inicio_x = (int)floorf((camera.target.x - LARGURA_JANELA / 2.0f) / tamanho) * tamanho;
    int inicio_y = (int)floorf((camera.target.y - ALTURA_JANELA / 2.0f) / tamanho) * tamanho;
    int posicao_x;
    int posicao_y;

    for (posicao_x = inicio_x; posicao_x <= camera.target.x + LARGURA_JANELA / 2;
         posicao_x += tamanho) {
        for (posicao_y = inicio_y; posicao_y <= camera.target.y + ALTURA_JANELA / 2;
             posicao_y += tamanho) {
            if (imagem_chao.id > 0) {
                DrawTexture(imagem_chao, posicao_x, posicao_y, WHITE);
            } else {
                DrawRectangleLines(posicao_x, posicao_y, tamanho, tamanho,
                                   mundo->cor_linhas_chao);
            }
        }
    }
}

/* Percorre a lista de inimigos desenhando cada um (imagem ou círculo). */
static void desenhar_inimigos(const ListaInimigos *lista, const Imagens *imagens,
                              Vetor2 posicao_jogador)
{
    const NoInimigo *atual;

    for (atual = lista->inicio; atual != NULL; atual = atual->proximo) {
        const Inimigo *inimigo = &atual->dados;
        Texture2D imagem = imagens->inimigos[inimigo->tipo];

        if (imagem.id > 0) {
            /* A imagem olha para a direita; espelha se o herói está à esquerda. */
            desenhar_imagem(imagem, inimigo->posicao,
                            posicao_jogador.x < inimigo->posicao.x, WHITE);
        } else {
            DrawCircle((int)inimigo->posicao.x, (int)inimigo->posicao.y, inimigo->raio,
                       dados_tipo_inimigo(inimigo->tipo)->cor);
        }
    }
}

/* Desenha uma lista de projéteis com a imagem dada ou um círculo da cor dada. */
static void desenhar_projeteis(const Projetil projeteis[], int quantidade,
                               Texture2D imagem, Color cor)
{
    int indice;

    for (indice = 0; indice < quantidade; indice++) {
        if (imagem.id > 0) {
            desenhar_imagem(imagem, projeteis[indice].posicao, 0, WHITE);
        } else {
            DrawCircle((int)projeteis[indice].posicao.x, (int)projeteis[indice].posicao.y,
                       projeteis[indice].raio, cor);
        }
    }
}

/* Herói: pisca enquanto está invulnerável. Anel do pulso quando ele dispara. */
static void desenhar_jogador(const Jogador *jogador, Texture2D imagem)
{
    int piscando = jogador->tempo_invulneravel > 0.0f
                   && ((int)(jogador->tempo_invulneravel * 20.0f) % 2 == 0);

    if (jogador->efeito_pulso > 0.0f) {
        DrawCircleLines((int)jogador->posicao.x, (int)jogador->posicao.y, jogador->raio_pulso,
                        (Color){ 200, 170, 255, 255 });
    }
    if (imagem.id > 0) {
        desenhar_imagem(imagem, jogador->posicao, jogador->olhando_esquerda,
                        piscando ? (Color){ 255, 255, 255, 110 } : WHITE);
    } else {
        DrawCircle((int)jogador->posicao.x, (int)jogador->posicao.y, jogador->raio,
                   piscando ? WHITE : dados_personagem(jogador->personagem)->cor);
    }
}

/* Portal: imagem (se existir) ou anéis que pulsam. */
static void desenhar_portal(Vector2 posicao_portal, Texture2D imagem)
{
    float pulso = 4.0f * sinf((float)GetTime() * 4.0f);

    if (imagem.id > 0) {
        desenhar_imagem(imagem, (Vetor2){ posicao_portal.x, posicao_portal.y }, 0, WHITE);
        return;
    }
    DrawCircle((int)posicao_portal.x, (int)posicao_portal.y, RAIO_PORTAL + pulso,
               (Color){ 90, 40, 160, 255 });
    DrawCircleLines((int)posicao_portal.x, (int)posicao_portal.y, RAIO_PORTAL + 12.0f + pulso,
                    (Color){ 200, 150, 255, 255 });
    DrawCircle((int)posicao_portal.x, (int)posicao_portal.y, RAIO_PORTAL * 0.5f, BLACK);
}

/* Seta na borda da tela apontando para o portal quando ele está longe. */
static void desenhar_indicador_portal(const Partida *partida)
{
    float diferenca_x = partida->posicao_portal.x - partida->jogador.posicao.x;
    float diferenca_y = partida->posicao_portal.y - partida->jogador.posicao.y;
    float distancia = sqrtf(diferenca_x * diferenca_x + diferenca_y * diferenca_y);
    int ponto_x;
    int ponto_y;

    if (distancia < ALTURA_JANELA / 2.0f) {
        return;                                  /* já está perto, dá para ver */
    }
    ponto_x = (int)(LARGURA_JANELA / 2.0f + diferenca_x / distancia * (LARGURA_JANELA / 2.0f - 60));
    ponto_y = (int)(ALTURA_JANELA / 2.0f + diferenca_y / distancia * (ALTURA_JANELA / 2.0f - 60));
    DrawCircle(ponto_x, ponto_y, 14.0f, (Color){ 200, 150, 255, 255 });
    DrawText("Portal", ponto_x - 24, ponto_y + 18, 18, RAYWHITE);
}

/* Barra horizontal preenchida de acordo com valor/maximo. */
static void desenhar_barra(int posicao_x, int posicao_y, int largura, int altura,
                           float valor, float maximo, Color cor)
{
    float fracao = (maximo > 0.0f) ? valor / maximo : 0.0f;

    if (fracao < 0.0f) {
        fracao = 0.0f;
    }
    DrawRectangle(posicao_x, posicao_y, largura, altura, (Color){ 0, 0, 0, 160 });
    DrawRectangle(posicao_x, posicao_y, (int)(largura * fracao), altura, cor);
}

static void desenhar_hud(const Partida *partida)
{
    const Jogador *jogador = &partida->jogador;
    const Mundo *mundo = dados_mundo(partida->mundo);
    float tempo_restante = mundo->duracao - partida->tempo_mundo;
    int segundos = (int)ceilf(tempo_restante > 0.0f ? tempo_restante : 0.0f);

    desenhar_barra(20, 20, LARGURA_BARRA_VIDA, ALTURA_BARRA, jogador->vida,
                   jogador->vida_maxima, (Color){ 220, 70, 80, 255 });
    DrawText(TextFormat("%d / %d", (int)jogador->vida, (int)jogador->vida_maxima), 28, 21, 16,
             RAYWHITE);
    DrawText(TextFormat("Nível %d", jogador->nivel), 20, 46, 22, RAYWHITE);
    if (partida->portal_aberto) {
        DrawText(TextFormat("%s   Portal aberto!", mundo->nome), LARGURA_JANELA - 360, 20, 24,
                 (Color){ 200, 150, 255, 255 });
        desenhar_indicador_portal(partida);
    } else {
        DrawText(TextFormat("%s   %d:%02d", mundo->nome, segundos / 60, segundos % 60),
                 LARGURA_JANELA - 260, 20, 24, RAYWHITE);
    }
    if (partida->aviso_horda > 0.0f) {
        DrawText("HORDA!", (LARGURA_JANELA - MeasureText("HORDA!", 48)) / 2, 90, 48,
                 (Color){ 255, 90, 90, 255 });
    }
    DrawText(TextFormat("Inimigos: %d", lista_contar(partida->inimigos)),
             LARGURA_JANELA - 260, 52, 20, LIGHTGRAY);

    desenhar_barra(0, ALTURA_JANELA - ALTURA_BARRA_XP, LARGURA_JANELA, ALTURA_BARRA_XP,
                   (float)jogador->xp, (float)jogador_xp_para_proximo_nivel(jogador),
                   (Color){ 110, 200, 255, 255 });
    DrawFPS(20, ALTURA_JANELA - 40);
}

void desenho_partida(const Partida *partida, const Imagens *imagens, Camera2D camera)
{
    const Jogador *jogador = &partida->jogador;

    ClearBackground((Color){ 24, 22, 36, 255 });

    BeginMode2D(camera);
    desenhar_chao(camera, dados_mundo(partida->mundo), imagens->chao[partida->mundo]);
    if (partida->portal_aberto) {
        desenhar_portal((Vector2){ partida->posicao_portal.x, partida->posicao_portal.y },
                        imagens->portal);
    }
    desenhar_inimigos(partida->inimigos, imagens, jogador->posicao);
    desenhar_projeteis(partida->projeteis, partida->qtd_projeteis, imagens->projetil_heroi,
                       (Color){ 190, 150, 255, 255 });
    desenhar_projeteis(partida->tiros_inimigos, partida->qtd_tiros_inimigos,
                       imagens->projetil_inimigo, (Color){ 255, 90, 90, 255 });
    desenhar_jogador(jogador, imagens->personagens[jogador->personagem]);
    EndMode2D();

    desenhar_hud(partida);
}
