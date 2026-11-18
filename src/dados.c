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
    {
        .nome = "Homem de Ferro", .arma = "Repulsor",
        .descricao = "Tiros rápidos, 1 alvo.",
        .vida = 100.0f, .velocidade = 180.0f, .dano = 14.0f, .cadencia = 0.55f, .qtd_alvos = 1,
        .alcance = 285.0f,
        .cor = { 220, 70, 60, 255 }, .cor_projetil = { 120, 200, 255, 255 },
        .arquivo_imagem = "assets/sprites/heroi_a.png",
        .arquivo_caminhada = "assets/sprites/heroi_a_walk.png", .quadros_caminhada = 4,
        .arquivo_projetil = "assets/sprites/projetil_arcano.png", .altura_visual = 56.0f,
        .nome_quantidade = "Mais tiros", .desc_quantidade = "+1 tiro por disparo.",
        .nome_segunda_arma = "Onda de impacto", .desc_segunda_arma = "Nova arma: onda em área.",
    },
    {
        .nome = "Capitão América", .arma = "Escudo",
        .descricao = "Escudo nos 2 mais próximos.",
        .vida = 120.0f, .velocidade = 170.0f, .dano = 14.0f, .cadencia = 1.1f, .qtd_alvos = 2,
        .alcance = 248.0f,
        .cor = { 70, 110, 220, 255 }, .cor_projetil = { 230, 230, 255, 255 },
        .arquivo_imagem = "assets/sprites/heroi_b.png",
        .arquivo_caminhada = "assets/sprites/heroi_b_walk.png", .quadros_caminhada = 4,
        .arquivo_projetil = "assets/sprites/projetil_escudo.png", .altura_visual = 58.0f,
        .nome_quantidade = "Mais escudos", .desc_quantidade = "+1 alvo por arremesso.",
        .nome_segunda_arma = "Onda de impacto", .desc_segunda_arma = "Nova arma: onda em área.",
    },
    {
        .nome = "Hulk", .arma = "Onda de impacto",
        .descricao = "Onda em área, muita vida.",
        .vida = 180.0f, .velocidade = 140.0f, .dano = 12.0f, .cadencia = 1.2f, .qtd_alvos = 0,
        .alcance = 240.0f,
        .raio_pulso = 82.0f, .dano_pulso = 16.0f, .intervalo_pulso = 1.1f,
        .cor = { 90, 180, 80, 255 }, .cor_projetil = { 150, 130, 90, 255 },
        .arquivo_imagem = "assets/sprites/heroi_c.png",
        .arquivo_caminhada = "assets/sprites/heroi_c_walk.png", .quadros_caminhada = 4,
        .arquivo_projetil = "assets/sprites/projetil_pedra.png", .altura_visual = 72.0f,
        .nome_quantidade = "Mais golpes", .desc_quantidade = "A onda bate mais uma vez.",
        .nome_segunda_arma = "Arremesso de pedras", .desc_segunda_arma = "Nova arma: atira pedras.",
    },
};

/* Textos gerais das melhorias. "Mais golpes" e "Nova arma" mudam de texto conforme o herói
 * (ver Personagem). */
static const DadosUpgrade upgrades[TOTAL_UPGRADES] = {
    { "Mais dano", "Mais dano nos ataques." },
    { "Mais cadência", "Ataca mais rápido." },
    { "Mais alcance", "Ataca de mais longe." },
    { "Mais golpes", "Mais alvos por ataque." },
    { "Mais velocidade", "Você anda mais rápido." },
    { "Mais vida", "+20 de vida máxima e cura." },
    { "Ímã de XP", "Atrai o XP de mais longe." },
    { "Armadura", "Leva menos dano." },
    { "Regeneração", "Cura vida aos poucos." },
    { "Crítico", "Chance de dano em dobro." },
    { "Nova arma", "Ganha uma segunda arma." },
};

static const Mundo mundos[] = {
    {
        .nome = "Latveria", .duracao = 90.0f,
        .intervalo_inicial = 0.85f, .intervalo_final = 0.38f,
        /* O Doombot aparece duas vezes na lista para ser mais comum que os outros. */
        .tipos = { TIPO_DOOMBOT, TIPO_DOOMBOT, TIPO_DOOMBOT_VELOZ, TIPO_MAGO },
        .qtd_tipos = 4,
        .cor_linhas_chao = { 60, 55, 85, 255 },
        .arquivo_chao = "assets/chao/chao_mundo1.png",
        .objetos = {
            { "assets/cenario/m1_poste.png", 96.0f },
            { "assets/cenario/m1_barril.png", 28.0f },
            { "assets/cenario/m1_carroca.png", 64.0f },
            { "assets/cenario/m1_estatua.png", 80.0f },
            { "assets/cenario/m1_entulho.png", 16.0f },
        },
        .qtd_objetos = 5,
    },
    {
        .nome = "Terra dos Sentinelas", .duracao = 90.0f,
        .intervalo_inicial = 0.77f, .intervalo_final = 0.34f,
        .tipos = { TIPO_DOOMBOT, TIPO_DOOMBOT, TIPO_DOOMBOT_ELITE, TIPO_SENTINELA },
        .qtd_tipos = 4,
        .cor_linhas_chao = { 110, 70, 40, 255 },
        .arquivo_chao = "assets/chao/chao_mundo2.png",
        .objetos = {
            { "assets/cenario/m2_engrenagem.png", 52.0f },
            { "assets/cenario/m2_robo_caido.png", 46.0f },
            { "assets/cenario/m2_caixote.png", 34.0f },
            { "assets/cenario/m2_antena.png", 42.0f },
            { "assets/cenario/m2_cacto_metal.png", 58.0f },
        },
        .qtd_objetos = 5,
    },
    {
        .nome = "Dimensão Sombria", .duracao = 90.0f,
        .intervalo_inicial = 0.68f, .intervalo_final = 0.30f,
        .tipos = { TIPO_ESPECTRO, TIPO_ESPECTRO, TIPO_MAGO, TIPO_SENTINELA,
                   TIPO_DOOMBOT_ELITE },
        .qtd_tipos = 5,
        .cor_linhas_chao = { 50, 90, 70, 255 },
        .arquivo_chao = "assets/chao/chao_mundo3.png",
        .objetos = {
            { "assets/cenario/m3_cristal.png", 56.0f },
            { "assets/cenario/m3_pedra_runa.png", 62.0f },
            { "assets/cenario/m3_galho.png", 72.0f },
            { "assets/cenario/m3_olho.png", 32.0f },
            { "assets/cenario/m3_fragmento.png", 44.0f },
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
