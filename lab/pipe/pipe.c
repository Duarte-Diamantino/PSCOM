/* Exemplo de utilizacao do mecanismo de comunicacao "pipe" (2 processos 
   independentes). -- CRA
*/
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctype.h>
#include <semaphore.h>
#include <string.h>

#define DIM 50

sem_t sem1, sem2, sem3;

char buf[DIM];

/*apaga espacos*/
void *thread_func1(void * pi){
    int i, posicao;
    char aux[DIM];
    
    
    
    sem_wait(&sem1);
    
    for (i = 0, posicao = 0; i < strlen(buf); i++) {
        if (buf[i] == ' ') continue;
        aux[posicao] = buf[i];
        posicao++;
    }
    
    strcpy (buf, aux);
    
    sem_post(&sem2);
    
    return 0;

	}


/*mete em caps*/
void *thread_func2(void * pi){
{
  int i;

  printf("thread_func2: wait\n");
  sem_wait(&sem2);
  printf("thread_func2: processa\n");
  for (i=0; buf[i]; i++)
    buf[i] = toupper(buf[i]);
  for (i=0; buf[i]; i++)
   	printf("%c",buf[i]);
  printf("thread_func: terminar\n");
  sem_post(&sem3);
  return 0;
}
}

int main()
{
  pid_t pid;
  int fd1[2];
  int fd2[2];
  int n1, n2, i;
  pthread_t  thread1;
  pthread_t  thread2;
  
	if (sem_init(&sem1, 0, 0) != 0) {
      		printf("Erro a inicializar semaforo 1\n");
      	return -1;
  	}
  	if (sem_init(&sem2, 0, 0) != 0) {
      		printf("Erro a inicializar semaforo 2\n");
      	return -1;
  	}
  	if (sem_init(&sem3, 0, 0) != 0) {
      		printf("Erro a inicializar semaforo 2\n");
      	return -1;
  	}  	

	if (pipe(fd1) != 0) {
    		printf("Erro a criar pipe1\n"); exit(-1);
  	}
  	if (pipe(fd2) != 0) {
    		printf("Erro a criar pipe2\n"); exit(-1);
  	}
  
  	if ((pid = fork()) == -1) {
  		printf("Erro a criar processo\n"); exit(-1);
  	}
  
  	while(1){
  	if (pid == 0) { /*filho*/
  		printf("entra filho\n");
  		n2 = read(fd1[0], buf, sizeof(buf));
  		printf("esta a ler\n");
		printf("main: a criar thread1\n");
		if (pthread_create(&thread1, NULL, thread_func1, NULL) != 0) {
		    printf("Erro a criar thread\n");
		}
		printf("main: a criar thread2\n");
		if (pthread_create(&thread2, NULL, thread_func2, NULL) != 0) {
		    printf("Erro a criar thread\n");
		 }
		 
		 
		sem_post(&sem1);	/* assinala */
  		printf("espera pela thread 2\n");
  		sem_wait(&sem3);
  		printf("foi para a thread 2\n");
		write(fd2[1], buf, sizeof(buf));
			  	
  	}
	  else { /*pai*/
	  	printf("entra pai:\n");
  		fgets(buf, DIM, stdin);
  		printf("escreve no buffer\n");
  		printf("envia para o filho\n");
	    	write(fd1[1], buf, sizeof(buf));
	   	printf("volta para o pai e envia\n");
	   	for (i=0; buf[i]; i++)
   	printf("%c",buf[i]);
   		printf("\n");
	   	n1 = read(fd2[0], buf, sizeof(buf));
	   	for (i=0; buf[i]; i++)
   	printf("%c",buf[i]);
   		printf("\n");
	   	printf("o programa vai terminar: %s \n", buf);
	   	if(buf[0] == '!'){
			exit (0);
		}
	    
  	}}
  return 0;}
