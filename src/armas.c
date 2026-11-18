/* armas.c - arma principal (disparo arcano) e pulso de energia.
 * A mira automática usa a lista de inimigos ordenada: os primeiros nós são os mais
 * próximos, então o disparo só precisa pegar os N primeiros. */

#include <math.h>
#include <stddef.h>

#include "raylib.h"

#include "armas.h"
#include "dados.h"
#include "jogador.h"

/* Sorteia se este ataque é crítico (chance_critico é em %). */
static int sortear_critico(const Jogador *jogador)
{
    return GetRandomValue(1, 100) <= jogador->chance_critico;
}

/* Cria um projétil saindo do herói em direção a um alvo. Ignora se o vetor de projéteis
 * estiver cheio ou se o alvo estiver exatamente em cima do herói. */
static void criar_projetil(Partida *partida, Vetor2 alvo)
{
    const Jogador *jogador = &partida->jogador;
    float diferenca_x = alvo.x - jogador->posicao.x;
    float diferenca_y = alvo.y - jogador->posicao.y;
    float distancia = sqrtf(diferenca_x * diferenca_x + diferenca_y * diferenca_y);
    Projetil *novo;

    if (partida->qtd_projeteis >= MAX_PROJETEIS || distancia < 0.001f) {
        return;
    }
    novo = &partida->projeteis[partida->qtd_projeteis];
    novo->posicao = jogador->posicao;
    novo->velocidade.x = diferenca_x / distancia * VELOCIDADE_PROJETIL;
    novo->velocidade.y = diferenca_y / distancia * VELOCIDADE_PROJETIL;
    novo->critico = sortear_critico(jogador);
    novo->dano = novo->critico ? jogador->dano * FATOR_CRITICO : jogador->dano;
    novo->raio = RAIO_PROJETIL;
    novo->personagem = jogador->personagem;
    novo->tempo_restante = jogador->alcance / VELOCIDADE_PROJETIL + 0.05f;   /* some no alcance */
    partida->qtd_projeteis++;
}

/* Um projétil para cada um dos inimigos mais próximos (primeiros nós da lista). */
static int disparar_arcano(Partida *partida)
{
    NoInimigo *alvos[MAX_ALVOS];
    const NoInimigo *chefe;
    float alcance = partida->jogador.alcance;
    int qtd_alvos = lista_obter_mais_proximos(partida->inimigos, partida->jogador.qtd_alvos,
                                              alcance, alvos);
    int disparos = qtd_alvos;
    int indice;

    for (indice = 0; indice < qtd_alvos; indice++) {
        criar_projetil(partida, alvos[indice]->dados.posicao);
    }

    /* Na arena, um tiro extra vai sempre para o chefe (se ele pode ser atingido e está no
     * alcance), senão os reforços, que ficam mais perto, tomariam todos os disparos. */
    chefe = lista_buscar_por_tipo(partida->inimigos, TIPO_CHEFE);
    if (partida->jogador.qtd_alvos > 0 && chefe != NULL
        && chefe->dados.tempo_intangivel <= 0.0f
        && chefe->dados.distancia_quadrada <= alcance * alcance) {
        criar_projetil(partida, chefe->dados.posicao);
        disparos++;
    }
    return disparos;
}

/* Cria um número de dano na posição do acerto (ignora se o vetor estiver cheio). */
static void criar_numero_dano(Partida *partida, Vetor2 posicao, float dano, int critico)
{
    NumeroDano *numero;

    if (partida->qtd_numeros_dano >= MAX_NUMEROS_DANO) {
        return;
    }
    numero = &partida->numeros_dano[partida->qtd_numeros_dano];
    numero->posicao = posicao;
    numero->valor = (int)dano;
    numero->critico = critico;
    numero->tempo_restante = DURACAO_NUMERO_DANO;
    partida->qtd_numeros_dano++;
}

/* Faz os números de dano subirem e tira os que já acabaram. */
static void atualizar_numeros_dano(Partida *partida, float dt)
{
    int indice = 0;

    while (indice < partida->qtd_numeros_dano) {
        NumeroDano *numero = &partida->numeros_dano[indice];

        numero->posicao.y -= VELOCIDADE_NUMERO_DANO * dt;
        numero->tempo_restante -= dt;
        if (numero->tempo_restante <= 0.0f) {
            partida->qtd_numeros_dano--;
            partida->numeros_dano[indice] = partida->numeros_dano[partida->qtd_numeros_dano];
        } else {
            indice++;
        }
    }
}

