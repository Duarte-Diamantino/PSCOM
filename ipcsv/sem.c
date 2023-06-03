/* Exemplo de utilizacao de semaforos IPC com 2 processos 
   dependentes hierarquicamente (fork). -- CRA
*/
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>

int main()
{
  int semid;
  union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
  } arg;
  struct sembuf sops[1];

  if ((semid=semget(IPC_PRIVATE, 1, 0666|IPC_CREAT)) < 0) {
    perror("Erro a criar semaforo");
    exit(-1);
  }

  arg.val = 0;
  if (semctl(semid, 0, SETVAL, arg) < 0) {
    perror("Erro a inicializar semaforo");
    exit(-1);
  }

  if (fork()==0) { /* filho */
    printf("Ola', eu sou o filho, pid=%d\n", getpid());
    sops[0].sem_num = 0;
    sops[0].sem_op  = -1;
    sops[0].sem_flg = 0;

    printf("Filho vai bloquear\n");
    if (semop(semid, sops, 1) < 0) {
      perror("Erro a esperar semaforo");
    }
    printf("Filho desbloqueado\n");
  }
  else { /* pai */
    printf("Ola', eu sou o pai, pid=%d\n", getpid());
    sops[0].sem_num = 0;
    sops[0].sem_op  = 1;
    sops[0].sem_flg = 0;

    sleep(5);
    printf("Pai vai desbloquear filho\n");
    if (semop(semid, sops, 1) < 0) {
      perror("Erro a assinalar semaforo\n");
    }
    printf("Pai continua. Vai dormir.\n");
    sleep(5);
    if (semctl(semid, 0, IPC_RMID) < 0) {
      perror("Erro a eliminar semaforo\n");
    }
  }

  return 0;
}
