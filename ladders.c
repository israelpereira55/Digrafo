#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "digrafo_lista.h"
#include "set.h"

#define num_palavras 5481
#define tam_palavra 5

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

Palavra* palavra_inicializa(char* info, int index){
	Palavra* palavra = (Palavra*) malloc (sizeof(Palavra));
	palavra->info = info;
	palavra->index = index;
	return palavra;
}

void* palavra_libera(void* palavra){
//	free( ((Palavra*)palavra)->info);
	free(  (Palavra*)palavra);
	return NULL;
}

int main(){
	FILE* leitura = fopen("palavras.txt", "r");
	
	char** palavras = (char**) malloc (num_palavras * sizeof(char*));
	
	int i = 0;
	while(i < num_palavras){
		palavras[i] = (char*) malloc ((tam_palavra +2) * sizeof(char));
		i++;
	}
	
	Palavra* palavra = NULL;
	Set* Hash = set_create(3 * num_palavras, set_compara_palavra, hash_code, palavra_libera);
	
	for(i = 0; i < num_palavras; i++){
		fscanf(leitura, "%s", palavras[i]);
		palavra = palavra_inicializa(palavras[i], i);
		set_insert(Hash, palavra, palavras[i]);
	}

	Digrafo* digrafo = processa_ladders(leitura, palavras, num_palavras, tam_palavra); 
	fclose(leitura);
	
	int executar = 1;
	char escolha = 0;
	char palavra1[tam_palavra +1];
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

		ladders(digrafo, Hash, palavras, num_palavras, palavra1, palavra2, tam_palavra);

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
	
	for(i = 0; i < num_palavras; i++)
		free(palavras[i]);
	
	free(palavras);

	Hash = set_complete_clear(Hash);	
	digrafo = digrafo_libera(digrafo);
	return 0;
}
