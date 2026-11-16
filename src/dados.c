/* dados.c - tabelas de conteúdo do jogo.
 * Aqui ficam os números de cada inimigo, personagem, mundo e upgrade. */

#include <stddef.h>

#include "dados.h"

/* altura_visual: quanto o desenho aparece de alto na tela (em pixels), contando só a parte
 * que não é transparente. Independe do tamanho do PNG: para mudar o tamanho de um
 * personagem na tela, mexa só nesse número. O herói tem 56. */
static const TipoInimigo tipos_inimigo[TOTAL_TIPOS_INIMIGO] = {
    [TIPO_DOOMBOT] = {
        .nome = "Doombot", .vida = 20.0f, .velocidade = 60.0f, .dano_contato = 8.0f,
        .raio = 12.0f, .xp = 5, .cor = { 150, 160, 170, 255 },
        .arquivo_imagem = "assets/sprites/inimigo_doombot.png",
        .arquivo_caminhada = "assets/sprites/inimigo_doombot_walk.png",
        .quadros_caminhada = 4, .altura_visual = 49.0f },
    [TIPO_DOOMBOT_VELOZ] = {
        .nome = "Doombot Veloz", .vida = 10.0f, .velocidade = 110.0f, .dano_contato = 5.0f,
        .raio = 10.0f, .xp = 4, .cor = { 230, 150, 60, 255 },
        .arquivo_imagem = "assets/sprites/inimigo_doombot_veloz.png",
        .arquivo_caminhada = "assets/sprites/inimigo_doombot_veloz_walk.png",
        .quadros_caminhada = 4, .altura_visual = 46.0f },
    [TIPO_MAGO] = {
        .nome = "Mago", .vida = 30.0f, .velocidade = 70.0f, .dano_contato = 4.0f,
        .raio = 13.0f, .xp = 10, .cor = { 170, 90, 220, 255 },
        .arquivo_imagem = "assets/sprites/inimigo_mago.png",
        .arquivo_caminhada = "assets/sprites/inimigo_mago_walk.png",
        .quadros_caminhada = 4, .altura_visual = 54.0f,
        .distancia_ataque = 320.0f, .intervalo_tiro = 2.0f, .dano_tiro = 10.0f },
    [TIPO_DOOMBOT_ELITE] = {
        .nome = "Doombot Elite", .vida = 50.0f, .velocidade = 75.0f, .dano_contato = 12.0f,
        .raio = 14.0f, .xp = 12, .cor = { 200, 70, 70, 255 },
        .arquivo_imagem = "assets/sprites/inimigo_doombot_elite.png",
        .arquivo_caminhada = "assets/sprites/inimigo_doombot_elite_walk.png",
        .quadros_caminhada = 4, .altura_visual = 70.0f },
    [TIPO_SENTINELA] = {
        .nome = "Sentinela", .vida = 80.0f, .velocidade = 40.0f, .dano_contato = 15.0f,
        .raio = 20.0f, .xp = 15, .cor = { 120, 120, 200, 255 },
        .arquivo_imagem = "assets/sprites/inimigo_sentinela.png",
        .arquivo_caminhada = "assets/sprites/inimigo_sentinela_walk.png",
        .quadros_caminhada = 4, .altura_visual = 120.0f },
    [TIPO_ESPECTRO] = {
        .nome = "Espectro", .vida = 15.0f, .velocidade = 130.0f, .dano_contato = 6.0f,
        .raio = 11.0f, .xp = 6, .cor = { 120, 255, 170, 255 },
        .arquivo_imagem = "assets/sprites/inimigo_espectro.png",
        .arquivo_caminhada = "assets/sprites/inimigo_espectro_walk.png",
        .quadros_caminhada = 4, .altura_visual = 52.0f },
    [TIPO_CHEFE] = {
        .nome = "Doutor Destino", .vida = 1500.0f, .velocidade = 35.0f, .dano_contato = 25.0f,
        .raio = 40.0f, .xp = 0, .cor = { 40, 120, 70, 255 },
        .arquivo_imagem = "assets/sprites/chefe_destino.png",
        .arquivo_caminhada = NULL, .quadros_caminhada = 0, .altura_visual = 160.0f },
};

