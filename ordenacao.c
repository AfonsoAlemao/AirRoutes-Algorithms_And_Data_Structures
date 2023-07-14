/***************************************************************************************
* (c) Rui Daniel, Afonso Alemao
* Ultima modificacao: 24/11/2020
*
* Nome:
*   ordenacao.c
*
* Descricao:
*   Implementacao de funcoes que resolvem o problema da ordenacao:
* 	Quick Sort, Insertion Sort
*
* Comentarios: Funcoes tendo como base os slides das aulas teoricas de AED 2020/2021
*   1ºsemestre, Professor Carlos Bispo
*
***************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Pilha.h"
#include "ordenacao.h"

/******************************************************************************
 * medianadetres ()
 *
 * Argumentos: a - Ponteiro para tabela de items a ordenar
 *            l, r - limites da tabela
 *            less - funcao que dita o criterio de ordenacao
 *
 * Retorna: (void)
 * Efeitos secundarios: nenhum
 *
 * Descricao: calcula a mediana entre o primeiro, o ultimo e o elemento a meio
 * 			  do vetor, e coloca este na ultima posicao: sera o pivot na
 * 			  proxima iteracao do quick sort
 *****************************************************************************/

void medianadetres(Item a[], int l, int r, int (*less) (Item, Item)) {
	int m = (l + r) / 2;
	int med = l;

	if (r <= l + 1) {return;}

	if (a[l] < a[m] && a[m] < a[r]) {
		med = m;
	}
	else if (a[r] < a[m] && a[m] < a[l]) {
		med = m;
	}
	else if (a[l] < a[r] && a[r] < a[m]) {
		med = r;
	}
	else if (a[m] < a[r] && a[r] < a[l]) {
		med = r;
	}
	if (med != r){
		exch(a[r], a[med]);
	}
	return;
}

/******************************************************************************
* eq()
*
* Argumentos: A, B - items a serem comparados
* 			  less - funcao que dita o criterio de comparacao
*
* Retorna: inteiro com o resultado da comparacao
* Efeitos Secundarios: nenhum
*
* Descricao: Implementa uma comparacao verificando se os elementos sao
* 			 de igual valor de chave de ordenacao
*****************************************************************************/

int eq(Item A, Item B, int (*less) (Item, Item)) {
	return (!less(A, B) && !less(B, A));
}

/************************************************************************************
 * QuickSort ()
 *
 * Argumentos: a - Ponteiro para tabela de items a ordenar
 *            l, r - limites da tabela
 *            less - funcao que dita o criterio de ordenacao
 *
 * Retorna: (void)
 * Efeitos Secundarios: A tabela fica ordenada
 *
 * Descricao: Implementa algoritmo de ordenacao Quick Sort para tipo abstrato (Item),
 *       	  utilizando a funcao de comparacao less
 *            Utiliza uma pilha explicita de modo a minimizar a memoria gasta,
 * 			  nao fazendo recursao e tratanto primeiro problemas menores, de modo
 *            a que a pilha cresca o minimo possivel. Utiliza mediana de tres para
 * 			  otimizar a escolha do pivot, particao em 3 para evitar ordenar tabelas
 * 			  com muitos elementos iguais, e ignora tabelas demasiado pequenas,
 * 			  sendo estas ordenadas com o Insertion Sort final
 ***********************************************************************************/

void *QuickSort(Item *a, int l, int r, int (*less) (Item, Item)) {
	int i, j, k, p, q;
	int aux1 = l, aux2 = r;
	Item v;
	Init();
	push2(l, r);
	while (!IsEmpty()) {
		r = Pop();
		l = Pop();
		if (r - l <= M)
            continue;
		medianadetres(a, l, r, less);
		v = a[r]; i = l - 1; j = r; p = l - 1; q = r;
		for (;;) {
			while (less(a[++i], v));
			while (less(v, a[--j])) {
				if (j == l){
					break;
				};
			}
			if (i >= j) break;
			exch(a[i], a[j]);
			if (eq(a[i],v, less)) {
				p++;
				exch(a[p],a[i]);
			}
			if (eq(v,a[j], less)) {
				q--;
				exch(a[q],a[j]);
			}
		}
		exch(a[i], a[r]); j = i-1; i = i+1;
		for (k = l ; k <= p; k++, j--) {exch(a[k], a[j]);}
		for (k = r-1; k >= q; k--, i++) {exch(a[k], a[i]);}
		if (j - l > r - i) {
			push2(l, j);
			push2(i, r);
		} else {
			push2(l, j);
			push2(i, r);
		}
	}
	a = insertionsort(a, aux1 , aux2, less);
	return a;
}

/************************************************************************************
 * insertionsort ()
 *
 * Argumentos: a - Ponteiro para tabela de items a ordenar
 *            l, r - limites da tabela
 *            less - funcao que dita o criterio de ordenacao
 *
 * Retorna: (void)
 * Efeitos Secundarios: A tabela fica ordenada
 *
 * Descricao: Implementa algoritmo de ordenacao Insertion Sort para tipo abstrato (Item),
 *       	  utilizando a funcao de comparacao less
 ***********************************************************************************/

void *insertionsort(Item *a, int l, int r, int (*less) (Item, Item))
{
   int i, j;

   for (i = l + 1; i <= r; i++) {
      Item v = a[i];
      j = i;
      while (j > l && less(v, a[j - 1])) {
         a[j] = a[j - 1];
         j--;
      }
      a[j] = v;
   }
   return a;
}

/************************************************************************************
 * OrdenaPrimeirosNelementos ()
 *
 * Argumentos: a - Ponteiro para tabela de items a ordenar
 *            l, r - limites da tabela
 * 			  N - ordenao ate elemento de indice N
 *            less - funcao que dita o criterio de ordenacao
 *
 * Retorna: (void)
 * Efeitos Secundarios: A tabela fica ordenada
 *
 * Descricao: Implementacao do Quick Sort como acima descrito, com a particularidade
 * 			  de nao ordenar sub-tabelas acima do limite N
 ***********************************************************************************/

void *OrdenaPrimeirosNelementos(Item *a, int l, int r, int N, int (*less) (Item, Item)) {
	int i, j, k, p, q;
	int aux1 = l;
	Item v;
	Init();
	push2(l, r);
	while (!IsEmpty()) {
		r = Pop();
		l = Pop();
		if ((r < N && r - l <= M) || l > N)
            continue;
		medianadetres(a, l, r, less);
		v = a[r]; i = l - 1; j = r; p = l - 1; q = r;
		for (;;) {
			while (less(a[++i], v));
			while (less(v, a[--j])) {
				if (j == l){
					break;
				};
			}
			if (i >= j) break;
			exch(a[i], a[j]);
			if (eq(a[i],v, less)) {
				p++;
				exch(a[p],a[i]);
			}
			if (eq(v,a[j], less)) {
				q--;
				exch(a[q],a[j]);
			}
		}
		exch(a[i], a[r]); j = i-1; i = i+1;
		for (k = l ; k <= p; k++, j--) {exch(a[k], a[j]);}
		for (k = r-1; k >= q; k--, i++) {exch(a[k], a[i]);}
		if (j - l > r - i) {
			push2(l, j);
			push2(i, r);
		} else {
			push2(l, j);
			push2(i, r);
		}
	}
	a = insertionsort(a, aux1 , N, less);
	return a;
}
