# Doomsday Survivors

Jogo 2D de sobrevivência em C com a biblioteca raylib, inspirado no filme
*Vingadores: Doutor Destino*. Projeto da disciplina de Algoritmos e Estruturas de Dados
(AED, 2026.2) — tema "Ode ao Cinema".

> Nome provisório: ajustar para o nome enviado na Entrega 01.

## Como executar

### 1. Instalar as dependências
**macOS** (Homebrew):
```
xcode-select --install
brew install raylib pkg-config
```
**Linux (Ubuntu/Debian)**:
```
sudo apt install build-essential pkg-config libraylib-dev
```
(se `libraylib-dev` não existir na sua versão, instale a raylib pelo guia oficial em raylib.com)

**Windows**: instalar o w64devkit e a raylib pelo instalador oficial (raylib.com); no terminal
do w64devkit, na pasta do projeto, `make`.

### 2. Compilar e jogar
Na pasta do projeto:
```
make
./doomsday
```
(ou apenas `make run`)

## Controles
| Ação | Tecla |
|---|---|
| Mover | W A S D ou setas |
| Pausar | ESC ou P |
| Navegar nos menus | setas / W S e ENTER (ou mouse) |
| Escolher melhoria ao subir de nível | 1, 2, 3 ou setas + ENTER |

## Como jogar
O herói ataca automaticamente os inimigos mais próximos. Você só precisa se mover, desviar
da horda e escolher uma melhoria a cada nível. Sobreviva ao tempo de cada um dos 3 mundos e
derrote o chefe final no 4º mundo.

## Estrutura de dados e algoritmo
- **Lista duplamente encadeada** guarda os inimigos em cena (inserir = nascer, remover = morrer).
- **Insertion sort** ordena a lista por distância ao herói a cada quadro; os primeiros da
  lista são os alvos do disparo automático.
- Código da lista: `src/lista_inimigos.c` / `src/lista_inimigos.h`.

## Estrutura do projeto
```
src/      código-fonte em C
assets/   imagens (opcionais; sem elas o jogo usa formas geométricas)
Makefile  compilação
```

## Créditos
- Biblioteca gráfica: [raylib](https://www.raylib.com)
- Arte: (preencher com a origem real: feita pelo grupo / ferramenta utilizada)
- Referências: (preencher com as referências da Entrega 01)

## Integrantes
(preencher)
