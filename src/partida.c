/* partida.c - controla uma partida: cria, encerra e avança um quadro de cada vez.
 * A ordem das etapas em partida_atualizar é importante (explicada lá). */

#include <stddef.h>

#include "armas.h"
#include "jogador.h"
#include "mundos.h"
#include "partida.h"

int partida_iniciar(Partida *partida, int indice_personagem, int indice_mundo)
{
    partida_encerrar(partida);
    partida->jogador = jogador_criar(indice_personagem);
    partida->inimigos = lista_criar();
    partida->qtd_projeteis = 0;
    partida->qtd_tiros_inimigos = 0;
    partida->mundo = indice_mundo;
    partida->tempo_mundo = 0.0f;
    partida->tempo_ate_spawn = 0.0f;
    partida->tempo_ate_horda = INTERVALO_HORDA;
    partida->hordas_ocorridas = 0;
    partida->aviso_horda = 0.0f;
    partida->portal_aberto = 0;
    return partida->inimigos != NULL;
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

    jogador_atualizar(jogador, dt);
    mundos_atualizar(partida, dt);                                  /* spawn: insere na lista */
    lista_mover_todos(partida->inimigos, jogador->posicao, dt);
    lista_ordenar_por_distancia(partida->inimigos, jogador->posicao);  /* prepara a mira */
    armas_atualizar(partida, dt);                                   /* mira usa a lista ordenada */

    jogador_receber_dano(jogador, lista_dano_de_contato(partida->inimigos,
                                                        jogador->posicao, jogador->raio));
    if (jogador->vida <= 0.0f) {
        return PARTIDA_DERROTA;
    }
    if (mundos_jogador_no_portal(partida)) {
        return PARTIDA_MUNDO_CONCLUIDO;
    }
    return PARTIDA_EM_ANDAMENTO;
}
