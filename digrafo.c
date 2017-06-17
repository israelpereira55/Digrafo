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

/* Função interna da biblioteca.
 */
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
			grafo->A--;
			return;
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
				printf("   %d %d (%d)\n", index, arco->w, arco->peso);
				
				iterator = iterator_next(iterator);
			}
		}

		index++;
	}
	return;
}

void grafo_imprime_adjacentes_entrada(Grafo* grafo, int* conjunto, int num_elementos){
	int num_vertices = grafo->V,
	    selecionados[num_vertices],
	    adjacentes[num_vertices]; //Indica os vizinhos elegiveis.
	
	int i;
	for(i = 0; i < num_vertices; i++)
		adjacentes[i] = 1;
		
	for(i = 0; i < num_elementos; i++)
		adjacentes[ conjunto[i] ] = 0;
	
	List* adj = NULL;
	Iterator* iterator = NULL;
	int num_adjacentes = 0,
	    index = 0,
	    w = 0;

	
	for(i = 0; i < num_elementos; i++){
		int j, elemento_atual = conjunto[i];
		
		for(j = 0; j < num_vertices; j++){
			adj = grafo->adj[j];
			iterator = iterator_create(adj);
			
			while(!iterator_is_empty(iterator)){
				w = ((Arc*) iterator_get_current(iterator))->w;
				
				if(elemento_atual == w && adjacentes[j]){
					num_adjacentes++;
					adjacentes[j] = 0;
					selecionados[index] = j;
					index++;
				}
			
				iterator = iterator_next(iterator);
			}
		}
	}
	
	printf("\n  Grau de entrada de de X: %d.\n", num_adjacentes);
	printf("  Vizinhos:");			
	
	for(i = 0; i < index; i++)
		printf(" %d", selecionados[i]);
	
	printf(".\n");	
	return;
}

void grafo_imprime_adjacentes_saida(Grafo* grafo, int* conjunto, int num_elementos){
	int num_vertices = grafo->V,
	    selecionados[num_vertices],
	    adjacentes[num_vertices]; //Indica os vizinhos elegíveis.
	
	int i = 0;
	for(i = 0; i < num_vertices; i++)
		adjacentes[i] = 1;
		
	for(i = 0; i < num_elementos; i++)
		adjacentes[ conjunto[i] ] = 0;
	
	List* adj = NULL;
	Iterator* iterator = NULL;
	
	int num_adjacentes = 0,
	    index = 0,
	    w = 0;

	for(i = 0; i < num_elementos; i++){
		adj = grafo->adj[ conjunto[i] ];
		iterator = iterator_create(adj);
		while(!iterator_is_empty(iterator) ){
			w = ((Arc*) iterator_get_current(iterator))->w;
		
			if(adjacentes[w]){
				num_adjacentes++;
				adjacentes[w] = 0;
				selecionados[index] = w;
				index++;
			}
			
			iterator = iterator_next(iterator);
		}
	}
	
	printf("  Grau de saída de de X: %d.\n", num_adjacentes);
	printf("  Vizinhos:");			
	
	for(i = 0; i < index; i++)
		printf(" %d", selecionados[i]);
	
	printf(".\n");	
	return;
}

void grafo_componentes(Grafo* grafo){
	int num_vertices = grafo->V;
	int rotulo[num_vertices];
	
	int alpha = 0,
	    beta = 0;

	int i = 0;
	while(i < num_vertices){
		rotulo[i] = i;
		i++;
	}
	
	List* adj = NULL; 
	Iterator* iterator = NULL;
	
	int index = 0;
	for(i = 0; i < num_vertices; i++){
		alpha = rotulo[i];
		
		adj = grafo->adj[i];
		iterator = iterator_create(adj);

		while(!iterator_is_empty(iterator)){
			beta = rotulo[ ((Arc*) iterator_get_current(iterator))->w ];
			if(alpha != beta){
				for(index = 0; index < num_vertices; index++)
					if(rotulo[index] == beta)
						rotulo[index] = alpha;
			}
			
			iterator = iterator_next(iterator);
		}
	}
	
	printf("Vértices x Componente:\n");
	for(i = 0; i < num_vertices; i++)
		printf("  %d %d\n", i, rotulo[i]);
	
	return;
}

void busca_largura(Grafo* grafo, int entrada){
	int num_vertices = grafo->V,
	    indices[num_vertices],
	    fechado[num_vertices];
//	    pai[num_vertices];
	
	int i = 0;
	while(i < num_vertices){
		indices[i] = i;
		fechado[i] = 0;
//		pai[i] = -1;
		i++;
	}
	
	Iterator* iterator = NULL;
	List* fila = list_create();
	list_insert_last(fila, &indices[entrada]);
	fechado[entrada] = 1;
	
	int v, w;
	while(!list_is_empty(fila)){
		v = *(int*) list_remove_first(fila);
		printf("%d - ", v);  
		
		iterator = iterator_create(grafo->adj[v]);
		while(!iterator_is_empty(iterator) ){
		
			w = ((Arc*) iterator_get_current(iterator))->w;
			if(!fechado[w]){
				list_insert_last(fila, &indices[w]);
				fechado[w] = 1;
//				pai[w] = v;
			
				printf("%d ", w);  
			}
			
			iterator = iterator_next(iterator);
		}
		printf("\n");
	}
	
	fila = list_clear(fila);
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
		
		/* Os arcos são inseridos mas apenas w possui relevância. */
		if(aberto[arco->w]){
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

