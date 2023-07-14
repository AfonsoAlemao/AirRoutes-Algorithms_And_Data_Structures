/**********************************************************************************
* (h) Rui Daniel, Afonso Alemao
* Ultima modificacao: 25/11/2020
*
* Nome:
*   Pilha.h
*
* Descricao:
*   Prototipos para operar sobre uma pilha
*
* Comentarios: 
**********************************************************************************/

#ifndef _PILHA_H
#define _PILHA_H

#include "defs.h"

int Empty();
void freePilha();
void Init();
bool IsEmpty();
int Pop();
void Push(int pI);

#endif
