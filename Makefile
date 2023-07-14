# Rui Daniel, Afonso Alemao 08-11-2020
#
# Para compilar programa: make
# Para apagar .o e exe: make clean

#   Compilador, Flags do compilador

CC = gcc
CFLAGS = -Wall -std=c99 -O3
#   Sources
SOURCES = grafo.c airroutes.c main.c Pilha.c ordenacao.c conetividade.c heap.c

#   Objects  ('make' automatically compiles .c to .o)
OBJECTS = grafo.o airroutes.o ordenacao.o main.o conetividade.o Pilha.o heap.o

backbone: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

main.o: main.c 

airroutes.o: airroutes.c airroutes.h defs.h

grafo.o: grafo.c grafo.h defs.h

ordenacao.o: ordenacao.c ordenacao.h defs.h

conetividade.o: conetividade.c conetividade.h

Pilha.o: Pilha.c Pilha.h defs.h

heap.o: heap.c heap.h

clean:
	rm *.o backbone
