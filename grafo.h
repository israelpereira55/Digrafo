#ifndef __GRAFO_H
#define __GRAFO_H

#include "linkedlist.h"

typedef struct grafo Grafo;

struct grafo{
	int V, A;
	List** adj; /* Lista de arcos (Arc). */
};

typedef struct arc Arc;

struct arc{
	int v, w;
	int peso;
};


Grafo* grafo_inicializa(int V);

Grafo* grafo_libera(Grafo* grafo);

void grafo_insere_arco(Grafo* grafo, int v, int w, int peso);

void grafo_remove_arco(Grafo* grafo, int v, int w);

void grafo_imprime(Grafo* grafo);


/* Imprime os adjacentes de entrada de um grafo, considerando um conjunto de 
   vértices.

    Entrada: grafo, 
	    vetor determinando os elementos do conjunto,
	    número de elementos do conjunto.
 */
void grafo_imprime_adjacentes_entrada(Grafo* grafo, int* conjunto, int n);


/* Imprime os adjacentes de saída de um grafo, considerando um conjunto de 
   vértices.

    Entrada: grafo, 
	    vetor determinando os elementos do conjunto,
	    número de elementos do conjunto.
 */
void grafo_imprime_adjacentes_saida(Grafo* grafo, int* conjunto, int num_elementos);


/* Imprime os vértices do grafo, determinando suas componentes.
 */
void grafo_componentes(Grafo* grafo);


/* Algorítmo básico de busca em largura implementado.
   É impresso no terminal a ordem de visitação dos vértices.
 */
void busca_largura(Grafo* grafo, int entrada);


/* Output: Árvore mínima.
 */
Grafo* kruskal(Grafo* grafo);

#endif
