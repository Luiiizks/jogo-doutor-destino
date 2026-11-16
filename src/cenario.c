/* cenario.c - decide onde ficam os objetos de cenário.
 * Em vez de guardar uma lista de objetos (o mapa é infinito), cada célula do mapa
 * "sorteia" seu objeto com uma conta que depende só da posição da célula. */

#include "cenario.h"
#include "config.h"

/* Mistura os números da célula num valor que parece aleatório mas é sempre o mesmo para a
 * mesma entrada. Usa as constantes clássicas de um gerador congruencial linear. */
static unsigned int embaralhar(unsigned int valor)
{
    return valor * 1103515245u + 12345u;
}

/* Tira um número de 0 a limite-1 do valor embaralhado (usa a parte alta, que é mais
 * aleatória que a parte baixa nesse tipo de gerador). */
static int numero_ate(unsigned int valor, int limite)
{
    return (int)((valor / 65536u) % (unsigned int)limite);
}

int cenario_sortear_objeto(int mundo, int celula_x, int celula_y, int qtd_tipos,
                           Vetor2 *posicao)
{
    unsigned int base = embaralhar(embaralhar((unsigned int)celula_x * 73856093u
                                              + (unsigned int)celula_y * 19349663u
                                              + (unsigned int)mundo * 83492791u));
    unsigned int variacao_tipo = embaralhar(base);
    unsigned int variacao_x = embaralhar(variacao_tipo);
    unsigned int variacao_y = embaralhar(variacao_x);
    float distancia_do_inicio_x;
    float distancia_do_inicio_y;

    if (qtd_tipos <= 0 || numero_ate(base, 100) >= CHANCE_OBJETO_CENARIO) {
        return -1;
    }
    posicao->x = (float)(celula_x * TAMANHO_CELULA_CENARIO
                         + numero_ate(variacao_x, TAMANHO_CELULA_CENARIO));
    posicao->y = (float)(celula_y * TAMANHO_CELULA_CENARIO
                         + numero_ate(variacao_y, TAMANHO_CELULA_CENARIO));

    /* Deixa livre a área onde o herói nasce. */
    distancia_do_inicio_x = posicao->x;
    distancia_do_inicio_y = posicao->y;
    if (distancia_do_inicio_x * distancia_do_inicio_x
        + distancia_do_inicio_y * distancia_do_inicio_y
        < RAIO_LIVRE_INICIO * RAIO_LIVRE_INICIO) {
        return -1;
    }
    return numero_ate(variacao_tipo, qtd_tipos);
}
