# Doomsday Survivors 

Jogo 2D de sobrevivência feito em C com a biblioteca raylib, inspirado no filme *Vingadores:
Doutor Destino*. É um trabalho de Algoritmos e Estruturas de Dados (AED, 2026.2), no tema "Ode ao
Cinema".

Você escolhe um herói e atravessa 3 mundos cheios de inimigos até chegar ao castelo do Doutor
Destino. O herói ataca sozinho: você só se movimenta, pega o XP que cai no chão e escolhe uma
melhoria a cada nível.

## Como executar

### Instalar as dependências
**macOS** (Homebrew):
```
xcode-select --install
brew install raylib pkg-config
```
**Linux (Ubuntu/Debian)**:
```
sudo apt install build-essential pkg-config libraylib-dev
```
Se o pacote `libraylib-dev` não existir na sua versão, instale a raylib pelo guia de
raylib.com.

**Windows**: instale o w64devkit e a raylib pelo instalador oficial (raylib.com). Depois, no
terminal do w64devkit, dentro da pasta do projeto, rode `make`.

### Compilar e jogar
Dentro da pasta do projeto:
```
make run
```
É importante rodar de dentro da pasta, porque as imagens são lidas de `assets/`.
Para só compilar, use `make` e abra com `./doomsday`.

Os testes da lista de inimigos, do cenário e do ranking rodam no terminal, sem abrir janela:
```
make test
```

## Controles
| Ação | Tecla |
|---|---|
| Mover | W A S D ou setas |
| Dash (impulso curto, sem levar dano, espera 3 s) | ESPAÇO |
| Pausar | ESC ou P |
| Menus | setas ou W S, ENTER (ou mouse) |
| Escolher melhoria ao subir de nível | 1, 2, 3 ou setas + ENTER |

## Heróis e modos de jogo
Depois de **Jogar** você escolhe o herói e o modo.

| Herói | Arma | Como é |
|---|---|---|
| Homem de Ferro | Repulsor | tiros rápidos no inimigo mais próximo |
| Capitão América | Escudo | acerta os 2 inimigos mais próximos de uma vez |
| Hulk | Onda de impacto | muita vida, bate no chão e machuca todos ao redor |

- **Modo normal:** 3 mundos de 90 segundos e depois o chefe.
- **Modo rápido:** mundos de cerca de 30 segundos e chefe mais fraco. Usa exatamente o mesmo
  código, só que mais curto, para mostrar o jogo inteiro em poucos minutos.

## Como jogar
- Sobreviva até acabar o tempo do mundo. Aí abre um portal: entre nele para ir ao próximo mundo,
  ou fique mais um pouco enfrentando a horda para juntar XP.
- Os inimigos soltam **gemas de XP** quando morrem. O XP só conta quando você passa por cima
  da gema. A melhoria "Ímã de XP" atrai as gemas de mais longe.
- Alguns inimigos soltam itens: o **ímã** puxa todas as gemas do mapa e o **coração** recupera
  parte da vida.
- A cada horda vêm 1 ou 2 **elites** (dourados, maiores e mais fortes) com boa chance de soltar
  item.
- Os tiros têm **alcance**: o herói só ataca quem está dentro do anel claro ao redor dele. A
  melhoria "Mais alcance" aumenta o anel e o tamanho da onda de impacto.
- A cada 3 hordas vem um **cerco**: o dobro de inimigos, formando um círculo ao redor do herói.
- Além de dano, cadência e alcance, há melhorias de personagem: **Armadura** (menos dano
  recebido), **Regeneração** (cura aos poucos) e **Crítico** (chance de dano em dobro).
- A partir do nível 3 aparece a carta **Nova arma**, com a segunda arma do herói.
- No último mundo está o **Doutor Destino**. Ele invoca reforços, fica intangível de vez em
  quando e fica mais rápido quando a vida dele passa da metade. Também ataca com orbes, alternando
  entre dois golpes: um **pulso** (anel de orbes que sai dele) e um **cerco** (anel que fecha em
  volta de você, com um vão por onde dá para escapar).

## Pontuação e ranking
Antes de cada partida o jogo pede o seu nome. A pontuação soma:
- **abates:** cada inimigo vale o XP dele x 10 (elites valem mais);
- **tempo:** 10 pontos por segundo sobrevivido (o modo rápido conta o tempo como se fosse o normal);
- **bônus:** 1000 por mundo concluído e 5000 por derrotar o Doutor Destino.

Quando a partida termina (derrota ou vitória no **modo normal**) a pontuação vai para o
**ranking top 10**, que fica no arquivo `ranking.txt` (criado ao lado do jogo). Se o nome já está no ranking, só vale se
a nova pontuação for maior que a anterior, e aí o jogador sobe de posição. O ranking pode ser
visto a qualquer momento pela opção **Ranking** do menu. O modo rápido é só demonstração e não
entra no ranking.

## Estrutura de dados e algoritmo
Os inimigos ficam numa **lista duplamente encadeada** (`src/lista_inimigos.c`). Ela é usada o
tempo todo:
- um inimigo que nasce é inserido no fim da lista;
- um inimigo que morre é removido do meio dela, sem percorrer nada, graças ao ponteiro
  `anterior`;
- todo quadro a lista é percorrida para mover os inimigos, testar tiros e causar dano;
- quando o chefe cai, a lista é esvaziada.

Todo quadro a lista também é **ordenada por distância até o herói com insertion sort**. Como
os inimigos quase não mudam de lugar de um quadro para o outro, a lista já chega quase
ordenada e a ordenação fica perto de O(n). Com ela, a mira automática só precisa pegar os
primeiros inimigos da lista, e para de procurar no primeiro que estiver fora do alcance, porque
todos os seguintes estão ainda mais longe.

As funções da lista (criar, inserir, remover, ordenar, obter os mais próximos, mover, colidir
com projétil, dano em área, contar, buscar por tipo, limpar, destruir e outras) estão
comentadas em `src/lista_inimigos.h` com o custo de cada uma.

## Estrutura do projeto
```
src/
  lista_inimigos.c/h   lista duplamente encadeada e insertion sort (sem raylib)
  armas.c/h            tiros, onda de impacto e a mira que usa a lista ordenada
  mundos.c/h           tempo do mundo, hordas, elites, portal e chefe
  gemas.c/h            XP e itens que caem no chão
  jogador.c/h          movimento, nível e melhorias
  partida.c/h          junta tudo, avança um quadro do jogo e calcula a pontuação
  ranking.c/h          top 10 de pontuações, salvo em arquivo (sem raylib)
  cenario.c/h          sorteio dos objetos espalhados pelo mapa (sem raylib)
  dados.c/h            tabelas de heróis, inimigos e mundos
  desenho.c/h          desenho da partida
  interface.c/h        menus e telas
  imagens.c/h          carrega as imagens
  main.c               janela e controle das telas
tests/                 testes da lista, do cenário e do ranking (make test)
assets/                imagens do jogo
Makefile
```

## Imagens
O jogo carrega as imagens de `assets/` quando elas existem. Sem alguma delas, ele desenha uma
forma simples no lugar e continua funcionando.
