/* Exemplo de utilizacao de semaforos IPC com 2 processos 
   independentes hierarquicamente. 
   Executar primeiro (em "background" ou noutra janela) o programa "sem1", 
   e depois este ("sem2").  -- CRA
*/
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>

#define PROC1KEY 11

int main()
{
  int semid;
  struct sembuf sops[1];

  if ((semid=semget(PROC1KEY, 1, 0666)) < 0) {
    perror("PROC2: Erro a obter semaforo");
    exit(-1);
  }

  sops[0].sem_num = 0;
  sops[0].sem_op  = 1;
  sops[0].sem_flg = 0;

  printf("PROC2 vai desbloquear PROC1\n");
  if (semop(semid, sops, 1) < 0) {
    perror("Erro a assinalar semaforo\n");
  }

  return 0;
}
