
/* Exemplo de programa com pthreads e semaforos. 
 * Sincronizacao no processamento de string. --CRA
 */

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <ctype.h>

#define DIM 50

sem_t sem1, sem2;
char buf[DIM];

void *thread_func(void * pi)
{
  int i;

  printf("thread_func: wait\n");
  sem_wait(&sem1);
  printf("thread_func: processa\n");
  for (i=0; buf[i]; i++)
    buf[i] = toupper(buf[i]);
  sem_post(&sem2);
  printf("thread_func: terminar\n");
  return 0;
}
  
int main()
{
  	pthread_t  thread;
  	
  	if (sem_init(&sem1, 0, 0) != 0) {
      		printf("Erro a inicializar semaforo 1\n");
      	return -1;
  	}
  	if (sem_init(&sem2, 0, 0) != 0) {
      		printf("Erro a inicializar semaforo 2\n");
      	return -1;
  	}
  printf("main: criar thread\n");
  	if (pthread_create(&thread, NULL, thread_func, NULL) != 0) {
  		printf("Erro a criar thread\n");
  	}

	
  		printf("buf: ");
  		fgets(buf, DIM, stdin);
  		sem_post(&sem1);	/* assinala */
  		sem_wait(&sem2);
  		printf("buf: %s\n", buf);
  		printf("main: esperar fim thread\n");
  		pthread_join(thread, NULL);
  		printf("main: terminar\n");	
 	}
  return 0;
}
