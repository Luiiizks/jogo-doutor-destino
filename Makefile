# Makefile do jogo
# Uso: make | make run | make debug | make test | make zip | make clean

NOME_JOGO = doomsday
CC        = cc

# Flags da raylib (pkg-config encontra no Mac com brew e no Linux)
RAYLIB_CFLAGS = $(shell pkg-config --cflags raylib 2>/dev/null)
RAYLIB_LIBS   = $(shell pkg-config --libs raylib 2>/dev/null)

CFLAGS  = -std=c11 -Wall -Wextra -Wpedantic -O2 $(RAYLIB_CFLAGS)
LDLIBS  = $(RAYLIB_LIBS) -lm

FONTES  = $(wildcard src/*.c)

# Compila o jogo
$(NOME_JOGO): $(FONTES) $(wildcard src/*.h)
	$(CC) $(CFLAGS) $(FONTES) -o $(NOME_JOGO) $(LDLIBS)

run: $(NOME_JOGO)
	./$(NOME_JOGO)

# Versão de depuração com verificação de memória (ASan) e comportamento indefinido (UBSan)
debug: $(FONTES) $(wildcard src/*.h)
	$(CC) -std=c11 -Wall -Wextra -Wpedantic -g -O0 -fsanitize=address,undefined \
		$(RAYLIB_CFLAGS) $(FONTES) -o $(NOME_JOGO)_debug $(RAYLIB_LIBS) -lm
	./$(NOME_JOGO)_debug

# Testes da lista, do cenário e do ranking em console (não usa a raylib)
test: tests/teste_lista.c tests/teste_cenario.c tests/teste_ranking.c \
		src/lista_inimigos.c src/cenario.c src/ranking.c
	$(CC) -std=c11 -Wall -Wextra -Wpedantic -g -fsanitize=address,undefined \
		tests/teste_lista.c src/lista_inimigos.c -o teste_lista -lm
	./teste_lista
	$(CC) -std=c11 -Wall -Wextra -Wpedantic -g -fsanitize=address,undefined \
		tests/teste_cenario.c src/cenario.c -o teste_cenario -lm
	./teste_cenario
	$(CC) -std=c11 -Wall -Wextra -Wpedantic -g -fsanitize=address,undefined \
		tests/teste_ranking.c src/ranking.c -o teste_ranking
	./teste_ranking

# Pacote de entrega: somente o necessário para reproduzir o jogo
zip:
	rm -f entrega.zip
	zip -r entrega.zip src assets Makefile README.md -x "*.DS_Store"

clean:
	rm -f $(NOME_JOGO) $(NOME_JOGO)_debug teste_lista teste_cenario teste_ranking entrega.zip
	rm -rf *.dSYM

.PHONY: run debug test zip clean
