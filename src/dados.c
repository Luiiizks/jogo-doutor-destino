/* dados.c - tabelas de conteúdo do jogo.
 * Aqui ficam os números de cada inimigo, personagem, mundo e upgrade. */

#include "dados.h"

static const TipoInimigo tipos_inimigo[TOTAL_TIPOS_INIMIGO] = {
    /* nome, vida, velocidade, dano contato, raio, xp, cor, imagem,
     * distância de ataque, intervalo do tiro, dano do tiro */
    { "Doombot", 20.0f, 60.0f, 8.0f, 12.0f, 5, { 150, 160, 170, 255 },
      "assets/sprites/inimigo_doombot.png", 0.0f, 0.0f, 0.0f },
    { "Doombot Veloz", 10.0f, 110.0f, 5.0f, 10.0f, 4, { 230, 150, 60, 255 },
      "assets/sprites/inimigo_doombot_veloz.png", 0.0f, 0.0f, 0.0f },
    { "Mago", 30.0f, 70.0f, 4.0f, 13.0f, 10, { 170, 90, 220, 255 },
      "assets/sprites/inimigo_mago.png", 320.0f, 2.0f, 10.0f },
};

static const Personagem personagens[] = {
    /* nome, vida, velocidade, dano, cadência, cor, imagem */
    { "Herói", 100.0f, 180.0f, 10.0f, 0.8f, { 120, 170, 255, 255 },
      "assets/sprites/heroi_a.png" },
};

static const DadosUpgrade upgrades[TOTAL_UPGRADES] = {
    { "Mais dano", "Mais dano por disparo." },
    { "Mais cadência", "Dispara mais rápido." },
    { "Projétil extra", "+1 alvo por disparo." },
    { "Mais velocidade", "Você anda mais rápido." },
    { "Mais vida", "+20 de vida máxima e cura." },
    { "Pulso de energia", "Onda de dano ao redor." },
};

static const Mundo mundos[] = {
    /* O Doombot aparece duas vezes na lista de tipos para ser mais comum que os outros. */
    { "Latveria", 90.0f, 1.0f, 0.45f,
      { TIPO_DOOMBOT, TIPO_DOOMBOT, TIPO_DOOMBOT_VELOZ, TIPO_MAGO }, 4,
      { 60, 55, 85, 255 }, "assets/chao/chao_mundo1.png" },
};

const TipoInimigo *dados_tipo_inimigo(int tipo)
{
    return &tipos_inimigo[tipo];
}

const Personagem *dados_personagem(int indice)
{
    return &personagens[indice];
}

int dados_total_personagens(void)
{
    return (int)(sizeof(personagens) / sizeof(personagens[0]));
}

const DadosUpgrade *dados_upgrade(TipoUpgrade upgrade)
{
    return &upgrades[upgrade];
}

const Mundo *dados_mundo(int indice)
{
    return &mundos[indice];
}

int dados_total_mundos(void)
{
    return (int)(sizeof(mundos) / sizeof(mundos[0]));
}

Inimigo dados_criar_inimigo(int tipo, Vetor2 posicao)
{
    const TipoInimigo *dados = &tipos_inimigo[tipo];
    Inimigo inimigo = { 0 };

    inimigo.tipo = tipo;
    inimigo.posicao = posicao;
    inimigo.vida = dados->vida;
    inimigo.velocidade = dados->velocidade;
    inimigo.dano_contato = dados->dano_contato;
    inimigo.raio = dados->raio;
    inimigo.xp = dados->xp;
    inimigo.distancia_ataque = dados->distancia_ataque;
    inimigo.intervalo_tiro = dados->intervalo_tiro;
    inimigo.tempo_ate_tiro = dados->intervalo_tiro;   /* primeiro tiro só depois de um tempo */
    inimigo.dano_tiro = dados->dano_tiro;
    return inimigo;
}
