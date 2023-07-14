/******************************************************************************
 * (c) Rui Daniel, Afonso Alemão
 * Ultima Modificacao: 05/12/20
 *
 * Nome:
 * 	 airroutes.c
 *
 * Descricao:
 *   Funcoes utilizadas ao longo do programa AirRoutes
 *	 Manipulacao de ficheiros, erros, gestao de memoria, codificacoes
 *   e descodificacoes de modos
 *
 * Comentarios:
 *
 ******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "airroutes.h"
#include "ordenacao.h"

/******************************************************************************
 * erro()
 *
 * Argumentos: (nenhum)
 *
 * Retorna: (void)
 * Efeitos secundarios: sai do programa
 *
 * Descricao:
 *   Se ocorrer um erro, termina o programa silenciosamente
 *****************************************************************************/

void erro() {
    exit(0);
}

/*************************************************************************************************************
 * EscreveFicheiroSaida_ModosX1()
 *
 * Argumentos: NumVertices - inteiro com numero de vertices no grafo
 * 			   NumArestas - inteiro com numero de arestas no grafo
 * 			   Modo - inteiro com modo a ser executado
 * 			   VerticeAnaliseA - inteiro com vertice a ser analisado
 * 			   VerticeAnaliseB - inteiro com segundo vertice a ser analisado, em certos modos
 *			   resposta - inteiro com a solucao do problema
 * 			   fp - ponteiro para ficheiro onde vao ser imprimidos os resultados
 * 			   backbone - ponteiro para vetor de arestas onde se encontra o backbone original
 * 			   backbone2 - ponteiro para vetor de arestas onde se encontra o backbone de substituicao
 * 			   NumArestasBackbone, NumArestasBackbone2 - inteiros com numero de arestas no backbone respetivo
 * 			   CustoTotalBackbone, CustoTotalBackbone2 - double com custo total do backbone respetivo
 * 			   RotaSubstituicao - ponteiro para vetor de arestas com as rotas de substituicao
 *			   VetorArestas - ponteiro para vetor de arestas com todas as rotas do grafo
 * 			   Aeroporto - ponteiro para grafo a ser analisado
 * 			   NumeroRotasSubstituicao - ponteiro para inteiro com numero de rotas de substituicao
 *
 * Retorna: (void)
 * Efeitos secundarios: (nenhum)
 *
 * Descricao:
 * 	 Envia os resultados do programa para um ficheiro
 ************************************************************************************************************/

void EscreveFicheiroSaida_ModosX1(int NumVertices, int NumArestas, int Modo, int VerticeAnaliseA, int VerticeAnaliseB,
								  int resposta, FILE *fp, Aresta **backbone, Aresta **backbone2, int NumArestasBackbone,
								  double CustoTotalBackbone, int NumArestasBackbone2, double CustoTotalBackbone2,
								  Aresta **RotaSubstituicao, Aresta **VetorAresta, Grafo *Aeroporto, int *NumeroRotasSubstituicao) {
	char *modo;
	int i = 0;
	modo = DescodificaModo(Modo);

	switch (Modo) {
		case 5:  /* -A1 */
			fprintf(fp, "%d %d %s %d %.2f\n", NumVertices, NumArestas, modo, NumArestasBackbone, CustoTotalBackbone);
			EscreveBackbone(backbone, NumArestasBackbone, fp);
			break;
		case 6:  /* -B1 */
			fprintf(fp, "%d %d %s %d %d %d %.2f %d\n", NumVertices, NumArestas, modo, VerticeAnaliseA, VerticeAnaliseB,
					 NumArestasBackbone, CustoTotalBackbone, resposta);
			EscreveBackbone(backbone, NumArestasBackbone, fp); /* Original */
			if (resposta == 1) {
				EscreveAresta(RotaSubstituicao[0], fp);
			}
			break;
		case 7:  /* -C1 */
            if (resposta == 2) {
                fprintf(fp, "%d %d %s %d %d %d %.2f -1\n", NumVertices, NumArestas, modo, VerticeAnaliseA, VerticeAnaliseB,
						NumArestasBackbone, CustoTotalBackbone);
                EscreveBackbone(backbone, NumArestasBackbone, fp);
            }
            else {
                fprintf(fp, "%d %d %s %d %d %d %.2f %d %.2f\n", NumVertices, NumArestas, modo, VerticeAnaliseA, VerticeAnaliseB,
						NumArestasBackbone, CustoTotalBackbone, NumArestasBackbone2, CustoTotalBackbone2);
                EscreveBackbone(backbone, NumArestasBackbone, fp);
                EscreveBackbone(backbone2, NumArestasBackbone2, fp);
            }
            break;
		case 8:  /* -D1 */
		    if (resposta == -1)
                *NumeroRotasSubstituicao = -1;
            else if(resposta == 0)
				*NumeroRotasSubstituicao = 0;
			fprintf(fp, "%d %d %s %d %d %.2f %d\n", NumVertices, NumArestas, modo, VerticeAnaliseA, NumArestasBackbone,
					CustoTotalBackbone, *NumeroRotasSubstituicao);
			EscreveBackbone(backbone, NumArestasBackbone, fp); /* Original */
			if (resposta == 1) {
				QuickSort((Item)RotaSubstituicao, 0, *NumeroRotasSubstituicao - 1, LessAresta);
				/* Escreve todas as rotas de substituicao */
                for (i = 0; i < *NumeroRotasSubstituicao; i++) {
					EscreveAresta(RotaSubstituicao[i], fp);
					/* Ultima libertacao vai ser feita em main.c */
					if (i > 0)
                        free(RotaSubstituicao[i]);
                }
            }
			break;
		case 9:  /* -E1 */
			fprintf(fp, "%d %d %s %d %.2f\n", NumVertices, NumArestas, modo, NumArestasBackbone, CustoTotalBackbone);
			EscreveBackboneEBackup(backbone, NumArestasBackbone, &resposta, RotaSubstituicao, VetorAresta,
								    Aeroporto, fp, NumeroRotasSubstituicao);
			break;
	}
	free(modo);
	return;
}

