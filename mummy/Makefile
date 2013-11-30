FLAGS = -Wall -Wextra -pedantic-errors -std=c99
ALLEGRO = -lallegro -lallegro_image -lallegro_primitives -lallegro_font -lallegro_ttf

all: client server

cliente.o: cliente.c
	gcc $(FLAGS) -c cliente.c -o cliente.o

colisao.o: colisao.c
	gcc $(FLAGS) -c colisao.c -o colisao.o

inimigo.o: inimigo.c
	gcc $(FLAGS) -c inimigo.c -o inimigo.o

jogo.o: jogo.c
	gcc $(FLAGS) -c jogo.c -o jogo.o

jogador.o: jogador.c
	gcc $(FLAGS) -c jogador.c -o jogador.o

menu.o: menu.c
	gcc $(FLAGS) -c menu.c -o menu.o

teclado.o: teclado.c
	gcc $(FLAGS) -c teclado.c -o teclado.o

tiro.o: tiro.c
	gcc $(FLAGS) -c tiro.c -o tiro.o

variaveis.o: variaveis.c
	gcc $(FLAGS) -c variaveis.c -o variaveis.o

client: cliente.o colisao.o inimigo.o jogo.o jogador.o menu.o teclado.o tiro.o variaveis.o mummy.c 
	gcc $(FLAGS) cliente.o colisao.o inimigo.o jogo.o jogador.o menu.o teclado.o tiro.o variaveis.o mummy.c -o client $(ALLEGRO) -lm -lpthread

server: colisao.o inimigo.o servidor.c
	gcc $(FLAGS) colisao.o inimigo.o servidor.c -o server $(ALLEGRO) -lm -lpthread

clean:
	rm -f server client cliente.o colisao.o inimigo.o jogo.o jogador.o menu.o teclado.o tiro.o variaveis.o 

install:
	sudo cp client /usr/bin
	sudo cp server /usr/bin

uninstall:
	sudo rm -rf /usr/bin/client
	sudo rm -rf /usr/bin/server
