/* Exemplo de utilizacao de memoria partilhada IPC com 2 processos 
   dependentes hierarquicamente (fork). -- CRA
*/
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

#define SHMSIZE 20

int main()
{
  int shmid;
  char * shmaddr;
  int i;

  if ((shmid=shmget(IPC_PRIVATE, SHMSIZE, 0666|IPC_CREAT)) < 0) {
    perror("Erro a criar memoria partilhada");
    exit(-1);
  }

  if ((shmaddr=shmat(shmid, NULL, 0)) == (void *) -1) {
    perror("Erro a associar endereco");
    exit(-1);
  }

  if (fork()==0) { /* filho */
    printf("Ola', eu sou o filho, pid=%d\n", getpid());
    printf("Filho vai dormir\n");
    sleep(3);
    printf("Filho vai ler\n");
    for (i=0; i<SHMSIZE; i++)
      printf("%c", shmaddr[i]);
    printf("Filho vai escrever\n");
    for (i=0; i<SHMSIZE; i++)
      shmaddr[i] = 'F';
  }
  else { /* pai */
    printf("Ola', eu sou o pai, pid=%d\n", getpid());
    printf("Pai vai escrever\n");
    for (i=0; i<SHMSIZE; i++)
      shmaddr[i] = 'P';
    printf("Pai continua. Vai dormir.\n");
    sleep(5);
    printf("Pai vai ler\n");
    for (i=0; i<SHMSIZE; i++)
      printf("%c", shmaddr[i]);

    if (shmdt(shmaddr) < 0) {
      perror("Erro a desassociar endereco");
    }
    if (shmctl(shmid, IPC_RMID, 0) < 0) {
      perror("Erro a eliminar memoria");
    }

  }

  return 0;
}
