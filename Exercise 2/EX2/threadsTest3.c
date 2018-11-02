#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#define N 5
#define TAMANHO 10

int buffer [N] [TAMANHO];
int nsomas;

void *soma_linha (void *linha) {
  int c, soma=0;
  int *b = (int*) linha;
  for (c=0;c<TAMANHO-1;c++) {
    soma += b[c];
    nsomas++;
  }
  b[c]=soma; /* soma->ult.col.*/
  return NULL;
}

int main (void) {
  int i,j;
  pthread_t tid[N];

  inicializaMatriz(buffer, N, TAMANHO);
  for (i=0; i< N; i++){
    if(pthread_create (&tid[i], 0, soma_linha,
    buffer[i])== 0) {
      printf ("Criada a tarefa %d\n", tid[i]);
    }
    else {
      printf("Erro na criação da tarefa\n");
      exit(1);
    }
  }
  for (i=0; i<N; i++){
    pthread_join (tid[i], NULL);
  }
  printf ("Terminaram todas as threads\n");
  imprimeResultados(buffer);
  exit(0);
}
