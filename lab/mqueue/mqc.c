/* Exemplo de utilizacao de "queues" com 2 processos 
   independentes hierarquicamente (Cliente, Servidor). 
   Executar primeiro (em "background" ou noutra janela) o programa 
   Servidor ("mqs"), e depois o programa 
   Cliente ("mqc").  -- CRA
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#define SERVNAME "/SERV"
#define TXTSIZE 10

char cliname[10];
struct mq_attr ma;

int main()
{
  int mqids, mqidc, i;
  int clipid;
  struct {
    char    id[10];
    char    mtext[TXTSIZE];   /* message text */
  } msg;

  ma.mq_flags = 0;
  ma.mq_maxmsg = 2;
  ma.mq_msgsize = sizeof(msg);
  
  clipid = getpid();
  snprintf(cliname, sizeof(cliname), "/CL-%05d", clipid);
  if ((mqidc=mq_open(cliname, O_RDWR|O_CREAT, 0666, &ma)) < 0) {
    perror("Cliente: Erro a criar queue cliente");
    exit(-1);
  }
  if ((mqids=mq_open(SERVNAME, O_RDWR)) < 0) {
    perror("Cliente: Erro a associar a queue servidor");
    exit(-1);
  }

  printf("Cliente vai enviar\n");
  strncpy(msg.id, cliname, sizeof(msg.id));
  for (i=0; i<TXTSIZE; i++)
    msg.mtext[i] = 'C';
  if (mq_send(mqids, (char *)&msg, sizeof(msg), 0) < 0) {
    perror("Cliente: erro a enviar mensagem");
  }
  else {
    printf("Cliente vai receber\n");

    if (mq_receive(mqidc, (char *)&msg, sizeof(msg), NULL) < 0) {
      perror("Cliente: erro a receber mensagem");
    }
    else {
      for (i=0; i<TXTSIZE; i++)
	printf("%c", msg.mtext[i]);
    }
  }
  if (mq_unlink(cliname) < 0) {
    perror("Cliente: Erro a eliminar queue cliente");
  }

  return 0;
}
