/******************************************************************************
 * (h) Rui Daniel, Afonso Alemao
 * Ultima Modoficacao: 05/12/20
 *
 * Nome:
 *   airroutes.h
 *
 * Descricao:
 *   Prototipos de funcoes a utilizar no projeto airroutes
 *   Envolvendo leitura e escrita com ficheiros, conversao de tipos
 *   e alocacoes de memoria para tipos de dados simples
 *
 * Comentarios: Prototipos ordenados alfabeticamente
 ******************************************************************************/

#ifndef _AIRROUTES_H
#define _AIRROUTES_H

#include "grafo.h"

FILE *AbreFicheiro(char *nome, char *modo);
char **alocarvetor2d(int linhas, int colunas);
int CodificaModo(char *modo);
char *DescodificaModo(int codigo);
void EscreveBackbone(Aresta **vetor, int N, FILE *fp);
void EscreveBackboneEBackup(Aresta **backbone, int NumArestasBackbone, int *resposta, Aresta **RotaSubstituicao,
							Aresta **VetorAresta, Grafo *Aeroporto, FILE *fp, int *NumeroRotasSubstituicao);
void EscreveNovaLinhaFicheiroSaida(FILE *fp);
void EscreveFicheiroSaida_ModosX1(int NumVertices, int NumArestas, int Modo, int VerticeAnaliseA, int VerticeAnaliseB,
								  int resposta, FILE *fp, Aresta **backbone, Aresta **backbone2, int NumArestasBackbone,
								  double CustoTotalBackbone, int NumArestasBackbone2, double CustoTotalBackbone2,
								  Aresta **RotaSubstituicao, Aresta **VetorAresta, Grafo *Aeroporto, int *NumeroRotasSubstituicao);
void EscreveFicheiroSaida_ModosX0(int NumVertices, int NumArestas, int Modo, int VerticeAnaliseA, int VerticeAnaliseB, int resposta, FILE *fp, double custo);
void erro();
void free2d(char** vetor, int linhas);
void Inicializacoes (double *custo, bool *TrocVertAnalise, int Modo, int *Indice, int *resposta, int *NumeroArestasBackbone2);
Grafo *LeituraFicheiroEntrada(int *NumVertices, int *NumArestas, int *Modo, int *VerticeAnaliseA, int *VerticeAnaliseB, FILE *fp, bool *TrocaVerticesAnalises);
int LeituraFicheiroEntradaCabecalho(int *NumVertices, int *NumArestas, int *Modo, int *VerticeAnaliseA, int *VerticeAnaliseB, FILE *fp, bool *TrocaVerticesAnalises);
Grafo *LeituraFicheiroEntradaFim(int *NumVertices, int *NumArestas, int *Modo, int *VerticeAnaliseA, int *VerticeAnaliseB, FILE *fp, bool *TrocaVerticesAnalises, Aresta **VetorArestas);
char *LeComponente (FILE *fp);
void LibertacoesMemoria(int Modo, Aresta **RotaSubstituicao, Aresta **backbone, Aresta **backbone2, Aresta **VetorArestas,
                        int NumArestas, int NumeroRotasSubstituicao, int NumeroArestasBackbone2, Grafo *Aeroporto, bool *flags,
                        int resposta, int NumVertices);
Aresta **ModoC1 (int VerticeAnaliseA, int VerticeAnaliseB, int *resposta, int NumVertices, int NumArestas, Aresta **VetorArestas, int *Indice,
                Aresta **backbone, Aresta **backbone2, double *custo2, Aresta **RotaSubstituicao, int NumeroArestasBackbone, int *NumeroArestasBackbone2,
                Grafo *Aeroporto);
char *NomeFicheiroEntradaValido(char *nome);
int ProcuraBinaria (Item a[], int l, int r, Item x, int (*less) (Item, Item), int (*equal) (Item, Item));
void Troca (int *a, int *b);

#endif