static const Personagem personagens[] = {
    { .nome = "Herói", .vida = 100.0f, .velocidade = 180.0f, .dano = 10.0f, .cadencia = 0.8f,
      .cor = { 120, 170, 255, 255 },
      .arquivo_imagem = "assets/sprites/heroi_a.png",
      .arquivo_caminhada = "assets/sprites/heroi_a_walk.png",
      .quadros_caminhada = 4, .altura_visual = 56.0f },
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
    {
        .nome = "Latveria", .duracao = 90.0f,
        .intervalo_inicial = 1.0f, .intervalo_final = 0.45f,
        /* O Doombot aparece duas vezes na lista para ser mais comum que os outros. */
        .tipos = { TIPO_DOOMBOT, TIPO_DOOMBOT, TIPO_DOOMBOT_VELOZ, TIPO_MAGO },
        .qtd_tipos = 4,
        .cor_linhas_chao = { 60, 55, 85, 255 },
        .arquivo_chao = "assets/chao/chao_mundo1.png",
        .objetos = {
            { "assets/cenario/m1_poste.png", 96.0f },
            { "assets/cenario/m1_barril.png", 40.0f },
            { "assets/cenario/m1_carroca.png", 64.0f },
            { "assets/cenario/m1_estatua.png", 80.0f },
            { "assets/cenario/m1_entulho.png", 24.0f },
        },
        .qtd_objetos = 5,
    },
    {
        .nome = "Terra dos Sentinelas", .duracao = 90.0f,
        .intervalo_inicial = 0.9f, .intervalo_final = 0.40f,
        .tipos = { TIPO_DOOMBOT, TIPO_DOOMBOT, TIPO_DOOMBOT_ELITE, TIPO_SENTINELA },
        .qtd_tipos = 4,
        .cor_linhas_chao = { 110, 70, 40, 255 },
        .arquivo_chao = "assets/chao/chao_mundo2.png",
        .objetos = {
            { "assets/cenario/m2_engrenagem.png", 64.0f },
            { "assets/cenario/m2_robo_caido.png", 64.0f },
            { "assets/cenario/m2_caixote.png", 40.0f },
            { "assets/cenario/m2_antena.png", 88.0f },
            { "assets/cenario/m2_cacto_metal.png", 56.0f },
        },
        .qtd_objetos = 5,
    },
    {
        .nome = "Dimensão Sombria", .duracao = 90.0f,
        .intervalo_inicial = 0.8f, .intervalo_final = 0.35f,
        .tipos = { TIPO_ESPECTRO, TIPO_ESPECTRO, TIPO_MAGO, TIPO_SENTINELA,
                   TIPO_DOOMBOT_ELITE },
        .qtd_tipos = 5,
        .cor_linhas_chao = { 50, 90, 70, 255 },
        .arquivo_chao = "assets/chao/chao_mundo3.png",
        .objetos = {
            { "assets/cenario/m3_cristal.png", 72.0f },
            { "assets/cenario/m3_pedra_runa.png", 64.0f },
            { "assets/cenario/m3_galho.png", 72.0f },
            { "assets/cenario/m3_olho.png", 40.0f },
            { "assets/cenario/m3_fragmento.png", 48.0f },
        },
        .qtd_objetos = 5,
    },
    {
        /* Mundo do chefe: sem tempo, sem spawn comum e sem portal (ver mundos.c). */
        .nome = "Castelo de Destino", .arena_chefe = 1,
        .cor_linhas_chao = { 80, 70, 40, 255 },
        .arquivo_chao = "assets/chao/chao_mundo4.png",
        .objetos = {
            { "assets/cenario/m4_coluna.png", 96.0f },
            { "assets/cenario/m4_tocha.png", 64.0f },
            { "assets/cenario/m4_armadura.png", 72.0f },
            { "assets/cenario/m4_tapete.png", 40.0f },
            { "assets/cenario/m4_brasao.png", 56.0f },
        },
        .qtd_objetos = 5,
    },
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
