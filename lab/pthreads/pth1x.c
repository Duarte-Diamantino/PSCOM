
/* Exemplo de programa que cria pthreads. --CRA
 */

#include <stdio.h>
#include <pthread.h>

#define NT 3
#define DIM 10000
#define T 1000


pthread_mutex_t mux;

int vec[DIM];
int cnt = 0;

void *thread_func(void * pi)
{
  int i = *(int *)pi;
  int n;

  printf("thread_func: arg=%d\n", i);
  printf("thread_func: terminar t=%d\n", i);

  while (cnt < DIM) {
    vec[cnt]++;
    for (n=0; n<T; n++);
    cnt++;
  }

  return 0;
}
  
int main()
{
  pthread_t  threads[NT];
  int i;
  int id[NT];

  printf("main: criar thread(s)\n");
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

  for (i=0; i<DIM; i++) {
    if (vec[i] != 1) printf("vec[%d]=%d\n", i, vec[i]);
  }

  printf("main: terminar\n");
 
  return 0;
}
