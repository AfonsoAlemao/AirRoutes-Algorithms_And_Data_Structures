/**********************************************************************************
* (c) Rui Daniel, Afonso Alemao
* Ultima modificacao: 05/12/2020
*
* Nome:
*   main.c
*
* Descricao:
*   Programa Main para o Projeto AirRoutes AED IST 2020 1º Semestre
*   "<name>.routes" : ficheiro de entrada; "<name>.bbones" : ficheiro de saida
*   Opcoes "A0, B0, C0, D0, A1, B1, C1, D1, E1"
* 			A0 - Determina grau (numero de ligacoes) a um aeroporto
* 			B0 - Determina se 2 aeroportos possuem uma ligacao direta
* 			C0 - Determina se aeroporto se encontra num clique
* 			D0 - Determina em quantos cliques esta um aeroporto
* 			A1 - Determina backbone de um grafo
* 			B1 - Determina backbone original seguido de rota de substituicao,
* 		como resultado da interdicao de uma rota
* 			C1 - Determina backbone original seguido de novo backbone, como
* 		resultado da interditas de uma rota
* 			D1 - Determina backbone original seguido de rotas de substituicao,
* 		como resultado da interdicao de um aeroporto
* 			E1 - Determinacao de um backbone e do seu backup
*
* Comentarios:
*
**********************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "defs.h"
#include "grafo.h"
#include "airroutes.h"
#include "ordenacao.h"

/**********************************************************************************
 * main()
 *
 * Argumentos: argc - numero de argumentos inseridos pelo utilizador
 * 			   argv - ponteiro para tabela de argumentos inseridor pelo utilizador
 *
 * Retorna: inteiro (0)
 * Efeitos secundarios: no seu fim termina o programa
 *
 * Descricao:
 *   Abertura, leitura e processamento de ficheiros de entrada e saida
 *   Resolucao de problemas no ficheiro de entrada e producao da sua solucao
 *   para o ficheiro de saida.
 ********************************************************************************/

