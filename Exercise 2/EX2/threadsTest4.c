#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int *total;

void* count(void* total){
  total++;
}

int main(int argc, char const *argv[]) {
  pthread_t tid[5];

  *total = 5;
  for (int i=0; i< 5; i++){
    if(pthread_create (&tid[i], 0, count, total)== 0) {
      printf ("Criada a tarefa %ld\n", tid[i]);
    }
    else {
      printf("Erro na criação da tarefa\n");
      exit(1);
    }
  }
  return 0;
}
