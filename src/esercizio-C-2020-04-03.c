/*
 ============================================================================
 Name        : Esercizio-2020-04-02.c
 Author      : Emiliano Rossi
 Version     :
 Copyright   : 
 Description : Es 2020-04-02
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_CHARS 80

unsigned int n_programmi =0;
size_t size_c_r=1;


char massimo(char * array, size_t size){
	char max = 0;
	for (unsigned int i = 0; i<size;i++){
		if (*(array+i)>max){
			max = *(array+i);
		}
	}
	return max;
}


char minimo(char * array, size_t size){
	char min =*(array);
	for (unsigned int i = 0; i<size;i++){
		if (*(array+i)<min){
			min = *(array+i);
		}
	}
	return min;
}


char **  arr_ricorrenze(char * array, unsigned int size){
	char **c=malloc(sizeof(char *));
	if (c==NULL){
		perror("malloc");
		exit(-1);
	}
	c[0]=calloc(2,sizeof(char));
	if(c[0]==NULL){
		perror("calloc");
		exit(-1);
	}
	c[0][0]=*(array);
	int match;
	for(int i=0;i<size;i++){
		match=0;
		for(int j =0;j<size_c_r;j++){
			if(array[i] == c[j][0]){ // se trovo che array[i] è contenuto nel mio array c interrompo
				match =1;
				c[j][1]++;
				break;
			}
		}
		if(match ==0){      //nel caso non è contenuto l'aggiungo
			size_c_r++;

			c = realloc(c,size_c_r*sizeof(char *));
			c[size_c_r-1]=malloc(2*sizeof(char));
			if(c[size_c_r-1]==NULL){
				perror("calloc");
				exit(-1);
			}
			c[(size_c_r)-1][0]=*(array+i);
			c[(size_c_r)-1][1]++;

 		 }
	}
	return c;

}

char minor_ricorrenze(char ** array, size_t size_a){
	char min = NUM_CHARS; //assume il valore massimo possibile (scrivo la stessa lettera in tutto l'array)
	unsigned int posizione =0;
	for(int i =0; i<size_a;i++){
		if( array[i][1]< min){
			min =  array[i][1];
			posizione = i;
		}
	}
	return array[posizione][0];
}

char maggior_ricorrenze(char ** array, size_t size_a){
	char max = 0; //assume il valore minimo possibile
	int posizione =0;
	for(int i =0; i<size_a;i++){
		if( array[i][1]> max){
			max =  array[i][1];
			posizione = i;
		}
	}
	return array[posizione][0];
}
void stampa_char (char carattere){
	if (carattere > 32){
		printf("\t %d \t '%c'\n", carattere,carattere);
	} else {
		printf("\t %d \t\n", carattere);
	}
}

void elaborazioni(char * char_array,size_t size){

	char ** ricorrenze;
	ricorrenze=arr_ricorrenze(char_array,size);
	printf("\n");
	printf("[child %d]\n\n", getpid());
	printf("\tValore minimo: \t");
	stampa_char(minimo(char_array,size));
	printf("\tValore massimo:\t");
	stampa_char(massimo(char_array,size));
	printf("\tMinor ricorrenza:");
	stampa_char(minor_ricorrenze(ricorrenze,size_c_r));
	printf("\tMaggior ricorrenza:");
	stampa_char(maggior_ricorrenze(ricorrenze,size_c_r));
	printf("\n");

	for(int i =0;i<size;i++)
		free(ricorrenze[i]);
	free(ricorrenze);
}


void child_process(char * char_array,size_t size){
	switch (fork()){
	case -1:
		perror("fork");
		exit(-1);
		break;
	case 0:
		elaborazioni(char_array,size);
		exit(0);

	default:
		if( wait(NULL)==-1){  //metto in attesa per evitare sovrapposizioni in stdout
			perror("wait");
		}
		n_programmi++;
		memset(char_array,0,size*sizeof(char));

	}
}


int main(int argc, char ** argv) {
	printf("[Padre %d]: Digitare del testo, per terminare Ctrl+D\n\n",getpid());
	char  char_array[NUM_CHARS];

	unsigned int i=0;
	while(( *(char_array+i) = getchar()) != EOF){
	  i++;
	   if (i==NUM_CHARS){
		 i=0;
		 child_process(char_array,NUM_CHARS);
	   }
	}
	child_process(char_array,i);

	printf("[Padre %d]:Child lanciati:%d\t\n",getpid(),n_programmi);


	return EXIT_SUCCESS;
}
