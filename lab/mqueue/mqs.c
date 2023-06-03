/* Exemplo de utilizacao de "queues" com 2 processos 
   independentes hierarquicamente (Cliente, Servidor). 
   Executar primeiro (em "background" ou noutra janela) o programa 
   Servidor (este programa - "mqs"), e depois o programa 
   Cliente ("mqc").  -- CRA
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#define SERVNAME "/SERV"
#define TXTSIZE 10

struct mq_attr ma;

int main()
{
  int mqids, mqidc, i;
  struct {
    char    id[10];
    char    mtext[TXTSIZE];   /* message text */
  } msg;

  ma.mq_flags = 0;
  ma.mq_maxmsg = 2;
  ma.mq_msgsize = sizeof(msg);
  
  if ((mqids=mq_open(SERVNAME, O_RDWR|O_CREAT, 0666, &ma)) < 0) {
    perror("Servidor: Erro a criar queue servidor");
    exit(-1);
  }

  printf("Servidor vai receber\n");

  if (mq_receive(mqids, (char *)&msg, sizeof(msg), NULL) < 0) {
    perror("Servidor: erro a receber mensagem");
  }
  else {
    for (i=0; i<TXTSIZE; i++)
      printf("%c", msg.mtext[i]);

    if ((mqidc=mq_open(msg.id, O_RDWR)) < 0) {
      perror("Servidor: Erro a associar a queue cliente");
    }
    else {
      printf("Servidor vai enviar\n");
      for (i=0; i<TXTSIZE; i++)
	msg.mtext[i] = 'S';
      if (mq_send(mqidc, (char *)&msg, sizeof(msg), 0) < 0) {
	perror("Servidor: erro a enviar mensagem");
      }
    }
  }
  if (mq_unlink(SERVNAME) < 0) {
    perror("Servidor: Erro a eliminar queue servidor");
  }

  return 0;
}
