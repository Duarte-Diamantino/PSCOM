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
  int status = 0;

  printf("Ola'! Eu sou o pai. O meu pid=%d\n", getpid());
  	if ((pid = fork()) == -1) { perror("fork"); return (-1); }
  	if (pid == 0) {
  	  	printf("Ola'! Eu sou o filho. O meu pid=%d\n", getpid());
   		printf("O filho vai listar o directorio\n");
    		//execl("/bin/ls", "ls", "-l", NULL);
  	}	
	else {
	
  		pid = wait(&status);
    	printf("Eu sou o pai. O filho foi criado com pid=%d\n", pid);
    	printf("Pai: vou esperar\n");
    		printf("Pai: filho pid=%d terminou com status=%d\n", pid, status);
  	}
	printf("\nSou o %d\n", getpid());
  /*
  
  
  
  
  
 printf("Ola'! Eu sou o pai2. O meu pid=%d\n", getpid());
  	if ((pid = fork()) == -1) { perror("fork"); return (-1); }
  	if (pid == 0) {
  	  	printf("Ola'! Eu sou o filho2. O meu pid=%d\n", getpid());
   		printf("O filho2 vai listar o directorio\n");
    		//execl("/bin/ls", "ls", "-l", NULL);
  	}	
	else {
	
  		pid = wait(&status);
    	printf("Eu sou o pai2. O filho foi criado com pid=%d\n", pid);
    	printf("Pai2: vou esperar\n");
    		printf("Pai2: filho pid=%d terminou com status=%d\n", pid, status);
  	}
	printf("\nSou o %d\n", getpid());
 
 
 */
 
 
  return 0;
}