/*************************************************************************************************************
 * EscreveBackbone()
 *
 * Argumentos: vetor - ponteiro para vetor de arestas
 * 			   N - inteiro com o numero de arestas do backbone
 * 			   fp - ponteiro para ficheiro onde vao ser imprimidos os resultados
 *
 * Retorna: (void)
 * Efeitos secundarios: nenhum
 *
 * Descricao:
 * 	 Envia os resultados do programa para um ficheiro
 ***********************************************************************************************************/

void EscreveBackbone(Aresta **vetor, int N, FILE *fp) {
	int i = 0;
	for (i = 0; i < N; i++) {
		fprintf(fp, "%d %d %.2f\n", Vertice1Aresta(vetor[i]), Vertice2Aresta(vetor[i]), CustoAresta(vetor[i]));
	}
	return;
}

/*************************************************************************************************************
 * EscreveBackboneEBackup()
 *
 * Argumentos: backbone - ponteiro para vetor de arestas onde se encontra o backbone original
 *			   NumArestasBackbone - inteiros com numero de arestas no backbone respetivo
 * 			   resposta - pointeiro para inteiro com a solucao do problema
 * 			   RotaSubstituicao - ponteiro para vetor de arestas com as rotas de substituicao
 *			   VetorAresta - ponteiro para vetor de arestas com todas as rotas do grafo
 * 			   Aeroporto - ponteiro para grafo a ser analisado
 * 			   fp - ponteiro para ficheiro onde vao ser imprimidos os resultados
 * 			   NumeroRotaSubstituicao - ponteiro para inteiro contendo o numero de rotas de substituicao
 *
 * Retorna: (void)
 * Efeitos secundarios: nenhum
 *
 * Descricao:
 * 	 Envia os resultados do programa para um ficheiro
 ***********************************************************************************************************/

void EscreveBackboneEBackup(Aresta **backbone, int NumArestasBackbone, int *resposta, Aresta **RotaSubstituicao,
							Aresta **VetorAresta, Grafo *Aeroporto, FILE *fp, int *NumeroRotaSubstituicao) {
	int i = 0, v1 = 0, v2 = 0;
	double custo = 0;
    QuickSort((Item)VetorAresta, 0, NumeroArestas(Aeroporto) - 1, LessArestaCustoOrdenacao);
	for (i = 0; i < NumArestasBackbone ; i++){
        v1 = Vertice1Aresta(backbone[i]);
        v2 = Vertice2Aresta(backbone[i]);
        custo = CustoAresta(backbone[i]);
        /* Verifica se existe rota de substituicao e se existir guarda essa rota em RotaSubstituicao[0] */
		RotaSubstituicao[0] = RemocaoERotadesubstituicaoE1(backbone, NumArestasBackbone, v1, v2, VetorAresta, Aeroporto, resposta, custo);

		/* Imprime no ficheiro a rota no backbone original seguido do seu backup, que caso nao exista sera representado por -1 */
		if (*resposta == 1) {
            (*NumeroRotaSubstituicao)++;
			fprintf(fp, "%d %d %.2f %d %d %.2f\n", Vertice1Aresta(backbone[i]), Vertice2Aresta(backbone[i]), CustoAresta(backbone[i]),
                     Vertice1Aresta(RotaSubstituicao[0]), Vertice2Aresta(RotaSubstituicao[0]), CustoAresta(RotaSubstituicao[0]));
			 /* Ultima libertacao a ser feita em main.c */
			if (i < NumArestasBackbone - 1) free(RotaSubstituicao[0]);
		}
		else {
			fprintf(fp, "%d %d %.2f -1\n", Vertice1Aresta(backbone[i]), Vertice2Aresta(backbone[i]), CustoAresta(backbone[i]));
		}
	}

	return;
}

