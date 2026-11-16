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
    jogador.qtd_alvos = dados->qtd_alvos;
    jogador.alcance = dados->alcance;
    if (dados->raio_pulso > 0.0f) {            /* herói que já começa com o pulso (Hulk) */
        jogador.raio_pulso = dados->raio_pulso;
        jogador.dano_pulso = dados->dano_pulso;
        jogador.intervalo_pulso = dados->intervalo_pulso;
        jogador.tempo_ate_pulso = dados->intervalo_pulso;
        jogador.golpes_pulso = 1;
    }
    jogador.nivel = 1;
    jogador.fator_xp = 1.0f;
    jogador.raio_ima = RAIO_IMA_BASE;
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
    jogador->andando = tamanho > 0.0f;
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
    return (int)((XP_BASE_NIVEL + XP_POR_NIVEL * jogador->nivel) * jogador->fator_xp);
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

/* Nível máximo de cada melhoria: a "Nova arma" só pode ser pega uma vez. */
static int nivel_maximo_do_upgrade(int upgrade)
{
    return (upgrade == UPGRADE_NOVA_ARMA) ? 1 : NIVEL_MAX_UPGRADE;
}

int jogador_sortear_upgrades(const Jogador *jogador, int escolhas[])
{
    int disponiveis[TOTAL_UPGRADES];
    int qtd_disponiveis = 0;
    int qtd_escolhidos = 0;
    int upgrade;

    /* A partir do nível NIVEL_NOVA_ARMA a carta da segunda arma aparece sempre, até ser
     * escolhida. As outras melhorias são sorteadas (sorte). */
    if (jogador->niveis_upgrade[UPGRADE_NOVA_ARMA] == 0 && jogador->nivel >= NIVEL_NOVA_ARMA) {
        escolhas[qtd_escolhidos] = UPGRADE_NOVA_ARMA;
        qtd_escolhidos++;
    }
    for (upgrade = 0; upgrade < TOTAL_UPGRADES; upgrade++) {
        if (upgrade != UPGRADE_NOVA_ARMA
            && jogador->niveis_upgrade[upgrade] < nivel_maximo_do_upgrade(upgrade)) {
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

/* Segunda arma: quem começa com tiros ganha a onda de impacto; quem começa com a onda
 * (Hulk) ganha o arremesso de pedras. */
static void ganhar_segunda_arma(Jogador *jogador)
{
    if (jogador->raio_pulso <= 0.0f) {
        jogador->raio_pulso = RAIO_PULSO_BASE;
        jogador->dano_pulso = DANO_PULSO_BASE;
        jogador->intervalo_pulso = INTERVALO_PULSO;
        jogador->tempo_ate_pulso = INTERVALO_PULSO;
        jogador->golpes_pulso = 1;
    } else {
        jogador->qtd_alvos = 1;
    }
}

void jogador_aplicar_upgrade(Jogador *jogador, TipoUpgrade upgrade)
{
    if (jogador->niveis_upgrade[upgrade] >= nivel_maximo_do_upgrade(upgrade)) {
        return;
    }
    jogador->niveis_upgrade[upgrade]++;

    if (upgrade == UPGRADE_DANO) {
        jogador->dano += BONUS_DANO;
        if (jogador->raio_pulso > 0.0f) {       /* a onda/pulso também fica mais forte */
            jogador->dano_pulso += BONUS_DANO_PULSO;
            jogador->raio_pulso += BONUS_RAIO_PULSO;
        }
    } else if (upgrade == UPGRADE_CADENCIA) {
        jogador->cadencia *= FATOR_CADENCIA;
        jogador->intervalo_pulso *= FATOR_CADENCIA;
    } else if (upgrade == UPGRADE_ALCANCE) {
        jogador->alcance += BONUS_ALCANCE;
        if (jogador->raio_pulso > 0.0f) {
            jogador->raio_pulso += BONUS_RAIO_ALCANCE;
        }
    } else if (upgrade == UPGRADE_QUANTIDADE) {
        if (jogador->qtd_alvos > 0) {
            jogador->qtd_alvos++;
        }
        if (jogador->raio_pulso > 0.0f) {
            jogador->golpes_pulso++;
        }
    } else if (upgrade == UPGRADE_VELOCIDADE) {
        jogador->velocidade += BONUS_VELOCIDADE;
    } else if (upgrade == UPGRADE_VIDA) {
        jogador->vida_maxima += BONUS_VIDA;
        jogador->vida += BONUS_VIDA;
    } else if (upgrade == UPGRADE_IMA) {
        jogador->raio_ima += BONUS_RAIO_IMA;
    } else {
        ganhar_segunda_arma(jogador);
    }
}
