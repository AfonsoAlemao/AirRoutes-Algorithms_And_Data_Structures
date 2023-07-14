/**********************************************************************************
* (h) Rui Daniel, Afonso Alemao
* Ultima modificacao: 04/12/2020
*
* Nome:
*   heap.h
*
* Descricao:
*   Prototipos de funcoes que operam sobre filas prioritarias representadas por
*   um acervo
*
* Comentarios: 
**********************************************************************************/

#ifndef HEAP_H_INCLUDED
#define HEAP_H_INCLUDED

#include "defs.h"

typedef struct _heap Heap;

void alteraPrio(int *FilaIndices, Heap *h, double *val, int v);
int FilaPrioVazia(Heap *h);
void FixDown(int *heapdata, int Idx, int N, int *TabelaIndices, double *val);
void FixUp(Heap *h, int Idx, int *FilaIndices, int v, double *val);
Heap *InitHeap(int tamanho);
void InsertInHeap(int v, double *val,int *FilaIndice, Heap *h);
void LibertaHeap(Heap *filaPrio);
int retiraMaiorPrio(Heap *h, int *FilaIndices, double *val);

#endif
