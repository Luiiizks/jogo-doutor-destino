/* desenho.c - desenho da partida na tela: chão, inimigos, herói, projéteis e HUD.
 * Usa imagens quando existem; senão desenha formas geométricas. */

#include <math.h>
#include <stddef.h>

#include "cenario.h"
#include "dados.h"
#include "desenho.h"
#include "jogador.h"
#include "mundos.h"

#define LARGURA_BARRA_VIDA  260
#define ALTURA_BARRA        18
#define ALTURA_BARRA_XP     12

/* Escolhe o quadro da animação de caminhada. Parado (ou sem spritesheet) mostra o primeiro.
 * Andando, troca FPS_ANIMACAO vezes por segundo, medido em segundos de jogo (não em quadros
 * da tela), então a animação não depende do FPS. "deslocamento" faz cada personagem
 * começar num quadro diferente, para a horda não andar toda sincronizada. */
static int escolher_quadro(const Sprite *sprite, int andando, float tempo, int deslocamento)
{
    if (sprite->quadros <= 1 || !andando) {
        return 0;
    }
    return ((int)(tempo * FPS_ANIMACAO) + deslocamento) % sprite->quadros;
}

/* Desenha um quadro do sprite centralizado em "centro".
 * altura_visual: altura desejada na tela da parte visível (0 = tamanho original do PNG).
 * A escala é a mesma nos dois eixos, então a proporção do desenho não distorce. */
static void desenhar_sprite(const Sprite *sprite, Vetor2 centro, int espelhar,
                            float altura_visual, int quadro, Color tinta)
{
    float escala = (altura_visual > 0.0f) ? altura_visual / sprite->altura_conteudo : 1.0f;
    float largura_quadro = (float)sprite->textura.width / (float)sprite->quadros;
    float altura = (float)sprite->textura.height;
    Rectangle origem = { quadro * largura_quadro, 0.0f,
                         espelhar ? -largura_quadro : largura_quadro, altura };
    Rectangle destino = { roundf(centro.x), roundf(centro.y),
                          largura_quadro * escala, altura * escala };
    Vector2 pivo = { destino.width / 2.0f, destino.height / 2.0f };

    DrawTexturePro(sprite->textura, origem, destino, pivo, 0.0f, tinta);
}

void desenho_retrato(const Sprite *sprite, Vetor2 centro, float altura)
{
    desenhar_sprite(sprite, centro, 0, altura, 0, WHITE);
}

/* Chão com a imagem repetida (se existir) ou uma grade simples. */
static void desenhar_chao(Camera2D camera, const Mundo *mundo, const Sprite *imagem_chao)
{
    int tamanho = (imagem_chao->textura.id > 0) ? imagem_chao->textura.width
                                                : TAMANHO_QUADRADO_CHAO;
    int inicio_x = (int)floorf((camera.target.x - LARGURA_JANELA / 2.0f) / tamanho) * tamanho;
    int inicio_y = (int)floorf((camera.target.y - ALTURA_JANELA / 2.0f) / tamanho) * tamanho;
    int posicao_x;
    int posicao_y;

    for (posicao_x = inicio_x; posicao_x <= camera.target.x + LARGURA_JANELA / 2;
         posicao_x += tamanho) {
        for (posicao_y = inicio_y; posicao_y <= camera.target.y + ALTURA_JANELA / 2;
             posicao_y += tamanho) {
            if (imagem_chao->textura.id > 0) {
                DrawTexture(imagem_chao->textura, posicao_x, posicao_y, WHITE);
            } else {
                DrawRectangleLines(posicao_x, posicao_y, tamanho, tamanho,
                                   mundo->cor_linhas_chao);
            }
        }
    }
}

/* Altura do inimigo na tela: a da tabela, e maior se for elite. */
static float altura_do_inimigo(const Inimigo *inimigo)
{
    float altura = dados_tipo_inimigo(inimigo->tipo)->altura_visual;

    return inimigo->elite ? altura * ELITE_FATOR_TAMANHO : altura;
}

