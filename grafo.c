/***************************************************************************************
* (c) Rui Daniel, Afonso Alemao
* Ultima modificacao: 05/12/2020
*
* Nome:
*   grafo.c
*
* Descricao:
*   Implementacao de funcoes que operam sobre as estruturas abstratas grafo,
*   aresta e nodo
*
* Comentarios:
*	Ficheiro criado tendo como base os slides das aulas teoricas de AED 2020/2021
*   1ºsemestre, Professor Carlos Bispo, estruturas utilizadas e algumas das funcoes
*   aqui implementadas sao extremamente semelhantes a essas mesmas funcoes implementadas
*   nestes slides, tendo em conta a opcao clara de projeto: Utilizacao de uma tabela
*   de listas de adjacencias para representacao do grafo em memoria, e por vezes de um
* 	vetor de arestas
***************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"
#include "ordenacao.h"
#include "conetividade.h"
#include "airroutes.h"
#include "heap.h"

struct nodo{int vertice; link *next; double custo;};
struct grafo{int NumVertices; int NumArestas; link **adj;};
struct aresta{int vertice1; int vertice2; double custo;};

/*************************************************************************************************************
 * NovoElementoLista()
 *
 * Argumentos: v - inteiro que contem o numero do vertice
 * 			   custo - custo da ligacao da aresta a criar (double)
 * 			   next - ponteiro para a cabeca da lista
 *
 * Retorna:
 *   Um ponteiro para uma estrutura do tipo link
 * Efeitos secundarios: Memoria alocada para estrutura do tipo link
 *
 * Descricao:
 * 	 Cria uma nova estrutura do tipo link e coloca-a no inicio da lista
 ************************************************************************************************************/

link *NovoElementoLista(int v, double custo, link *next) {
	link *x = (link *) malloc(sizeof(struct nodo));
    x->vertice = v;
    x->next = next;
    x->custo = custo;
    return x;
}

/*************************************************************************************************************
 * CriaAresta()
 *
 * Argumentos: vertice1, vertice2 - inteiros com vertices constituintes da aresta a criar
 * 			   custo - custo da ligacao da aresta a criar (double)
 *
 * Retorna:
 * 	 Um ponteiro para uma estrutura criada do tipo aresta
 * Efeitos secundarios: Memoria alocada para estrutura do tipo aresta
 *
 * Descricao:
 * 	 Cria uma nova estrutura do tipo aresta
 ************************************************************************************************************/

Aresta *CriaAresta(int vertice1, int vertice2, double custo) {
	Aresta *x = (Aresta *) malloc(sizeof(Aresta));
	if (vertice1 < vertice2) {
        x->vertice1 = vertice1;
        x->vertice2 = vertice2;
	}
	else {
        x->vertice1 = vertice2;
        x->vertice2 = vertice1;
	}
    x->custo = custo;
    return x;
}

/*************************************************************************************************************
 * EscreveAresta()
 *
 * Argumentos: x - ponteiro para aresta a ser escrita
 * 			   fp - ponteiro para ficheiro onde vai ser efetuada a escrita
 *
 * Retorna: (void)
 * Efeitos secundarios: nenhum
 *
 * Descricao:
 * 	 Escreve o conteudo de uma estrutura do tipo Aresta para um ficheiro de saida
 ************************************************************************************************************/

void EscreveAresta(Aresta *x, FILE *fp) {
    fprintf(fp, "%d %d %.2f\n", x->vertice1, x->vertice2, x->custo);
    return;
}

 /*************************************************************************************************************
 * InicializaGrafo()
 *
 * Argumentos: Vertices, Arestas - inteiros com os componetes do grafo a criar
 *
 * Retorna:
 * 	 Um ponteiro para uma estrutura do tipo Grafo
 * Efeitos secundarios: Memoria alocada para estrutura do tipo grafo
 *
 * Descricao:
 * 	 Cria grafo com o numero final de vertices, sem arestas
 ************************************************************************************************************/

Grafo *InicializaGrafo(int Vertices, int Arestas) {
	int i = 0;
	Grafo *Aeroporto = (Grafo*) malloc(sizeof(struct grafo));
	Aeroporto->NumVertices = Vertices;
	Aeroporto->NumArestas = 0;
	Aeroporto->adj = (link **) malloc(Vertices * sizeof(link*));
	for (i = 0; i < Vertices; i++)
		Aeroporto->adj[i] = NULL;
    return Aeroporto;
}

/***********************************************************************
 * InsereArestaGrafo()
 *
 * Argumentos: Aeroporto - ponteiro para uma estrutura do tipo Grafo
 *             e - ponteiro para uma estrutura do tipo Aresta
 *
 * Retorna: (void)
 * Efeitos secundarios: Numero de arestas do grafo incrementado
 *
 * Descricao:
 * 	Insere aresta no grafo, representacao por lista de adjacencias,
 *  dupla insercao, na lista das adjacencias de cada vertice,
 *  custo tambem inserido
 **********************************************************************/

void InsereArestaGrafo(Grafo *Aeroporto, Aresta *e) {
	Aeroporto->adj[e->vertice1 - 1] = NovoElementoLista(e->vertice2, e->custo, Aeroporto->adj[e->vertice1 - 1]);
    Aeroporto->adj[e->vertice2 - 1] = NovoElementoLista(e->vertice1, e->custo, Aeroporto->adj[e->vertice2 - 1]);
    Aeroporto->NumArestas++;
    return;
}

/***********************************************************************
 * GrafoArestas()
 *
 * Argumentos: a - ponteiro para vetor de arestas
 *             Aeroporto - ponteiro para uma estrutura do tipo Grafo
 *
 * Retorna: inteiro com numero de arestas do vetor de arestas
 * Efeitos secundarios: vetor de arestas preenchido
 *
 * Descricao:
 * 	Preenche o vetor de arestas a,
 *  retorna contagem do seu numero de elementos
 **********************************************************************/

int GrafoArestas(Aresta *a[], Grafo *Aeroporto) {
	int v, NumTotalArestas = 0;
    link *t;
    for (v = 0; v < Aeroporto->NumVertices; v++)
		for (t = Aeroporto->adj[v]; t != NULL; t = t->next)
			if (v < t->vertice)
				a[NumTotalArestas++] = CriaAresta(v, t->vertice, t->custo);

    return NumTotalArestas;
}

/*************************************************************************************
 * DeterminaGrau()
 *
 * Argumentos: Aeroporto - ponteiro para uma estrutura do tipo Grafo
 * 			   principal - vertice que vai ser analisado
 *
 *
 * Retorna: inteiro com o grau do vertice inserido como argumento
 * Efeitos secundarios: (nenhum)
 *
 * Descricao:
 * 	 Percorre a lista de adjacencias do vertice principal e retorna a sua contagem
 ************************************************************************************/

int DeterminaGrau(Grafo *Aeroporto, int principal) {
    link *t;
    int contador = 0;

	/* Verificacao se o vertice esta no grafo em analise */
    if (principal > Aeroporto->NumVertices || principal <= 0) {
		return -1;
	}

	/* Contagem do numero de adjacencias do vertice em analise */
    for (t = Aeroporto->adj[principal - 1]; t != NULL; t = t->next) {
        contador++;
    }

    return contador;
}

