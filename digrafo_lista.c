#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "digrafo_lista.h"
#include "linkedlist.h"

int** matriz_inicializa(int linhas, int colunas, int valor){
	int** m = (int**) malloc (linhas * sizeof(int*));

	int i = 0;
	while(i < linhas){
		m[i] = (int*) malloc (colunas * sizeof(int*));
		i++;
	}
	
	int j = 0;
	for(i = 0; i < linhas; i++){
		for(j = 0; j < colunas; j++)
			m[i][j] = valor;
	}

	return m;
}

int** matriz_libera(int** matriz, int linhas){
	int i = 0;
	while(i < linhas){
		free(matriz[i]);
		i++;
	}
	free(matriz);
	return NULL;
}

void* libera_conteudo(void *vertice){
	free( (int*) vertice);
	return NULL;
}

Digrafo* digrafo_inicializa(int V){
	Digrafo* digrafo = (Digrafo*) malloc (sizeof(Digrafo));
	
	digrafo->V = V;
	digrafo->A = 0;
	digrafo->adj = (List**) malloc (V * sizeof(List*));;
	
	int i = 0;
	List** adj = digrafo->adj;
	while(i < V){
		adj[i] = list_create();
		i++;
	}
	return digrafo;
}

Digrafo* digrafo_libera(Digrafo* digrafo){
	int num_vertices = digrafo->V;
	List** adj = digrafo->adj;

	int i = 0;
	while(i < num_vertices){
		adj[i] = list_complete_clear(adj[i], libera_conteudo);
		i++;
	}
	
	free(digrafo->adj);
	free(digrafo);
	return NULL;
}

void digrafo_insere_arco(Digrafo* digrafo, int v, int w){
	List* adj = digrafo->adj[v];
	
	int tam = list_size(adj);
	int* pointer = NULL;
	if(tam == 0){
		pointer = (int*) malloc (sizeof(int));
		*pointer = w;
	
		list_insert_last(adj, pointer);
		digrafo->A++;
		return;
	}
	
	Iterator* iterator = iterator_create(adj);
	
	int possui_elemento = 0;
	int elemento = 0;
	int i = 0;
	while(i < tam){
		elemento = *((int*) iterator_get_current(iterator));
		
		if(elemento == w){
			possui_elemento = 1;
			break;
		}
		
		i++;
		iterator = iterator_next(iterator);
	}
	
	if(!possui_elemento){
		pointer = (int*) malloc (sizeof(int));
		*pointer = w;
		
		list_insert_last(adj, pointer);
		digrafo->A++;
	}
	return;
}

void digrafo_remove_arco(Digrafo* digrafo, int v, int w){
	List* adj = digrafo->adj[v];
	
	int tam = list_size(adj);
	if(tam == 0)
		return;
	
	Iterator* iterator = iterator_create(adj);
	
	int elemento = 0;
	int index = 0;
	int* vertice = 0;
	while(index < tam){
		elemento = *((int*) iterator_get_current(iterator));
			
		if(elemento == w){
			vertice = (int*) list_remove(adj, index);
			free(vertice);
			digrafo->A--;
			return;
		}
		index++;
		iterator = iterator_next(iterator);
	}	
	
	return;
}

