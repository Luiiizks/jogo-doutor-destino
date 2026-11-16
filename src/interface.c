/* interface.c - menus e telas de texto.
 * Só desenha e lê entrada de menu; não conhece as regras do jogo. */

#include "raylib.h"
#include "config.h"
#include "dados.h"
#include "interface.h"

#define LARGURA_BOTAO  320
#define ALTURA_BOTAO   56
#define ESPACO_BOTAO   20
#define TOPO_MENU      300
#define TOPO_TELAS     330

#define LARGURA_CARTA  300
#define ALTURA_CARTA   260
#define ESPACO_CARTA   30
#define TOPO_CARTAS    220

static const Color COR_FUNDO = { 20, 16, 28, 255 };
static const Color COR_BOTAO = { 45, 38, 64, 255 };
static const Color COR_BOTAO_SELECIONADO = { 90, 60, 160, 255 };

static const char *const rotulos_menu[TOTAL_OPCOES_MENU] = { "Jogar", "Como jogar", "Sair" };
static const char *const rotulos_pausa[TOTAL_OPCOES_PAUSA] = { "Continuar", "Voltar ao menu" };
static const char *const rotulos_fim[TOTAL_OPCOES_FIM] = { "Jogar de novo", "Voltar ao menu" };

static int mouse_mexeu(void)
{
    return GetMouseDelta().x != 0.0f || GetMouseDelta().y != 0.0f;
}

/* Desenha um texto centralizado na horizontal. */
static void desenhar_texto_centralizado(const char *texto, int y, int tamanho, Color cor)
{
    DrawText(texto, (LARGURA_JANELA - MeasureText(texto, tamanho)) / 2, y, tamanho, cor);
}

/* Retângulo do botão de uma opção (centralizado na tela). */
static Rectangle retangulo_botao(int indice, int topo)
{
    Rectangle botao;

    botao.x = (LARGURA_JANELA - LARGURA_BOTAO) / 2.0f;
    botao.y = (float)(topo + indice * (ALTURA_BOTAO + ESPACO_BOTAO));
    botao.width = LARGURA_BOTAO;
    botao.height = ALTURA_BOTAO;
    return botao;
}

/* Topo da lista de botões: o menu principal fica mais abaixo por causa do título. */
static int topo_dos_botoes(int total)
{
    return (total == TOTAL_OPCOES_MENU) ? TOPO_MENU : TOPO_TELAS;
}

int interface_atualizar_opcoes(int *selecionada, int total)
{
    Vector2 mouse = GetMousePosition();
    int indice;

    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        *selecionada = (*selecionada + 1) % total;
    }
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        *selecionada = (*selecionada + total - 1) % total;
    }

    /* Mouse: passar por cima seleciona, clicar confirma. */
    for (indice = 0; indice < total; indice++) {
        if (CheckCollisionPointRec(mouse, retangulo_botao(indice, topo_dos_botoes(total)))) {
            if (mouse_mexeu()) {
                *selecionada = indice;
            }
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                return indice;
            }
        }
    }
    return IsKeyPressed(KEY_ENTER) ? *selecionada : -1;
}

static void desenhar_opcoes(const char *const rotulos[], int total, int selecionada)
{
    int indice;

    for (indice = 0; indice < total; indice++) {
        Rectangle botao = retangulo_botao(indice, topo_dos_botoes(total));
        int largura_texto = MeasureText(rotulos[indice], 28);

        DrawRectangleRec(botao, indice == selecionada ? COR_BOTAO_SELECIONADO : COR_BOTAO);
        DrawText(rotulos[indice], (int)(botao.x + (botao.width - largura_texto) / 2),
                 (int)(botao.y + 14), 28, RAYWHITE);
    }
}

/* Escurece o jogo que está desenhado atrás (pausa, level-up, fim). */
static void escurecer_tela(void)
{
    DrawRectangle(0, 0, LARGURA_JANELA, ALTURA_JANELA, (Color){ 0, 0, 0, 170 });
}

void interface_desenhar_menu_principal(int selecionada)
{
    ClearBackground(COR_FUNDO);
    desenhar_texto_centralizado(TITULO_JOGO, 120, 64, RAYWHITE);
    desenhar_texto_centralizado("Sobreviva ao multiverso do Doutor Destino", 200, 22, LIGHTGRAY);
    desenhar_opcoes(rotulos_menu, TOTAL_OPCOES_MENU, selecionada);
}

void interface_desenhar_como_jogar(void)
{
    ClearBackground(COR_FUNDO);
    DrawText("Como jogar", 80, 60, 48, RAYWHITE);
    DrawText("O herói ataca sozinho os inimigos mais próximos.", 80, 160, 26, LIGHTGRAY);
    DrawText("Mover: WASD ou setas   |   Pausar: ESC ou P", 80, 220, 26, LIGHTGRAY);
    DrawText("A cada nível, escolha uma melhoria (teclas 1, 2 e 3).", 80, 270, 26, LIGHTGRAY);
    DrawText("Sobreviva até o fim do tempo de cada mundo.", 80, 320, 26, LIGHTGRAY);
    DrawText("ESC ou ENTER para voltar", 80, ALTURA_JANELA - 80, 22, GRAY);
}