/****************************************************************************************************
 * VerticesSaoAdjacentes()
 *
 * Argumentos: Aeroporto- ponteiro para uma estrutura do tipo Grafo
 *             VerticeAnaliseA, VerticeAnaliseB - inteiros que representam vertices em analise
 *             custo - ponteiro para custo da ligacao a ser preenchido (double)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        teiro que representa um vertice
 * Retorna:
 *    inteiro com 1 se VerticeAnaliseA e VerticeAnaliseB são adjacentes, caso contrário, retorna -1
 * Efeitos secundarios: (nenhum)
 *
 * Descricao:
 *    Procura se VerticeAnaliseB existe na lista de adjacencias do VerticeAnaliseA, e verifica custo
 ***************************************************************************************************/

int VerticesSaoAdjacentes(Grafo *Aeroporto, int VerticeAnaliseA, int VerticeAnaliseB, double *custo) {
    link *t;

	/* Verificacao se os vertices estao no grafo em analise */
    if (VerticeAnaliseA > Aeroporto->NumVertices || VerticeAnaliseA <= 0) {
		return -1;
	}
	if (VerticeAnaliseB > Aeroporto->NumVertices || VerticeAnaliseB <= 0) {
		return -1;
	}

	/* Verificacao da ligacao: verifica se o VerticeAnaliseB se encontra na
	 * lista de adjacencias do VerticeAnaliseA */
    for (t = Aeroporto->adj[VerticeAnaliseA - 1]; t != NULL; t = t->next) {
        if (t->vertice == VerticeAnaliseB) {
			*custo = t->custo;
            return 1;
        }
	}
	return -1;
}

/**********************************************************************************************
 * DetetaClique()
 *
 * Argumentos: Aeroporto - ponteiro para tabela com cada aeroporto e a sua lista de adjacencias
 * 			   principal - identificador do aeroporto em analise
 * 			   flags - vetor de flags com simbologia de "visitado"
 *
 * Retorna:  inteiro que indica se existe um clique para o aeroporto em analise
 * Efeitos secundarios: (nenhum)
 *
 * Descricao:
 *   Retorna 1 se o aeroporto em análise possui um clique, e 0 em caso contrário
 *********************************************************************************************/

int DetetaClique(Grafo *Aeroporto, int principal, bool *flags) {
	link *t, *s;

	/* Verificacao se o vertice esta no grafo em analise */
	if (principal > Aeroporto->NumVertices || principal <= 0) {
		return -1;
	}

	flags = InicializaFlags(flags, Aeroporto->NumVertices);

	/* Colocar a flag de todos os adjacentes ao principal a 1 */
	for (t = Aeroporto->adj[principal - 1]; t!= NULL; t = t->next) {
		flags[t->vertice - 1] = 1;
	}

	/* Percorrer a lista de cada elemento ligado ao principal em busca de
	 * outro nas mesmas condicoes que forme um clique */

	for (t = Aeroporto->adj[principal - 1]; t != NULL; t = t->next) {
		for (s = Aeroporto->adj[t->vertice - 1]; s != NULL; s = s->next) {
			if (flags[s->vertice - 1]) {
				return 1;
			}
		}
	}
	return 0;
}

/**********************************************************************************************
 * NumeroCliquesDistintos()
 *
 * Argumentos: Aeroporto - ponteiro para tabela com cada aeroporto e a sua lista de adjacencias
 * 			   principal - inteiro identificador do aeroporto em analise
 * 			   flags - vetor de flags com simbologia de "visitado"
 *
 * Retorna:  inteiro que indica o numero de cliques do aeroporto em analise
 * Efeitos secundarios: (nenhum)
 *
 * Descricao:
 *   Retorna o numero de cliques distintos de um aeroporto
 *********************************************************************************************/

int NumeroCliquesDistintos(Grafo *Aeroporto, int principal, bool *flags) {
	link *t, *s;
	int contador = 0;

	/* Verificacao se o vertice esta no grafo em analise */
	if (principal > Aeroporto->NumVertices || principal <= 0) {
		return -1;
	}

    flags = InicializaFlags(flags, Aeroporto->NumVertices);

	/* Colocar a flag de todos os vertices adjacentes ao principal a 1 */
	for (t = Aeroporto->adj[principal - 1]; t != NULL; t = t->next) {
		flags[t->vertice - 1] = 1;
	}

	/* Percorrer a lista de cada elemento ligado ao principal em busca de
	 * outro nas mesmas condicoes que forme um clique */

	for (t = Aeroporto->adj[principal - 1]; t != NULL; t = t->next) {
		flags[t->vertice - 1] = 0;
		for (s = Aeroporto->adj[t->vertice - 1]; s != NULL; s = s->next) {
			if (flags[s->vertice - 1]) {
				contador++;
			}
		}
	}
	return contador;
}

/*************************************************************************************************************
 * InicializaFlags()
 *
 * Argumentos: flags - ponteiro para tabela bool a ser inicializada
 *			   N - inteiro com o numero de elementos do vetor a ser inicializado
 *
 * Retorna: ponteiro para elemento do tipo bool
 * Efeitos secundarios: tabela de flags inicializada a 0
 *
 * Descricao:
 * 	 Inicializa todos os valores da tabela a 0
 ************************************************************************************************************/

bool *InicializaFlags(bool *flags, int N) {
    int i = 0;
    for (i = 0; i < N; i++) {
        flags[i] = 0;
    }
    return flags;
}

/******************************************************************************
 * freelink ()
 *
 * Argumentos: Listhead - pointeiro para link
 *
 * Retorna:  (void)
 * Efeitos Secundarios: liberta espaco ocupado pelos items da lista
 *
 * Descricao: liberta uma lista
 *****************************************************************************/

void freelink(link *Listhead) {
    link *Aux;
    Aux = Listhead;
    while (Listhead != NULL) {
        Aux = Listhead;
        Listhead = (Listhead->next);
        free(Aux);
    }
    return;
}

/******************************************************************************
 * FreeGrafo()
 *
 * Argumentos: Aeroporto - pointeiro para grafo
 *
 * Retorna:  (void)
 * Efeitos Secundarios: libertao o grafo
 *
 * Descricao: liberta um grafo
 *****************************************************************************/

void FreeGrafo(Grafo *Aeroporto) {
    int i;
    if (Aeroporto == NULL) {return;}
    for (i = 0; i < Aeroporto->NumVertices; i++) {
        freelink(Aeroporto->adj[i]);
    }
    free(Aeroporto->adj);
    free(Aeroporto);
    return;
}

/*************************************************************************************************************
 * NumeroArestas()
 *
 * Argumentos: G - ponteiro para grafo em analise
 *
 * Retorna: inteiro com o numero de arestas do grafo
 * Efeitos secundarios: nenhum
 *
 * Descricao:
 * 	 Devolve o numero de arestas do grafo
 ************************************************************************************************************/

int NumeroArestas(Grafo *G) {
	return G->NumArestas;
}

/*************************************************************************************************************
 * NumeroVertices()
 *
 * Argumentos: G - ponteiro para grafo em analise
 *
 * Retorna: inteiro com o numero de vertices do grafo
 * Efeitos secundarios: nenhum
 *
 * Descricao:
 * 	 Devolve o numero de vertices do grafo
 ************************************************************************************************************/

int NumeroVertices(Grafo *G) {
	return G->NumVertices;
}

/*************************************************************************************
 * DeterminaDensidade()
 *
 * Argumentos: G - ponteiro para uma estrutura do tipo Grafo
 *
 * Retorna:
 *   Double com a densidade do grafo
 * Efeitos secundarios: nenhum
 *
 * Descricao:
 * 	 Determina a densidade do grafo
 ************************************************************************************/

