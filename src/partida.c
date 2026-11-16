/* partida.c - controla uma partida: cria, encerra e avança um quadro de cada vez.
 * A ordem das etapas em partida_atualizar é importante (explicada lá). */

#include <stddef.h>

#include "armas.h"
#include "dados.h"
#include "jogador.h"
#include "mundos.h"
#include "partida.h"

/* Zera tudo que pertence a um mundo (lista de inimigos, tiros, tempo, hordas, portal).
 * O herói não é mexido aqui. Devolve 0 se faltar memória para a lista. */
static int preparar_mundo(Partida *partida, int indice_mundo)
{
    partida->inimigos = lista_criar();
    partida->qtd_projeteis = 0;
    partida->qtd_tiros_inimigos = 0;
    partida->qtd_numeros_dano = 0;
    partida->tempo_tremor = 0.0f;
    partida->mundo = indice_mundo;
    partida->tempo_mundo = 0.0f;
    partida->tempo_ate_spawn = 0.0f;
    partida->tempo_ate_horda = INTERVALO_HORDA;
    partida->hordas_ocorridas = 0;
    partida->aviso_horda = 0.0f;
    partida->portal_aberto = 0;
    partida->chefe_criado = 0;
    partida->chefe_fase2 = 0;
    partida->tempo_ate_reforcos = INTERVALO_REFORCOS;
    partida->tempo_ate_intangivel = INTERVALO_INTANGIVEL;
    return partida->inimigos != NULL;
}

int partida_iniciar(Partida *partida, int indice_personagem, int indice_mundo)
{
    partida_encerrar(partida);
    partida->jogador = jogador_criar(indice_personagem);
    return preparar_mundo(partida, indice_mundo);
}

int partida_tem_proximo_mundo(const Partida *partida)
{
    return partida->mundo + 1 < dados_total_mundos();
}

int partida_proximo_mundo(Partida *partida)
{
    Jogador *jogador = &partida->jogador;
    float vida_recuperada = jogador->vida + jogador->vida_maxima * CURA_TROCA_MUNDO;

    /* O herói leva nível, melhorias e vida para o mundo seguinte. */
    partida_encerrar(partida);
    jogador->posicao.x = 0.0f;
    jogador->posicao.y = 0.0f;
    jogador->vida = (vida_recuperada > jogador->vida_maxima) ? jogador->vida_maxima
                                                             : vida_recuperada;
    jogador->efeito_pulso = 0.0f;
    return preparar_mundo(partida, partida->mundo + 1);
}

int partida_eh_arena(const Partida *partida)
{
    return dados_mundo(partida->mundo)->arena_chefe;
}

void partida_encerrar(Partida *partida)
{
    if (partida->inimigos != NULL) {
        lista_destruir(partida->inimigos);
        partida->inimigos = NULL;
    }
}

/* Na arena do chefe o herói não pode sair do retângulo. */
static void limitar_jogador_na_arena(Jogador *jogador)
{
    if (jogador->posicao.x < -ARENA_METADE_LARGURA) {
        jogador->posicao.x = -ARENA_METADE_LARGURA;
    }
    if (jogador->posicao.x > ARENA_METADE_LARGURA) {
        jogador->posicao.x = ARENA_METADE_LARGURA;
    }
    if (jogador->posicao.y < -ARENA_METADE_ALTURA) {
        jogador->posicao.y = -ARENA_METADE_ALTURA;
    }
    if (jogador->posicao.y > ARENA_METADE_ALTURA) {
        jogador->posicao.y = ARENA_METADE_ALTURA;
    }
}

ResultadoPartida partida_atualizar(Partida *partida, float dt)
{
    Jogador *jogador = &partida->jogador;
    float vida_antes = jogador->vida;

    jogador_atualizar(jogador, dt);
    if (partida_eh_arena(partida)) {
        limitar_jogador_na_arena(jogador);
    }
    mundos_atualizar(partida, dt);                                  /* spawn: insere na lista */
    lista_mover_todos(partida->inimigos, jogador->posicao, dt);
    lista_ordenar_por_distancia(partida->inimigos, jogador->posicao);  /* prepara a mira */
    armas_atualizar(partida, dt);                                   /* mira usa a lista ordenada */

    jogador_receber_dano(jogador, lista_dano_de_contato(partida->inimigos,
                                                        jogador->posicao, jogador->raio));
    if (jogador->vida < vida_antes) {
        partida->tempo_tremor = DURACAO_TREMOR;      /* levou dano neste quadro */
    } else if (partida->tempo_tremor > 0.0f) {
        partida->tempo_tremor -= dt;
    }
    if (jogador->vida <= 0.0f) {
        return PARTIDA_DERROTA;
    }
    if (mundos_concluido(partida)) {
        lista_limpar(partida->inimigos);             /* chefe caiu: os reforços somem */
        partida->qtd_tiros_inimigos = 0;
        return PARTIDA_MUNDO_CONCLUIDO;
    }
    return PARTIDA_EM_ANDAMENTO;
}
