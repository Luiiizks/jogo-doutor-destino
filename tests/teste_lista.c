/* teste_lista.c - testes de console da lista de inimigos (sem raylib).
 * Roda com "make test". Cada conferência mostra OK ou FALHOU. */

#include <stdio.h>

#include "../src/lista_inimigos.h"

static int total_conferencias = 0;
static int total_falhas = 0;

static void conferir(int condicao, const char *descricao)
{
    total_conferencias++;
    if (!condicao) {
        total_falhas++;
        printf("  FALHOU: %s\n", descricao);
    }
}

/* Monta um inimigo parado na posição (x, 0), com vida e xp fixos. */
static Inimigo inimigo_em(float x)
{
    Inimigo inimigo = { 0 };

    inimigo.posicao.x = x;
    inimigo.vida = 10.0f;
    inimigo.raio = 5.0f;
    inimigo.xp = 3;
    inimigo.velocidade = 10.0f;
    return inimigo;
}

/* Confere se as posições x da lista batem com o vetor esperado. */
static int lista_tem_ordem(const ListaInimigos *lista, const float esperado[], int quantidade)
{
    const NoInimigo *atual = lista->inicio;
    int indice = 0;

    while (atual != NULL && indice < quantidade) {
        if (atual->dados.posicao.x != esperado[indice]) {
            return 0;
        }
        atual = atual->proximo;
        indice++;
    }
    return atual == NULL && indice == quantidade;
}

static void teste_inserir_e_contar(void)
{
    ListaInimigos *lista = lista_criar();
    NoInimigo *primeiro;

    printf("inserir e contar\n");
    conferir(lista_esta_vazia(lista), "lista nova esta vazia");
    primeiro = lista_inserir_fim(lista, inimigo_em(1));
    lista_inserir_fim(lista, inimigo_em(2));
    lista_inserir_fim(lista, inimigo_em(3));
    conferir(lista_contar(lista) == 3, "contar devolve 3");
    conferir(!lista_esta_vazia(lista), "lista com itens nao esta vazia");
    conferir(primeiro->dados.id == 1, "primeiro id e 1");
    conferir(lista->fim->dados.id == 3, "ultimo id e 3");
    conferir(lista->fim->anterior->dados.posicao.x == 2, "anterior do fim e o segundo");
    lista_destruir(lista);
}

static void teste_remover(void)
{
    ListaInimigos *lista = lista_criar();
    NoInimigo *primeiro = lista_inserir_fim(lista, inimigo_em(1));
    NoInimigo *meio = lista_inserir_fim(lista, inimigo_em(2));
    NoInimigo *ultimo = lista_inserir_fim(lista, inimigo_em(3));
    const float depois_do_meio[] = { 1, 3 };

    printf("remover\n");
    lista_remover_no(lista, meio);
    conferir(lista_contar(lista) == 2, "remover do meio: tamanho 2");
    conferir(lista_tem_ordem(lista, depois_do_meio, 2), "remover do meio: ordem 1,3");
    conferir(ultimo->anterior == primeiro, "remover do meio: religou os vizinhos");

    lista_remover_no(lista, primeiro);
    conferir(lista->inicio == ultimo && ultimo->anterior == NULL, "remover o inicio");

    lista_remover_no(lista, ultimo);
    conferir(lista_esta_vazia(lista) && lista->fim == NULL, "remover o unico deixa vazia");
    conferir(lista_contar(lista) == 0, "tamanho 0 no final");
    lista_destruir(lista);
}

