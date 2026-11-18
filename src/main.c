/* main.c - ponto de entrada do jogo.
 * Abre a janela, controla a câmera e a máquina de estados das telas
 * (menu, jogando, pausa, level-up, fim). */

#include "raylib.h"
#include "config.h"
#include "dados.h"
#include "desenho.h"
#include "imagens.h"
#include "interface.h"
#include "jogador.h"
#include "partida.h"
#include "ranking.h"

typedef enum {
    TELA_MENU,
    TELA_COMO_JOGAR,
    TELA_RANKING,
    TELA_NOME,
    TELA_SELECAO_HEROI,
    TELA_SELECAO_MODO,
    TELA_JOGANDO,
    TELA_PAUSA,
    TELA_LEVEL_UP,
    TELA_DERROTA,
    TELA_VITORIA,
    TELA_TRANSICAO
} Tela;

/* Tudo que o loop principal precisa lembrar entre um quadro e outro. */
typedef struct {
    Tela tela;
    int opcao;                          /* botão destacado no menu atual */
    int sair;
    int heroi;                          /* herói escolhido (índice na tabela) */
    ModoJogo modo;
    Partida partida;
    Camera2D camera;
    Imagens imagens;
    int escolhas[OPCOES_LEVEL_UP];      /* melhorias sorteadas no level-up */
    int qtd_escolhas;
    char nome[TAMANHO_NOME];            /* nome do jogador (fica lembrado entre as partidas) */
    Ranking ranking;
    int pontuacao_final;                /* pontos da partida que acabou de terminar */
    int posicao_ranking;                /* onde ela ficou no ranking (0 = não entrou) */
} Jogo;

/* Começa uma partida nova com o herói e o modo escolhidos, no primeiro mundo. */
static void iniciar_partida(Jogo *jogo)
{
    if (partida_iniciar(&jogo->partida, jogo->heroi, 0, jogo->modo)) {
        jogo->tela = TELA_JOGANDO;
    } else {
        jogo->tela = TELA_MENU;     /* faltou memória: volta ao menu em vez de travar */
    }
    jogo->opcao = 0;
}

/* Fim da partida (derrota ou vitória): coloca a pontuação no ranking e grava o arquivo. */
static void registrar_pontuacao(Jogo *jogo)
{
    jogo->pontuacao_final = partida_pontuacao(&jogo->partida);
    jogo->posicao_ranking = ranking_registrar(&jogo->ranking, jogo->nome,
                                              jogo->pontuacao_final);
    ranking_salvar(&jogo->ranking, ARQUIVO_RANKING);
}

/* Se o herói juntou XP, sorteia as melhorias e abre a tela de level-up. */
static void verificar_level_up(Jogo *jogo)
{
    Jogador *jogador = &jogo->partida.jogador;

    if (!jogador_pode_subir_nivel(jogador)) {
        return;
    }
    jogo->qtd_escolhas = jogador_sortear_upgrades(jogador, jogo->escolhas);
    if (jogo->qtd_escolhas == 0) {
        jogador_subir_nivel(jogador);       /* tudo no máximo: só sobe de nível */
        return;
    }
    jogo->opcao = 0;
    jogo->tela = TELA_LEVEL_UP;
}

static void atualizar_menu(Jogo *jogo)
{
    int escolhida = interface_atualizar_opcoes(&jogo->opcao, TOTAL_OPCOES_MENU);

    if (escolhida == OPCAO_JOGAR) {
        jogo->opcao = 0;
        jogo->tela = TELA_NOME;
    } else if (escolhida == OPCAO_RANKING) {
        jogo->posicao_ranking = 0;
        jogo->tela = TELA_RANKING;
    } else if (escolhida == OPCAO_COMO_JOGAR) {
        jogo->tela = TELA_COMO_JOGAR;
    } else if (escolhida == OPCAO_SAIR) {
        jogo->sair = 1;
    }
}

/* Digitação do nome; ENTER segue para a escolha do herói e ESC volta ao menu. */
static void atualizar_nome(Jogo *jogo)
{
    int resultado = interface_atualizar_nome(jogo->nome);

    if (resultado == 1) {
        jogo->opcao = 0;
        jogo->tela = TELA_SELECAO_HEROI;
    } else if (resultado == -1) {
        jogo->opcao = 0;
        jogo->tela = TELA_MENU;
    }
}

