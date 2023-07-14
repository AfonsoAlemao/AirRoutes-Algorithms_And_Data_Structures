/***************************************************************************************
* (c) Rui Daniel, Afonso Alemao
* Ultima modificacao: 04/12/2020
*
* Nome:
*   heap.c
*
* Descricao:
*   Implementacao de funcoes que operam sobre filas prioritarias representadas por um
*   acervo
*
* Comentarios:
*	  Ficheiro criado tendo como base os slides das aulas teoricas de AED 2020/2021
*   1ºsemestre, Professor Carlos Bispo, estruturas utilizadas e algumas das funcoes
*   aqui implementadas sao extremamente semelhantes a essas mesmas funcoes implementadas
*   nestes slides no capitulo de acervos
***************************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "heap.h"

/* Estrutura que representa o acervo */
struct _heap {
  int n_elementos;               /* Numero de elementos no heap */
  int size;                     /* Tamanho maximo do heap */
  int *heapdata;               /* Vetor de inteiros que representa o acervo */
};

/******************************************************************************
 * InitHeap()
 *
 * Argumentos: tamanho - inteiro que representa o tamanho do acervo
 * 
 * Retorna: ponteiro para o novo acervo
 * Efeitos Secundarios: aloca memoria para novo heap e heapdata
 *
 * Descricao: Cria um acervo
 *****************************************************************************/

Heap *InitHeap(int tamanho) {
    Heap *novo;

    novo = (Heap *) malloc(sizeof(Heap));
    novo->n_elementos = 0;
    novo->size = tamanho;

    novo->heapdata = (int *) malloc(tamanho * sizeof(int));
    if (novo->heapdata == NULL) {
        fprintf(stderr, "Error in malloc of heap data\n");
        exit(1);
    }

    return novo;
}

/***************************************************************************************************
 * InsertInHeap()
 *
 * Argumentos: v - vertice a inserir
 * 			   val - vetor de doubles que representam os custos associados a cada vertice
 * 			   FilaIndice - vetor de inteiros representativos do indice de cada vertice no acervo
 * 			   h - ponteiro para heap
 * 
 * Retorna: (void)
 * Efeitos Secundarios: nenhum
 *
 * Descricao: Insere um novo elemento na heap
 ****************************************************************************************************/

void InsertInHeap(int v, double *val, int *FilaIndice, Heap *h) {
    /* Insere novo elemento no fim e restabelece condicao de acervo com FixUp */
	(h->heapdata)[h->n_elementos] = v;
	FixUp(h, h->n_elementos, FilaIndice, v, val);
	h->n_elementos++;

    return;
}

/*************************************************************************************************
 * FixUp()
 *
 * Argumentos: h - ponteiro para heap
 * 			   v - vertice em analise
 * 			   val - vetor de doubles que representam os custos
 * 			   FilaIndices - vetor de inteiros que guarda indice onde o elemento esta no acervo
 * 			   Idx - inteiro que representa o indice de um vertice no heapdata
 * 
 * Retorna: (void)
 * Efeitos Secundarios: nenhum
 *
 * Descricao: Mantem a condicao de acervo, subindo se necessario o elemento 
 * 			  na arvore representada pelo acervo
 ************************************************************************************************/

void FixUp(Heap *h, int Idx, int *FilaIndices, int v, double *val) {
    int i = 0;
    /* Testa se elemento ja e raiz da arvore, caso nao seja compara com o seu pai
	se filho for mais prioritario que pai ha troca */
    while (Idx > 0 && (val[(h->heapdata[(Idx - 1) / 2]) - 1] > val[(h->heapdata[Idx]) - 1])) {
		/* Guarda o indice na heapdata do vertice (h->heapdata[(Idx-1)/2]) 
		 * Fila Indices guarda o indice onde o elemento esta no acervo
		 * para posteriormente podermos atraves do vertice fazer o acesso numa posicao do acervo */
        FilaIndices[(h->heapdata[(Idx - 1) / 2]) - 1] = Idx;
        i = h->heapdata[Idx];
        h->heapdata[Idx] = h->heapdata[(Idx - 1) / 2];
        h->heapdata[(Idx - 1) / 2] = i;
        Idx = (Idx - 1) / 2;
    }
    FilaIndices[v - 1] = Idx;
    return;
}

