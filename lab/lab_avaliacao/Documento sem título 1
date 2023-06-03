/* Exemplo de utilizacao do mecanismo de comunicacao "pipe" (2 processos 
   independentes). -- CRA
*/
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
  pid_t pid;
  int fd1[2];
  int fd2[2];
  int n1, n2;
  char buf[30];
  char msg1[]="ola tudo bem?";
  char msg2[]="sim esta";

  if (pipe(fd1) != 0) {
    printf("Erro a criar pipe\n"); exit(-1);
  }
  if (pipe(fd2) != 0) {
    printf("Erro a criar pipe\n"); exit(-1);
  }
	

  if ((pid = fork()) == -1) {
    printf("Erro a criar processo\n"); exit(-1);
  }
  if (pid == 0) {
    close(fd1[0]);
    write(fd1[1], msg1, sizeof(msg1));
    n2 = read(fd2[0],buf, sizeof(buf));    
    printf("pessoa2: recebi msg. dim=%d info: %s \n", n2, buf);
    
    exit(0);
  }
  else {
    close(fd1[1]);
    n1 = read(fd1[0], buf, sizeof(buf));
    printf("pesso1: recebi msg. dim=%d info: %s \n", n1, buf);
    write(fd2[1], msg2, sizeof(msg2));
    
  }

  return 0;
}