/* Escolha do herói: ENTER, clique ou teclas 1-3; ESC volta ao menu. */
static void atualizar_selecao_heroi(Jogo *jogo)
{
    int escolhido = interface_atualizar_cartas(&jogo->opcao, dados_total_personagens(), 1);

    if (IsKeyPressed(KEY_ESCAPE)) {
        jogo->opcao = 0;
        jogo->tela = TELA_MENU;
    } else if (escolhido >= 0) {
        jogo->heroi = escolhido;
        jogo->opcao = 0;
        jogo->tela = TELA_SELECAO_MODO;
    }
}

/* Escolha do modo (normal ou rápido) e início da partida; ESC volta à seleção de herói. */
static void atualizar_selecao_modo(Jogo *jogo)
{
    int escolhida = interface_atualizar_opcoes(&jogo->opcao, TOTAL_OPCOES_MODO);

    if (IsKeyPressed(KEY_ESCAPE)) {
        jogo->opcao = jogo->heroi;
        jogo->tela = TELA_SELECAO_HEROI;
    } else if (escolhida >= 0) {
        jogo->modo = (escolhida == OPCAO_MODO_RAPIDO) ? MODO_RAPIDO : MODO_NORMAL;
        iniciar_partida(jogo);
    }
}

static void atualizar_jogando(Jogo *jogo)
{
    float dt = GetFrameTime();
    ResultadoPartida resultado;

    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_P)) {
        jogo->opcao = 0;
        jogo->tela = TELA_PAUSA;
        return;
    }
    if (dt > DT_MAXIMO) {
        dt = DT_MAXIMO;
    }
    resultado = partida_atualizar(&jogo->partida, dt);
    jogo->camera.target = (Vector2){ jogo->partida.jogador.posicao.x,
                                     jogo->partida.jogador.posicao.y };

    if (resultado == PARTIDA_DERROTA) {
        registrar_pontuacao(jogo);
        jogo->opcao = 0;
        jogo->tela = TELA_DERROTA;
    } else if (resultado == PARTIDA_MUNDO_CONCLUIDO) {
        jogo->opcao = 0;
        if (partida_tem_proximo_mundo(&jogo->partida)) {
            jogo->tela = TELA_TRANSICAO;
        } else {
            registrar_pontuacao(jogo);
            jogo->tela = TELA_VITORIA;
        }
    } else {
        verificar_level_up(jogo);
    }
}

static void atualizar_pausa(Jogo *jogo)
{
    int escolhida = interface_atualizar_opcoes(&jogo->opcao, TOTAL_OPCOES_PAUSA);

    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_P)) {
        escolhida = OPCAO_PAUSA_CONTINUAR;
    }
    if (escolhida == OPCAO_PAUSA_CONTINUAR) {
        jogo->tela = TELA_JOGANDO;
    } else if (escolhida == OPCAO_PAUSA_MENU) {
        partida_encerrar(&jogo->partida);
        jogo->opcao = 0;
        jogo->tela = TELA_MENU;
    }
}

static void atualizar_level_up(Jogo *jogo)
{
    Jogador *jogador = &jogo->partida.jogador;
    int escolhida = interface_atualizar_cartas(&jogo->opcao, jogo->qtd_escolhas, 0);

    if (escolhida >= 0) {
        jogador_aplicar_upgrade(jogador, (TipoUpgrade)jogo->escolhas[escolhida]);
        jogador_subir_nivel(jogador);
        jogo->tela = TELA_JOGANDO;
    }
}

/* Derrota e vitória têm os mesmos botões: jogar de novo (pede o nome outra vez) ou menu. */
static void atualizar_fim(Jogo *jogo)
{
    int escolhida = interface_atualizar_opcoes(&jogo->opcao, TOTAL_OPCOES_FIM);

    if (escolhida == OPCAO_FIM_JOGAR_DE_NOVO) {
        partida_encerrar(&jogo->partida);
        jogo->opcao = 0;
        jogo->tela = TELA_NOME;
    } else if (escolhida == OPCAO_FIM_MENU) {
        partida_encerrar(&jogo->partida);
        jogo->opcao = 0;
        jogo->tela = TELA_MENU;
    }
}

/* Passa para o mundo seguinte quando o jogador confirma na tela de transição. */
static void atualizar_transicao(Jogo *jogo)
{
    if (!interface_atualizar_transicao()) {
        return;
    }
    if (partida_proximo_mundo(&jogo->partida)) {
        jogo->tela = TELA_JOGANDO;
    } else {
        jogo->tela = TELA_MENU;     /* faltou memória: volta ao menu em vez de travar */
    }
}