/***********************************************************************************************
 * alteraPrio()
 *
 * Argumentos: FilaIndices - vetor de inteiros que guarda indice onde o elemento esta no acervo
 * 			   h - ponteiro para heap
 * 			   val - vetor de doubles que representam os custos
 * 			   v - vertice em analise
 * 
 * Retorna: (void)
 * Efeitos Secundarios: nenhum
 *
 * Descricao: Preserva a condicao de acervo apos a modificacao da prioridade de um dos seus
 * 			  elementos
 **********************************************************************************************/
 
void alteraPrio(int *FilaIndices, Heap *h, double *val, int v) { 
	/* Sempre que ele altera a prioridade o custo é sempre menor 
	 * logo vai apenas ser feito FixUp do elemento
	 * Menor custo = maior prioridade */
    FixUp(h, FilaIndices[v - 1], FilaIndices, v, val);
    return;
}

/***********************************************************************************************************
 * FixDown()
 *
 * Argumentos: heapdata- vetor de inteiros que representa o acervo
 * 			   Idx - inteiro que representa o indice no heapdata de um dado vertice
 * 			   N - inteiro que representa o numero de elementos do acervo
 * 			   FilaIndices - vetor de inteiros que guarda indice onde o elemento esta no acervo
 * 			   val - vetor de doubles que representam os custos
 * 
 * Retorna: (void)
 * Efeitos Secundarios: nenhum
 *
 * Descricao: Mantem a condicao de acervo, descendo se necessario o elemento 
 * 			  na arvore representada pelo acervo
************************************************************************************************************/

void FixDown(int *heapdata, int Idx, int N, int *FilaIndices, double *val) {
    int Child = 0, i = 0;
	
	/* Verificacao se ha filhos a analisar*/
    while(2 * Idx < N - 1) {
        Child = 2 * Idx + 1;
		/* Caso exista 1 filho, analise do primeiro,
        * Caso existam 2 filhos, analise do filho de maior prioridade */
        if (Child < (N - 1) && val [ (heapdata[Child] - 1) ] > val [(heapdata[Child + 1] - 1)]) Child++;
        /* Vamos comparar o filho em analise com o elemento em analise,
		 * caso o filho tenha maior prioridade ha uma troca */
        if (!(val[(heapdata[Idx] - 1)] > val[(heapdata[Child] - 1)])) break;

		/* Troca pai com filho em analise */
        FilaIndices[heapdata[Idx] - 1] = Child;
        FilaIndices[heapdata[Child] - 1] = Idx;
        i = heapdata[Idx];
        heapdata[Idx] = heapdata[Child];
        heapdata[Child] = i;

		/* Na proxima iteracao analise do pai que agora esta na folha abaixo */
        Idx = Child;
    }
    return;
}

/**********************************************************************************************
* retiraMaiorPrio()
*
* Argumentos: h - ponteiro para Heap
* 			  FilaIndices - vetor de inteiros que guarda indice onde o elemento esta no acervo
* 			  val - vetor de doubles que representam os custos
* 
* Retorna: inteiro que representa o vertice de menor custo (= mais prioritario)
* Efeitos Secundarios: nenhum
*
* Descricao: remove o elemento de maior prioridade do heap (de menor val)
************************************************************************************************/

int retiraMaiorPrio(Heap *h, int *FilaIndices, double *val) {
    int i = 0;
    /* Troca primeiro elemento do acervo com o ultimo */
    i = h->heapdata[0];
    h->heapdata[0] = h->heapdata[h->n_elementos - 1];
    h->heapdata[h->n_elementos - 1] = i;
    FilaIndices[h->heapdata[0] - 1] = 0;
    
    /* Restabelece condicao de acervo */
    FixDown(h->heapdata, 0, h->n_elementos - 1, FilaIndices, val);
    i = h->n_elementos;
    FilaIndices[h->heapdata[i - 1] - 1] = -1;
    
    /* Ultimo elemento não considerado na reordenação */
    return h->heapdata[--h->n_elementos];
}

/******************************************************************************
* LibertaHeap()
*
* Argumentos: h - ponteiro para heap
* 
* Retorna: (void)
* Efeitos Secundarios: liberta memoria associada ao heap 
*
* Descricao: liberta a fila (heap)
*****************************************************************************/

void LibertaHeap(Heap *h) {
    free(h->heapdata);
    free(h);
}

/******************************************************************************
* FilaPrioVazia()
*
* Argumentos: h - ponteiro para heap
* 
* Retorna: inteiro que indica se a fila esta ou nao vazia
* Efeitos Secundarios: nenhum
*
* Descricao: Determina se a fila esta ou nao vazia
*****************************************************************************/

int FilaPrioVazia(Heap *h) {
    if (h->n_elementos == 0) {
        return 1;
    }
    else
        return 0;
}
