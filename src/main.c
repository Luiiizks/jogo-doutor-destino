/* main.c - ponto de entrada do jogo.
 * Abre a janela da raylib e roda o loop principal do jogo. */

#include "raylib.h"
#include "config.h"

/* Desenha um quadro: fundo escuro, o título e o FPS atual. */
static void desenhar_quadro(void)
{
    BeginDrawing();
    ClearBackground((Color){ 20, 16, 28, 255 });

    DrawText(TITULO_JOGO, 40, 40, 40, RAYWHITE);
    DrawText("Ambiente OK - aperte ESC para sair", 40, 100, 20, LIGHTGRAY);
    DrawFPS(40, ALTURA_JANELA - 40);

    EndDrawing();
}

int main(void)
{
    InitWindow(LARGURA_JANELA, ALTURA_JANELA, TITULO_JOGO);
    SetTargetFPS(FPS_ALVO);

    /* A raylib já fecha a janela ao apertar ESC (WindowShouldClose). */
    while (!WindowShouldClose()) {
        desenhar_quadro();
    }

    CloseWindow();
    return 0;
}