double DeterminaDensidade(Grafo *G) {
    return (double) (G->NumArestas * 2) / G->NumVertices;
}

/******************************************************************************
 * CriaVetorArestas()
 *
 * Argumentos: a - ponteiro para um vetor do tipo Aresta
 *             G - ponteiro para uma estrutura do tipo Grafo
 *
 * Retorna:  ponteiro que aresta
 * Efeitos Secundarios:
 *
 * Descricao: cria um vetor de arestas
 *****************************************************************************/

Aresta *CriaVetorArestas(Aresta *a[], Grafo *G)
{
	int v, E = 0;
	link *t;
	for (v = 0; v < G->NumVertices; v++)
		for (t = G->adj[v]; t != NULL; t = t->next)
            if (v < t->vertice)
                a[E++] = CriaAresta(v + 1, t->vertice, t->custo);
	return *a;
}

/**********************************************************************************************
 * CalculoBackbone()
 *
 * Argumentos: Aeroporto - ponteiro para tabela com cada aeroporto e a sua lista de adjacencias
 * 			   NumArestasBackbone - ponteiro para inteiro com numero de arestas no backbone
 * 			   Custo - ponteiro para double com custo total do backbone
 *			   a - ponteiro para vetor de arestas com todas as rotas do grafo
 * 			   backbone - ponteiro para vetor de arestas onde se encontra o backbone
 *
 * Retorna: ponteiro para vetor de arestas: o backbone
 * Efeitos secundarios: (nenhum)
 *
 * Descricao:
 *   Calcula a arvore minima de suporte de um grafo ponderado, retorna o backbone
 * 	 Utiliza um de 3 algoritmos estudados
 *   Apos uma testagem e analise dos 3 algoritmos implementados: PrimOtimizado, Kruskal e Kruskal
 * 	 OtimizacaoParcial, obtamos por utilizacao do PrimOtimizado
 *********************************************************************************************/

Aresta *CalculoBackbone(Grafo *Aeroporto, int *NumArestasBackbone, double *Custo, Aresta **a, Aresta **backbone) {
    *backbone = AlgoritmoPrimOtimizado(Aeroporto, backbone, Custo, a, NumArestasBackbone);
	return *backbone;
}

/******************************************************************************
* LessCusto()
*
* Argumentos: a, b - items a comparar
* Retorna: inteiro com o resultado da comparacao
* Efeitos Secundarios: nenhum
*
* Descricao: Implementa a comparacao por custo de aresta
*****************************************************************************/

int LessCusto(Item a, Item b) {
    if (((Aresta *) a)->custo < ((Aresta *) b)->custo)
		return 1;
    else
		return 0;
}

/**********************************************************************************************
 * AlgoritmoKruskal()
 *
 * Argumentos: G - ponteiro para tabela com cada aeroporto e a sua lista de adjacencias
 *			   mst - ponteiro para vetor de arestas onde sera colocado o backbone
 * 			   CustoTotal - ponteiro para double com custo total do backbone
 * 			   a - ponteiro para vetor de arestas com todas as rotas do grafo
 * 			   NumArestasMST - ponteiro para inteiro com numero de arestas no backbone
 *
 * Retorna: Ponteiro para mst
 * Efeitos secundarios: (nenhum)
 *
 * Descricao:
 *   Calcula a arvore minima de suporte de um grafo ponderado pelo Algoritmo de Kruskal
 *********************************************************************************************/

Aresta *AlgoritmoKruskal(Grafo *G, Aresta **mst, double *CustoTotal, Aresta **a, int *NumArestasMST) {
	int i = 0, RaizUm = 0, RaizDois = 0;
	int NumeroArestas = G->NumArestas;
	int *sz = (int *) malloc(G->NumVertices * sizeof(int));
	int *id = (int *) malloc(G->NumVertices * sizeof(int));

	/* Ordena o vetor de arestas por custo */
	QuickSort((Item)a, 0, NumeroArestas - 1, LessCusto);
	CWQU_InicializaConetividade(G->NumVertices, id, sz);

	/* Cria a MST, pelos seguintes criterios: inserindo em primeiro lugar rotas mais baratas,
	 * sempre que estas nao fechem um ciclo, ou seja nao estejam ja no mesmo conjunto
	 * Utilizacao do problema da conetividade para representar os dados, e executar
	 * operacoes de procura e uniao entre os aeroportos */
	for (i = 0, *NumArestasMST = 0; i < NumeroArestas && *NumArestasMST < G->NumVertices - 1; i++) {
		if (!CWQU_Procura(a[i]->vertice1, a[i]->vertice2, id, &RaizUm, &RaizDois)) {
			CWQU_Uniao(a[i]->vertice1, a[i]->vertice2, sz, id, RaizUm, RaizDois);
			mst[(*NumArestasMST)++] = a[i];
			*CustoTotal += a[i]->custo;
		}
	}
	free(sz);
	free(id);
	return *mst;
}

/**********************************************************************************************
 * AlgoritmoKruskalOrdenacaoParcial()
 *
 * Argumentos: G - ponteiro para tabela com cada aeroporto e a sua lista de adjacencias
 *			   mst - ponteiro para vetor de arestas onde sera colocado o backbone
 * 			   CustoTotal - ponteiro para double com custo total do backbone
 * 			   a - ponteiro para vetor de arestas com todas as rotas do grafo
 * 			   NumArestasMST - ponteiro para inteiro com numero de arestas no backbone
 *
 * Retorna: Ponteiro para mst
 * Efeitos secundarios: (nenhum)
 *
 * Descricao:
 *   Calcula a arvore minima de suporte de um grafo ponderado pelo Algoritmo de Kruskal.
 *   Algoritmo otimizado com ordenacao parcial
 *
 *********************************************************************************************/

Aresta *AlgoritmoKruskalOrdenacaoParcial(Grafo *G, Aresta **mst, double *CustoTotal, Aresta **a, int *NumArestasMST) {
	int i = 0, RaizUm = 0, RaizDois = 0, NumeroArestasParaOrdenar = G->NumVertices - 1, NumeroArestasMaximoMST = G->NumVertices - 1;
	int NumeroArestas = G->NumArestas, NArestasOrdenadas = 0;
	int *sz = (int *) malloc(G->NumVertices * sizeof(int));
	int *id = (int *) malloc(G->NumVertices * sizeof(int));
	bool SaiNoFim = 0;

	*NumArestasMST = 0;
    CWQU_InicializaConetividade(G->NumVertices, id, sz);

    if (NumeroArestas < G->NumVertices - 1) {
        NumeroArestasParaOrdenar = NumeroArestas;
        NumeroArestasMaximoMST = NumeroArestas;
    }

	while (1) {
		/* Ordena o vetor de arestas por custo */
		if (NArestasOrdenadas + NumeroArestasParaOrdenar >= NumeroArestas) {
            NumeroArestasParaOrdenar = 0;
            SaiNoFim = 1;
		}
		OrdenaPrimeirosNelementos((Item)a, NArestasOrdenadas, NumeroArestas - 1, NumeroArestasParaOrdenar + NArestasOrdenadas - 1, LessCusto);

		/* Cria a MST, pelos seguintes criterios: inserindo em primeiro lugar rotas mais baratas,
		 * sempre que estas nao fechem um ciclo, ou seja nao estejam ja no mesmo conjunto
		 * Utilizacao do problema da conetividade para representar os dados, e executar
		 * operacoes de procura e uniao entre os aeroportos */
		for (i = NArestasOrdenadas; i < NumeroArestasParaOrdenar + NArestasOrdenadas && *NumArestasMST < G->NumVertices - 1; i++) {
			if (!CWQU_Procura(a[i]->vertice1, a[i]->vertice2, id, &RaizUm, &RaizDois)) {
				CWQU_Uniao(a[i]->vertice1, a[i]->vertice2, sz, id, RaizUm, RaizDois);
				mst[(*NumArestasMST)++] = a[i];
				*CustoTotal += a[i]->custo;
			}
		}
		NArestasOrdenadas += NumeroArestasParaOrdenar;
		if (*NumArestasMST >= G->NumVertices - 1 || SaiNoFim)
			break;
		else
			NumeroArestasParaOrdenar = NumeroArestasMaximoMST - *NumArestasMST;
	}
	free(sz);
	free(id);
	return *mst;
}