static void teste_ordenar(void)
{
    ListaInimigos *lista = lista_criar();
    Vetor2 jogador = { 0, 0 };
    const float espalhados[] = { 50, 10, 30, 20, 40 };
    const float ordenados[] = { 10, 20, 30, 40, 50 };
    const float invertidos[] = { 50, 40, 30, 20, 10 };
    int indice;

    printf("ordenar por distancia\n");
    lista_ordenar_por_distancia(lista, jogador);
    conferir(lista_esta_vazia(lista), "ordenar lista vazia nao quebra");

    lista_inserir_fim(lista, inimigo_em(7));
    lista_ordenar_por_distancia(lista, jogador);
    conferir(lista_contar(lista) == 1, "ordenar com 1 elemento");

    lista_destruir(lista);
    lista = lista_criar();
    for (indice = 0; indice < 5; indice++) {
        lista_inserir_fim(lista, inimigo_em(espalhados[indice]));
    }
    lista_ordenar_por_distancia(lista, jogador);
    conferir(lista_tem_ordem(lista, ordenados, 5), "ordena lista embaralhada");

    lista_ordenar_por_distancia(lista, jogador);
    conferir(lista_tem_ordem(lista, ordenados, 5), "lista ja ordenada continua igual");

    lista_destruir(lista);
    lista = lista_criar();
    for (indice = 0; indice < 5; indice++) {
        lista_inserir_fim(lista, inimigo_em(invertidos[indice]));
    }
    lista_ordenar_por_distancia(lista, jogador);
    conferir(lista_tem_ordem(lista, ordenados, 5), "pior caso: lista invertida");
    lista_destruir(lista);
}

static void teste_ordenar_depende_do_jogador(void)
{
    ListaInimigos *lista = lista_criar();
    Vetor2 jogador = { 100, 0 };
    /* 110 e 90 empatam na distancia: fica a ordem de entrada (ordenacao estavel) */
    const float esperado[] = { 110, 90, 50, 200 };

    printf("ordenar a partir de outra posicao\n");
    lista_inserir_fim(lista, inimigo_em(200));
    lista_inserir_fim(lista, inimigo_em(50));
    lista_inserir_fim(lista, inimigo_em(110));
    lista_inserir_fim(lista, inimigo_em(90));
    lista_ordenar_por_distancia(lista, jogador);
    conferir(lista_tem_ordem(lista, esperado, 4), "ordem 110,90,50,200 (empate estavel)");
    lista_destruir(lista);
}

static void teste_mais_proximos(void)
{
    ListaInimigos *lista = lista_criar();
    Vetor2 jogador = { 0, 0 };
    NoInimigo *alvos[5];
    int encontrados;

    printf("mais proximos\n");
    lista_inserir_fim(lista, inimigo_em(30));
    lista_inserir_fim(lista, inimigo_em(10));
    lista_inserir_fim(lista, inimigo_em(20));
    lista_ordenar_por_distancia(lista, jogador);

    encontrados = lista_obter_mais_proximos(lista, 2, alvos);
    conferir(encontrados == 2, "pediu 2 e veio 2");
    conferir(alvos[0]->dados.posicao.x == 10 && alvos[1]->dados.posicao.x == 20,
             "os 2 mais perto sao 10 e 20");

    encontrados = lista_obter_mais_proximos(lista, 5, alvos);
    conferir(encontrados == 3, "pediu 5 com so 3 na lista: veio 3");
    lista_destruir(lista);
}

static void teste_mover(void)
{
    ListaInimigos *lista = lista_criar();
    Vetor2 jogador = { 0, 0 };

    printf("mover todos\n");
    lista_inserir_fim(lista, inimigo_em(100));
    lista_mover_todos(lista, jogador, 1.0f);
    conferir(lista->inicio->dados.posicao.x == 90.0f, "andou 10 px em direcao ao heroi");
    lista_destruir(lista);
}

static void teste_projetil(void)
{
    ListaInimigos *lista = lista_criar();
    Projetil projetil = { { 0, 0 }, { 0, 0 }, 4.0f, 2.0f, 1.0f };
    int xp_ganho = 0;

    printf("colisao com projetil\n");
    lista_inserir_fim(lista, inimigo_em(100));
    conferir(lista_colidir_com_projetil(lista, &projetil, &xp_ganho) == 0, "longe: erra");

    lista_inserir_fim(lista, inimigo_em(3));
    conferir(lista_colidir_com_projetil(lista, &projetil, &xp_ganho) == 1, "perto: acerta");
    conferir(xp_ganho == 0 && lista_contar(lista) == 2, "vida 10 - dano 4: nao morre");

    projetil.dano = 50.0f;
    lista_colidir_com_projetil(lista, &projetil, &xp_ganho);
    conferir(lista_contar(lista) == 1 && xp_ganho == 3, "dano grande: morre, remove e da xp");
    lista_destruir(lista);
}

