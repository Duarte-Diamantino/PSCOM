/* Exemplo de utilizacao do mecanismo de comunicacao "pipe" (2 processos 
   dependentes hierarquicamente (fork)). -- CRA
*/
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
  pid_t pid;
  int fd[2];
  int n;
  char buf[30];
  char msg[]="Filho comunica com pai";

  if (pipe(fd) != 0) {
    printf("Erro a criar pipe\n"); exit(-1);
  }

  if ((pid = fork()) == -1) {
    printf("Erro a criar processo\n"); exit(-1);
  }
  if (pid == 0) {
    close(fd[0]);
    printf("Ola'! Eu sou o filho. O meu pid=%d\n", getpid());
    printf("O filho vai enviar mensagem para o pai\n");
    write(fd[1], msg, sizeof(msg));
    exit(0);
  }
  else {
    close(fd[1]);
    printf("Eu sou o pai. O filho foi criado com pid=%d\n", pid);
    printf("Pai: vou esperar mensagem\n");
    n = read(fd[0], buf, sizeof(buf));
    printf("Pai: recebi msg. dim=%d info: %s \n", n, buf);
  }

  return 0;
}
