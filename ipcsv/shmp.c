/* Exemplo de utilizacao de memoria partilhada IPC com 2 processos 
   independentes hierarquicamente (Produtor, Consumidor). 
   Executar primeiro o Produtor (este programa - "shmp"), e depois o 
   programa Consumidor ("shmc").  -- CRA
*/
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

#define SHMSIZE 20
#define MEMKEY 10

int main()
{
  int shmid;
  char *shmaddr;
  int i;

  if ((shmid=shmget(MEMKEY, SHMSIZE, 0666|IPC_CREAT)) < 0) {
    perror("Produtor: Erro a criar memoria partilhada");
    exit(-1);
  }

  if ((shmaddr=shmat(shmid, NULL, 0)) == (void *) -1) {
    perror("Produtor: Erro a associar endereco");
    exit(-1);
  }

  printf("Produtor vai escrever\n");
  for (i=0; i<SHMSIZE; i++)
    shmaddr[i] = 'P';
  
  if (shmdt(shmaddr) < 0) {
    perror("Produtor: Erro a desassociar endereco");
  }

  return 0;
}
