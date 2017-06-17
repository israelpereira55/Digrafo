#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grafo.h"
#include "set.h"

#define nome_arquivo "palavras.txt"
#define num_palavras 5481
#define tam_palavra 5

typedef struct palavra Palavra;

struct palavra{
	char* info;
	int index;
};

/* Funções necessárias para o uso do HashSet.
 */
int hash_code(void* key){
	int sum = 0;
	int i = 0;
	
	while(i < tam_palavra){
		sum = sum + ((char*)key)[i];
		i++;
	}
	return sum;
}

int set_compara_palavra(void* info1, void* info2){
	return strcmp( (char*)info1, (char*)info2);
}


/* Funções necessárias para o Ladders.
 */
Palavra* palavra_inicializa(char* info, int index){
	Palavra* palavra = (Palavra*) malloc (sizeof(Palavra));
	palavra->info = strdup(info);
	palavra->index = index;
	return palavra;
}

void* palavra_libera(void* palavra){
	free(  (Palavra*)palavra);
	return NULL;
}

Grafo* processa_ladders(FILE* leitura, Palavra** palavras){
	Grafo* grafo = grafo_inicializa(num_palavras);
	
	int i = 0;
	int dif = 0;
	int index_p1 = 0;
	int index_p2 = 1;
	char* palavra1 = NULL;
	char* palavra2 = NULL;
	
	while(index_p1 < num_palavras -1){
		palavra1 = palavras[index_p1]->info;
		
		index_p2 = index_p1 +1;

		while(index_p2 < num_palavras){
			palavra2 = palavras[index_p2]->info;
			
			dif = 0;
			for(i = 0; i < tam_palavra && dif < 2; i++){
				if(palavra1[i] != palavra2[i])
					dif++;
			}
		
			if(dif == 1)
				grafo_insere_arco(grafo, index_p1, index_p2, 1);
	//			grafo_insere_arco(grafo, index_p2, index_p1);
			
			index_p2++;
			if(index_p2 == index_p1)
				index_p2++;
		}
		index_p1++;
	}
	
	return grafo;
}

void ladders(Grafo* grafo, Set* Hash, Palavra** palavras, char* palavra1, char* palavra2){
	int visitado[num_palavras],
	    pai[num_palavras];

	int i = 0;
	int index = 0;
	while(i < num_palavras){
		visitado[i] = 0;
		pai[i] = -1;
		i++;
	}
	
	int index_p1 = ((Palavra*) set_get(Hash, palavra1))->index;
	int index_p2 = ((Palavra*) set_get(Hash, palavra2))->index;
	
	List** adj = grafo->adj;
	Iterator* iterator = NULL;
	
	List* fila = list_create();
	list_insert_last(fila, palavras[index_p1]);
	visitado[index_p1] = 1;
	
	int v, w;
	while(!list_is_empty(fila)){
		v = ((Palavra*) list_remove_first(fila))->index;
		
		iterator = iterator_create(adj[v]);
		if(!iterator_is_empty(iterator) ){
		
			do{
				w = ((Arc*) iterator_get_current(iterator))->w;

				if(!visitado[w]){
					list_insert_last(fila, palavras[w]);
					visitado[w] = 1;
					pai[w] = v;
				
					//Encontrou a palavra2.
					if(w == index_p2)
						goto FINALIZA_BUSCA;
				}
				
				iterator = iterator_next(iterator);
			}while(!iterator_is_empty(iterator));
		}	
	}

FINALIZA_BUSCA:

	fila = list_clear(fila);
	
	if(pai[index_p2] == -1){
		printf("    Não existe caminho.\n");
	} else {

		fila = list_create();
		list_insert_last(fila, palavras[index_p2]);
		
		index = 1;
		v = index_p2;
		while(v != index_p1){
			v = pai[v];
			list_insert_last(fila, palavras[v]);
			index++;
		}
		
		iterator = iterator_create(fila);
		index++;
		for(i = 1; i < index; i++){
			printf("     %2d - %s\n", i, ((Palavra*) iterator_get_current(iterator))->info );
			iterator = iterator_next(iterator);
		}
		
		fila = list_clear(fila);
	}
	
	return;
}

int main(){
	FILE* leitura = fopen(nome_arquivo, "r");
	Set* Hash = set_create(3 * num_palavras, set_compara_palavra, hash_code, palavra_libera);
	
	Palavra** palavras = (Palavra**) malloc (num_palavras * sizeof(Palavra*));
	char palavra1[tam_palavra +1];
	
	int i;
	for(i = 0; i < num_palavras; i++){
		fscanf(leitura, "%s", palavra1);
		palavras[i] = palavra_inicializa(palavra1, i);
		set_insert(Hash, palavras[i], palavras[i]->info);
	}
	
	fclose(leitura);
	Grafo* grafo = processa_ladders(leitura, palavras); 
	
	int executar = 1;
	char escolha = 0;
	char palavra2[tam_palavra +1];
	
	printf( "=================================================================== \n"
		"                             Ladders\n"
		"===================================================================\n\n");
	
	do{
		printf("\n    Palavra inicial: ");
		scanf("%s", palavra1);
	
		printf("    Palavra destino: ");
		scanf("%s", palavra2);
		
		printf("\n");

		ladders(grafo, Hash, palavras, palavra1, palavra2);

		do{
			printf("\n    Deseja continuar? (S/N): ");
			scanf("\n%c", &escolha);
		
			switch(escolha){
				case 'S':
				case 's':
					executar = 1;
					break;
				
				case 'N':
				case 'n':
					goto FINALIZA_PROGRAMA;
			
				default:
					printf("    Comando inválido!\n\n");
					executar = -1;
			}
		}while(executar == -1);
	}while(executar);
	
FINALIZA_PROGRAMA:
	
	for(i = 0; i < num_palavras; i++){
		free(palavras[i]->info);
		free(palavras[i]);
	}
	free(palavras);

	Hash = set_clear(Hash);	
	grafo = grafo_libera(grafo);
	return 0;
}