/**********************************************************************************************
 * AlgoritmoPrimOtimizado()
 *
 * Argumentos: Aeroporto - ponteiro para tabela com cada aeroporto e a sua lista de adjacencias
 *			   mst - ponteiro para vetor de arestas onde sera colocado o backbone
 * 			   custoTotal - ponteiro para double com custo total do backbone
 * 			   a - ponteiro para vetor de arestas com todas as rotas do grafo
 * 			   NumArestasMST - ponteiro para inteiro com numero de arestas no backbone
 *
 * Retorna: Ponteiro para mst
 * Efeitos secundarios: (nenhum)
 *
 * Descricao:
 *   Calcula a arvore minima de suporte de um grafo ponderado utilizando o Algoritmo de Prim
 *   otimizado com a utilizacao de uma fila prioritaria (acervo)
 *********************************************************************************************/

Aresta *AlgoritmoPrimOtimizado(Grafo *Aeroporto, Aresta **mst, double *custoTotal, Aresta **a, int *NumArestasMST) {
	int v, w = 0, min = 0, indice = 0;
	link *t;
	double maxWT = INFINITO_DOUBLE;
	int *franja, *st, *FilaIndice;
    double *val;
    Aresta *aux = NULL;
    Heap *filaPrio;

	/* Inicializacoes de vetores auxiliares e de fila prioritaria */
	FilaIndice = (int *) malloc(Aeroporto->NumVertices * sizeof(int));
	st = (int *) malloc((Aeroporto->NumVertices + 1) * sizeof(int));
	franja = (int *) malloc(Aeroporto->NumVertices * sizeof(int));
	val = (double *) malloc((Aeroporto->NumVertices + 2) * sizeof(double));

	filaPrio = InitHeap(Aeroporto->NumVertices);

    for (v = 1; v <= Aeroporto->NumVertices; v++) {
        st[v - 1] = -1; franja[v - 1] = v; val[v - 1] = maxWT; FilaIndice[v - 1] = -1;
        /* Insere inicialmente no heap todos os vertices */
        InsertInHeap(v, val, FilaIndice, filaPrio);
    }
    st[Aeroporto->NumVertices] = 0;
    val[Aeroporto->NumVertices + 1] = -1;
	min = (Aeroporto->NumVertices + 2); st[0] = 1; val[Aeroporto->NumVertices] = maxWT;
	for (v = retiraMaiorPrio(filaPrio, FilaIndice, val) ; val[min - 1] != maxWT; st[v - 1] = franja[v - 1]) {
		/* Percorre a lista de adjacencias do elemento mais prioritario */
		for (t = Aeroporto->adj[v - 1], min = (Aeroporto->NumVertices + 1); t != NULL; t = t->next) {
			w = t->vertice;
			if (st[w - 1] == -1) {
				if ((t->custo) < val[w - 1]) {
                        val[w - 1] = t->custo; franja[w - 1] = v;
                        /* Como o val diminui a prioridade do vertice tambem diminuiu,
                         * logo tem de se alterar a sua posicao na fila prioritaria */
                        alteraPrio(FilaIndice, filaPrio, val, w);
                }
            }
		}

		/* Caso a fila prioritaria nao esteja vazia, retira-se desta o vertice com menor val (mais prioritario) */
        if (!(FilaPrioVazia(filaPrio))) {
            min = retiraMaiorPrio(filaPrio, FilaIndice, val);
        }

		/* Cria nova aresta e insere-a na mst */
		if (val[min - 1] != maxWT) {
            aux = CriaAresta(franja[min - 1], min, val[min - 1]);
            indice = ProcuraBinaria((Item) a, 0, Aeroporto->NumArestas - 1, (Item)aux, LessAresta, EqualAresta);
			mst[(*NumArestasMST)++] = a[indice];
            free(aux);
			*custoTotal += val[min - 1];
			v = min;
		}

		/* Caso o elemento mais prioritario esteja desconetado dos anteriormente inseridos
		 * atualiza o seu val de modo a que na proxima iteracao seja permitido (pela condicao
		 * no ciclo for), o varrimento da sua lista de adjacencias : caso excecao de um
		 * grafo constituido por 2 arvores nao conectadas */
		if (val[min - 1] == maxWT) {
            if (st[min -1] == -1) {
                val[min - 1] = maxWT -1;
                franja[min -1] = min;
                v = min;
            }
		}
	}

	/* Liberta memoria auxiliar utilizada */
	free(franja);
	free(st);
	free(val);
	LibertaHeap(filaPrio);
	free(FilaIndice);

	return *mst;
}

/******************************************************************************
 * FreeVetorArestas()
 *
 * Argumentos: a - ponteiro para o vetor de arestas
 * 			   N - inteiro com o tamanho do vetor de arestas
 *
 * Retorna:  (void)
 * Efeitos Secundarios: liberta o o vetor de arestas
 *
 * Descricao: liberta um vetor de arestas de N elementos
 *****************************************************************************/

void FreeVetorArestas(Aresta **a, int N) {
    int i = 0;
    if (a == NULL) return;
    for (i = 0; i < N; i++) {
		free(a[i]);
	}
    free(a);
    return;
}

/******************************************************************************
 * Vertice1Aresta()
 *
 * Argumentos: a - ponteiro para estrutura do tipo Aresta
 *
 * Retorna: inteiro com parametro vertice1 da estrutura Aresta
 * Efeitos Secundarios: nenhum
 *
 * Descricao: acesso a campo de uma aresta
 *****************************************************************************/

int Vertice1Aresta(Aresta *a) {
	return a->vertice1;
}

/******************************************************************************
 * Vertice2Aresta()
 *
 * Argumentos: a - ponteiro para estrutura do tipo Aresta
 *
 * Retorna:  inteiro com parametro vertice2 da estrutura Aresta
 * Efeitos Secundarios: nenhum
 *
 * Descricao: acesso a campo de uma aresta
 *****************************************************************************/

int Vertice2Aresta(Aresta *a) {
	return a->vertice2;
}

/******************************************************************************
 * CustoAresta()
 *
 * Argumentos: a - ponteiro para estrutura do tipo Aresta
 *
 * Retorna:  double com parametro custo da estrutura do tipo Aresta
 * Efeitos Secundarios: nenhum
 *
 * Descricao: acesso a campo de uma aresta
 *****************************************************************************/

double CustoAresta(Aresta *a) {
	return a->custo;
}

