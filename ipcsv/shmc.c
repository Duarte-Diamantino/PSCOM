/* Exemplo de utilizacao de memoria partilhada IPC com 2 processos 
   independentes hierarquicamente (Produtor, Consumidor). 
   Executar primeiro o programa Produtor ("shmp"), e depois o 
   programa Consumidor (este programa - "shmc").  -- CRA
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
  char * shmaddr;
  int i;

  if ((shmid=shmget(MEMKEY, SHMSIZE, 0666)) < 0) {
    perror("Consumidor: Erro a criar memoria partilhada");
    exit(-1);
  }

  if ((shmaddr=shmat(shmid, NULL, 0)) == (void *) -1) {
    perror("Consumidor: Erro a associar endereco");
    exit(-1);
  }

  printf("Consumidor vai ler\n");
  for (i=0; i<SHMSIZE; i++)
    printf("%c", shmaddr[i]);

  if (shmdt(shmaddr) < 0) {
    perror("Consumidor: Erro a desassociar endereco");
  }
  if (shmctl(shmid, IPC_RMID, 0) < 0) {
    perror("Consumidor: Erro a eliminar memoria");
  }

  return 0;
}
