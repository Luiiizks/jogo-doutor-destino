/* interface.c - menu principal e tela de ajuda.
 * Só desenha e lê entrada de menu; não conhece as regras do jogo. */

#include "raylib.h"
#include "config.h"
#include "interface.h"

#define LARGURA_BOTAO  320
#define ALTURA_BOTAO   56
#define ESPACO_BOTAO   20
#define TOPO_BOTOES    300

static const char *rotulos_menu[TOTAL_OPCOES_MENU] = { "Jogar", "Como jogar", "Sair" };

/* Retângulo do botão de uma opção (centralizado na tela). */
static Rectangle retangulo_botao(int indice)
{
    Rectangle botao;

    botao.x = (LARGURA_JANELA - LARGURA_BOTAO) / 2.0f;
    botao.y = (float)(TOPO_BOTOES + indice * (ALTURA_BOTAO + ESPACO_BOTAO));
    botao.width = LARGURA_BOTAO;
    botao.height = ALTURA_BOTAO;
    return botao;
}

int interface_atualizar_menu(int *selecionada)
{
    Vector2 mouse = GetMousePosition();
    int indice;

    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        *selecionada = (*selecionada + 1) % TOTAL_OPCOES_MENU;
    }
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        *selecionada = (*selecionada + TOTAL_OPCOES_MENU - 1) % TOTAL_OPCOES_MENU;
    }

    /* Mouse: passar por cima seleciona, clicar confirma. */
    for (indice = 0; indice < TOTAL_OPCOES_MENU; indice++) {
        if (CheckCollisionPointRec(mouse, retangulo_botao(indice))) {
            if (GetMouseDelta().x != 0.0f || GetMouseDelta().y != 0.0f) {
                *selecionada = indice;
            }
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                return indice;
            }
        }
    }

    if (IsKeyPressed(KEY_ENTER)) {
        return *selecionada;
    }
    return -1;
}

void interface_desenhar_menu(int selecionada)
{
    int indice;

    ClearBackground((Color){ 20, 16, 28, 255 });
    DrawText(TITULO_JOGO, (LARGURA_JANELA - MeasureText(TITULO_JOGO, 64)) / 2, 120, 64,
             RAYWHITE);
    DrawText("Sobreviva ao multiverso do Doutor Destino",
             (LARGURA_JANELA - MeasureText("Sobreviva ao multiverso do Doutor Destino", 22)) / 2,
             200, 22, LIGHTGRAY);

    for (indice = 0; indice < TOTAL_OPCOES_MENU; indice++) {
        Rectangle botao = retangulo_botao(indice);
        Color cor_fundo = (indice == selecionada) ? (Color){ 90, 60, 160, 255 }
                                                  : (Color){ 45, 38, 64, 255 };
        int largura_texto = MeasureText(rotulos_menu[indice], 28);

        DrawRectangleRec(botao, cor_fundo);
        DrawText(rotulos_menu[indice], (int)(botao.x + (botao.width - largura_texto) / 2),
                 (int)(botao.y + 14), 28, RAYWHITE);
    }
}

void interface_desenhar_como_jogar(void)
{
    ClearBackground((Color){ 20, 16, 28, 255 });
    DrawText("Como jogar", 80, 60, 48, RAYWHITE);
    DrawText("O heroi ataca sozinho os inimigos mais proximos.", 80, 160, 26, LIGHTGRAY);
    DrawText("Mover: WASD ou setas", 80, 220, 26, LIGHTGRAY);
    DrawText("A cada nivel, escolha uma melhoria.", 80, 270, 26, LIGHTGRAY);
    DrawText("Sobreviva aos 3 mundos e derrote o chefe final.", 80, 320, 26, LIGHTGRAY);
    DrawText("ESC ou ENTER para voltar", 80, ALTURA_JANELA - 80, 22, GRAY);
}