/*************************************************************************************************************************************************
 * RemocaoERotadesubstituicao()
 *
 * Argumentos: backbone - ponteiro para vetor de arestas
 * 			   NumeroArestasBackbone - inteiro que contem o numero de arestas do backbone
 * 			   VerticeAnaliseA, VerticeAnaliseB  - inteiros que representam aresta a remover do backbone original
 * 			   a - ponteiro para vetor de arestas que representa o grafo em analise
 * 			   Aeroporto - ponteiro para estrutura do tipo grafo que contem as listas de adjacencias, o numero de vertices e o numero de arestas
 * 			   resposta - ponteiro para inteiro que verificara se existe uma rota de substituicao
 * 			      		  (resposta na resolucao ao problema sera -1 se algum dos vertices nao pertencer ao grafo ou nao existe rota que
 * 						  restabeleca a conetividade, 0 se a rota interditada nao faz parte do backbone e 1 se existe rota de substituicao)
 *
 * Retorna: ponteiro para Rota de Substituicao (Estrutura do tipo Aresta)
 * Efeitos secundarios: Aloca memoria para uma estrutura do tipo Aresta
 *
 * Descricao: Interditando uma aresta, reposicao da conetividade original do backbone calculando a Rota de Substituicao mais barata
 ************************************************************************************************************************************************/

Aresta *RemocaoERotadesubstituicao(Aresta **backbone, int NumeroArestasBackbone, int VerticeAnaliseA, int VerticeAnaliseB, Aresta **a, Grafo *Aeroporto, int *resposta) {
	int min = -1, IndiceArestaRemovida = 0, NumeroArestas = Aeroporto->NumArestas, RaizUm = 0, RaizDois = 0;
	double salvaguardacusto = 0;
	int *sz, *id, i = 0, IndiceArestaRemovidaBackbone = 0;
	Aresta *arestainterditada, *RotaSubstituicao = NULL;

	/* Verificacao se os vertices e rota estao no grafo em analise */
    if (VerticeAnaliseA > Aeroporto->NumVertices || VerticeAnaliseA <= 0) {
		*resposta = -1;
		return RotaSubstituicao;
	}
	if (VerticeAnaliseB > Aeroporto->NumVertices || VerticeAnaliseB <= 0) {
		*resposta = -1;
		return RotaSubstituicao;
	}

	arestainterditada = CriaAresta(VerticeAnaliseA, VerticeAnaliseB, 0);

	/* Verifica se a rota existe */
	IndiceArestaRemovida = ProcuraBinaria((Item) a, 0, NumeroArestas - 1, (Item) arestainterditada, LessAresta, EqualAresta);
	if (IndiceArestaRemovida == -1) {
		*resposta = -1;
		free(arestainterditada);
		return RotaSubstituicao;
	}

	/* Verifica se aresta existe no backbone, se existir "esconde" essa
	 * aresta para fazer a conetividade das restantes */
	IndiceArestaRemovidaBackbone = ProcuraBinaria((Item) backbone, 0, NumeroArestasBackbone - 1, (Item) arestainterditada, LessAresta, EqualAresta);
	if (IndiceArestaRemovidaBackbone == -1) {
		*resposta = 0;
		free(arestainterditada);
		return RotaSubstituicao;
	}

	/* Salvaguarda o contexto da aresta removida */
	a[IndiceArestaRemovida]->vertice1 = 0;
	a[IndiceArestaRemovida]->vertice2 = 0;
	salvaguardacusto = a[IndiceArestaRemovida]->custo;
	a[IndiceArestaRemovida]->custo = INFINITO_DOUBLE;

	/* Estabelece a conetividade do backbone excepto aresta removida */
	sz = (int *) malloc((Aeroporto->NumVertices) * sizeof(int));
	id = (int *) malloc((Aeroporto->NumVertices) * sizeof(int));

	CWQU_InicializaConetividade(Aeroporto->NumVertices, id, sz);

	for (i = 0; i < NumeroArestasBackbone; i++) {
		if (!CWQU_Procura(backbone[i]->vertice1, backbone[i]->vertice2, id, &RaizUm, &RaizDois)) {
			CWQU_Uniao(backbone[i]->vertice1, backbone[i]->vertice2, sz, id, RaizUm, RaizDois);
		}
	}

	/* Pesquisa da melhor rota de substituicao */
	for (i = 0; i < NumeroArestas; i++) {
		/* Se aresta une 2 vertices desconetados */
		if (!CWQU_Procura(a[i]->vertice1, a[i]->vertice2, id, &RaizUm, &RaizDois)) {
			/* Se aresta assegura rota de substituicao */
			if ((CWQU_Procura(a[i]->vertice1, VerticeAnaliseA, id, &RaizUm, &RaizDois) && CWQU_Procura(a[i]->vertice2, VerticeAnaliseB, id, &RaizUm, &RaizDois)) ||
                (CWQU_Procura(a[i]->vertice2, VerticeAnaliseA, id, &RaizUm, &RaizDois) == 1 && CWQU_Procura(a[i]->vertice1, VerticeAnaliseB, id, &RaizUm, &RaizDois))) {
				/* Se temos a rota de substituicao mais barata */
				if (min == -1) min = i;
				else if (a[i]->custo < a[min]->custo) {
					min = i;
				}
			}
		}
	}

	/* Repoe o contexto da aresta removida */
	a[IndiceArestaRemovida]->vertice1 = VerticeAnaliseA;
	a[IndiceArestaRemovida]->vertice2 = VerticeAnaliseB;
	a[IndiceArestaRemovida]->custo = salvaguardacusto;

	/* Libertacao de memoria auxiliar utilizada */
	free(sz);
    free(id);
    free(arestainterditada);

	/* Caso em que nao existe rota de substituicao */
	if (min == -1) {
        *resposta = -1;
		return RotaSubstituicao;
	}

	/* Caso em que existe rota de substituicao */
	RotaSubstituicao = CriaAresta(a[min]->vertice1, a[min]->vertice2, a[min]->custo);
    *resposta = 1;
	return RotaSubstituicao;
}

/******************************************************************************
 * LessAresta()
 *
 * Argumentos: a, b - items a serem comparados
 * Retorna: inteiro com o resultado da comparacao
 * Efeitos Secundarios: nenhum
 *
 * Descricao: Implementa a comparacao de arestas por vertices da aresta
 *****************************************************************************/

int LessAresta(Item a, Item b) {
	if (((Aresta *) a)->vertice1 < ((Aresta *) b)->vertice1)
		return 1;
	else if (((Aresta *) a)->vertice1 > ((Aresta *) b)->vertice1)
		return 0;
	else {
		if (((Aresta *) a)->vertice2 < ((Aresta *) b)->vertice2)
            return 1;
        else if (((Aresta *) a)->vertice2 > ((Aresta *) b)->vertice2)
            return 0;
	}
	return 0;
}

/******************************************************************************
 * EqualAresta()
 *
 * Argumentos: a, b - items abstratos a serem comparados
 * Retorna: inteiro com o resultado da comparacao
 * Efeitos Secundarios: nenhum
 *
 * Descricao: Implementa a comparacao de arestas
 *****************************************************************************/

int EqualAresta(Item a, Item b) {
	if (((Aresta *) a)->vertice1 == ((Aresta *) b)->vertice1 && ((Aresta *) a)->vertice2 == ((Aresta *) b)->vertice2)
		return 1;
	else
		return 0;
}

