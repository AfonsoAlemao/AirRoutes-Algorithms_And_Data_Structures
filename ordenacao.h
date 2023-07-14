/**********************************************************************************
* (h) Rui Daniel, Afonso Alemao
* Ultima modificacao: 25/11/2020
*
* Nome:
*   ordenacao.h
*
* Descricao:
*   Prototipos de funcoes que resolvem problemas de ordenacao
*	Definicao de 3 macros tendo em vista ajudar a implementacao do algoritmo
* 	de Quick Sort
*
* Comentarios: 
**********************************************************************************/

#ifndef _ORDENACAO_H
#define _ORDENACAO_H

#define push2(A, B) Push(A); Push(B);
#define exch(A, B) {Item t = A; A = B; B = t;}

/* Parametro de decisao do tamanho das tabelas a ordenar com o Quick Sort
 * e a partir de quais deixar essa ordenacao para o Insertion Sort final*/
#define M 10

#include "defs.h"

void medianadetres(Item a[], int l, int r, int (*less) (Item, Item));
int eq(Item A, Item B, int (*less) (Item, Item));
void *QuickSort(Item *a, int l, int r, int (*less) (Item, Item));
void *insertionsort(Item *a, int l, int r, int (*less) (Item, Item));
void *OrdenaPrimeirosNelementos(Item *a, int l, int r, int N, int (*less) (Item, Item));

#endif
