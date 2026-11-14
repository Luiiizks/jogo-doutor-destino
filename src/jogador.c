/* jogador.c - herói: movimento pelo teclado, vida, nível e melhorias.
 * O herói só anda; o ataque é automático (fica em armas.c). */

#include <math.h>

#include "raylib.h"
#include "config.h"
#include "dados.h"
#include "jogador.h"

Jogador jogador_criar(int indice_personagem)
{
    const Personagem *dados = dados_personagem(indice_personagem);
    Jogador jogador = { 0 };

    jogador.personagem = indice_personagem;
    jogador.vida = dados->vida;
    jogador.vida_maxima = dados->vida;
    jogador.velocidade = dados->velocidade;
    jogador.raio = 16.0f;
    jogador.dano = dados->dano;
    jogador.cadencia = dados->cadencia;
    jogador.qtd_alvos = 1;
    jogador.nivel = 1;
    return jogador;
}

/* Move o herói segundo as teclas apertadas. */
static void mover_pelo_teclado(Jogador *jogador, float dt)
{
    float direcao_x = 0.0f;
    float direcao_y = 0.0f;
    float tamanho;

    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        direcao_x += 1.0f;
    }
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        direcao_x -= 1.0f;
    }
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
        direcao_y += 1.0f;
    }
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
        direcao_y -= 1.0f;
    }

    /* Na diagonal o vetor fica maior que 1; dividimos para não andar mais rápido. */
    tamanho = sqrtf(direcao_x * direcao_x + direcao_y * direcao_y);
    if (direcao_x != 0.0f) {
        jogador->olhando_esquerda = direcao_x < 0.0f;
    }
    if (tamanho > 0.0f) {
        jogador->posicao.x += direcao_x / tamanho * jogador->velocidade * dt;
        jogador->posicao.y += direcao_y / tamanho * jogador->velocidade * dt;
    }
}

void jogador_atualizar(Jogador *jogador, float dt)
{
    mover_pelo_teclado(jogador, dt);
    if (jogador->tempo_invulneravel > 0.0f) {
        jogador->tempo_invulneravel -= dt;
    }
}

void jogador_receber_dano(Jogador *jogador, float dano)
{
    if (dano <= 0.0f || jogador->tempo_invulneravel > 0.0f) {
        return;
    }
    jogador->vida -= dano;
    jogador->tempo_invulneravel = TEMPO_INVULNERAVEL;
}

int jogador_xp_para_proximo_nivel(const Jogador *jogador)
{
    return XP_BASE_NIVEL + XP_POR_NIVEL * jogador->nivel;
}

int jogador_pode_subir_nivel(const Jogador *jogador)
{
    return jogador->xp >= jogador_xp_para_proximo_nivel(jogador);
}

void jogador_subir_nivel(Jogador *jogador)
{
    jogador->xp -= jogador_xp_para_proximo_nivel(jogador);
    jogador->nivel++;
}

int jogador_sortear_upgrades(const Jogador *jogador, int escolhas[])
{
    int disponiveis[TOTAL_UPGRADES];
    int qtd_disponiveis = 0;
    int qtd_escolhidos = 0;
    int upgrade;

    for (upgrade = 0; upgrade < TOTAL_UPGRADES; upgrade++) {
        if (jogador->niveis_upgrade[upgrade] < NIVEL_MAX_UPGRADE) {
            disponiveis[qtd_disponiveis] = upgrade;
            qtd_disponiveis++;
        }
    }

    /* Sorteia um, copia para as escolhas e tira da lista de disponíveis (sem repetir). */
    while (qtd_escolhidos < OPCOES_LEVEL_UP && qtd_disponiveis > 0) {
        int sorteado = GetRandomValue(0, qtd_disponiveis - 1);

        escolhas[qtd_escolhidos] = disponiveis[sorteado];
        qtd_escolhidos++;
        qtd_disponiveis--;
        disponiveis[sorteado] = disponiveis[qtd_disponiveis];
    }
    return qtd_escolhidos;
}

/* Liga ou melhora o pulso de energia conforme o nível dele. */
static void melhorar_pulso(Jogador *jogador)
{
    int nivel_pulso = jogador->niveis_upgrade[UPGRADE_PULSO];

    jogador->raio_pulso = RAIO_PULSO_BASE + (nivel_pulso - 1) * RAIO_PULSO_POR_NIVEL;
    jogador->dano_pulso = DANO_PULSO_BASE + (nivel_pulso - 1) * DANO_PULSO_POR_NIVEL;
    if (nivel_pulso == 1) {
        jogador->tempo_ate_pulso = INTERVALO_PULSO;
    }
}

void jogador_aplicar_upgrade(Jogador *jogador, TipoUpgrade upgrade)
{
    if (jogador->niveis_upgrade[upgrade] >= NIVEL_MAX_UPGRADE) {
        return;
    }
    jogador->niveis_upgrade[upgrade]++;

    if (upgrade == UPGRADE_DANO) {
        jogador->dano += BONUS_DANO;
    } else if (upgrade == UPGRADE_CADENCIA) {
        jogador->cadencia *= FATOR_CADENCIA;
    } else if (upgrade == UPGRADE_PROJETIL) {
        jogador->qtd_alvos++;
    } else if (upgrade == UPGRADE_VELOCIDADE) {
        jogador->velocidade += BONUS_VELOCIDADE;
    } else if (upgrade == UPGRADE_VIDA) {
        jogador->vida_maxima += BONUS_VIDA;
        jogador->vida += BONUS_VIDA;
    } else {
        melhorar_pulso(jogador);
    }
}
