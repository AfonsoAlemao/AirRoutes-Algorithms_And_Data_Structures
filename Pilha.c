/*****************************************************************************************************
 * (c) Rui Daniel, Afonso Alemao
 * Ultima Modificacao: 25/11/2020
 *
 * Nome:
 *   Pilha.c
 *
 * Descricao:
 *   Implementacao do tipo pilha, com lista simplesmente ligada
 *
 * Comentarios : No projeto AirRoutes foi utilizada uma implementacao com lista simplesmente
 * 			     ligada. Ficheiro criado tendo como base os slides das aulas teoricas de AED 2020/2021
 *   			 1ºsemestre, Professor Carlos Bispo, algumas das funcoes aqui implementadas sao 
 * 				 extremamente semelhantes a essas mesmas funcoes implementadas
 *   			 nestes acetatos
 *****************************************************************************************************/

#include "Pilha.h"
#include <stdlib.h>

typedef struct pilha {
	int item;
	struct pilha *next;
} pilha;

static pilha *gP;

/*************************************************************************************************************
 * Init()
 *
 * Argumentos: nenhum
 *
 * Retorna: (void)
 * Efeitos secundarios: nenhum
 *
 * Descricao:
 * 	 Inicializa uma estrutura do tipo pilha
 ************************************************************************************************************/
 
void Init() {
	gP = NULL;
}

/*************************************************************************************************************
 * Push()
 *
 * Argumentos: pI - item a ser inserido na pilha (inteiro neste caso)
 *
 * Retorna: (void)
 * Efeitos secundarios: aloca memoria para um elemento da pilha
 *
 * Descricao:
 * 	 Cria e coloca um elemento na pilha
 ************************************************************************************************************/
 
void Push(int pI) {
	pilha *Top = (pilha *)malloc(sizeof(pilha));
	Top->item = pI; Top->next = gP;
	gP = Top;
}

/*************************************************************************************************************
 * Pop()
 *
 * Argumentos: nenhum
 *
 * Retorna: inteiro com valor do item do elemento que foi removido da pilha
 * Efeitos secundarios: liberta memoria de elemento removido da pilha
 *
 * Descricao:
 * 	 Retira elemento do topo da pilha
 ************************************************************************************************************/
 
int Pop() {
    int elemento = 0;
    elemento = gP->item;
	pilha *temp;

	if (gP == NULL) {
        exit (0);
	}
	else {
        temp = gP;
        gP = gP->next;
        temp->next = NULL;
        free(temp);
	}
	return elemento;
}

/*************************************************************************************************************
 * IsEmpty()
 *
 * Argumentos: nenhum
 *
 * Retorna: bool com valor 1 se pilha esta vazia, 0 caso contrario
 * Efeitos secundarios: nenhum
 *
 * Descricao:
 * 	 Verifica se a pilha esta vazia
 ************************************************************************************************************/
 
bool IsEmpty() {
	return gP == NULL ? true : false;
}
