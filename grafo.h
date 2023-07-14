/**********************************************************************************
* (h) Rui Daniel, Afonso Alemao
* Ultima modificacao: 05/12/2020
*
* Nome:
*   grafo.h
*
* Descricao:
*   Prototipos de funcoes que operam sobre as estruturas abstratas grafo,
*   aresta e nodo, estruturas utilizadas no projeto airroutes
*	Implementacao de um grafo com uma tabela para lista de adjacencias
*
* Comentarios: Prototipos ordenados alfabeticamente
*			   Declaracao de 3 estruturas a serem definidas num ficheiro grafo.c
**********************************************************************************/

#ifndef GRAFO_H_INCLUDED
#define GRAFO_H_INCLUDED

#include "defs.h"

typedef struct aresta Aresta;
typedef struct grafo Grafo;
typedef struct nodo link;

Aresta *AlgoritmoKruskal(Grafo *G, Aresta **mst, double *CustoTotal, Aresta **a, int *NumArestasMST);
Aresta *AlgoritmoKruskalOrdenacaoParcial(Grafo *G, Aresta **mst, double *CustoTotal, Aresta **a, int *NumArestasMST);
Aresta *AlgoritmoPrimOtimizado(Grafo *Aeroporto, Aresta **mst, double *custoTotal, Aresta **a, int *NumArestasMST);
void AlteraValoresBackbone(Aresta **backbone2, Aresta *RotaSubstituicao, int Indice);
double CalculaCustoBackbone(Aresta **backbone2, int NumeroArestasBackbone2);
Aresta *CalculoBackbone(Grafo *Aeroporto, int *NumArestasBackbone, double *Custo, Aresta **a, Aresta **backbone);
Aresta *CriaAresta(int vertice1, int vertice2, double custo);
void CriaBackbone2(Aresta **backbone, Aresta **backbone2Aresta, Aresta **RotaSubstituicao, int NumeroArestasBackbone,
                    int VerticeAnaliseA, int VerticeAnaliseB, Grafo *Aeroporto);
Aresta *CriaVetorArestas(Aresta *a[], Grafo *G);
void CriaVetorArestasNovo(Aresta **VetorAresta2, Aresta **VetorArestas, int NumVertices);
double CustoAresta(Aresta *a);
int DetetaClique(Grafo *Aeroporto, int principal, bool *flags);
double DeterminaDensidade(Grafo *G);
int DeterminaGrau(Grafo *Aeroporto, int principal);
void EscreveAresta(Aresta *x, FILE *fp);
int EqualAresta(Item a, Item b);
int EqualCustoeVertices (Item a, Item b);
void freelink(link *Listhead);
void FreeGrafo(Grafo *Aeroporto);
void FreeVetorArestas(Aresta **, int N);
int GrafoArestas(Aresta **, Grafo *);
bool *InicializaFlags(bool *flags, int N);
Grafo *InicializaGrafo(int , int);
void InsereArestaGrafo(Grafo *, Aresta *);
int LessAresta(Item a, Item b);
int LessArestaCusto(Item a, Item b);
int LessArestaCustoOrdenacao(Item a, Item b);
int LessCusto(Item a, Item b);
int LessVertice(Item a, Item b);
int maxArestaCusto(Item a, Item b);
int MaxArestaCustoOrdenacao(Item a, Item b);
link *NovoElementoLista(int v, double custo, link *next);
int NumeroArestas(Grafo *G);
int NumeroCliquesDistintos(Grafo *Aeroporto, int principal, bool *flags);
int NumeroVertices(Grafo *G);
int PesquisaMelhorRotaSubstituicao(Aresta **a, int *id, int *sz, int NArestas, int VerticeAnaliseA, int VerticeAnaliseB);
Aresta **RemocaoAeroporto(Aresta **backbone, int NumeroArestasBackbone, int VerticeAnaliseA, Aresta **RotaSubstituicao, Aresta **VetorArestas,
						  Grafo *Aeroporto, int *resposta, int *NRotSubstituicao, double *CustoTotalRotasSubstituicao);
Aresta *RemocaoERotadesubstituicao(Aresta **backbone, int NumeroArestasBackbone, int VerticeAnaliseA, int VerticeAnaliseB,
								   Aresta **a, Grafo *Aeroporto, int *resposta);
Aresta *RemocaoERotadesubstituicaoE1(Aresta **backbone, int NumeroArestasBackbone, int VerticeAnaliseA, int VerticeAnaliseB, Aresta **a, Grafo *Aeroporto, int *resposta, double custo);
int Vertice1Aresta(Aresta *a);
int Vertice2Aresta(Aresta *a);
int VerticesSaoAdjacentes(Grafo *Aeroporto, int VerticeAnaliseA, int VerticeAnaliseB, double *custo);

#endif
