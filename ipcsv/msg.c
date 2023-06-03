/* Exemplo de utilizacao de mensagens IPC com 2 processos 
   dependentes hierarquicamente (fork). -- CRA
*/
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

#define TXTSIZE 10

int main()
{
  int msqid;
  struct {
    long    mtype;      /* message type */
    char    mtext[TXTSIZE];   /* message text */
  } msg;
  int i;

  if ((msqid=msgget(IPC_PRIVATE, 0666|IPC_CREAT)) < 0) {
    perror("Erro a criar queue");
    exit(-1);
  }

  msg.mtype = 1;

  if (fork()==0) { /* filho */
    printf("Filho vai receber\n");

    if (msgrcv(msqid, &msg, sizeof(msg.mtext), 0, 0) < 0) {
      perror("Filho: erro a receber mensagem");
    }

    for (i=0; i<TXTSIZE; i++)
      printf("%c", msg.mtext[i]);

    printf("Filho vai enviar\n");
    for (i=0; i<TXTSIZE; i++)
      msg.mtext[i] = 'F';
    if (msgsnd(msqid, &msg, sizeof(msg.mtext), 0) < 0) {
      perror("Filho: erro a enviar mensagem\n");
    }
  }
  else { /* pai */
    printf("Ola', eu sou o pai, pid=%d\n", getpid());
    printf("Pai vai enviar\n");
    for (i=0; i<TXTSIZE; i++)
      msg.mtext[i] = 'P';
    if (msgsnd(msqid, &msg, sizeof(msg.mtext), 0) < 0) {
      perror("Pai: erro a enviar mensagem\n");
    }
    printf("Pai continua e vai dormir.\n");
    sleep(5);
    printf("Pai vai receber\n");
    if (msgrcv(msqid, &msg, sizeof(msg.mtext), 0, 0) < 0) {
      perror("Pai: erro a receber mensagem\n");
    }
    for (i=0; i<TXTSIZE; i++)
      printf("%c", msg.mtext[i]);

    if (msgctl(msqid, IPC_RMID, 0) < 0) {
      perror("Erro a eliminar queue\n");
    }

  }

  return 0;
}
