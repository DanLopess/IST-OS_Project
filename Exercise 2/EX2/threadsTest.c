#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

/*
A normal C function that is executed as a thread
when its name is specified in pthread_create()
*/
void* myThreadFunc(void* vargp){
	sleep(3);
	int* a = vargp;
	printf("Printing a loop from Thread \n");
	for(int i = 0; i < *a; i++){
		printf("Number: %d\n", i);
	}
}

int main(int argc, char const *argv[]) {
	int* vargp;
	*vargp = 20;
	pthread_t thread_id;
	printf("Before Thread\n");
	pthread_create(&thread_id, NULL, myThreadFunc, vargp);
	pthread_join(thread_id, NULL);
	printf("After thread\n");


	return 0;
}
