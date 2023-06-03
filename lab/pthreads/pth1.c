
/* Exemplo de programa que cria pthreads. --CRA
 */

#include <stdio.h>
#include <pthread.h>

#define NT 3

int kika = 3;

void *thread_func(void * pi)
{
  int i = *(int *)pi;  
  printf("variavel global=%d\n", kika);
  kika++;
  printf("thread_func: arg=%d\n", i);
  printf("thread_func: terminar t=%d\n", i);
  return 0;
}
  
int main()
{
  pthread_t  threads[NT];
  int i;
  int id[NT];

/* cria threads que correm em segundo plano */

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
  printf("main: terminar\n");
 
  return 0;
}