/*************************************************************************************************************
 * EscreveFicheiroSaida_ModosX0()
 *
 * Argumentos: NumVertices - inteiro com numero de vertices no grafo
 * 			   NumArestas - inteiro com numero de arestas no grafo
 * 			   Modo - inteiro com modo a ser executado
 * 			   VerticeAnaliseA - inteiro com vertice a ser analisado
 * 			   VerticeAnaliseB - inteiro com segundo vertice a ser analisado, em certos modos
 * 			   resposta - pointeiro para inteiro com a solucao do problema
 *			   fp - ponteiro para ficheiro onde vao ser imprimidos os resultados
 * 			   custo - double com o custo da aresta no modo B0
 *
 * Retorna: (void)
 * Efeitos secundarios: (nenhum)
 *
 * Descricao:
 * 	 Envia os resultados do programa para um ficheiro
 ************************************************************************************************************/

void EscreveFicheiroSaida_ModosX0(int NumVertices, int NumArestas, int Modo, int VerticeAnaliseA, int VerticeAnaliseB, int resposta, FILE *fp, double custo) {
	char *modo;
	modo = DescodificaModo(Modo);

	if (VerticeAnaliseB != -1) {
		/* Modo -B0 */
		if (resposta != -1) {
			fprintf(fp, "%d %d %s %d %d %.2f\n", NumVertices, NumArestas, modo, VerticeAnaliseA, VerticeAnaliseB, custo);
		}
		else {
			fprintf(fp, "%d %d %s %d %d %d\n", NumVertices, NumArestas, modo, VerticeAnaliseA, VerticeAnaliseB, resposta);
		}
		free(modo);
		return;
	}
	else {
		/* Restantes modos */
		fprintf(fp, "%d %d %s %d %d\n", NumVertices, NumArestas, modo, VerticeAnaliseA, resposta);
	}
	free(modo);
	return;
}

/*************************************************************************************************************
 * EscreveNovaLinhaFicheiroSaida()
 *
 * Argumentos: fp - ponteiro para ficheiro onde vao ser imprimidos os resultados
 *
 * Retorna: (void)
 * Efeitos secundarios: (nenhum)
 *
 * Descricao:
 * 	 Envia uma nova linha para um ficheiro
 ************************************************************************************************************/

void EscreveNovaLinhaFicheiroSaida(FILE *fp) {
	fprintf(fp, "\n");
	return;
}

/******************************************************************************
 * DescodificaModo()
 *
 * Argumentos: codigo - inteiro com o modo a ser descodificado
 *
 * Retorna: (char *) com modo descodificado
 * Efeitos secundarios: (nenhum)
 *
 * Descricao:
 *   Descodifica o modo
* 	 Chave : A0, B0, C0, D0, A1, B1, C1, D1, E1 -> 1, 2, 3, 4, 5, 6, 7, 8, 9
 *****************************************************************************/

char *DescodificaModo(int codigo) {
	char *modo = (char *) calloc(3, sizeof(char));
	if (modo == NULL) {erro();}
	strcpy(modo, "A0");
	switch (codigo) {
		case 1:
			break;
		case 2:
			modo[0] = 'B';
			break;
		case 3:
			modo[0] = 'C';
			break;
		case 4:
			modo[0] = 'D';
			break;
		case 5:
			modo[1] = '1';
			break;
		case 6:
			modo[0] = 'B';
			modo[1] = '1';
			break;
		case 7:
			modo[0] = 'C';
			modo[1] = '1';
			break;
		case 8:
			modo[0] = 'D';
			modo[1] = '1';
			break;
		case 9:
			modo[0] = 'E';
			modo[1] = '1';
			break;
    }
    return modo;
}

