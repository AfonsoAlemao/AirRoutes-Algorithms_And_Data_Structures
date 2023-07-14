/**********************************************************************************
* (h) Rui Daniel, Afonso Alemao
* Ultima modificacao: 13/11/2020
*
* Nome:
*   conetividade.h
*
* Descricao:
*   Prototipos de funcoes que resolvem problemas de conetividade
*
* Comentarios: Utilizacao do Algoritmo de União Rápida com Compressão de Caminho
**********************************************************************************/

#ifndef conetividade
#define conetividade

#include "defs.h"

void CWQU_InicializaConetividade(int N, int *id, int *sz);
bool CWQU_Procura(int p, int q, int *id, int *raiz1, int *raiz2);
void CWQU_Uniao(int p, int q, int *sz, int *id, int raiz1, int raiz2);

#endif
