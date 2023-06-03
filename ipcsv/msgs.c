/* Exemplo de utilizacao de mensagens IPC com 2 processos 
   independentes hierarquicamente (Cliente, Servidor). 
   Executar primeiro (em "background" ou noutra janela) o programa 
   Servidor (este programa - "msgs"), e depois o programa 
   Cliente ("msgc").  -- CRA
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
  struct {
    long    mtype;      /* message type */
    int     id;
    char    mtext[TXTSIZE];   /* message text */
  } msg;

  if ((msqids=msgget(SERVKEY, 0666|IPC_CREAT)) < 0) {
    perror("Servidor: Erro a criar queue servidor");
    exit(-1);
  }

  printf("Servidor vai receber\n");

  if (msgrcv(msqids, &msg, sizeof(msg.mtext)+sizeof(msg.id), 0, 0) < 0) {
    perror("Servidor: erro a receber mensagem");
  }
  else {
    for (i=0; i<TXTSIZE; i++)
      printf("%c", msg.mtext[i]);

    if ((msqidc=msgget(msg.id, 0)) < 0) {
      perror("Servidor: Erro a associar a queue cliente");
    }
    else {
    	while (1){
	      printf("Servidor vai enviar\n");
	      for (i=0; i<TXTSIZE; i++)
	      	msg.mtext[i] = '\0';
	      fgets(msg.mtext, TXTSIZE, stdin);
	      /*for (i=0; i<TXTSIZE; i++){
		}*/
	      printf("\n");
	      if (msgsnd(msqidc, &msg, sizeof(msg.mtext)+sizeof(msg.id), 0) < 0) {
		perror("Servidor: erro a enviar mensagem");
	      }
    		
    	if(msg.mtext[0] == '!')	
    		return 0;
    	
    	}}
  
  }
  if (msgctl(msqids, IPC_RMID, 0) < 0) {
    perror("Servidor: Erro a eliminar queue servidor");
  }

  return 0;
}