/******************************************************************************
 * CodificaModo()
 *
 * Argumentos: modo - string com modo a ser codificado
 *
 * Retorna:  inteiro com o modo codificado
 * Efeitos secundarios: (nenhum)
 *
 * Descricao:
 *   Codifica o modo
 * 	 Chave : A0, B0, C0, D0, A1, B1, C1, D1, E1 -> 1, 2, 3, 4, 5, 6, 7, 8, 9
 *****************************************************************************/

int CodificaModo(char *modo) {
	int codigo = 1;
	switch (modo[0]) {
		case 'A':
			break;
		case 'B':
			codigo = 2;
			break;
		case 'C':
			codigo = 3;
			break;
		case 'D':
			codigo = 4;
			break;
		case 'E':
			codigo = 5;
			break;
    }
    if (modo[1] == '1')
		codigo += 4;
    return codigo;
}

/******************************************************************************
 * Abreficheiro()
 *
 * Argumentos: nome - string com o nome do ficheiro a ser aberto
 * 			   modo - string com o modo de abertura do ficheiro
 *
 * Retorna:  ponteiro para ficheiro aberto
 * Efeitos secundarios: (nenhum)
 *
 * Descricao:
 *   Abre ficheiro recebido, e retorna ponteiro para este
 *****************************************************************************/

FILE *AbreFicheiro(char *nome, char *modo) {
   FILE *fp;

   fp = fopen(nome, modo);
   if (fp == NULL) {
      erro();
   }
   return fp;
}

/******************************************************************************
 * NomeFicheiroEntradaValido()
 *
 * Argumentos: nome - string com o nome do ficheiro a ser validado
 *
 * Retorna: (char *) nome do ficheiro de entrada sem a extensao
 * Efeitos secundarios: memoria alocada para nome do ficheiro sem a extensao
 *
 * Descricao:
 *   Se o nome do ficheiro for invalido ocorre um erro e o programa termina
 *   Caso contrario, retorna nome do ficheiro sem a extensao
 *****************************************************************************/

char *NomeFicheiroEntradaValido(char *nome) {
    char *palavra = ".routes", *neww;
    int i = 0, tamanhoficheiro = 0, size = 0;

    tamanhoficheiro = strlen(nome);

    /* Ficheiro sem extensao */
    if (tamanhoficheiro < strlen(palavra)) {
		erro();
	}
	/* Verificacao da extensao */
	for (i = 0; i < strlen(palavra); i++){
		if (palavra[i] != nome[tamanhoficheiro - strlen(palavra) + i]) {
			erro();
		}
	}

    size = strlen(nome) - strlen(palavra);
    if (size <= 0) { return NULL; }

    /* Criacao nome ficheiro sem a extensao */
	neww = (char *) calloc((size + 1) , sizeof(char));
	for (i = 0; i < size; i++){
		neww[i] = nome[i];
	}
	return neww;
}

/******************************************************************************
 * LeComponente()
 *
 * Argumentos: fp - ponteiro para ficheiro a ser lido
 *
 * Retorna:  ponteiro para string lida
 * Efeitos secundarios: (nenhum)
 *
 * Descricao:
 *   Retorna ponteiro para buffer local com componente,
 *   ou NULL se a leitura do ficheiro falhar
 *****************************************************************************/

char *LeComponente (FILE *fp) {
  static char componente[7];       /* Forma de retornar variável não local */

  if (fscanf(fp, "%s", componente) == 1)
    return (componente);
  else
    return ((char *) NULL);
}


/*************************************************************************************************************
 * LeituraFicheiroEntrada()
 *
 * Argumentos: NumVertices - ponteiro para inteiro com o numero de vertices no grafo (a ser preenchido)
 * 			   NumArestas - ponteiro para inteiro numero de vertices no grafo (a ser preenchido)
 * 			   Modo - ponteiro para inteiro com o modo a ser executado (a ser preenchido)
 * 			   VerticeAnaliseA - ponteiro para inteiro com vertice a ser analisado (a ser preenchido)
 * 			   VerticeAnaliseB - ponteiro com inteiro com o segundo vertice a ser analisado (a ser preenchido)
 * 			   fp - ponteiro para ficheiro a ser lido
 *			   TrocaVerticesAnalise - ponteiro para bool que diz se os vertices em analise foram trocados
 *
 * Retorna:  ponteiro para estrutura de dados que contem o grafo: Representado por um vetor de listas de
 * 			 adjacencias
 * Efeitos secundarios: (nenhum)
 *
 * Descricao:
 * 	 Recolhe dados de 1 dos problemas no ficheiro de entrada
 *   Retorna ponteiro para estrutura de dados com o grafo,
 * 	 ou NULL no caso de já não haver problemas a resolver
 *   Escolhemos representar o grafo com uma lista de adjacencias
 ************************************************************************************************************/