void digrafo_imprime(Digrafo* digrafo){
	int num_vertices = digrafo->V;
	printf("\n  Número de vértices x Número de arestas:\n  %d x %d\n", num_vertices, digrafo->A);
	
	if(num_vertices == 0){
		printf("\n");
		return;
	}
	
	List** adj = digrafo->adj;
	
	Iterator* iterator = NULL;

	int elemento = 0;
	int index = 0;
	int tam = 0;
	int i = 0;

	while(index < num_vertices){
		tam = list_size(adj[index]);
		if(tam > 0){
			iterator = iterator_create(adj[index]);
			for(i = 0; i < tam; i++){
				elemento = *((int*) iterator_get_current(iterator));
				printf("   %d %d\n", index, elemento);
				
				iterator = iterator_next(iterator);
			}
		}

		index++;
	}
	return;
}
/*
void digrafo_imprime_adjacentes_saida(Digrafo* digrafo, int* conjunto, int n){
	int num_linhas = digrafo->V,
	    selecionados[num_linhas],
	    adjacentes[num_linhas]; //Indica os vizinhos elegiveis.
	
	int i = 0;
	for(i = 0; i < num_linhas; i++)
		adjacentes[i] = 1;
		
	for(i = 0; i < n; i++)
		adjacentes[ conjunto[i] ] = 0;
	
	int** adj = digrafo->adj;
	int* adj_linha = NULL;
	int num_adjacentes = 0,
	    index = 0,
	    j = 0;
	    
	for(i = 0; i < n; i++){
		adj_linha = adj[ conjunto[i] ]; //Seleciona a linha de um elemento do conjunto X.
		for(j = 0; j < num_linhas; j++){
			if(adj_linha[j] > 0 && adjacentes[j]){
				num_adjacentes++;
				adjacentes[j] = 0;
				selecionados[index] = j;
				index++;
			}
		}
	}
	
	printf("\n  Grau de saída de de X: %d.\n", num_adjacentes);
	printf("  Vizinhos:");			
	
	for(i = 0; i < index; i++)
		printf(" %d", selecionados[i]);
	
	printf(".\n");	
	return;
}

void digrafo_imprime_adjacentes_entrada(Digrafo* digrafo, int* conjunto, int n){
	int num_colunas = digrafo->V,
	    selecionados[num_colunas],
	    adjacentes[num_colunas]; //Indica os vizinhos elegiveis.
	
	int i = 0;
	for(i = 0; i < num_colunas; i++)
		adjacentes[i] = 1;
		
	for(i = 0; i < n; i++)
		adjacentes[ conjunto[i] ] = 0;
	
	int** adj = digrafo->adj;
	int* adj_coluna = NULL;
	int num_adjacentes = 0,
	    index = 0,
	    elemento = 0,
	    j = 0;
	    
	for(i = 0; i < n; i++){
		elemento = conjunto[i];
		for(j = 0; j < num_colunas; j++){
			if(adj[j][elemento] > 0 && adjacentes[j]){
				num_adjacentes++;
				adjacentes[j] = 0;
				selecionados[index] = j;
				index++;
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
*/
void lista_adj_componentes(Digrafo* digrafo){
	int num_vertices = digrafo->V;
	int rotulo[num_vertices];
	
	List** adj = digrafo->adj;
	List* adj_vertice = NULL; 
	
	int alpha = 0,
	    beta = 0;

	int i = 0;
	while(i < num_vertices){
		rotulo[i] = i;
		i++;
	}
	Iterator* iterator = NULL;
	
	int tam = 0;
	int index = 0,
	    j = 0;

	for(i = 0; i < num_vertices; i++){
		alpha = rotulo[i];
		
		adj_vertice = adj[i];
		tam = list_size(adj_vertice);
		if(tam) iterator = iterator_create(adj_vertice);
		
		for(j = 0; j < tam; j++){
			beta = rotulo[ *((int*) iterator_get_current(iterator)) ];
			
			if(alpha != beta){
				for(index = 0; index < num_vertices; index++){
					if(rotulo[index] == beta)
						rotulo[index] = alpha;
				}
			}
			
			iterator = iterator_next(iterator);
		}
	}
	
	printf("Vértices x Componente:\n");
	for(i = 0; i < num_vertices; i++)
		printf("  %d %d\n", i, rotulo[i]);
	
	return;
}

Digrafo* processa_ladders(FILE* leitura, char** palavras, int num_palavras, int tam_palavra){
	Digrafo* digrafo = digrafo_inicializa(num_palavras);
	
	int i = 0;
	int dif = 0;
	int index_p1 = 0;
	int index_p2 = 1;
	char* palavra1 = NULL;
	char* palavra2 = NULL;
	
	while(index_p1 < num_palavras -1){
		palavra1 = palavras[index_p1];
		
		index_p2 = index_p1 +1;
	//	if(index_p2 == index_p1)
	//		index_p2++;
			
		while(index_p2 < num_palavras){
			palavra2 = palavras[index_p2];
			
			dif = 0;
			for(i = 0; i < tam_palavra && dif < 2; i++){
				if(palavra1[i] != palavra2[i])
					dif++;
			}
		
			if(dif == 1){
				digrafo_insere_arco(digrafo, index_p1, index_p2);
				digrafo_insere_arco(digrafo, index_p2, index_p1);
			}
			
			index_p2++;
			if(index_p2 == index_p1)
				index_p2++;
		}
		index_p1++;
	}
	
	return digrafo;
}

void ladders(Digrafo* digrafo, Set* Hash, char** palavras, int num_palavras, char* palavra1, char* palavra2, int tam_palavra){
	int visitado[num_palavras];
	int indices[num_palavras];
	int pai[num_palavras];
	int caminho[num_palavras];
	int i = 0;
	int index = 0;
	while(i < num_palavras){
		visitado[i] = 0;
		indices[i] = i;
		pai[i] = -1;
		i++;
	}
	
	int index_p1 = ((Palavra*) set_get(Hash, palavra1))->index;
	int index_p2 = ((Palavra*) set_get(Hash, palavra2))->index;
	
	int *u, *w;
	List** adj = digrafo->adj;
	int v;
	Iterator* iterator = NULL;
	
	List* fila = list_create();
	list_insert_last(fila, &indices[index_p1]);
	visitado[index_p1] = 1;
	
	while(!list_is_empty(fila)){
		u = list_remove_first(fila);
		
		iterator = iterator_create(adj[*u]);
		if(!iterator_is_empty(iterator) ){
		
			do{
				w = (int*) iterator_get_current(iterator);
				v = *w;
			
				if(!visitado[v]){
					list_insert_last(fila, &indices[v]);
					visitado[v] = 1;
					pai[v] = *u;
				
					//Encontrou a palavra2.
					if(v == index_p2)
						goto FINALIZA_BUSCA;
				}
				
				iterator = iterator_next(iterator);
			}while(!iterator_is_empty(iterator));
		}	
	}

FINALIZA_BUSCA:
	v = index_p2;
	
	
	if(pai[v] == -1){
		printf("    Não existe caminho.\n");
	} else {
	
		caminho[0] = index_p2;
		index = 1;
		while(v != index_p1){
			v = pai[v];
			caminho[index] = v;
			index++;
		}
		
		for(i = index -1; i >= 0; i--)
			printf("     %2d - %s\n", index -i, palavras[ caminho[i] ]);
	}
	
	list_clear(fila);
	return;
}
