/***************************************************************************************
* (c) Rui Daniel, Afonso Alemao
* Ultima modificacao: 13/11/2020
*
* Nome:
*   conetividade.c
*
* Descricao:
*   Implementacao de funcoes que resolvem o problema de conetividade:
*   baseado no algoritmo de Compressed Weighted Quick Union
*
* Comentarios:
***************************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "conetividade.h"

/*******************************************************************************************************************
 * CWQU_InicializaConetividade()
 *
 * Argumentos: N - inteiro com o numero de elementos dos vetores a serem inicializados
 * 			   id, sz - vetores que contem informacao sobre a conetividade do conjunto
 * 
 * Retorna: (void)
 * Efeitos secundarios: inicializacoes de id e de sz
 *
 * Descricao:
 * 	 Inicializa todos os valores de sz a 1 e de id com o seu proprio id, ou seja, 
 * 	 elementos encontram-se desconetados inicialmente
 ******************************************************************************************************************/

void CWQU_InicializaConetividade(int N, int *id, int *sz) {
    int i = 0;
    for (i = 0; i < N; i++) {
        id[i] = i;
        sz[i] = 1;
    }
    return;
}

/***********************************************************************************************************************
* CWQU_Procura()
*
* Argumentos: p, q - inteiros a serem unidos
* 			  id, sz - vetores que contem informacao sobre a conetividade do conjunto
* 			  raiz1, raiz2 - inteiros que representam a raiz da arvore de conetividade de p e de q, respetivamente
* 			  
* Retorna: bool com valor 1 se elementos p e q estiverem no mesmo conjunto, 0 caso contrario
* Efeitos Secundarios: nenhum
*
* Descricao: Procura se p e q estao no mesmo conjunto. Base algoritmo de União Rápida com Compressão de Caminho
************************************************************************************************************************/

bool CWQU_Procura (int p, int q, int *id, int *raiz1, int *raiz2) {
	int i = 0, j = 0;
	
	/* Caso em que os elementos de procura sao iguais entre si */
	if (p == q) return 1;
	
	/* Vertices numerados de 1 a N, indices de 0 a N-1 */
    p--;
    q--;

	/* Percorre a arvore onde se encontra cada elemento, ate encontrar a raiz de cada uma destas 2 arvores*/
	for (i = p; i != id[i]; i = id[i]);
    for (j = q; j != id[j]; j = id[j]);

	/* Verificacao se estao no mesmo conjunto, ou seja, se a arvore onde estao possui uma raiz comum */
    if (i == j)
		return 1;

	/* Salvaguarda das raizes para facilitar possiveis unioes futuras */
    *raiz1 = i;
    *raiz2 = j;
    
    return 0;
}

/***********************************************************************************************************************
* CWQU_Uniao()
*
* Argumentos: p, q - inteiros a serem unidos
* 			  id, sz - vetores que contem informacao sobre a conetividade do conjunto
* 			  raiz1, raiz2 - inteiros que representam a raiz da arvore de conetividade de p e de q, respetivamente
* 			  
* Retorna: (void)
* Efeitos Secundarios: nenhum
*
* Descricao: Uniao dos conjuntos onde se encontra cada elemento. Base algoritmo de União Rápida com Compressão de Caminho
************************************************************************************************************************/

void CWQU_Uniao (int p, int q, int *sz, int *id, int raiz1, int raiz2) {
	int i = raiz1, j = raiz2, t = 0, x = 0;
	
	/* Vertices numerados de 1 a N, indices de 0 a N-1 */
	p--;
	q--;
	
	/* Ligacao da raiz da menor arvore para a raiz da arvore maior, de modo a esta aumentar o minimo possivel, colocar ex do 2n e complexidade */
	if (sz[i] < sz[j]) {
		id[i] = j;
		sz[j] += sz[i];
		t = j;
	}
	else {
		id[j] = i;
		sz[i] += sz[j];
		t = i;
	}

    /* Compressao da arvore: melhorar rapidez de futuras procuras */
	for (i = p; i != id[i]; i = x) {
		x = id[i];
		id[i] = t;
	}
	for (j = q; j != id[j]; j = x) {
		x = id[j];
		id[j] = t;
	}
	
	return;
}
