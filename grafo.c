#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grafo.h"
#include "linkedlist.h"

Grafo* grafo_inicializa(int V){
	Grafo* grafo = (Grafo*) malloc (sizeof(Grafo));
	
	grafo->V = V;
	grafo->A = 0;
	grafo->adj = (List**) malloc (V * sizeof(List*));;
	
	int i = 0;
	List** adj = grafo->adj;
	while(i < V){
		adj[i] = list_create();
		i++;
	}
	return grafo;
}

void* libera_conteudo(void *arco){
	free((int*) arco);
	return NULL;
}

Grafo* grafo_libera(Grafo* grafo){
	int num_vertices = grafo->V;
	List** adj = grafo->adj;

	int i = 0;
	while(i < num_vertices){
		adj[i] = list_complete_clear(adj[i], libera_conteudo);
		i++;
	}
	
	free(grafo->adj);
	free(grafo);
	return NULL;
}

Arc* arco_inicializa(int v, int w, int peso){
	Arc* a = (Arc*) malloc (sizeof(Arc));
	a->v = v;
	a->w = w;
	a->peso = peso;
	return a;
}

void grafo_insere_arco(Grafo* grafo, int v, int w, int peso){
	list_insert_last(grafo->adj[v], arco_inicializa(v, w, peso) );
	list_insert_last(grafo->adj[w], arco_inicializa(w, v, peso) );
	
	grafo->A++;
	return;
}

void grafo_remove_arco(Grafo* grafo, int v, int w){
	List* adj = grafo->adj[v];

	int tam = list_size(adj);
	if(tam == 0)
		return;
	
	Iterator* iterator = iterator_create(adj);
	
	int index = 0;
	int elemento = 0;
	while(index < tam){
		elemento = ((Arc*) iterator_get_current(iterator))->w;
			
		if(elemento == w){
			free((Arc*) list_remove(adj, index));
			break;
		}
		index++;
		iterator = iterator_next(iterator);
	}
	
	adj = grafo->adj[w];
	tam = list_size(adj);
	iterator = iterator_create(adj);
	
	index = 0;
	elemento = 0;
	while(index < tam){
		elemento = ((Arc*) iterator_get_current(iterator))->w;
			
		if(elemento == v){
			free((Arc*) list_remove(adj, index));
			break;
		}
		index++;
		iterator = iterator_next(iterator);
	}	
	
	grafo->A--;
	return;
}

void grafo_imprime(Grafo* grafo){
	int num_vertices = grafo->V;
	printf("\n  Número de vértices x Número de arestas (Peso):\n  %d x %d\n", num_vertices, grafo->A);
	
	if(num_vertices == 0){
		printf("\n");
		return;
	}
	
	List** adj = grafo->adj;
	Iterator* iterator = NULL;

	Arc* arco = 0;
	int index = 0;
	int tam = 0;
	int i = 0;

	while(index < num_vertices){
		tam = list_size(adj[index]);
		if(tam > 0){
			iterator = iterator_create(adj[index]);
			for(i = 0; i < tam; i++){
				arco = ((Arc*) iterator_get_current(iterator));
				if(index <= arco->w)
					printf("   %d %d (%d)\n", index, arco->w, arco->peso);
				
				iterator = iterator_next(iterator);
			}
		}

		index++;
	}
	return;
}

/* Função interna da biblioteca para ordenação das arestas
   utilizando o quick sort.
 */
int aresta_compara(const void* arc1, const void* arc2){
	return ((Arc*)arc1)->peso - ((Arc*)arc2)->peso;
}

/* Condição de funcionamento: grafo simétrico, sendo, ou não, direcionado.
 */
Grafo* kruskal(Grafo* grafo){
	int num_arestas = grafo->A;
	Arc arestas[num_arestas];
	
	Arc* arco = NULL;
	Iterator* iterator = NULL;
	int i = 0, index = 0;
	int num_vertices = grafo->V;
	while(i < num_vertices){
	
		iterator = iterator_create(grafo->adj[i]);
		while(!iterator_is_empty(iterator)){
			arco = (Arc*) iterator_get_current(iterator);
			if(arco->v <= arco->w){
				arestas[index].v = arco->v;
				arestas[index].w = arco->w;
				arestas[index].peso = arco->peso;
				index++;
			}
			
			iterator = iterator_next(iterator);
		}
		
		i++;
	}

	num_arestas = index;
	qsort(arestas, num_arestas , sizeof(Arc), aresta_compara);
	
	int aberto[num_vertices];
	List* listas[num_vertices];
	for(i = 0; i < num_vertices; i++){
		aberto[i] = 1;
		listas[i] = NULL;
	}
	
	Grafo* g2 = grafo_inicializa(num_vertices);
	int max_arestas = num_vertices -1,
	    num_arestas_atuais = 0;
	
	int v, temp;
	for(i = 0; i < num_arestas && num_arestas_atuais < max_arestas; i++){
		arco = &arestas[i];

		/* Corrigindo caso v seja maior que w. */
		if(arco->v > arco->w){
			temp = arco->v;
			arco->v = arco->w;
			arco->w = temp;
		}
		
		/* Os arcos são inseridos mas apenas w possui relevância. */
		if(aberto[arco->v] || aberto[arco->w]){
			v = arco->v;
			if(listas[v] == NULL)
				listas[v] = list_create();
			
			grafo_insere_arco(g2, v, arco->w, arco->peso);
			list_insert_last(listas[v], arco);
			
			aberto[v] = aberto[arco->w] = 0;
			num_arestas_atuais++;
		}
	}

	for(i = 0; i < num_vertices; i++){
		if(listas[i] != NULL)
			listas[i] = list_clear(listas[i]);
	}
	
	/* Identificação das componentes.
	 */

	return g2;
}

