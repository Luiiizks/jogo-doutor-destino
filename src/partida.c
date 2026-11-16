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

void partida_encerrar(Partida *partida)
{
    if (partida->inimigos != NULL) {
        lista_destruir(partida->inimigos);
        partida->inimigos = NULL;
    }
}

ResultadoPartida partida_atualizar(Partida *partida, float dt)
{
    Jogador *jogador = &partida->jogador;
    float vida_antes = jogador->vida;

    jogador_atualizar(jogador, dt);
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
    if (mundos_jogador_no_portal(partida)) {
        return PARTIDA_MUNDO_CONCLUIDO;
    }
    return PARTIDA_EM_ANDAMENTO;
}