/**************************************************************************************************************************************************
 * RemocaoAeroporto()
 *
 * Argumentos: backbone - ponteiro para uma tabela onde estao as arestas do backbone
 *			   NumeroArestasBackbone - inteiro com o numero de arestas do backbone
 * 			   VerticeAnaliseA - inteiro com o vertice a ser removido do backbone
 * 			   RotaSubstituicao - ponteiro para vetor de arestas onde serao colocadas as rotas de substituicao
 * 			   VetorArestas - ponteiro para vetor de arestas que representa o grafo
 * 			   Aeroporto - ponteiro para estrutura do tipo grafo que contem as listas de adjacencias, o numero de vertices e o numero de arestas
 * 			   resposta - ponteiro para inteiro que indicara se ha solucao
 * 			   NumeroRotasSubstituicao - ponteiro para inteiro que vai recebe numero de rotas de substituicao
 * 			   CustoTotalRotasSubstituicao - ponteiro para double que vai receber o custo das rotas de substituicao
 *
 * Retorna: Ponteiro para Vetor de Rotas de Substituicao
 * Efeitos secundarios: Aloca memoria para vetor a ser retornado
 *
 * Descricao: Calcula as rotas de substituicao que restabelecem dentro das possibilidades
 * 			  a conetividade do conjunto original ao ser removido um vertice, faz este processo
 * 			  priviligiando sempre o menor custo possivel
 ************************************************************************************************************************************************/

Aresta **RemocaoAeroporto(Aresta **backbone, int NumeroArestasBackbone, int VerticeAnaliseA, Aresta **RotaSubstituicao, Aresta **VetorArestas,
						  Grafo *Aeroporto, int *resposta, int *NumeroRotasSubstituicao, double *CustoTotalRotasSubstituicao) {
	int i = 0, j = 0, NumeroCandidatas = 0, NumeroArestasRemovidas = 0, *indice = NULL, RaizUm = 0, RaizDois = 0, NumeroArestas = 0, min = 0;
	double *salvaguardacusto = NULL;
	int *SalvaguardaVertice2 = NULL, *SalvaguardaVertice1 = NULL, *id = NULL, *sz = NULL, *VerticesAUnir = NULL;
	Aresta **CandidataRotaSubstituicao = NULL;

	/* Verificacao se o vertice esta no grafo em analise */
    if (VerticeAnaliseA > Aeroporto->NumVertices || VerticeAnaliseA <= 0) {
		*resposta = -1;
		return RotaSubstituicao;
	}

	/* Anula e salvaguarda as arestas que utilizam o vertice em analise */
    indice = (int *) calloc(NumeroArestasBackbone, sizeof(int));
    salvaguardacusto = (double *) malloc (NumeroArestasBackbone * sizeof(double));
	SalvaguardaVertice2 = (int *) malloc (NumeroArestasBackbone * sizeof(int));
    SalvaguardaVertice1 = (int *) malloc (NumeroArestasBackbone * sizeof(int));
    for (i = 0; i < NumeroArestasBackbone; i++) {
        if (backbone[i]->vertice1 == VerticeAnaliseA || backbone[i]->vertice2 == VerticeAnaliseA) {
            indice[NumeroArestasRemovidas++] = i;
            /* Vetor dos indices das arestas do backbone que desaparecem (sal salvaguardadas) */
            SalvaguardaVertice1[NumeroArestasRemovidas - 1] = backbone[i]->vertice1;
            backbone[i]->vertice1 = 0;
            SalvaguardaVertice2[NumeroArestasRemovidas - 1] = backbone[i]->vertice2;
            backbone[i]->vertice2 = 0;
            salvaguardacusto[NumeroArestasRemovidas - 1] = backbone[i]->custo;
            backbone[i]->custo = INFINITO_DOUBLE;
		}
    }

	if (!NumeroArestasRemovidas) {
        *resposta = 0;
        free(indice);
        free(salvaguardacusto);
        free(SalvaguardaVertice1);
        free(SalvaguardaVertice2);
        return RotaSubstituicao;
	}

    /* Estabelece uma conetividade para o backbone sem as arestas removidas */
	sz = (int *) malloc((Aeroporto->NumVertices) * sizeof(int));
	id = (int *) malloc((Aeroporto->NumVertices) * sizeof(int));
	CWQU_InicializaConetividade(Aeroporto->NumVertices, id, sz);
	for (i = 0; i < NumeroArestasBackbone; i++) {
		if (!CWQU_Procura(backbone[i]->vertice1, backbone[i]->vertice2, id, &RaizUm, &RaizDois)) {
			CWQU_Uniao(backbone[i]->vertice1, backbone[i]->vertice2, sz, id, RaizUm, RaizDois);
		}
	}

	/* Vertices que estavam conetados ao vertice removido perdem conetividade entre si
	 * esta tera que ser restabelecida, primeiro passo consiste em verificar quais sao esses vertices */
	VerticesAUnir = (int *) malloc (NumeroArestasRemovidas * sizeof(int));
	for (i = 0; i < NumeroArestasRemovidas; i++) {
		if (SalvaguardaVertice1[i] != VerticeAnaliseA)
			VerticesAUnir[i] = SalvaguardaVertice1[i];
		else
			VerticesAUnir[i] = SalvaguardaVertice2[i];
	}

    i = 0;
	for (i = 1; i < NumeroArestasRemovidas; i++) {
		int v = VerticesAUnir[i];
		j = i;
		while (j > 0 && v < VerticesAUnir[j - 1]) {
			VerticesAUnir[j] = VerticesAUnir[j - 1];
			j--;
		}
		VerticesAUnir[j] = v;
    }

	/* Verificacao das melhores rotas de substituicao entre cada par de vertices a serem unidos,
	 * gerando uma serie de candidatos a serem rotas de substituicao para o conjunto */
	CandidataRotaSubstituicao = (Aresta **) malloc ((NumeroArestasRemovidas * NumeroArestasRemovidas / 2) * sizeof(Aresta *));
	NumeroArestas = Aeroporto->NumArestas;
    *NumeroRotasSubstituicao = NumeroArestasRemovidas;
    QuickSort((Item)VetorArestas, 0, Aeroporto->NumArestas - 1, LessArestaCustoOrdenacao);
	for (i = 0; i < NumeroArestasRemovidas; i++) {
		for (j = i + 1; j < NumeroArestasRemovidas; j++) {
			min = PesquisaMelhorRotaSubstituicao(VetorArestas, id, sz, NumeroArestas, VerticesAUnir[i], VerticesAUnir[j]);
			if (min >= 0 && min < NumeroArestas) {//vertice analise a e b de todas as salvaguarda vertice que e dif de verticeanalisea
				CandidataRotaSubstituicao[NumeroCandidatas++] = CriaAresta(VetorArestas[min]->vertice1, VetorArestas[min]->vertice2, VetorArestas[min]->custo);
			}
		}
    }

    /* Depois de determinar todas as possiveis todas de substituicao, criar a mst das rotas de substituicao,
     * semelhante ao algoritmo de kruskal */
	QuickSort((Item)CandidataRotaSubstituicao, 0, NumeroCandidatas - 1, LessCusto);
	for (i = 0, *NumeroRotasSubstituicao = 0; i < NumeroCandidatas && *NumeroRotasSubstituicao < Aeroporto->NumVertices - 1; i++) {
		if (!CWQU_Procura(CandidataRotaSubstituicao[i]->vertice1, CandidataRotaSubstituicao[i]->vertice2, id, &RaizUm, &RaizDois)) {
			CWQU_Uniao(CandidataRotaSubstituicao[i]->vertice1, CandidataRotaSubstituicao[i]->vertice2, sz, id, RaizUm, RaizDois);
			RotaSubstituicao[(*NumeroRotasSubstituicao)++] = CandidataRotaSubstituicao[i];
			*CustoTotalRotasSubstituicao += CandidataRotaSubstituicao[i]->custo;
		}
		else
            free(CandidataRotaSubstituicao[i]);
	}

	/* Repoe o contexto original */
	for (i = 0; i < NumeroArestasRemovidas; i++) {
		backbone[indice[i]]->vertice1 = SalvaguardaVertice1[i];
		backbone[indice[i]]->vertice2 = SalvaguardaVertice2[i];
		backbone[indice[i]]->custo = salvaguardacusto[i];
	}

	/* Liberta memoria associada aos vetores auxiliares utilizados */
	free(indice);
    free(salvaguardacusto);
	free(SalvaguardaVertice2);
    free(SalvaguardaVertice1);
    free(VerticesAUnir);
	free(CandidataRotaSubstituicao);
	free(sz);
	free(id);

    *resposta = 1;
	return RotaSubstituicao;
}