static void teste_dano_em_area(void)
{
    ListaInimigos *lista = lista_criar();
    Vetor2 centro = { 0, 0 };
    int xp_ganho = 0;
    int atingidos;

    printf("dano em area\n");
    lista_inserir_fim(lista, inimigo_em(10));
    lista_inserir_fim(lista, inimigo_em(20));
    lista_inserir_fim(lista, inimigo_em(500));

    atingidos = lista_dano_em_area(lista, centro, 30.0f, 100.0f, &xp_ganho);
    conferir(atingidos == 2, "atingiu os 2 que estavam no raio");
    conferir(lista_contar(lista) == 1 && lista->inicio->dados.posicao.x == 500,
             "so o distante sobrou");
    conferir(xp_ganho == 6, "somou o xp dos 2 mortos");
    lista_destruir(lista);
}

static void teste_dano_de_contato(void)
{
    ListaInimigos *lista = lista_criar();
    Vetor2 jogador = { 0, 0 };
    Inimigo fraco = inimigo_em(10);
    Inimigo forte = inimigo_em(12);

    printf("dano de contato\n");
    fraco.dano_contato = 3.0f;
    forte.dano_contato = 8.0f;
    conferir(lista_dano_de_contato(lista, jogador, 5.0f) == 0.0f, "lista vazia: dano 0");
    lista_inserir_fim(lista, fraco);
    lista_inserir_fim(lista, forte);
    conferir(lista_dano_de_contato(lista, jogador, 7.0f) == 8.0f, "encostando: maior dano");
    conferir(lista_dano_de_contato(lista, jogador, 0.0f) == 0.0f, "longe: dano 0");
    lista_destruir(lista);
}

static void teste_atirador(void)
{
    ListaInimigos *lista = lista_criar();
    Vetor2 jogador = { 0, 0 };
    Inimigo mago = inimigo_em(100);
    Projetil tiros[4];

    printf("inimigo atirador\n");
    mago.distancia_ataque = 80.0f;
    mago.intervalo_tiro = 1.0f;
    mago.tempo_ate_tiro = 1.0f;
    mago.dano_tiro = 7.0f;
    lista_inserir_fim(lista, mago);

    lista_mover_todos(lista, jogador, 1.0f);
    conferir(lista->inicio->dados.posicao.x == 90.0f, "longe: continua andando");
    lista_mover_todos(lista, jogador, 1.0f);
    lista->inicio->dados.posicao.x = 80.0f;
    lista_mover_todos(lista, jogador, 1.0f);
    conferir(lista->inicio->dados.posicao.x == 80.0f, "na distancia de ataque: para");

    conferir(lista_coletar_tiros(lista, jogador, 0.5f, tiros, 4) == 0, "ainda recarregando");
    conferir(lista_coletar_tiros(lista, jogador, 0.6f, tiros, 4) == 1, "recarregou: atira");
    conferir(tiros[0].dano == 7.0f && tiros[0].velocidade.x < 0.0f, "tiro vai para o heroi");
    conferir(lista_coletar_tiros(lista, jogador, 0.1f, tiros, 4) == 0, "nao atira em seguida");
    lista_destruir(lista);
}

int main(void)
{
    teste_inserir_e_contar();
    teste_remover();
    teste_ordenar();
    teste_ordenar_depende_do_jogador();
    teste_mais_proximos();
    teste_mover();
    teste_projetil();
    teste_dano_em_area();
    teste_dano_de_contato();
    teste_atirador();

    printf("\n%d conferencias, %d falhas\n", total_conferencias, total_falhas);
    return total_falhas == 0 ? 0 : 1;
}
