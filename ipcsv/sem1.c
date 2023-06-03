/* Exemplo de utilizacao de semaforos IPC com 2 processos 
   independentes hierarquicamente. 
   Executar primeiro (em "background" ou noutra janela) este 
   programa ("sem1"), e depois o programa ("sem2").  -- CRA
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
  union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
  } arg;
  struct sembuf sops[1];

  if ((semid=semget(PROC1KEY, 1, 0666|IPC_CREAT)) < 0) {
    perror("PROC1: Erro a criar semaforo");
    exit(-1);
  }

  arg.val = 0;
  if (semctl(semid, 0, SETVAL, arg) < 0) {
    perror("PROC1: Erro a inicializar semaforo");
    exit(-1);
  }

  sops[0].sem_num = 0;
  sops[0].sem_op  = -1;
  sops[0].sem_flg = 0;

  printf("PROC1 vai bloquear\n");
  if (semop(semid, sops, 1) < 0) {
    perror("PROC1: Erro a esperar semaforo");
  }
  printf("PROC1 desbloqueado\n");

  if (semctl(semid, 0, IPC_RMID) < 0) {
    perror("Erro a eliminar semaforo\n");
  }

  return 0;
}
