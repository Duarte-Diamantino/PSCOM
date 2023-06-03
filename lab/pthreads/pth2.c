
/* Exemplo de programa com pthreads e mutexes. --CRA
 */

#include <stdio.h>
#include <pthread.h>

#define NT 10

pthread_mutex_t mux;
int cont=0;

void *thread_func(void * pi)
{
  int i = *(int *)pi;
  pthread_mutex_lock(&mux);
  cont++;
  printf("thread_func: t=%d cont=%d\n", i, cont);
  pthread_mutex_unlock(&mux);
  return 0;
}
  
int main()
{
  pthread_t  threads[NT];
  int i;
  int id[NT];
  if (pthread_mutex_init(&mux, NULL) != 0) {
      printf("Erro a inicializar mutex\n");
      return -1;
  }
  printf("main: criar thread(s), cont=%d\n", cont);
  for (i=0; i<NT; i++) {
    id[i]=i;
    if (pthread_create(&threads[i], NULL, thread_func, (void *)&id[i]) != 0) {
      printf("Erro a criar thread=%d\n", i);
    }
  }

  printf("main: esperar fim thread(s)\n");
  for (i=0; i<NT; i++) {
    pthread_join(threads[i], NULL);
  }
  printf("main: terminar, cont=%d\n", cont);
 
  return 0;
}