Grafo *LeituraFicheiroEntrada(int *NumVertices, int *NumArestas, int *Modo, int *VerticeAnaliseA, int *VerticeAnaliseB, FILE *fp, bool *TrocaVerticesAnalises) {
	char modo[3];
	int v1 = 0, v2 = 0;
	int contadortamanhoproblema = 0, contador = 0;
	double custo = 0;
	Aresta *novarota;
	Grafo *aeroportos;

	/* Le cabecalho do problema
	* Modo A0, C0, D0, D1: 2 inteiros, modo e 1 inteiro
	* Modo B0, B1, C1: 2 inteiros, modo e 2 inteiros
	* Modo A1, E1: 2 inteiros e modo */

	if (fscanf(fp, "%d %d %s", NumVertices, NumArestas, modo) == EOF)
		return NULL;

    *Modo = CodificaModo(modo);
	if (*Modo == 2 || *Modo == 6 || *Modo == 7) {
		if (fscanf(fp, "%d %d", VerticeAnaliseA, VerticeAnaliseB) == EOF)
			return NULL;
		/* Coloca menor dos vertices em VerticeAnaliseA */
        if (*VerticeAnaliseA > *VerticeAnaliseB) {
			Troca(VerticeAnaliseA, VerticeAnaliseB);
			*TrocaVerticesAnalises = 1;
		}

	}
	else if (*Modo == 1 || *Modo == 3 || *Modo == 4 || *Modo == 8) {
		if (fscanf(fp, "%d", VerticeAnaliseA) == EOF)
			return NULL;
	}

	/* Criacao da estrutura do tipo Grafo, alocacao de memoria e preenchimento */
	aeroportos = InicializaGrafo(*NumVertices, *NumArestas);
	contadortamanhoproblema = *NumArestas;

	while (fscanf(fp, "%d %d %lf", &v1, &v2, &custo) != EOF) {
		contador++;
		novarota = CriaAresta(v1, v2, custo);
		InsereArestaGrafo(aeroportos, novarota);
		free(novarota);
		if (contador >= contadortamanhoproblema) {
			return aeroportos;
		}
	}

	/* Erro na leitura do ficheiro */
	return NULL;
}

/*************************************************************************************************************
 * LeituraFicheiroEntradaFim()
 *
 * Argumentos: NumVertices - ponteiro para inteiro com o numero de vertices no grafo (a ser preenchido)
 * 			   NumArestas - ponteiro para inteiro numero de vertices no grafo (a ser preenchido)
 * 			   Modo - ponteiro para inteiro com o modo a ser executado (a ser preenchido)
 * 			   VerticeAnaliseA - ponteiro para inteiro com vertice a ser analisado (a ser preenchido)
 * 			   VerticeAnaliseB - ponteiro com inteiro com o segundo vertice a ser analisado (a ser preenchido)
 * 			   fp - ponteiro para ficheiro a ser lido
 *			   TrocaVerticesAnalise - ponteiro para bool que diz se os vertices em analise foram trocados
 *
 * Retorna:  ponteiro para estrutura de dados que contem o grafo: Representado por um vetor de listas de
 * 			 adjacencias
 * Efeitos secundarios: (nenhum)
 *
 * Descricao:
 * 	 Recolhe dados de 1 dos problemas no ficheiro de entrada
 *   Retorna ponteiro para estrutura de dados com o grafo,
 * 	 ou NULL no caso de já não haver problemas a resolver
 *   Escolhemos representar o grafo com uma lista de adjacencias
 ************************************************************************************************************/

Grafo *LeituraFicheiroEntradaFim(int *NumVertices, int *NumArestas, int *Modo, int *VerticeAnaliseA, int *VerticeAnaliseB, FILE *fp, bool *TrocaVerticesAnalises, Aresta **VetorArestas) {
	int v1 = 0, v2 = 0;
	int contadortamanhoproblema = 0, contador = 0;
	double custo = 0;
	Aresta *novarota;
	Grafo *aeroportos;

	/* Criacao da estrutura do tipo Grafo, alocacao de memoria e preenchimento */
	aeroportos = InicializaGrafo(*NumVertices, *NumArestas);
	contadortamanhoproblema = *NumArestas;

	while (fscanf(fp, "%d %d %lf", &v1, &v2, &custo) != EOF) {
		contador++;
		novarota = CriaAresta(v1, v2, custo);
		InsereArestaGrafo(aeroportos, novarota);
		VetorArestas[contador - 1] = novarota;
		if (contador >= contadortamanhoproblema) {
			return aeroportos;
		}
	}

	/* Erro na leitura do ficheiro */
	return NULL;
}

