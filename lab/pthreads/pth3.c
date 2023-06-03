
/* Exemplo de programa com pthreads e semaforos. --CRA
 */

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define NT 1

sem_t sem1, sem2;

void *thread_func(void * pi)
{
  int i = *(int *)pi;
  printf("thread_func: arg=%d\n", i);
  sem_wait(&sem1);
  printf("thread_func: sem1-sem2 t=%d\n", i);
  sem_post(&sem2);
  printf("thread_func: terminar t=%d\n", i);
  return 0;
}
  
int main()
{
  pthread_t  threads[NT];
  int i;
  int id[NT];
  if (sem_init(&sem1, 0, 0) != 0) {
      printf("Erro a inicializar semaforo 1\n");
      return -1;
  }
  if (sem_init(&sem2, 0, 0) != 0) {
      printf("Erro a inicializar semaforo 2\n");
      return -1;
  }
  printf("main: criar thread(s)\n");
  for (i=0; i<NT; i++) {
    id[i]=i;
    if (pthread_create(&threads[i], NULL, thread_func, (void *)&id[i]) != 0) {
      printf("Erro a criar thread=%d\n", i);
    }
  }

  sem_post(&sem1);
  printf("main: sem1-sem2\n");
  sem_wait(&sem2);

  printf("main: esperar fim thread(s)\n");
  for (i=0; i<NT; i++) {
    pthread_join(threads[i], NULL);
  }
  printf("main: terminar\n");
 
  return 0;
}