int main(int argc, char *argv[]) {
	char *terminacao = ".bbones", *NomeFicheiroEntrada, *NomeFicheiroSaida;
	int Modo = 1, NumeroArestasBackbone = 0, NumeroArestasBackbone2 = 0;
	int NumVertices = 0, NumArestas = 0, resposta = -1, Indice = 0;
	int VerticeAnaliseA = -1, VerticeAnaliseB = -1, NumeroRotasSubstituicao = 0;
	Grafo *Aeroporto = NULL;
	bool terminarprograma = 0, *flags = NULL, TrocaVertAnalise = 0;
    FILE *fpEntrada, *fpSaida;
    double custo = 0, custo2 = 0;
    Aresta **backbone = NULL, **backbone2 = NULL, **RotaSubstituicao = NULL, **VetorArestas = NULL;

    /* Deve ter 1 argumento depois do nome do executavel */
    if (argc != 2) {
        erro();
    }

    /* Validar, abrir ficheiro de entrada */
	NomeFicheiroEntrada = NomeFicheiroEntradaValido(argv[1]);
	if (NomeFicheiroEntrada == NULL) {
		erro();
	}
	fpEntrada = AbreFicheiro(argv[1], "r");

    /* Criacao de um ficheiro de saida que recebe a solucao ao problema proposto, e a sua abertura */
    NomeFicheiroSaida =  (char*) malloc((strlen(NomeFicheiroEntrada) + strlen(terminacao) + 1) * sizeof(char));
	if (NomeFicheiroSaida == NULL) {
		erro();
	}
	strcpy (NomeFicheiroSaida, NomeFicheiroEntrada);
	strcat (NomeFicheiroSaida, terminacao);
	fpSaida = AbreFicheiro(NomeFicheiroSaida, "w");

	/* Resolucao de problemas no ficheiro de entrada */
    while (!terminarprograma) {
		Inicializacoes (&custo, &TrocaVertAnalise, Modo, &Indice, &resposta, &NumeroArestasBackbone2);
		/* Ler o ficheiro de entrada para cada problema distinto */
		Aeroporto = LeituraFicheiroEntrada(&NumVertices, &NumArestas, &Modo, &VerticeAnaliseA, &VerticeAnaliseB, fpEntrada, &TrocaVertAnalise);
		if (Aeroporto == NULL) {
			/* Caso em que nao ha mais problemas a ser resolvidos */
			terminarprograma = 1;
			continue;
		}

		/* Alocacao de memoria auxiliares dependendo do Modo a ser resolvido */
		if (Modo == 3 || Modo == 4) {
			flags = (bool*) malloc(NumVertices * sizeof(bool));
		}

        if (Modo > 4) {
            NumeroArestasBackbone = 0;
            VetorArestas = (Aresta **) malloc (NumArestas * sizeof(Aresta*));
            backbone = (Aresta **) malloc((NumVertices - 1) * sizeof(Aresta*));
            *VetorArestas = CriaVetorArestas(VetorArestas, Aeroporto);
            QuickSort((Item)VetorArestas, 0, NumArestas - 1, LessAresta);
        }

        if (Modo == 8) {
			/* Modo D1, 0 < Numero de rotas de substituicao < Numero de Vertices do grafo */
            RotaSubstituicao = (Aresta **) malloc (NumVertices * sizeof(Aresta*));
            NumeroRotasSubstituicao = 0;
        }
        else if (Modo > 5) {
            NumeroRotasSubstituicao = 0;
			/* Modo B1, C1, E1, possivel trabalhar com uma rota de substituicao de cada vez */
            RotaSubstituicao = (Aresta **) malloc (sizeof(Aresta*));
            RotaSubstituicao[0] = NULL;
        }

		/* Executar modo selecionado */
		switch (Modo) {
			case 1:  /* -A0 */
				resposta = DeterminaGrau(Aeroporto, VerticeAnaliseA);
				break;
			case 2:  /* -B0 */
				resposta = VerticesSaoAdjacentes(Aeroporto, VerticeAnaliseA, VerticeAnaliseB, &custo);
				break;
			case 3:  /* -C0 */
				resposta = DetetaClique(Aeroporto, VerticeAnaliseA, flags);
				break;
			case 4:  /* -D0 */
				resposta = NumeroCliquesDistintos(Aeroporto, VerticeAnaliseA, flags);
				break;
			case 5:  /* -A1 */
				*backbone = CalculoBackbone(Aeroporto, &NumeroArestasBackbone, &custo, VetorArestas, backbone);
				QuickSort((Item)backbone, 0, NumeroArestasBackbone - 1, LessAresta);
				break;
			case 6:  /* -B1 */
				*backbone = CalculoBackbone(Aeroporto, &NumeroArestasBackbone, &custo, VetorArestas, backbone);
				QuickSort((Item)backbone, 0, NumeroArestasBackbone - 1, LessAresta);
				RotaSubstituicao[0] = RemocaoERotadesubstituicao(backbone, NumeroArestasBackbone, VerticeAnaliseA, VerticeAnaliseB, VetorArestas, Aeroporto, &resposta);
				if (resposta)
                    NumeroRotasSubstituicao++;
				break;
			case 7:  /* -C1 */
				*backbone = CalculoBackbone(Aeroporto, &NumeroArestasBackbone, &custo, VetorArestas, backbone);
                QuickSort((Item)backbone, 0, NumeroArestasBackbone - 1, LessAresta);
                backbone2 = ModoC1 (VerticeAnaliseA, VerticeAnaliseB, &resposta, NumVertices, NumArestas, VetorArestas, &Indice, backbone, backbone2, &custo2, RotaSubstituicao,
									NumeroArestasBackbone, &NumeroArestasBackbone2, Aeroporto);
				break;
			case 8:  /* -D1 */
                *backbone = CalculoBackbone(Aeroporto, &NumeroArestasBackbone, &custo, VetorArestas, backbone); /*para este modo nao e necessario a lista de adjacencias*/
                RotaSubstituicao = RemocaoAeroporto(backbone, NumeroArestasBackbone, VerticeAnaliseA, RotaSubstituicao, VetorArestas, Aeroporto, &resposta, &NumeroRotasSubstituicao, &custo2);
                QuickSort((Item)backbone, 0, NumeroArestasBackbone - 1, LessAresta);
				break;
			case 9:  /* -E1 */
				*backbone = CalculoBackbone(Aeroporto, &NumeroArestasBackbone, &custo, VetorArestas, backbone);
				QuickSort((Item)backbone, 0, NumeroArestasBackbone - 1, LessAresta);
				break;
		}
		if (TrocaVertAnalise)
			Troca(&VerticeAnaliseA, &VerticeAnaliseB);

		/* Apresentacao da solucao do problema no ficheiro de saida */
		if (Modo < 5)
			EscreveFicheiroSaida_ModosX0(NumVertices, NumArestas, Modo, VerticeAnaliseA, VerticeAnaliseB, resposta, fpSaida, custo);
		else
			EscreveFicheiroSaida_ModosX1(NumVertices, NumArestas, Modo, VerticeAnaliseA, VerticeAnaliseB, resposta, fpSaida, backbone, backbone2, NumeroArestasBackbone, custo,
                                NumeroArestasBackbone2, custo2, RotaSubstituicao, VetorArestas, Aeroporto, &NumeroRotasSubstituicao);
		EscreveNovaLinhaFicheiroSaida(fpSaida);

		/* Libertacoes de memoria tendo em conta o Modo resolvido */
		LibertacoesMemoria(Modo, RotaSubstituicao, backbone, backbone2, VetorArestas, NumArestas, NumeroRotasSubstituicao, NumeroArestasBackbone2, Aeroporto, flags,
						   resposta, NumVertices);

	}

	/* Fechar ficheiros e libertacao de memoria */
	fclose(fpEntrada);
	fclose(fpSaida);
	free(NomeFicheiroEntrada);
	free(NomeFicheiroSaida);

    exit(0);
}