/*****************************************************************************************************************
 * PesquisaMelhorRotaSubstituicao()
 *
 * Argumentos: a - ponteiro para vetor de arestas que representa o grafo
 * 			  id, sz - vetores que contem informacao sobre a conetividade do conjunto onde vai ser feita a procura
 * 			  NArestas - inteiro que representa o tamanho do vetor de arestas
 * 			  VerticeAnaliseA, VerticeAnaliseB - inteiros representando a aresta a substituir
 *
 * Retorna: inteiro com o indice onde se encontra melhor rota de substituicao no vetor de arestas a
 * Efeitos Secundarios: nenhum
 *
 * Descricao: Pesquisa no vetor de arestas todas as rotas de substituicao possiveis,
 *            guardando o indice da que contem o menor custo
 ******************************************************************************************************************/

int PesquisaMelhorRotaSubstituicao(Aresta **a, int *id, int *sz, int NArestas, int VerticeAnaliseA, int VerticeAnaliseB) {
    int i = 0, min = -1, RaizUm = 0, RaizDois = 0;
    for (i = 0; i < NArestas; i++) {
        /* Se aresta une 2 vertices desconetados */
        if (!CWQU_Procura(a[i]->vertice1, a[i]->vertice2, id, &RaizUm, &RaizDois)) {
            /* Se aresta assegura rota de substituicao */
            if ((CWQU_Procura(a[i]->vertice1, VerticeAnaliseA, id, &RaizUm, &RaizDois) && CWQU_Procura(a[i]->vertice2, VerticeAnaliseB, id, &RaizUm, &RaizDois)) ||
                (CWQU_Procura(a[i]->vertice2, VerticeAnaliseA, id, &RaizUm, &RaizDois) == 1 && CWQU_Procura(a[i]->vertice1, VerticeAnaliseB, id, &RaizUm, &RaizDois))) {
                /* Se temos a rota de substituicao mais barata */
                return i;
            }
        }
    }
    return min;
}

/*****************************************************************************************************************
 * CriaBackbone2()
 *
 * Argumentos: backbone - ponteiro para vetor de arestas onde se encontra o backbone original
 * 			   backbone2Aresta - ponteiro para backbone de substituicao a ser ordenado por aresta
 * 			   RotaSubstituicao- ponteiro para vetor de arestas que contera as rotas de substituicao
 * 			   NumArestasBackbone - inteiro com numero de aresta no backbone respetivo
 *			   VerticeAnaliseA, VerticeAnaliseB - inteiros representando a aresta interditada
 * 			   Aeroporto - ponteiro para grafo a ser analisado
 *
 * Retorna: (void)
 * Efeitos Secundarios: nenhum
 *
 * Descricao: Cria uma nova backbone sem a aresta interditada
******************************************************************************************************************/

void CriaBackbone2(Aresta **backbone, Aresta **backbone2Aresta, Aresta **RotaSubstituicao, int NumeroArestasBackbone,
                    int VerticeAnaliseA, int VerticeAnaliseB, Grafo *Aeroporto) {
    int IndiceArestaRemovida = -1, i = 0;
    Aresta *arestainterdita = NULL, *aresta2 = NULL;

    /* Pesquisa da aresta interditada no backbone original */
    arestainterdita = CriaAresta(VerticeAnaliseA, VerticeAnaliseB, 0);
    IndiceArestaRemovida = ProcuraBinaria((Item) backbone, 0, NumeroArestasBackbone - 1, (Item) arestainterdita, LessAresta, EqualAresta);
    free(arestainterdita);

    /* Copia backbone original para backbone final */
    for (i = 0; i < NumeroArestasBackbone; i++) {
        aresta2 = CriaAresta(backbone[i]->vertice1, backbone[i]->vertice2, backbone[i]->custo);
        backbone2Aresta[i] = aresta2;
    }

    /* Troca a aresta interditada pela rota de substituicao */
    AlteraValoresBackbone(backbone2Aresta, RotaSubstituicao[0], IndiceArestaRemovida);

    insertionsort((Item)backbone2Aresta, 0, NumeroArestasBackbone - 1, LessAresta);
    free(RotaSubstituicao[0]);

    return;
}

/******************************************************************************
 * MaxArestaCustoOrdenacao()
 *
 * Argumentos: a, b - items a serem comparados
 *
 * Retorna: inteiro com o resultado da comparacao
 * Efeitos Secundarios: nenhum
 *
 * Descricao: Implementa a comparacao de arestas pelo seu custo, e caso
 * tenham igual custo, compara por vertice
 *****************************************************************************/

int MaxArestaCustoOrdenacao(Item a, Item b) {
    if (((Aresta *) a)->custo > ((Aresta *) b)->custo)
        return 1;
    else if (((Aresta *) a)->custo < ((Aresta *) b)->custo)
        return 0;
    else
        return LessAresta(a, b);
}

/******************************************************************************
 * LessArestaCustoOrdenacao()
 *
 * Argumentos: a, b - items a serem comparados
 *
 * Retorna: inteiro com o resultado da comparacao
 * Efeitos Secundarios: nenhum
 *
 * Descricao: Implementa a comparacao de arestas pelo seu custo, e caso
 * tenham igual custo, compara por vertice
 *****************************************************************************/

int LessArestaCustoOrdenacao(Item a, Item b) {
    if (((Aresta *) a)->custo > ((Aresta *) b)->custo)
        return 0;
    else if (((Aresta *) a)->custo < ((Aresta *) b)->custo)
        return 1;
    else
        return LessAresta(a, b);
}

/******************************************************************************
 * EqualCustoeVertices ()
 *
 * Argumentos: a, b - items a serem comparados
 *
 * Retorna: inteiro com o resultado da comparacao
 * Efeitos Secundarios: nenhum
 *
 * Descricao: Verifica se 2 arestas sao iguais
 *****************************************************************************/

int EqualCustoeVertices (Item a, Item b) {
    if ((((Aresta *) a)->custo == ((Aresta *) b)->custo) && (((Aresta *) a)->vertice1 == ((Aresta *) b)->vertice1) && (((Aresta *) a)->vertice2 == ((Aresta *) b)->vertice2))
        return 1;
    else
        return 0;
}

/******************************************************************************************
 * CriaVetorArestasNovo()
 *
 * Argumentos: VetorAresta2 - ponteiro para vetor de arestas auxiliar
 * 			   VetorArestas - ponteiro para vetor de arestas com todas as rotas do grafo
 * 			   NumArestas - inteiro com numero de arestas no grafo
 *
 * Retorna: (void)
 * Efeitos Secundarios: nenhum
 *
 * Descricao: Copia o vetor de arestas para vetor de arestas auxiliar
 *****************************************************************************************/

