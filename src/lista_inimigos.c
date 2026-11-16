/* lista_inimigos.c - lista duplamente encadeada de inimigos e a ordenação por distância.
 * É o centro do jogo: spawn insere, morte remove, a mira usa a lista ordenada.
 * Não usa a raylib de propósito, para poder ser testada no terminal. */

#include <math.h>
#include <stdlib.h>

#include "config.h"
#include "lista_inimigos.h"

/* Distância ao quadrado entre dois pontos. Evita a raiz quadrada (mais barato) e
 * serve para comparar quem está mais perto, porque a ordem é a mesma. */
static float distancia_quadrada_entre(Vetor2 ponto_a, Vetor2 ponto_b)
{
    float diferenca_x = ponto_a.x - ponto_b.x;
    float diferenca_y = ponto_a.y - ponto_b.y;

    return diferenca_x * diferenca_x + diferenca_y * diferenca_y;
}

ListaInimigos *lista_criar(void)
{
    ListaInimigos *lista = malloc(sizeof(ListaInimigos));

    if (lista == NULL) {
        return NULL;
    }
    lista->inicio = NULL;
    lista->fim = NULL;
    lista->tamanho = 0;
    lista->proximo_id = 1;
    return lista;
}

NoInimigo *lista_inserir_fim(ListaInimigos *lista, Inimigo novo)
{
    NoInimigo *no = malloc(sizeof(NoInimigo));

    if (no == NULL) {
        return NULL;
    }
    novo.id = lista->proximo_id;
    lista->proximo_id++;
    no->dados = novo;
    no->proximo = NULL;
    no->anterior = lista->fim;

    if (lista->fim == NULL) {
        lista->inicio = no;          /* lista estava vazia */
    } else {
        lista->fim->proximo = no;
    }
    lista->fim = no;
    lista->tamanho++;
    return no;
}

void lista_remover_no(ListaInimigos *lista, NoInimigo *no)
{
    /* Religa os vizinhos um no outro, tratando início e fim separadamente. */
    if (no->anterior == NULL) {
        lista->inicio = no->proximo;
    } else {
        no->anterior->proximo = no->proximo;
    }
    if (no->proximo == NULL) {
        lista->fim = no->anterior;
    } else {
        no->proximo->anterior = no->anterior;
    }
    free(no);
    lista->tamanho--;
}

void lista_ordenar_por_distancia(ListaInimigos *lista, Vetor2 posicao_jogador)
{
    NoInimigo *atual;

    /* Primeiro calcula a distância de todo mundo (uma vez só por inimigo). */
    for (atual = lista->inicio; atual != NULL; atual = atual->proximo) {
        atual->dados.distancia_quadrada =
            distancia_quadrada_entre(atual->dados.posicao, posicao_jogador);
    }

    if (lista->inicio == NULL) {
        return;
    }

    /* Insertion sort: a parte da esquerda de "atual" já está ordenada. Pegamos o
     * elemento de "atual" e vamos empurrando para a direita os que são mais longe,
     * até achar o lugar dele. Copiamos os dados entre nós vizinhos em vez de
     * religar ponteiros: é mais simples e o custo é pequeno com poucos inimigos. */
    for (atual = lista->inicio->proximo; atual != NULL; atual = atual->proximo) {
        Inimigo escolhido = atual->dados;
        NoInimigo *lugar = atual;

        while (lugar->anterior != NULL
               && lugar->anterior->dados.distancia_quadrada > escolhido.distancia_quadrada) {
            lugar->dados = lugar->anterior->dados;
            lugar = lugar->anterior;
        }
        lugar->dados = escolhido;
    }
}

int lista_obter_mais_proximos(const ListaInimigos *lista, int quantidade,
                              NoInimigo *alvos[])
{
    NoInimigo *atual = lista->inicio;
    int encontrados = 0;

    while (atual != NULL && encontrados < quantidade) {
        alvos[encontrados] = atual;
        encontrados++;
        atual = atual->proximo;
    }
    return encontrados;
}

void lista_mover_todos(ListaInimigos *lista, Vetor2 posicao_jogador, float dt)
{
    NoInimigo *atual;

    for (atual = lista->inicio; atual != NULL; atual = atual->proximo) {
        float diferenca_x = posicao_jogador.x - atual->dados.posicao.x;
        float diferenca_y = posicao_jogador.y - atual->dados.posicao.y;
        float distancia = sqrtf(diferenca_x * diferenca_x + diferenca_y * diferenca_y);
        float passo = atual->dados.velocidade * dt;

        /* Inimigo à distância para de andar quando chega perto o bastante para atirar. */
        int parou_para_atirar = atual->dados.distancia_ataque > 0.0f
                                && distancia <= atual->dados.distancia_ataque;

        if (atual->dados.tempo_flash > 0.0f) {
            atual->dados.tempo_flash -= dt;
        }
        if (atual->dados.tempo_intangivel > 0.0f) {
            atual->dados.tempo_intangivel -= dt;
        }
        atual->dados.andando = distancia > 0.001f && !parou_para_atirar;
        if (atual->dados.andando) {                       /* evita dividir por zero */
            atual->dados.posicao.x += diferenca_x / distancia * passo;
            atual->dados.posicao.y += diferenca_y / distancia * passo;
        }
    }
}