/* Move os projéteis, testa colisão com a lista e tira os que acertaram ou expiraram. */
static void atualizar_projeteis(Partida *partida, float dt)
{
    int indice = 0;

    while (indice < partida->qtd_projeteis) {
        Projetil *projetil = &partida->projeteis[indice];
        int acertou;

        projetil->posicao.x += projetil->velocidade.x * dt;
        projetil->posicao.y += projetil->velocidade.y * dt;
        projetil->tempo_restante -= dt;
        acertou = lista_colidir_com_projetil(partida->inimigos, projetil);

        if (acertou) {
            criar_numero_dano(partida, projetil->posicao, projetil->dano, projetil->critico);
        }
        if (acertou || projetil->tempo_restante <= 0.0f) {
            /* Troca pelo último do vetor (a ordem dos projéteis não importa). */
            partida->qtd_projeteis--;
            partida->projeteis[indice] = partida->projeteis[partida->qtd_projeteis];
        } else {
            indice++;
        }
    }
}

/* Um golpe da onda de impacto: machuca todos num raio ao redor do herói. */
static void dar_golpe(Partida *partida)
{
    Jogador *jogador = &partida->jogador;

    jogador->efeito_pulso = DURACAO_EFEITO_PULSO;
    lista_dano_em_area(partida->inimigos, jogador->posicao, jogador->raio_pulso,
                       sortear_critico(jogador) ? jogador->dano_pulso * FATOR_CRITICO
                                                : jogador->dano_pulso);
}

/* Onda de impacto (soco do Hulk / pulso): a cada "intervalo_pulso" segundos o herói bate
 * "golpes_pulso" vezes seguidas, com um pequeno intervalo entre elas. */
static void atualizar_pulso(Partida *partida, float dt)
{
    Jogador *jogador = &partida->jogador;

    if (jogador->efeito_pulso > 0.0f) {
        jogador->efeito_pulso -= dt;
    }
    if (jogador->raio_pulso <= 0.0f) {
        return;
    }
    if (jogador->golpes_restantes > 0) {
        jogador->tempo_ate_golpe -= dt;
        if (jogador->tempo_ate_golpe <= 0.0f) {
            dar_golpe(partida);
            jogador->golpes_restantes--;
            jogador->tempo_ate_golpe = INTERVALO_ENTRE_GOLPES;
        }
        return;
    }
    jogador->tempo_ate_pulso -= dt;
    if (jogador->tempo_ate_pulso <= 0.0f) {
        jogador->tempo_ate_pulso = jogador->intervalo_pulso;
        jogador->golpes_restantes = jogador->golpes_pulso;
        jogador->tempo_ate_golpe = 0.0f;
    }
}

/* Tiros dos inimigos à distância: a lista cria os tiros novos, aqui eles andam e
 * machucam o herói se encostarem nele. */
static void atualizar_tiros_inimigos(Partida *partida, float dt)
{
    Jogador *jogador = &partida->jogador;
    int indice = 0;

    partida->qtd_tiros_inimigos += lista_coletar_tiros(
        partida->inimigos, jogador->posicao, dt,
        &partida->tiros_inimigos[partida->qtd_tiros_inimigos],
        MAX_TIROS_INIMIGOS - partida->qtd_tiros_inimigos);

    while (indice < partida->qtd_tiros_inimigos) {
        Projetil *tiro = &partida->tiros_inimigos[indice];
        float soma_raios = tiro->raio + jogador->raio;
        float diferenca_x = tiro->posicao.x - jogador->posicao.x;
        float diferenca_y = tiro->posicao.y - jogador->posicao.y;
        int acertou;

        tiro->posicao.x += tiro->velocidade.x * dt;
        tiro->posicao.y += tiro->velocidade.y * dt;
        tiro->tempo_restante -= dt;
        acertou = diferenca_x * diferenca_x + diferenca_y * diferenca_y
                  <= soma_raios * soma_raios;
        if (acertou) {
            jogador_receber_dano(jogador, tiro->dano);
        }
        if (acertou || tiro->tempo_restante <= 0.0f) {
            partida->qtd_tiros_inimigos--;
            partida->tiros_inimigos[indice] =
                partida->tiros_inimigos[partida->qtd_tiros_inimigos];
        } else {
            indice++;
        }
    }
}

void armas_atualizar(Partida *partida, float dt)
{
    Jogador *jogador = &partida->jogador;

    jogador->tempo_ate_disparo -= dt;
    if (jogador->tempo_ate_disparo <= 0.0f && !lista_esta_vazia(partida->inimigos)
        && disparar_arcano(partida) > 0) {
        jogador->tempo_ate_disparo = jogador->cadencia;   /* só recarrega se atirou */
    }
    atualizar_projeteis(partida, dt);
    atualizar_pulso(partida, dt);
    atualizar_tiros_inimigos(partida, dt);
    atualizar_numeros_dano(partida, dt);
}
