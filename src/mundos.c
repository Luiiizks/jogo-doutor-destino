/* mundos.c - controle do tempo do mundo, spawn dos inimigos, hordas, portal e chefe.
 * Cada inimigo que nasce é inserido no fim da lista de inimigos. */

#include <math.h>
#include <stddef.h>

#include "raylib.h"
#include "dados.h"
#include "mundos.h"

#define GRAUS_PARA_RADIANOS 0.0174533f

/* Ponto a "distancia" do centro, na direção do ângulo dado (em graus). */
static Vetor2 ponto_no_angulo(Vetor2 centro, float angulo_graus, float distancia)
{
    float angulo = angulo_graus * GRAUS_PARA_RADIANOS;
    Vetor2 ponto;

    ponto.x = centro.x + cosf(angulo) * distancia;
    ponto.y = centro.y + sinf(angulo) * distancia;
    return ponto;
}

/* Intervalo entre spawns agora: vai do valor inicial ao final de forma linear
 * e fica no valor final depois que o tempo do mundo acaba. */
static float intervalo_de_spawn_atual(const Mundo *mundo, float tempo_mundo)
{
    float progresso = tempo_mundo / mundo->duracao;

    if (progresso > 1.0f) {
        progresso = 1.0f;
    }
    return mundo->intervalo_inicial
           + (mundo->intervalo_final - mundo->intervalo_inicial) * progresso;
}

/* Faz nascer um inimigo de um tipo sorteado entre os que o mundo permite. */
static void criar_inimigo(Partida *partida, const Mundo *mundo, Vetor2 posicao)
{
    int sorteado = GetRandomValue(0, mundo->qtd_tipos - 1);

    lista_inserir_fim(partida->inimigos, dados_criar_inimigo(mundo->tipos[sorteado], posicao));
}

/* Spawn comum: um inimigo num ponto aleatório do círculo ao redor do herói. */
static void spawn_comum(Partida *partida, const Mundo *mundo)
{
    float angulo = (float)GetRandomValue(0, 359);

    criar_inimigo(partida, mundo, ponto_no_angulo(partida->jogador.posicao, angulo,
                                                  RAIO_SPAWN));
}

/* Horda: vários inimigos de uma vez, vindos do mesmo lado e crescendo a cada horda. */
static void criar_horda(Partida *partida, const Mundo *mundo)
{
    float angulo_central = (float)GetRandomValue(0, 359);
    int quantidade = HORDA_TAMANHO_BASE + HORDA_TAMANHO_CRESCIMENTO * partida->hordas_ocorridas;
    int indice;

    for (indice = 0; indice < quantidade && lista_contar(partida->inimigos) < LIMITE_INIMIGOS;
         indice++) {
        float angulo = angulo_central + (float)GetRandomValue(-HORDA_ABERTURA_GRAUS,
                                                              HORDA_ABERTURA_GRAUS);
        float distancia = RAIO_SPAWN + (float)GetRandomValue(0, 100);

        criar_inimigo(partida, mundo, ponto_no_angulo(partida->jogador.posicao, angulo,
                                                      distancia));
    }
    partida->hordas_ocorridas++;
    partida->aviso_horda = DURACAO_AVISO_HORDA;
}

/* Abre o portal longe do herói (só uma vez, quando o tempo do mundo acaba). */
static void abrir_portal(Partida *partida)
{
    float angulo = (float)GetRandomValue(0, 359);

    partida->posicao_portal = ponto_no_angulo(partida->jogador.posicao, angulo,
                                              DISTANCIA_PORTAL);
    partida->portal_aberto = 1;
}

/* Faz o chefe nascer acima do herói (só uma vez, no começo da arena). */
static void criar_chefe(Partida *partida)
{
    Vetor2 posicao = partida->jogador.posicao;

    posicao.y -= DISTANCIA_INICIAL_CHEFE;
    lista_inserir_fim(partida->inimigos, dados_criar_inimigo(TIPO_CHEFE, posicao));
    partida->chefe_criado = 1;
}