/* Sombra oval nos pés do personagem, para ele não parecer flutuando. */
static void desenhar_sombra(Vetor2 centro, float altura)
{
    DrawEllipse((int)centro.x, (int)(centro.y + altura / 2.0f - 3.0f), altura * 0.28f,
                altura * 0.09f, (Color){ 0, 0, 0, 90 });
}

/* Sombras de todos (herói e inimigos), desenhadas antes dos personagens. */
static void desenhar_sombras(const Partida *partida, const Imagens *imagens)
{
    const NoInimigo *atual;
    const Jogador *jogador = &partida->jogador;

    for (atual = partida->inimigos->inicio; atual != NULL; atual = atual->proximo) {
        float altura = (imagens->inimigos[atual->dados.tipo].textura.id > 0)
                       ? altura_do_inimigo(&atual->dados) : atual->dados.raio * 2.0f;

        desenhar_sombra(atual->dados.posicao, altura);
    }
    desenhar_sombra(jogador->posicao,
                    (imagens->personagens[jogador->personagem].textura.id > 0)
                    ? dados_personagem(jogador->personagem)->altura_visual
                    : jogador->raio * 2.0f);
}

/* Números de dano: sobem e vão ficando transparentes. */
static void desenhar_numeros_dano(const Partida *partida)
{
    int indice;

    for (indice = 0; indice < partida->qtd_numeros_dano; indice++) {
        const NumeroDano *numero = &partida->numeros_dano[indice];
        float transparencia = numero->tempo_restante / DURACAO_NUMERO_DANO;

        DrawText(TextFormat("%d", numero->valor), (int)numero->posicao.x + 1,
                 (int)numero->posicao.y + 1, 20, Fade(BLACK, transparencia));
        DrawText(TextFormat("%d", numero->valor), (int)numero->posicao.x,
                 (int)numero->posicao.y, 20, Fade(YELLOW, transparencia));
    }
}

/* Desenha um objeto de cenário com o "pé" dele em "base". O pé é o meio da borda de baixo da
 * parte visível da imagem (não do quadro todo, que pode ter margem transparente). Não desenha
 * sombra: muitas imagens já trazem a sombra delas. */
