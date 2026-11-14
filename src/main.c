/* main.c - ponto de entrada do jogo.
 * Abre a janela, controla a câmera e a máquina de estados das telas
 * (menu, como jogar, jogando). */

#include <math.h>

#include "raylib.h"
#include "config.h"
#include "interface.h"
#include "jogador.h"

typedef enum {
    TELA_MENU,
    TELA_COMO_JOGAR,
    TELA_JOGANDO
} Tela;

/* Desenha o chão como uma grade, só na parte visível pela câmera. */
static void desenhar_chao(Camera2D camera)
{
    int inicio_x = (int)floorf((camera.target.x - LARGURA_JANELA / 2.0f)
                               / TAMANHO_QUADRADO_CHAO) * TAMANHO_QUADRADO_CHAO;
    int inicio_y = (int)floorf((camera.target.y - ALTURA_JANELA / 2.0f)
                               / TAMANHO_QUADRADO_CHAO) * TAMANHO_QUADRADO_CHAO;
    int x;
    int y;

    for (x = inicio_x; x <= camera.target.x + LARGURA_JANELA / 2; x += TAMANHO_QUADRADO_CHAO) {
        for (y = inicio_y; y <= camera.target.y + ALTURA_JANELA / 2;
             y += TAMANHO_QUADRADO_CHAO) {
            DrawRectangleLines(x, y, TAMANHO_QUADRADO_CHAO, TAMANHO_QUADRADO_CHAO,
                               (Color){ 40, 36, 58, 255 });
        }
    }
}

/* Desenha o mundo (chão e herói) com a câmera seguindo o herói. */
static void desenhar_jogo(const Jogador *jogador, Camera2D camera)
{
    ClearBackground((Color){ 24, 22, 36, 255 });

    BeginMode2D(camera);
    desenhar_chao(camera);
    DrawCircle((int)jogador->posicao.x, (int)jogador->posicao.y, jogador->raio, SKYBLUE);
    EndMode2D();

    DrawText("ESC: voltar ao menu", 20, 20, 20, LIGHTGRAY);
    DrawFPS(20, ALTURA_JANELA - 40);
}

int main(void)
{
    Tela tela = TELA_MENU;
    int opcao_menu = OPCAO_JOGAR;
    int sair = 0;
    Jogador jogador = jogador_criar();
    Camera2D camera = { 0 };

    InitWindow(LARGURA_JANELA, ALTURA_JANELA, TITULO_JOGO);
    SetTargetFPS(FPS_ALVO);
    SetExitKey(KEY_NULL);    /* ESC não fecha a janela direto: usamos ele nas telas */

    camera.offset = (Vector2){ LARGURA_JANELA / 2.0f, ALTURA_JANELA / 2.0f };
    camera.zoom = 1.0f;

    while (!sair && !WindowShouldClose()) {
        BeginDrawing();
        if (tela == TELA_MENU) {
            int escolhida = interface_atualizar_menu(&opcao_menu);

            if (escolhida == OPCAO_JOGAR) {
                jogador = jogador_criar();
                tela = TELA_JOGANDO;
            } else if (escolhida == OPCAO_COMO_JOGAR) {
                tela = TELA_COMO_JOGAR;
            } else if (escolhida == OPCAO_SAIR) {
                sair = 1;
            }
            interface_desenhar_menu(opcao_menu);
        } else if (tela == TELA_COMO_JOGAR) {
            if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER)) {
                tela = TELA_MENU;
            }
            interface_desenhar_como_jogar();
        } else {
            if (IsKeyPressed(KEY_ESCAPE)) {
                tela = TELA_MENU;
            }
            jogador_atualizar(&jogador, GetFrameTime());
            camera.target = (Vector2){ jogador.posicao.x, jogador.posicao.y };
            desenhar_jogo(&jogador, camera);
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