/* O chefe invoca Doombots num círculo ao redor dele (várias inserções na lista). */
static void invocar_reforcos(Partida *partida, Vetor2 centro)
{
    int indice;

    for (indice = 0; indice < QTD_REFORCOS_CHEFE
                     && lista_contar(partida->inimigos) < MAX_INIMIGOS_ARENA; indice++) {
        float angulo = 360.0f * (float)indice / (float)QTD_REFORCOS_CHEFE;
        Vetor2 posicao = ponto_no_angulo(centro, angulo, RAIO_REFORCOS_CHEFE);

        lista_inserir_fim(partida->inimigos, dados_criar_inimigo(TIPO_DOOMBOT, posicao));
    }
    partida->aviso_horda = DURACAO_AVISO_HORDA;
}

/* Comportamento do chefe a cada quadro: fase 2, reforços periódicos e intangibilidade. */
static void atualizar_chefe(Partida *partida, float dt)
{
    NoInimigo *chefe;
    float vida_maxima = dados_tipo_inimigo(TIPO_CHEFE)->vida;

    if (!partida->chefe_criado) {
        criar_chefe(partida);
    }
    chefe = lista_buscar_por_tipo(partida->inimigos, TIPO_CHEFE);
    if (chefe == NULL) {
        return;                                  /* chefe derrotado */
    }
    if (!partida->chefe_fase2 && chefe->dados.vida < vida_maxima * FASE2_FRACAO_VIDA) {
        partida->chefe_fase2 = 1;                /* fica mais rápido e invoca mais seguido */
        chefe->dados.velocidade *= FASE2_FATOR_VELOCIDADE;
    }
    partida->tempo_ate_reforcos -= dt;
    partida->tempo_ate_intangivel -= dt;
    if (partida->tempo_ate_reforcos <= 0.0f) {
        partida->tempo_ate_reforcos = partida->chefe_fase2 ? INTERVALO_REFORCOS_FASE2
                                                           : INTERVALO_REFORCOS;
        invocar_reforcos(partida, chefe->dados.posicao);
    }
    if (partida->tempo_ate_intangivel <= 0.0f) {
        partida->tempo_ate_intangivel = INTERVALO_INTANGIVEL;
        chefe->dados.tempo_intangivel = DURACAO_INTANGIVEL;
    }
}

void mundos_atualizar(Partida *partida, float dt)
{
    const Mundo *mundo = dados_mundo(partida->mundo);

    partida->tempo_mundo += dt;
    if (mundo->arena_chefe) {
        atualizar_chefe(partida, dt);
        return;
    }
    partida->tempo_ate_spawn -= dt;
    partida->tempo_ate_horda -= dt;
    if (partida->aviso_horda > 0.0f) {
        partida->aviso_horda -= dt;
    }

    if (partida->tempo_ate_spawn <= 0.0f) {
        partida->tempo_ate_spawn = intervalo_de_spawn_atual(mundo, partida->tempo_mundo);
        if (lista_contar(partida->inimigos) < LIMITE_INIMIGOS) {
            spawn_comum(partida, mundo);
        }
    }
    if (partida->tempo_ate_horda <= 0.0f) {
        partida->tempo_ate_horda = INTERVALO_HORDA;
        criar_horda(partida, mundo);
    }
    if (!partida->portal_aberto && partida->tempo_mundo >= mundo->duracao) {
        abrir_portal(partida);
    }
}

/* Portal aberto e herói encostando nele? */
static int jogador_no_portal(const Partida *partida)
{
    float diferenca_x = partida->jogador.posicao.x - partida->posicao_portal.x;
    float diferenca_y = partida->jogador.posicao.y - partida->posicao_portal.y;
    float soma_raios = RAIO_PORTAL + partida->jogador.raio;

    return partida->portal_aberto
           && diferenca_x * diferenca_x + diferenca_y * diferenca_y <= soma_raios * soma_raios;
}

int mundos_concluido(const Partida *partida)
{
    if (dados_mundo(partida->mundo)->arena_chefe) {
        return partida->chefe_criado
               && lista_buscar_por_tipo(partida->inimigos, TIPO_CHEFE) == NULL;
    }
    return jogador_no_portal(partida);
}
