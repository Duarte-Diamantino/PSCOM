/* Exemplo de utilizacao do "fork" e do "execl" 
   para criar um processo. -- CRA
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
 

int main()
{
  pid_t pid;
  int status;

  printf("Ola'! Eu sou o pai. O meu pid=%d\n", getpid());

  if ((pid = fork()) == -1) { perror("fork"); return (-1); }
  if (pid == 0) {
    printf("Ola'! Eu sou o filho. O meu pid=%d\n", getpid());
    printf("O filho vai listar o directorio\n");
    execl("/bin/ls", "ls", "-l", NULL);
  }
  else {
    printf("Eu sou o pai. O filho foi criado com pid=%d\n", pid);
    printf("Pai: vou esperar\n");
    pid = wait(&status);
    printf("Pai: filho pid=%d terminou com status=%d\n", pid, status);
  }

  return 0;
}