static void desenhar_objeto(const Sprite *sprite, Vetor2 base, float altura_visual)
{
    float escala = altura_visual / sprite->altura_conteudo;
    float pe_x = (sprite->conteudo.x + sprite->conteudo.width / 2.0f) * escala;
    float pe_y = (sprite->conteudo.y + sprite->conteudo.height) * escala;
    Rectangle origem = { 0.0f, 0.0f, (float)sprite->textura.width,
                         (float)sprite->textura.height };
    Rectangle destino = { roundf(base.x - pe_x), roundf(base.y - pe_y),
                          sprite->textura.width * escala, sprite->textura.height * escala };

    DrawTexturePro(sprite->textura, origem, destino, (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);
}

/* Desenha os objetos de cenário que aparecem na tela. Cada célula do mapa sorteia seu
 * objeto (ver cenario.c). Só desenha os que têm imagem. "atras_do_heroi" separa os objetos
 * acima do herói (desenhados antes dele) dos abaixo (desenhados depois), para o herói passar
 * "por trás" de um poste quando está acima dele. */
static void desenhar_cenario(const Partida *partida, const Imagens *imagens, Camera2D camera,
                             int atras_do_heroi)
{
    const Mundo *mundo = dados_mundo(partida->mundo);
    int primeira_x = (int)floorf((camera.target.x - LARGURA_JANELA / 2.0f)
                                 / TAMANHO_CELULA_CENARIO) - 1;
    int ultima_x = (int)floorf((camera.target.x + LARGURA_JANELA / 2.0f)
                               / TAMANHO_CELULA_CENARIO) + 1;
    int primeira_y = (int)floorf((camera.target.y - ALTURA_JANELA / 2.0f)
                                 / TAMANHO_CELULA_CENARIO) - 1;
    int ultima_y = (int)floorf((camera.target.y + ALTURA_JANELA / 2.0f)
                               / TAMANHO_CELULA_CENARIO) + 1;
    int celula_x;
    int celula_y;

    for (celula_x = primeira_x; celula_x <= ultima_x; celula_x++) {
        for (celula_y = primeira_y; celula_y <= ultima_y; celula_y++) {
            Vetor2 base;
            int objeto = cenario_sortear_objeto(partida->mundo, celula_x, celula_y,
                                                mundo->qtd_objetos, &base);
            const Sprite *sprite;
            float altura;

            if (objeto < 0 || (base.y <= partida->jogador.posicao.y) != atras_do_heroi) {
                continue;
            }
            sprite = &imagens->cenario[partida->mundo][objeto];
            altura = mundo->objetos[objeto].altura_visual;
            if (sprite->textura.id > 0) {
                desenhar_objeto(sprite, base, altura);
            }
        }
    }
}

/* Cor de mistura do inimigo: translúcido se intangível, avermelhado se levou dano. */
static Color escolher_tinta(const Inimigo *inimigo)
{
    if (inimigo->tempo_intangivel > 0.0f) {
        return (Color){ 255, 255, 255, 90 };
    }
    if (inimigo->tempo_flash > 0.0f) {
        return (Color){ 255, 80, 80, 255 };
    }
    return inimigo->elite ? (Color){ 255, 220, 110, 255 } : WHITE;     /* elite = dourado */
}

/* Desenha um inimigo (imagem ou círculo). */
static void desenhar_inimigo(const Inimigo *inimigo, const Imagens *imagens,
                             Vetor2 posicao_jogador, float tempo)
{
    const Sprite *sprite = &imagens->inimigos[inimigo->tipo];
    const TipoInimigo *dados = dados_tipo_inimigo(inimigo->tipo);

    if (sprite->textura.id > 0) {
        /* A imagem olha para a direita; espelha se o herói está à esquerda. */
        desenhar_sprite(sprite, inimigo->posicao, posicao_jogador.x < inimigo->posicao.x,
                        dados->altura_visual,
                        escolher_quadro(sprite, inimigo->andando, tempo, inimigo->id),
                        escolher_tinta(inimigo));
    } else {
        DrawCircle((int)inimigo->posicao.x, (int)inimigo->posicao.y, inimigo->raio,
                   inimigo->tempo_intangivel > 0.0f ? Fade(dados->cor, 0.35f)
                   : (inimigo->tempo_flash > 0.0f ? WHITE
                      : (inimigo->elite ? GOLD : dados->cor)));
    }
}

/* Percorre a lista desenhando os inimigos de um lado do herói: os que estão mais acima
 * (atras_do_heroi = 1) ou mais abaixo (0). Assim quem está embaixo aparece na frente,
 * como numa visão de cima inclinada. */
static void desenhar_inimigos(const Partida *partida, const Imagens *imagens,
                              int atras_do_heroi)
{
    const NoInimigo *atual;
    Vetor2 posicao_jogador = partida->jogador.posicao;

    for (atual = partida->inimigos->inicio; atual != NULL; atual = atual->proximo) {
        int esta_atras = atual->dados.posicao.y <= posicao_jogador.y;

        if (esta_atras == atras_do_heroi) {
            desenhar_inimigo(&atual->dados, imagens, posicao_jogador, partida->tempo_mundo);
        }
    }
}

/* Cor da gema de XP conforme o valor (azul = pouco, verde = médio, dourado = muito). */
static Color cor_da_gema(int valor)
{
    if (valor <= 6) {
        return (Color){ 120, 210, 255, 255 };
    }
    return (valor <= 12) ? (Color){ 120, 255, 150, 255 } : (Color){ 255, 215, 90, 255 };
}

/* Um drop no chão: imagem (se existir) ou forma simples. Flutua um pouquinho. */
static void desenhar_drop(const Drop *drop, const Sprite *imagem, float tempo)
{
    Vetor2 centro = { drop->posicao.x, drop->posicao.y + 2.0f * sinf(tempo * 6.0f
                                                                     + drop->posicao.x * 0.1f) };

    if (imagem->textura.id > 0) {
        desenhar_sprite(imagem, centro, 0, 0.0f, 0, WHITE);
    } else if (drop->tipo == DROP_XP) {
        DrawPoly((Vector2){ centro.x, centro.y }, 4, 9.0f, 0.0f, cor_da_gema(drop->valor));
        DrawPolyLines((Vector2){ centro.x, centro.y }, 4, 9.0f, 0.0f, WHITE);
    } else {
        Color cor = (drop->tipo == DROP_IMA) ? (Color){ 230, 80, 200, 255 }
                                             : (Color){ 230, 60, 70, 255 };

        DrawCircle((int)centro.x, (int)centro.y, 10.0f, cor);
        DrawCircleLines((int)centro.x, (int)centro.y, 12.0f, WHITE);
        DrawText(drop->tipo == DROP_IMA ? "I" : "+", (int)centro.x - 4, (int)centro.y - 8, 16,
                 WHITE);
    }
}

static void desenhar_todos_os_drops(const Partida *partida, const Imagens *imagens)
{
    int indice;

    for (indice = 0; indice < partida->qtd_drops; indice++) {
        desenhar_drop(&partida->drops[indice], &imagens->drops[partida->drops[indice].tipo],
                      partida->tempo_mundo);
    }
}

/* Desenha um projétil: imagem (se existir) ou um círculo da cor dada. */
static void desenhar_projetil(const Projetil *projetil, const Sprite *imagem, Color cor)
{
    if (imagem->textura.id > 0) {
        desenhar_sprite(imagem, projetil->posicao, 0, 0.0f, 0, WHITE);
    } else {
        DrawCircle((int)projetil->posicao.x, (int)projetil->posicao.y, projetil->raio, cor);
    }
}

/* Tiros do herói (cada herói tem a sua imagem e cor) e tiros dos inimigos. */
static void desenhar_todos_os_tiros(const Partida *partida, const Imagens *imagens)
{
    int indice;

    for (indice = 0; indice < partida->qtd_projeteis; indice++) {
        const Projetil *tiro = &partida->projeteis[indice];

        desenhar_projetil(tiro, &imagens->projetil_heroi[tiro->personagem],
                          dados_personagem(tiro->personagem)->cor_projetil);
    }
    for (indice = 0; indice < partida->qtd_tiros_inimigos; indice++) {
        desenhar_projetil(&partida->tiros_inimigos[indice], &imagens->projetil_inimigo,
                          (Color){ 255, 90, 90, 255 });
    }
}

/* Herói: pisca enquanto está invulnerável. Anel do pulso quando ele dispara. */
static void desenhar_jogador(const Jogador *jogador, const Sprite *imagem, float tempo)
{
    int piscando = jogador->tempo_invulneravel > 0.0f
                   && ((int)(jogador->tempo_invulneravel * 20.0f) % 2 == 0);
    const Personagem *dados = dados_personagem(jogador->personagem);

    if (jogador->qtd_alvos > 0) {          /* anel discreto mostrando o alcance dos tiros */
        DrawCircleLines((int)jogador->posicao.x, (int)jogador->posicao.y, jogador->alcance,
                        Fade(WHITE, 0.12f));
    }
    if (jogador->efeito_pulso > 0.0f) {
        DrawCircle((int)jogador->posicao.x, (int)jogador->posicao.y, jogador->raio_pulso,
                   Fade((Color){ 200, 170, 255, 255 }, 0.3f * jogador->efeito_pulso
                                                       / DURACAO_EFEITO_PULSO));
        DrawCircleLines((int)jogador->posicao.x, (int)jogador->posicao.y, jogador->raio_pulso,
                        (Color){ 200, 170, 255, 255 });
    }
    if (imagem->textura.id > 0) {
        desenhar_sprite(imagem, jogador->posicao, jogador->olhando_esquerda,
                        dados->altura_visual, escolher_quadro(imagem, jogador->andando, tempo, 0),
                        piscando ? (Color){ 255, 255, 255, 110 } : WHITE);
    } else {
        DrawCircle((int)jogador->posicao.x, (int)jogador->posicao.y, jogador->raio,
                   piscando ? WHITE : dados->cor);
    }
}

/* Portal: imagem (se existir) ou anéis que pulsam. */
static void desenhar_portal(Vector2 posicao_portal, const Sprite *imagem)
{
    float pulso = 4.0f * sinf((float)GetTime() * 4.0f);

    if (imagem->textura.id > 0) {
        desenhar_sprite(imagem, (Vetor2){ posicao_portal.x, posicao_portal.y }, 0, 0.0f, 0,
                        WHITE);
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

/* Escurece tudo fora da arena e desenha a borda dourada dela. */
static void desenhar_paredes_da_arena(void)
{
    float esquerda = -ARENA_METADE_LARGURA;
    float topo = -ARENA_METADE_ALTURA;
    float largura = ARENA_METADE_LARGURA * 2.0f;
    float altura = ARENA_METADE_ALTURA * 2.0f;
    float margem = 2000.0f;          /* grande o bastante para cobrir a tela inteira */
    Color escuro = { 8, 6, 12, 255 };

    DrawRectangle((int)(esquerda - margem), (int)(topo - margem), (int)margem,
                  (int)(altura + 2 * margem), escuro);
    DrawRectangle((int)(esquerda + largura), (int)(topo - margem), (int)margem,
                  (int)(altura + 2 * margem), escuro);
    DrawRectangle((int)esquerda, (int)(topo - margem), (int)largura, (int)margem, escuro);
    DrawRectangle((int)esquerda, (int)(topo + altura), (int)largura, (int)margem, escuro);
    DrawRectangleLinesEx((Rectangle){ esquerda, topo, largura, altura }, 8.0f,
                         (Color){ 200, 170, 60, 255 });
}

/* Texto encostado na direita da tela (para nomes longos de mundo não saírem da janela). */
static void desenhar_texto_direita(const char *texto, int posicao_y, int tamanho, Color cor)
{
    DrawText(texto, LARGURA_JANELA - MeasureText(texto, tamanho) - 20, posicao_y, tamanho, cor);
}

/* Barra de vida grande do chefe, no topo da tela, com aviso quando está intangível. */
static void desenhar_barra_chefe(const Partida *partida)
{
    const NoInimigo *chefe = lista_buscar_por_tipo(partida->inimigos, TIPO_CHEFE);
    const TipoInimigo *dados = dados_tipo_inimigo(TIPO_CHEFE);
    int largura = 600;
    int posicao_x = (LARGURA_JANELA - largura) / 2;

    if (chefe == NULL) {
        return;
    }
    DrawText(dados->nome, posicao_x, 12, 24, RAYWHITE);
    desenhar_barra(posicao_x, 42, largura, 22, chefe->dados.vida, partida->vida_chefe_maxima,
                   (Color){ 70, 190, 110, 255 });
    if (chefe->dados.tempo_intangivel > 0.0f) {
        DrawText("INTANGÍVEL", posicao_x, 68, 20, (Color){ 200, 220, 255, 255 });
    }
}

/* Informações do mundo no canto direito: tempo, portal ou nada (arena do chefe). */
static void desenhar_info_do_mundo(const Partida *partida)
{
    const Mundo *mundo = dados_mundo(partida->mundo);
    float tempo_restante = mundos_duracao(partida) - partida->tempo_mundo;
    int segundos = (int)ceilf(tempo_restante > 0.0f ? tempo_restante : 0.0f);

    if (mundo->arena_chefe) {
        desenhar_texto_direita(mundo->nome, 20, 24, (Color){ 230, 200, 90, 255 });
        desenhar_barra_chefe(partida);
    } else if (partida->portal_aberto) {
        desenhar_texto_direita(TextFormat("%s   Portal aberto!", mundo->nome), 20, 24,
                               (Color){ 200, 150, 255, 255 });
        desenhar_indicador_portal(partida);
    } else {
        desenhar_texto_direita(TextFormat("%s   %d:%02d", mundo->nome, segundos / 60,
                                          segundos % 60), 20, 24, RAYWHITE);
    }
}

static void desenhar_hud(const Partida *partida)
{
    const Jogador *jogador = &partida->jogador;

    desenhar_barra(20, 20, LARGURA_BARRA_VIDA, ALTURA_BARRA, jogador->vida,
                   jogador->vida_maxima, (Color){ 220, 70, 80, 255 });
    DrawText(TextFormat("%d / %d", (int)jogador->vida, (int)jogador->vida_maxima), 28, 21, 16,
             RAYWHITE);
    DrawText(TextFormat("Nível %d", jogador->nivel), 20, 46, 22, RAYWHITE);
    desenhar_info_do_mundo(partida);
    if (partida->aviso_horda > 0.0f) {
        const char *aviso = partida_eh_arena(partida) ? "REFORÇOS!" : "HORDA!";

        DrawText(aviso, (LARGURA_JANELA - MeasureText(aviso, 48)) / 2, 90, 48,
                 (Color){ 255, 90, 90, 255 });
    }
    desenhar_texto_direita(TextFormat("Inimigos: %d", lista_contar(partida->inimigos)), 52, 20,
                           LIGHTGRAY);

    desenhar_barra(0, ALTURA_JANELA - ALTURA_BARRA_XP, LARGURA_JANELA, ALTURA_BARRA_XP,
                   (float)jogador->xp, (float)jogador_xp_para_proximo_nivel(jogador),
                   (Color){ 110, 200, 255, 255 });
    DrawFPS(20, ALTURA_JANELA - 40);
}

void desenho_partida(const Partida *partida, const Imagens *imagens, Camera2D camera)
{
    const Jogador *jogador = &partida->jogador;

    /* Tremor: desloca a câmera um pouco, usando seno do tempo de jogo (assim para na
     * pausa) e diminuindo até acabar. */
    if (partida->tempo_tremor > 0.0f) {
        float forca = FORCA_TREMOR * partida->tempo_tremor / DURACAO_TREMOR;

        camera.offset.x += sinf(partida->tempo_mundo * 90.0f) * forca;
        camera.offset.y += cosf(partida->tempo_mundo * 110.0f) * forca;
    }
    ClearBackground((Color){ 24, 22, 36, 255 });

    BeginMode2D(camera);
    desenhar_chao(camera, dados_mundo(partida->mundo), &imagens->chao[partida->mundo]);
    if (partida->portal_aberto) {
        desenhar_portal((Vector2){ partida->posicao_portal.x, partida->posicao_portal.y },
                        &imagens->portal);
    }
    desenhar_cenario(partida, imagens, camera, 1);    /* objetos acima do herói */
    desenhar_todos_os_drops(partida, imagens);        /* gemas e itens no chão */
    desenhar_sombras(partida, imagens);
    desenhar_inimigos(partida, imagens, 1);          /* inimigos acima do herói */
    desenhar_jogador(jogador, &imagens->personagens[jogador->personagem], partida->tempo_mundo);
    desenhar_inimigos(partida, imagens, 0);          /* inimigos abaixo do herói */
    desenhar_cenario(partida, imagens, camera, 0);    /* objetos abaixo do herói */
    desenhar_todos_os_tiros(partida, imagens);
    desenhar_numeros_dano(partida);
    if (partida_eh_arena(partida)) {
        desenhar_paredes_da_arena();
    }
    EndMode2D();

    desenhar_hud(partida);
}
