/* teste_cenario.c - testes de console do sorteio de objetos de cenário (sem raylib).
 * Roda com "make test". */

#include <stdio.h>

#include "../src/cenario.h"
#include "../src/config.h"

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

/* O mesmo sorteio repetido tem que dar sempre o mesmo resultado. */
static void teste_repetivel(void)
{
    Vetor2 primeira;
    Vetor2 segunda;
    int objeto_a = cenario_sortear_objeto(0, 7, -3, 5, &primeira);
    int objeto_b = cenario_sortear_objeto(0, 7, -3, 5, &segunda);

    printf("repetivel\n");
    conferir(objeto_a == objeto_b, "mesmo objeto nas duas vezes");
    conferir(objeto_a < 0 || (primeira.x == segunda.x && primeira.y == segunda.y),
             "mesma posicao nas duas vezes");
}

/* Varre muitas células conferindo as regras: tipo válido, posição dentro da célula,
 * proporção de células com objeto perto do configurado e início do mapa livre. */
static void teste_varredura(void)
{
    int com_objeto = 0;
    int tipo_invalido = 0;
    int fora_da_celula = 0;
    int perto_do_inicio = 0;
    int celula_x;
    int celula_y;

    printf("varredura de 10000 celulas\n");
    for (celula_x = -50; celula_x < 50; celula_x++) {
        for (celula_y = -50; celula_y < 50; celula_y++) {
            Vetor2 posicao = { 0, 0 };
            int objeto = cenario_sortear_objeto(0, celula_x, celula_y, 5, &posicao);

            if (objeto < 0) {
                continue;
            }
            com_objeto++;
            if (objeto >= 5) {
                tipo_invalido++;
            }
            if (posicao.x < celula_x * TAMANHO_CELULA_CENARIO
                || posicao.x >= (celula_x + 1) * TAMANHO_CELULA_CENARIO
                || posicao.y < celula_y * TAMANHO_CELULA_CENARIO
                || posicao.y >= (celula_y + 1) * TAMANHO_CELULA_CENARIO) {
                fora_da_celula++;
            }
            if (posicao.x * posicao.x + posicao.y * posicao.y
                < RAIO_LIVRE_INICIO * RAIO_LIVRE_INICIO) {
                perto_do_inicio++;
            }
        }
    }
    conferir(tipo_invalido == 0, "todo tipo sorteado esta entre 0 e 4");
    conferir(fora_da_celula == 0, "posicao sempre dentro da celula");
    conferir(perto_do_inicio == 0, "nenhum objeto na area livre do inicio");
    conferir(com_objeto > 3000 && com_objeto < 4500, "cerca de 40% das celulas tem objeto");
}

static void teste_sem_tipos(void)
{
    Vetor2 posicao;

    printf("sem tipos\n");
    conferir(cenario_sortear_objeto(0, 3, 3, 0, &posicao) == -1,
             "mundo sem objetos nunca sorteia");
}

int main(void)
{
    teste_repetivel();
    teste_varredura();
    teste_sem_tipos();

    printf("\n%d conferencias, %d falhas\n", total_conferencias, total_falhas);
    return total_falhas == 0 ? 0 : 1;
}
