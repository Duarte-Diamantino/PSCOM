/* Exemplo de utilizacao de mensagens IPC com 2 processos 
   independentes hierarquicamente (Cliente, Servidor). 
   Executar primeiro (em "background" ou noutra janela) o programa 
   Servidor ("msgs"), e depois o programa 
   Cliente (este programa - "msgc").  -- CRA
*/


/*

le do servidor e salta se !

*/


#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

#define TXTSIZE 10
#define SERVKEY 1

int main()
{
  int msqids, msqidc, i;
  int cliid;
  struct {
    long    mtype;      /* message type */
    int     id;
    char    mtext[TXTSIZE];   /* message text */
  } msg;

  cliid = getpid();
  
  if ((msqidc=msgget(cliid, 0666|IPC_CREAT)) < 0) {
    perror("Cliente: Erro a criar queue cliente");
    exit(-1);
  }
  if ((msqids=msgget(SERVKEY, 0)) < 0) {
    perror("Cliente: Erro a associar a queue servidor");
    exit(-1);
  }
	
  printf("Cliente vai enviar\n");
  
  msg.mtype = 1;
  msg.id = cliid;
  for (i=0; i<TXTSIZE; i++)
    msg.mtext[i] = 'C';
  if (msgsnd(msqids, &msg, sizeof(msg.mtext)+sizeof(msg.id), 0) < 0) {
    perror("Cliente: erro a enviar mensagem");
  }
  else {
    
    printf("Cliente vai receber\n");
    while (msg.mtext[0] != '!'){

    if (msgrcv(msqidc, &msg, sizeof(msg.mtext)+sizeof(msg.id), 0, 0) < 0) {
      perror("Cliente: erro a receber mensagem");
    }
    else {
      for (i=0; i<TXTSIZE; i++)
	printf("%c", msg.mtext[i]);
    }
     }
  if (msgctl(msqidc, IPC_RMID, 0) < 0) {
    perror("Cliente: Erro a eliminar queue cliente");
 
  }
}
  return 0;
}