void interface_desenhar_pausa(int selecionada)
{
    escurecer_tela();
    desenhar_texto_centralizado("Pausado", 200, 56, RAYWHITE);
    desenhar_opcoes(rotulos_pausa, TOTAL_OPCOES_PAUSA, selecionada);
}

void interface_desenhar_derrota(int selecionada)
{
    escurecer_tela();
    desenhar_texto_centralizado("Você caiu...", 200, 56, (Color){ 230, 90, 90, 255 });
    desenhar_opcoes(rotulos_fim, TOTAL_OPCOES_FIM, selecionada);
}

void interface_desenhar_vitoria(int selecionada)
{
    escurecer_tela();
    desenhar_texto_centralizado("VITÓRIA!", 150, 72, GOLD);
    desenhar_texto_centralizado("Doutor Destino foi derrotado e o multiverso está salvo.", 250,
                                26, RAYWHITE);
    desenhar_opcoes(rotulos_fim, TOTAL_OPCOES_FIM, selecionada);
}

int interface_atualizar_transicao(void)
{
    return IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)
           || IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

void interface_desenhar_transicao(const char *mundo_concluido, const char *proximo_mundo)
{
    escurecer_tela();
    desenhar_texto_centralizado(TextFormat("%s concluído!", mundo_concluido), 190, 52,
                                (Color){ 120, 220, 140, 255 });
    desenhar_texto_centralizado("Próximo mundo:", 290, 26, LIGHTGRAY);
    desenhar_texto_centralizado(proximo_mundo, 330, 44, RAYWHITE);
    desenhar_texto_centralizado("Você recupera parte da vida e mantém suas melhorias.", 420, 22,
                                LIGHTGRAY);
    desenhar_texto_centralizado("ENTER para continuar", 520, 26, GOLD);
}

/* Retângulo da carta de melhoria de um índice (as cartas ficam lado a lado, centralizadas). */
static Rectangle retangulo_carta(int indice, int quantidade)
{
    float largura_total = (float)(quantidade * LARGURA_CARTA + (quantidade - 1) * ESPACO_CARTA);
    Rectangle carta;

    carta.x = (LARGURA_JANELA - largura_total) / 2.0f + indice * (LARGURA_CARTA + ESPACO_CARTA);
    carta.y = TOPO_CARTAS;
    carta.width = LARGURA_CARTA;
    carta.height = ALTURA_CARTA;
    return carta;
}

int interface_atualizar_level_up(int *selecionada, int quantidade)
{
    Vector2 mouse = GetMousePosition();
    int indice;

    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
        *selecionada = (*selecionada + 1) % quantidade;
    }
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
        *selecionada = (*selecionada + quantidade - 1) % quantidade;
    }
    for (indice = 0; indice < quantidade; indice++) {
        if (IsKeyPressed(KEY_ONE + indice)) {
            return indice;
        }
        if (CheckCollisionPointRec(mouse, retangulo_carta(indice, quantidade))) {
            if (mouse_mexeu()) {
                *selecionada = indice;
            }
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                return indice;
            }
        }
    }
    return IsKeyPressed(KEY_ENTER) ? *selecionada : -1;
}

void interface_desenhar_level_up(const Jogador *jogador, const int escolhas[],
                                 int quantidade, int selecionada)
{
    int indice;

    escurecer_tela();
    desenhar_texto_centralizado("Subiu de nível! Escolha uma melhoria", 120, 40, RAYWHITE);

    for (indice = 0; indice < quantidade; indice++) {
        Rectangle carta = retangulo_carta(indice, quantidade);
        const DadosUpgrade *dados = dados_upgrade((TipoUpgrade)escolhas[indice]);
        int nivel_atual = jogador->niveis_upgrade[escolhas[indice]];

        DrawRectangleRec(carta, indice == selecionada ? COR_BOTAO_SELECIONADO : COR_BOTAO);
        DrawText(TextFormat("%d", indice + 1), (int)carta.x + 14, (int)carta.y + 10, 24, GRAY);
        DrawText(dados->nome, (int)carta.x + 20, (int)carta.y + 60, 28, RAYWHITE);
        DrawText(TextFormat("Nível %d -> %d", nivel_atual, nivel_atual + 1),
                 (int)carta.x + 20, (int)carta.y + 110, 20, GOLD);
        DrawText(dados->descricao, (int)carta.x + 20, (int)carta.y + 150, 18, LIGHTGRAY);
    }
}