static void atualizar(Jogo *jogo)
{
    if (jogo->tela == TELA_MENU) {
        atualizar_menu(jogo);
    } else if (jogo->tela == TELA_COMO_JOGAR) {
        if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER)) {
            jogo->tela = TELA_MENU;
        }
    } else if (jogo->tela == TELA_RANKING) {
        if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER)) {
            jogo->tela = TELA_MENU;
        }
    } else if (jogo->tela == TELA_NOME) {
        atualizar_nome(jogo);
    } else if (jogo->tela == TELA_SELECAO_HEROI) {
        atualizar_selecao_heroi(jogo);
    } else if (jogo->tela == TELA_SELECAO_MODO) {
        atualizar_selecao_modo(jogo);
    } else if (jogo->tela == TELA_JOGANDO) {
        atualizar_jogando(jogo);
    } else if (jogo->tela == TELA_PAUSA) {
        atualizar_pausa(jogo);
    } else if (jogo->tela == TELA_LEVEL_UP) {
        atualizar_level_up(jogo);
    } else if (jogo->tela == TELA_TRANSICAO) {
        atualizar_transicao(jogo);
    } else {
        atualizar_fim(jogo);
    }
}

static void desenhar(const Jogo *jogo)
{
    if (jogo->tela == TELA_MENU) {
        interface_desenhar_menu_principal(&jogo->imagens, jogo->opcao);
    } else if (jogo->tela == TELA_COMO_JOGAR) {
        interface_desenhar_como_jogar(&jogo->imagens);
    } else if (jogo->tela == TELA_RANKING) {
        interface_desenhar_ranking(&jogo->imagens, &jogo->ranking, 0);
    } else if (jogo->tela == TELA_NOME) {
        interface_desenhar_nome(&jogo->imagens, jogo->nome);
    } else if (jogo->tela == TELA_SELECAO_HEROI) {
        interface_desenhar_selecao_heroi(&jogo->imagens, jogo->opcao);
    } else if (jogo->tela == TELA_SELECAO_MODO) {
        interface_desenhar_selecao_modo(&jogo->imagens, jogo->opcao);
    } else {
        /* Todas as outras telas mostram a partida ao fundo. */
        desenho_partida(&jogo->partida, &jogo->imagens, jogo->camera);
        if (jogo->tela == TELA_PAUSA) {
            interface_desenhar_pausa(jogo->opcao);
        } else if (jogo->tela == TELA_LEVEL_UP) {
            interface_desenhar_level_up(&jogo->partida.jogador, jogo->escolhas,
                                        jogo->qtd_escolhas, jogo->opcao);
        } else if (jogo->tela == TELA_TRANSICAO) {
            interface_desenhar_transicao(dados_mundo(jogo->partida.mundo)->nome,
                                         dados_mundo(jogo->partida.mundo + 1)->nome);
        } else if (jogo->tela == TELA_DERROTA) {
            interface_desenhar_derrota(jogo->opcao, jogo->pontuacao_final,
                                       jogo->posicao_ranking);
        } else if (jogo->tela == TELA_VITORIA) {
            interface_desenhar_vitoria(jogo->opcao, jogo->pontuacao_final,
                                       jogo->posicao_ranking);
        }
    }
}

int main(void)
{
    Jogo jogo = { 0 };

    InitWindow(LARGURA_JANELA, ALTURA_JANELA, TITULO_JOGO);
    SetTargetFPS(FPS_ALVO);
    SetExitKey(KEY_NULL);    /* ESC não fecha a janela direto: usamos ele nas telas */

    ranking_carregar(&jogo.ranking, ARQUIVO_RANKING);
    imagens_carregar(&jogo.imagens);
    jogo.camera.offset = (Vector2){ LARGURA_JANELA / 2.0f, ALTURA_JANELA / 2.0f };
    jogo.camera.zoom = 1.0f;

    while (!jogo.sair && !WindowShouldClose()) {
        atualizar(&jogo);
        BeginDrawing();
        desenhar(&jogo);
        EndDrawing();
    }

    partida_encerrar(&jogo.partida);
    imagens_descarregar(&jogo.imagens);
    CloseWindow();
    return 0;
}