/* Monta um tiro que sai de "origem" em direção ao herói. */
static Projetil montar_tiro(Vetor2 origem, Vetor2 alvo, float dano)
{
    float diferenca_x = alvo.x - origem.x;
    float diferenca_y = alvo.y - origem.y;
    float distancia = sqrtf(diferenca_x * diferenca_x + diferenca_y * diferenca_y);
    Projetil tiro = { 0 };

    if (distancia < 0.001f) {
        distancia = 1.0f;
    }
    tiro.posicao = origem;
    tiro.velocidade.x = diferenca_x / distancia * VELOCIDADE_TIRO_INIMIGO;
    tiro.velocidade.y = diferenca_y / distancia * VELOCIDADE_TIRO_INIMIGO;
    tiro.dano = dano;
    tiro.raio = RAIO_TIRO_INIMIGO;
    tiro.tempo_restante = TEMPO_VIDA_TIRO_INIMIGO;
    return tiro;
}

int lista_coletar_tiros(ListaInimigos *lista, Vetor2 posicao_jogador, float dt,
                        Projetil tiros[], int maximo)
{
    NoInimigo *atual;
    int qtd_tiros = 0;

    for (atual = lista->inicio; atual != NULL; atual = atual->proximo) {
        Inimigo *inimigo = &atual->dados;
        float distancia;

        if (inimigo->distancia_ataque <= 0.0f) {
            continue;                                /* inimigo corpo a corpo */
        }
        inimigo->tempo_ate_tiro -= dt;
        distancia = sqrtf(distancia_quadrada_entre(inimigo->posicao, posicao_jogador));

        if (inimigo->tempo_ate_tiro <= 0.0f && qtd_tiros < maximo
            && distancia <= inimigo->distancia_ataque + MARGEM_DISTANCIA_TIRO) {
            tiros[qtd_tiros] = montar_tiro(inimigo->posicao, posicao_jogador,
                                           inimigo->dano_tiro);
            inimigo->tempo_ate_tiro = inimigo->intervalo_tiro;
            qtd_tiros++;
        }
    }
    return qtd_tiros;
}

/* Tira vida do inimigo. Se morreu, soma o xp e remove o nó. Devolve 1 se morreu. */
static int aplicar_dano(ListaInimigos *lista, NoInimigo *no, float dano, int *xp_ganho)
{
    no->dados.vida -= dano;
    if (no->dados.vida > 0.0f) {
        no->dados.tempo_flash = DURACAO_FLASH_DANO;
        return 0;
    }
    *xp_ganho += no->dados.xp;
    lista_remover_no(lista, no);
    return 1;
}

int lista_colidir_com_projetil(ListaInimigos *lista, const Projetil *projetil,
                               int *xp_ganho)
{
    NoInimigo *atual;

    for (atual = lista->inicio; atual != NULL; atual = atual->proximo) {
        float soma_raios = atual->dados.raio + projetil->raio;

        if (atual->dados.tempo_intangivel <= 0.0f
            && distancia_quadrada_entre(atual->dados.posicao, projetil->posicao)
               <= soma_raios * soma_raios) {
            aplicar_dano(lista, atual, projetil->dano, xp_ganho);
            return 1;
        }
    }
    return 0;
}

int lista_dano_em_area(ListaInimigos *lista, Vetor2 centro, float raio, float dano,
                       int *xp_ganho)
{
    NoInimigo *atual = lista->inicio;
    int atingidos = 0;

    while (atual != NULL) {
        /* Guarda o próximo ANTES, porque o nó atual pode ser removido. */
        NoInimigo *proximo = atual->proximo;
        float soma_raios = atual->dados.raio + raio;

        if (atual->dados.tempo_intangivel <= 0.0f
            && distancia_quadrada_entre(atual->dados.posicao, centro)
               <= soma_raios * soma_raios) {
            aplicar_dano(lista, atual, dano, xp_ganho);
            atingidos++;
        }
        atual = proximo;
    }
    return atingidos;
}

float lista_dano_de_contato(const ListaInimigos *lista, Vetor2 posicao_jogador,
                            float raio_jogador)
{
    const NoInimigo *atual;
    float maior_dano = 0.0f;

    for (atual = lista->inicio; atual != NULL; atual = atual->proximo) {
        float soma_raios = atual->dados.raio + raio_jogador;
        float distancia_quadrada = distancia_quadrada_entre(atual->dados.posicao,
                                                            posicao_jogador);

        if (atual->dados.tempo_intangivel <= 0.0f
            && distancia_quadrada <= soma_raios * soma_raios
            && atual->dados.dano_contato > maior_dano) {
            maior_dano = atual->dados.dano_contato;
        }
    }
    return maior_dano;
}

int lista_contar(const ListaInimigos *lista)
{
    return lista->tamanho;
}

int lista_esta_vazia(const ListaInimigos *lista)
{
    return lista->inicio == NULL;
}

NoInimigo *lista_buscar_por_tipo(const ListaInimigos *lista, int tipo)
{
    NoInimigo *atual;

    for (atual = lista->inicio; atual != NULL; atual = atual->proximo) {
        if (atual->dados.tipo == tipo) {
            return atual;
        }
    }
    return NULL;
}

void lista_limpar(ListaInimigos *lista)
{
    NoInimigo *atual = lista->inicio;

    while (atual != NULL) {
        NoInimigo *proximo = atual->proximo;     /* guarda antes do free */

        free(atual);
        atual = proximo;
    }
    lista->inicio = NULL;
    lista->fim = NULL;
    lista->tamanho = 0;
}

void lista_destruir(ListaInimigos *lista)
{
    lista_limpar(lista);
    free(lista);
}