void CriaVetorArestasNovo(Aresta **VetorAresta2, Aresta **VetorArestas, int NumArestas) {
    int i = 0;
    for (i = 0; i < NumArestas; i++) {
        VetorAresta2[i] = VetorArestas[i];
    }
    return;
}

/********************************************************************************************
 * CalculaCustoBackbone()
 *
 * Argumentos: backbone - ponteiro para vetor de arestas onde se encontra o backbone original
 * 			   NumArestasBackbone - inteiro com numero de arestas no backbone
 *
 * Retorna: double com o custo do backbone
 * Efeitos Secundarios: nenhum
 *
 * Descricao: Calcula a soma dos custos de cada aresta do backbone
 *******************************************************************************************/

double CalculaCustoBackbone(Aresta **backbone, int NumeroArestasBackbone) {
    int i = 0;
    double custo = -1;
    if (backbone != NULL) {
        custo = 0;
        for (i = 0; i < NumeroArestasBackbone; i++) {
            custo += backbone[i]->custo;
        }
    }
    return custo;
}

/******************************************************************************
 * AlteraValoresBackbone()
 *
 * Argumentos: backbone - ponteiro para vetor de arestas
 * 			   RotaSubstituicao - ponteiro para aresta a inserir no backbone
 * 			   Indice - inteiro que representa o indice onde vai ocorrer a alteracao no backbone2
 *
 * Retorna: (void)
 * Efeitos Secundarios: nenhum
 *
 * Descricao: Escreve a Rota de Substituicao no indice do backbone
 *****************************************************************************/

void AlteraValoresBackbone(Aresta **backbone, Aresta *RotaSubstituicao, int Indice) {
    backbone[Indice]->vertice1 = RotaSubstituicao->vertice1;
    backbone[Indice]->vertice2 = RotaSubstituicao->vertice2;
    backbone[Indice]->custo = RotaSubstituicao->custo;
    return;
}

/*************************************************************************************************************************************************
 * RemocaoERotadesubstituicaoE1()
 *
 * Argumentos: backbone - ponteiro para vetor de arestas
 * 			   NumeroArestasBackbone - inteiro que contem o numero de arestas do backbone
 * 			   VerticeAnaliseA, VerticeAnaliseB  - inteiros que representam aresta a remover do backbone original
 * 			   a - ponteiro para vetor de arestas que representa o grafo em analise
 * 			   Aeroporto - ponteiro para estrutura do tipo grafo que contem as listas de adjacencias, o numero de vertices e o numero de arestas
 * 			   resposta - ponteiro para inteiro que verificara se existe uma rota de substituicao
 * 			      		  (resposta na resolucao ao problema sera -1 se algum dos vertices nao pertencer ao grafo ou nao existe rota que
 * 						  restabeleca a conetividade, 1 se existe rota de substituicao)
 *			   custo - double com o custo da aresta interditada
 *
 * Retorna: ponteiro para Rota de Substituicao (Estrutura do tipo Aresta)
 * Efeitos secundarios: Aloca memoria para uma estrutura do tipo Aresta
 *
 * Descricao: Interditando uma aresta, reposicao da conetividade original do backbone calculando a Rota de Substituicao mais barata
 ************************************************************************************************************************************************/

Aresta *RemocaoERotadesubstituicaoE1(Aresta **backbone, int NumeroArestasBackbone, int VerticeAnaliseA, int VerticeAnaliseB, Aresta **a, Grafo *Aeroporto, int *resposta, double custo) {
	int IndiceArestaRemovida = 0, NumeroArestas = Aeroporto->NumArestas, RaizUm = 0, RaizDois = 0;
	double salvaguardacusto = 0;
	int *sz, *id, i = 0;
	Aresta *arestainterditada, *RotaSubstituicao = NULL;

	arestainterditada = CriaAresta(VerticeAnaliseA, VerticeAnaliseB, custo);

	/* Verifica se a rota existe */
	IndiceArestaRemovida = ProcuraBinaria((Item) a, 0, NumeroArestas - 1, (Item) arestainterditada, LessArestaCustoOrdenacao, EqualCustoeVertices);
	if (IndiceArestaRemovida == -1) {
		*resposta = -1;
		free(arestainterditada);
		return RotaSubstituicao;
	}

	/* Salvaguarda o contexto da aresta removida */
	a[IndiceArestaRemovida]->vertice1 = 0;
	a[IndiceArestaRemovida]->vertice2 = 0;
	salvaguardacusto = a[IndiceArestaRemovida]->custo;
	a[IndiceArestaRemovida]->custo = INFINITO_DOUBLE;

	/* Estabelece a conetividade do backbone excepto aresta removida */
	sz = (int *) malloc((Aeroporto->NumVertices) * sizeof(int));
	id = (int *) malloc((Aeroporto->NumVertices) * sizeof(int));

	CWQU_InicializaConetividade(Aeroporto->NumVertices, id, sz);

	for (i = 0; i < NumeroArestasBackbone; i++) {
		if (!CWQU_Procura(backbone[i]->vertice1, backbone[i]->vertice2, id, &RaizUm, &RaizDois)) {
			CWQU_Uniao(backbone[i]->vertice1, backbone[i]->vertice2, sz, id, RaizUm, RaizDois);
		}
	}

	/* Pesquisa da melhor rota de substituicao */
	for (i = 0; i < NumeroArestas; i++) {
		/* Se aresta une 2 vertices desconetados */
		if (!CWQU_Procura(a[i]->vertice1, a[i]->vertice2, id, &RaizUm, &RaizDois)) {
			/* Se aresta assegura rota de substituicao */
			if ((CWQU_Procura(a[i]->vertice1, VerticeAnaliseA, id, &RaizUm, &RaizDois) && CWQU_Procura(a[i]->vertice2, VerticeAnaliseB, id, &RaizUm, &RaizDois)) ||
                (CWQU_Procura(a[i]->vertice2, VerticeAnaliseA, id, &RaizUm, &RaizDois) == 1 && CWQU_Procura(a[i]->vertice1, VerticeAnaliseB, id, &RaizUm, &RaizDois))) {
				/* Se temos a rota de substituicao mais barata */
				/* Repoe o contexto da aresta removida */
                a[IndiceArestaRemovida]->vertice1 = VerticeAnaliseA;
                a[IndiceArestaRemovida]->vertice2 = VerticeAnaliseB;
                a[IndiceArestaRemovida]->custo = salvaguardacusto;

                /* Libertacao de memoria auxiliar utilizada */
                free(sz);
                free(id);
                free(arestainterditada);
				RotaSubstituicao = CriaAresta(a[i]->vertice1, a[i]->vertice2, a[i]->custo);
                *resposta = 1;
                return RotaSubstituicao;
			}
		}
	}

	/* Repoe o contexto da aresta removida */
	a[IndiceArestaRemovida]->vertice1 = VerticeAnaliseA;
	a[IndiceArestaRemovida]->vertice2 = VerticeAnaliseB;
	a[IndiceArestaRemovida]->custo = salvaguardacusto;

	/* Libertacao de memoria auxiliar utilizada */
	free(sz);
    free(id);
    free(arestainterditada);
	/* Caso em que nao existe rota de substituicao */

    *resposta = -1;
    return RotaSubstituicao;
}