/*************************************************************************************************************
 * LeituraFicheiroEntradaCabecalho()
 *
 * Argumentos: NumVertices - ponteiro para inteiro com o numero de vertices no grafo (a ser preenchido)
 * 			   NumArestas - ponteiro para inteiro numero de vertices no grafo (a ser preenchido)
 * 			   Modo - ponteiro para inteiro com o modo a ser executado (a ser preenchido)
 * 			   VerticeAnaliseA - ponteiro para inteiro com vertice a ser analisado (a ser preenchido)
 * 			   VerticeAnaliseB - ponteiro com inteiro com o segundo vertice a ser analisado (a ser preenchido)
 * 			   fp - ponteiro para ficheiro a ser lido
 *			   TrocaVerticesAnalise - ponteiro para bool que diz se os vertices em analise foram trocados
 *
 * Retorna:  ponteiro para estrutura de dados que contem o grafo: Representado por um vetor de listas de
 * 			 adjacencias
 * Efeitos secundarios: (nenhum)
 *
 * Descricao:
 * 	 Recolhe dados de 1 dos problemas no ficheiro de entrada
 *   Retorna ponteiro para estrutura de dados com o grafo,
 * 	 ou NULL no caso de já não haver problemas a resolver
 *   Escolhemos representar o grafo com uma lista de adjacencias
 ************************************************************************************************************/

int LeituraFicheiroEntradaCabecalho(int *NumVertices, int *NumArestas, int *Modo, int *VerticeAnaliseA, int *VerticeAnaliseB, FILE *fp, bool *TrocaVerticesAnalises) {
	char modo[3];

	/* Le cabecalho do problema
	* Modo A0, C0, D0, D1: 2 inteiros, modo e 1 inteiro
	* Modo B0, B1, C1: 2 inteiros, modo e 2 inteiros
	* Modo A1, E1: 2 inteiros e modo */

	if (fscanf(fp, "%d %d %s", NumVertices, NumArestas, modo) == EOF)
		return 1;

    *Modo = CodificaModo(modo);
	if (*Modo == 2 || *Modo == 6 || *Modo == 7) {
		if (fscanf(fp, "%d %d", VerticeAnaliseA, VerticeAnaliseB) == EOF)
			return 1;
		/* Coloca menor dos vertices em VerticeAnaliseA */
        if (*VerticeAnaliseA > *VerticeAnaliseB) {
			Troca(VerticeAnaliseA, VerticeAnaliseB);
			*TrocaVerticesAnalises = 1;
		}

	}
	else if (*Modo == 1 || *Modo == 3 || *Modo == 4 || *Modo == 8) {
		if (fscanf(fp, "%d", VerticeAnaliseA) == EOF)
			return 1;
	}
	return 0;
}

/******************************************************************************
 * alocarvetor2d()
 *
 * Argumentos: linhas, colunas - dimensoes da tabela a ser alocada
 *
 * Retorna: ponteiro para tabela (char **)
 * Efeitos secundarios: aloca memoria para tabela de char *
 *
 * Descricao:
 *   Alocacao de memoria para vetor bidimensional de char
 *****************************************************************************/

char **alocarvetor2d(int linhas, int colunas) {
    int i;
    char **vetor;
    if ((vetor = (char **) calloc(linhas, sizeof(char *))) == NULL) {
        erro();
    }
    for (i = 0; i < linhas; i++) {
        if ((vetor[i] = (char *) calloc(colunas, sizeof(char))) == NULL) {
            erro();
        }
    }
    return vetor;
}

/******************************************************************************
 * free2d()
 *
 * Argumentos: vetor - tabela de strings a ser desalocado
 * 			   linhas - dimensao da tabela a ser desalocada
 *
 * Retorna: (void)
 * Efeitos secundarios: liberta memoria para vetor 2d de char
 *
 * Descricao:
 *   Libertacao de memoria para vetor bidimensional de char
 *****************************************************************************/

void free2d(char **vetor, int linhas) {
    int i;
    for (i = 0; i < linhas; i++){
        free(vetor[i]);
    }
    free(vetor);
    return;
}

/**************************************************************************************
 * ProcuraBinaria ()
 *
 * Argumentos: a - tabela de items onde sera efetuada a procura
 *             l, r - limites da tabela
 *             x - elemento a pesquisar
 * 			   less, equal - funcoes com comparacoes a ser feitas para encontrar x em a
 * Retorna: inteiro com o indice onde x se encontra na tabela a,
 * 			ou -1 se nao for encontrado
 * Efeitos Secundarios: nenhum
 *
 * Descricao: caso encontre o elemento na tabela retorna indice, caso contrario -1
 * 			  O(lgN)
 **************************************************************************************/

int ProcuraBinaria (Item a[], int l, int r, Item x, int (*less) (Item, Item), int (*equal) (Item, Item)) {
	while (r >= l) {
		int m = (l + r) / 2;
		if (equal(x, a[m])) return m;
		if (less(x, a[m])) r = m - 1;
		else l = m + 1;
	}
	return -1;
}

/**************************************************************************************
 * Troca()
 *
 * Argumentos: a, b - ponteiros para elementos a serem trocados (inteiros)
 *
 * Retorna: (void)
 * Efeitos Secundarios: troca a com b
 *
 * Descricao: troca de elementos
 **************************************************************************************/

void Troca (int *a, int *b) {
	int temp = 0;
	temp = *a;
	*a = *b;
	*b = temp;
}


/*************************************************************************************************************
 * LibertacoesMemoria()
 *
 * Argumentos: Modo - inteiro com modo a ser executado
 * 			   RotaSubstituicao - ponteiro para vetor de arestas com as rotas de substituicao
 * 			   backbone - ponteiro para vetor de arestas onde se encontra o backbone original
 * 			   backbone2 - ponteiro para vetor de arestas onde se encontra o backbone de substituicao
 * 			   VetorArestas - ponteiro para vetor de arestas com todas as rotas do grafo
 * 			   NumArestas - inteiro com numero de arestas no grafo
 * 			   NumeroRotasSubstituicao - inteiro com numero de rotas de substituicao
 * 			   NumArestasBackbone2 - inteiro com numero de arestas no backbone2
 * 			   Aeroporto - ponteiro para grafo a ser analisado
 * 			   flags - ponteiro para vetor do tipo bool
 * 			   resposta - inteiro com a solucao do problema
 * 			   NumVertices - inteiro com numero de vertices no grafo
 *
 * Retorna: (void)
 * Efeitos secundarios: liberta memoria associada a cada modo
 *
 * Descricao:
 * 	 Liberta a memoria auxiliar alocada em cada um dos modos
 ************************************************************************************************************/

void LibertacoesMemoria(int Modo, Aresta **RotaSubstituicao, Aresta **backbone, Aresta **backbone2, Aresta **VetorArestas,
                        int NumArestas, int NumeroRotasSubstituicao, int NumeroArestasBackbone2, Grafo *Aeroporto, bool *flags,
                        int resposta, int NumVertices) {
    int i = 0;
    if (Modo == 3 || Modo == 4)
        free(flags);

    if (Modo > 4) {
        FreeVetorArestas(VetorArestas, NumArestas);
        free(backbone);
        if (Modo == 6 || Modo == 8 || Modo == 9) {
            if (NumeroRotasSubstituicao > 0)
                free(RotaSubstituicao[0]);
            free(RotaSubstituicao);
        }
    }
    if (Modo == 7) {
        if (resposta == 1) {
            for (i = 0; i < NumeroArestasBackbone2; i++) {
                free(backbone2[i]);
            }
            free(backbone2);
            free(RotaSubstituicao);
        }
        else if (resposta == -1){
            free(backbone2);
            free(RotaSubstituicao[0]);
            free(RotaSubstituicao);
        }
        else if (resposta == 0){
            free(RotaSubstituicao[0]);
            free(RotaSubstituicao);
        }
        else if (resposta == 2){
            free(RotaSubstituicao);
        }
    }
    FreeGrafo(Aeroporto);
}

/*************************************************************************************************************
 * Inicializacoes()
 *
 * Argumentos: custo - ponteiro para double com custo
 * 			   TrocaVertAnalise - ponteiro para bool que garante a ordem dos vertices em analise
 * 			   Modo - inteiro com modo a ser executado
 * 			   Indice - ponteiro para inteiro a ser utilizado em pesquisa
 * 			   resposta - ponteiro para inteiro com a solucao do problema
 * 			   NumArestasBackbone2, 3 - ponteiros para inteiros com numeros de arestas nos backbones
 *
 * Retorna: (void)
 * Efeitos secundarios: nenhum
 *
 * Descricao:
 * 	 Inicializacoes gerais
 ************************************************************************************************************/

void Inicializacoes (double *custo, bool *TrocaVertAnalise, int Modo, int *Indice, int *resposta, int *NumeroArestasBackbone2) {
	*custo = 0;
	*TrocaVertAnalise = 0;
	if (Modo == 7) {
		*Indice = 0;
		*resposta = -1;
		*NumeroArestasBackbone2 = 0;
	}
	return;
}

/*************************************************************************************************************
 * ModoC1()
 *
 * Argumentos: VerticeAnaliseA, VerticeAnalise B - inteiros que representam aresta interditada
 * 			   resposta - ponteiro para inteiro com a solucao do problema
 * 			   NumVertices - inteiro com numero de vertices no grafo
 * 			   NumArestas - inteiro com numero de arestas no grafo
 * 			   VetorArestas - ponteiro para vetor de arestas com todas as rotas do grafo
 * 			   Indice - ponteiro para inteiro a ser utilizado em pesquisa
 * 			   backbone - ponteiro para vetor de arestas onde se encontra o backbone original
 * 			   backbone2 - ponteiro para vetor de arestas onde se encontra o backbone de substituicao
 * 			   custo2 - ponteiro para double com custo do backbone2
 * 			   RotaSubstituicao - ponteiro para vetor de arestas com as rotas de substituicao
 * 			   NumArestasBackbone - inteiro com numero de arestas no backbone
 * 			   NumArestasBackbone2 - ponteiro para inteiro com numeros de aresta nos backbone
 * 			   Aeroporto - ponteiro para grafo a ser analisado
 *
 * Retorna: ponteiro para backbone2
 * Efeitos secundarios: aloca memoria para backbone2
 *
 * Descricao:
 * 	 Executa o modo C1
 ************************************************************************************************************/

Aresta **ModoC1 (int VerticeAnaliseA, int VerticeAnaliseB, int *resposta, int NumVertices, int NumArestas, Aresta **VetorArestas, int *Indice,
                Aresta **backbone, Aresta **backbone2, double *custo2, Aresta **RotaSubstituicao, int NumeroArestasBackbone, int *NumeroArestasBackbone2,
                Grafo *Aeroporto) {
    int i = 0;
    Aresta *aux = NULL, *aux1 = NULL, **VetorAresta2 = NULL;

    /* Verifica se vertices sao validos */
    if (VerticeAnaliseA > NumVertices || VerticeAnaliseA <= 0) {
        *resposta = 2;
        return NULL;
    }
    if (VerticeAnaliseB > NumVertices || VerticeAnaliseB <= 0) {
        *resposta = 2;
        return NULL;
    }
    aux1 = CriaAresta(VerticeAnaliseA, VerticeAnaliseB, 0);
    *Indice = ProcuraBinaria((Item)VetorArestas, 0, NumArestas - 1, aux1, LessAresta, EqualAresta);
    /* Verifica se a aresta se encontra no grafo */
    if (*Indice == -1) {
        *resposta = 2;
    }
    free(aux1);

    if (*resposta != 2) {
		/* Calcula uma rota de substituicao */
        RotaSubstituicao[0] = RemocaoERotadesubstituicao(backbone, NumeroArestasBackbone, VerticeAnaliseA,
                                                         VerticeAnaliseB, VetorArestas, Aeroporto, resposta);
        if (*resposta == 1) {
            *NumeroArestasBackbone2 = NumeroArestasBackbone;
            backbone2 = (Aresta **) malloc(*NumeroArestasBackbone2 * sizeof(Aresta*));

            /* Calcula backbone2 */
            CriaBackbone2(backbone, backbone2, RotaSubstituicao, NumeroArestasBackbone, VerticeAnaliseA, VerticeAnaliseB, Aeroporto);
            *custo2 = CalculaCustoBackbone(backbone2, *NumeroArestasBackbone2);
            free(VetorAresta2);
        }
        else if (*resposta == 0) {
			/* Quando a aresta nao existe no backbone */
            *resposta = 2;
            *custo2 = -1;
        }
        else if (*resposta == -1) {
            /* Quando nao existe rota de substituicao */
            backbone2 = (Aresta **)malloc(NumeroArestasBackbone * sizeof(Aresta*));
            aux = CriaAresta(VerticeAnaliseA, VerticeAnaliseB, 0);
            *Indice = ProcuraBinaria((Item)backbone, 0, NumeroArestasBackbone - 1, aux, LessAresta, EqualAresta);
            for (i = 0; i < NumeroArestasBackbone; i++) {
                if (i != *Indice) {
                    backbone2[(*NumeroArestasBackbone2)++] = backbone[i];
                }
            }
            free(aux);
            *custo2 = CalculaCustoBackbone(backbone2, *NumeroArestasBackbone2);
        }
    }

    return backbone2;
}
