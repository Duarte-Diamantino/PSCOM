/***************************************************************************
| File: mqc.c
|
| Autor: Duarte Diamantino e Francisca Rodrigues 
| Data:  Abril 2023
***************************************************************************/

#include "ctrl_acess.h"


/***************************************

Variaveis da queue para mandar comandos
			sobre o CTLP

***************************************/
char queename[10];
int mqids, mqidc, i;
struct mq_attr ma;
struct {
    char    id[TXTSIZE];
    char 	porta;
    char    mtext[10];
  } msg;
  
/***************************************

Variaveis da queue para mandar comandos
			sobre o CTLP

***************************************/
char queename2[10];
int mqids2, mqidc2, j;
struct mq_attr ma2;
struct {
    char    id[TXTSIZE];
    char    mtext[10];
  } msg2;
 

/***************************************

Variaveis globais do CTLP

***************************************/ 
char estado;  
char porta;
pthread_t  thread_ctlp;

int espera_seg;

/*--------------------------------------------------------------------------------------------------------------------------------------------------
* 
* 															FUNCIONAMENTO DA PORTA
*
*-----------------------------------------------------------------------------------------------------------------------------------------------+*/  	

/*-------------------------------------------------------------------------+
* 
* Funcao: abre_porta
*
+-------------------------------------------------------------------------*/

void my_handler(int sig) {
    printf("CTLP> processo:Signal %d received.\n", sig);
	espera_seg = 1;
}


void abre_porta(void){

	
	espera_seg = 0;

	printf("CTLP> abre porta\n");

	// Configurar um tratador de sinal para sigalarm
    signal(SIGALRM, my_handler);
	
	
	 // Agendar um alarme para 3 segundos
    alarm(3);

    // Esperar por sinais
    while(1){
     if(espera_seg == 1)
     	break;
    }
	
	espera_seg = 0;

}

/*-------------------------------------------------------------------------+
* 
* Funcao: verf_porta_local
*
+-------------------------------------------------------------------------*/  

void verf_porta_local (int argc, char** argv){
	printf("%s %s\n", argv[0], argv[1]);
	if(strlen(argv[1]) < 2){
			if(strcmp(argv[1], "A") == 0){
				porta = 'A';
				strcpy(queename, CTLA);
			}else{
				if(strcmp(argv[1], "B") == 0){
					porta = 'B';
				strcpy(queename, CTLB);	
				}else{
					if(strcmp(argv[1], "C") == 0){ 
						porta = 'C';
						strcpy(queename, CTLC);
					}else{
						printf(">>CTLP: Erro - porta errada\n");
					}
				}
			}		
		printf("\nCTLP> porta pode ser criada %s\n", queename);
		
	}
		
}

/*-------------------------------------------------------------------------+
* 
* Funcao: verf_porta_srv
*
+-------------------------------------------------------------------------*/  

void verf_porta_srv(void){
	
	
	
	if (mq_send(mqids, (char *)&msg, sizeof(msg), 0) < 0) {
    	perror("Cliente: erro a enviar mensagem");
  	}
		
	printf("Cliente vai receber\n");
    if (mq_receive(mqidc, (char *)&msg, sizeof(msg), NULL) < 0) {
    	perror("Cliente: erro a receber mensagem");
    }else {
		printf("%c", msg.porta);
    }

	if((strcmp(msg.mtext, "porta")== 0)){
		printf("porta criada\n");
	}
	if((strcmp(msg.mtext, "erro")== 0)){
		printf("porta nao criada\n");
		exit(0);
	}


}

/*-------------------------------------------------------------------------------------------------------+
* 
* Funcao: inicia_ctlp_intges: funcionamento da porta;;
*
+-------------------------------------------------------------------------------------------------------*/

void inicia_ctlp_intges(){

/*Atribuir os parametros do queue*/	
	ma.mq_flags = 0;
	ma.mq_maxmsg = 3;
	ma.mq_msgsize = sizeof(msg);

/*Inicia o queue da porta*/		
  	strcpy (msg.id, queename);
	
	printf("id:%s e %s\n",msg.id, queename);
	
  	if ((mqidc=mq_open(msg.id, O_RDWR|O_CREAT, 0666, &ma)) < 0) {
  		perror("Cliente: Erro a criar queue cliente");
  		exit(-1);
  	}
  	if ((mqids=mq_open(SERVQ, O_RDWR)) < 0) {
    	perror("Cliente: Erro a associar a queue servidor");
    	exit(-1);
  	}
}

/*-------------------------------------------------------------------------+
* 
* Funcao: env_srv_ctlp
*
+-------------------------------------------------------------------------*/

void env_srv_ctlp(char** argv){
	
	printf("Cliente vai enviar\n");
  	strncpy(msg.mtext , argv[0], sizeof(msg.id));
  	msg.porta = porta;
  	
  	printf("SRV> id:%s\n", msg.id);
	printf("SRV> porta:%c\n", msg.porta);	
	printf("SRV> text:%s\n", msg.mtext);
  	
    if (mq_send(mqids, (char *)&msg, sizeof(msg), 0) < 0) {
    	perror("Cliente: erro a enviar mensagem");
  }
}


/*-------------------------------------------------------------------------+
* 
* Funcao: recebe_srv_ctlp
*
+-------------------------------------------------------------------------*/

void recebe_srv_ctlp (void){
	strcpy(msg.mtext, "");
	printf("Cliente vai receber\n");
    
    printf("SRV> id:%s\n", msg.id);
	printf("SRV> porta:%c\n", msg.porta);	
	printf("SRV> text:%s\n", msg.mtext);
    
    if (mq_receive(mqidc, (char *)&msg, sizeof(msg), NULL) < 0) {
    	perror("Cliente: erro a receber mensagem");
    }else {
		printf("porta: %c\n", msg.porta);
		printf("acao: %s\n", msg.mtext);
		if(strcmp( "abre",msg.mtext)==0){
			abre_porta();			
		}
    	strcpy(msg.mtext, "");
    }
}


/*-------------------------------------------------------------------------+
* 
* Funcao: my_getline
*
+-------------------------------------------------------------------------*/

int my_getline (char** argv, int argvsize){
  static char line[MAX_LINE];
  char *p;
  int argc;

  fgets(line, MAX_LINE, stdin);

  for (argc=0,p=line; (*line != '\0') && (argc < argvsize); p=NULL,argc++) {
    p = strtok(p, " \t\n");
    argv[argc] = p;
    if (p == NULL) return argc;
  }
  argv[argc] = p;
  return argc;
}




/*-------------------------------------------------------------------------+
* 
* Funcao: func_porta
*
+-------------------------------------------------------------------------*/

void func_porta(int argc, char** argv){
	
/*Variaveis*/	
	static char *argv2[ARGVECSIZE+1], *p;
	int argc2;
	
/*Ciclo do funcionamento da porta*/	
	for (;;) {	
		printf("--------Escreva o seu <id>---------\n");
		printf("CTLP>>");		

/*le o id escrito no terminal*/
		if ((argc2 = my_getline(argv2, ARGVECSIZE)) > 0) {
			for (p=argv2[0]; *p != '\0'; *p=tolower(*p), p++);
/*se o id tem so 3 caracteres*/    		
    			if (strlen(argv2[0]) == 3){
/*se estado: A ou se id: IDX*/    			
    				if((strcmp(argv2[0], IDX)==0) || (estado == 'A')){
    					abre_porta();
    				}else{
/*se porta trancada (estado:F)*/    					
    					if (estado == 'F'){
    						printf("PORTA FECHADA\n");
    					}else{
/*testa id com o servidor*/    												
								env_srv_ctlp(argv2);	
								recebe_srv_ctlp();		
	   	  				}
	   	  			}	
	   	  		}else{  
						printf("CTLP> ID demaisado grande\n");
					}
				}
    	}
  	 
	    
	if (mq_unlink(msg.id) < 0) {
		perror("Cliente: Erro a eliminar queue cliente");
	}

}

/*--------------------------------------------------------------------------------------------------------------------------------------------------
* 
*													    COMANDOS DO INTGES: -CEP ; -MEP; TCTLP
*
*-----------------------------------------------------------------------------------------------------------------------------------------------+*/  	

/*-------------------------------------------------------------------------+
* 
* Funcao: inicia_ctlp_srv
*
+-------------------------------------------------------------------------*/

void inicia_ctlp_srv(){

	printf("CTLP>> incia a segunda queue\n");
	
/*Atribuir os parametros do queue*/	
	ma2.mq_flags = 0;
	ma2.mq_maxmsg = 2;
	ma2.mq_msgsize = sizeof(msg2);
	
/*Atribui o id da porta*/			
  	if(porta == 'A'){
  		strcpy(queename2, "/CLSERVA");  	
  	}else{
  		if(porta == 'B'){
  			strcpy(queename2, "/CLSERVB");  	
  		}else{
  			if(porta == 'C'){
  				strcpy(queename2, "/CLSERVC"); 	
  			}else{
  				printf("ERRO A ASSOCIAR PORTA\n");
  			}
  		}
  	}	
	
/*Inicia o queue da porta*/		

  	strcpy (msg2.id, queename2);
  	
  	if ((mqids2=mq_open(msg2.id,O_RDWR|O_CREAT, 0666, &ma2)) < 0) {
    	perror("Cliente: Erro a criar queue cliente");
    	exit(-1);
  	} 
  	  	  	
  	if ((mqidc2=mq_open("/CLSERV",  O_RDWR)) < 0) {
  		perror("Cliente: Erro a associar a queue servidor");
  		exit(-1);
  	}
  	
  	
}


/*-------------------------------------------------------------------------+
* Function: ctlpth 
+-------------------------------------------------------------------------*/ 

void *ctlpth(void *){
	
	inicia_ctlp_srv();
	
	while(1){
		if (mq_receive(mqids2, (char *)&msg2, sizeof(msg2), NULL) < 0) {
    		perror("Cliente: erro a receber mensagem");
    	}else {
			printf("CTLP> revebeu: %s", msg2.mtext);
    	}
		
		if(strcmp("cep", msg2.mtext)==0){
			strcpy(msg2.mtext, "A");
			msg2.mtext[0]=estado;
			printf("estado: %s\n", msg2.mtext);
			if (mq_send(mqidc2, (char *)&msg2, sizeof(msg2), 0) < 0) {
    					perror("Cliente: erro a enviar mensagem");
  					}
				}else{
					if(strcmp("tctl", msg2.mtext)==0){
						if (mq_unlink(queename) < 0) {
    						perror("Cliente: Erro a eliminar queue cliente");
  						}
						if (mq_unlink(queename2) < 0) {
    						perror("Servidor: Erro a eliminar queue servidor");
  						}
  						
						if (mq_unlink(SERVQ) < 0) {
    						perror("Servidor: Erro a eliminar queue servidor");
  						}
  						
  						if (mq_unlink("/CLSERV") < 0) {
    						perror("Servidor: Erro a eliminar queue servidor");
  						}
  						
						exit (0);
					}else{
							sscanf(msg2.mtext, "mep %s", msg2.mtext);
							estado = msg2.mtext[0];
							printf("estado %s", msg2.mtext);
							if (mq_send(mqidc2, (char *)&msg2, sizeof(msg2), 0) < 0) {
    							perror("Cliente: erro a enviar mensagem");
  							}	
						
					}	
				}		
	}
			
  
  return 0;
}

/*-------------------------------------------------------------------------+
| Function: lac 
+-------------------------------------------------------------------------*/ 

void lac(void){
	time_t t;
  	struct tm tm;
  	struct timespec ts;
  	char time [50];

  	clock_gettime(CLOCK_REALTIME, &ts);

  	t = ts.tv_sec;
  	memset(&tm, 0, sizeof(struct tm));
  	localtime_r (&t, &tm);
  	strftime(time, sizeof(time), "%d/%m/%Y %H:%M:%S", &tm);
  	printf ("Data:%s %ld \n", time, ts.tv_sec);

}

/*--------------------------------------------------------------------------------------------------------------------------------------------------
* 
* 														    MAIN E THREAD
*
*-----------------------------------------------------------------------------------------------------------------------------------------------+*/  	


/*-------------------------------------------------------------------------+
* 
* Funcao: inicia_thread
*
+-------------------------------------------------------------------------*/

void inicia_thread(void){
	
	if (pthread_create(&thread_ctlp, NULL, ctlpth, NULL) != 0) {
		printf("Erro a criar thread_integes\n");
    }
    
}

/*-------------------------------------------------------------------------+
* 
* Funcao: Main
*
+-------------------------------------------------------------------------*/

int main(int argc, char **argv){

	estado = 'N';
	
	printf("\nCTLP>> verifica se é possivel criar porta\n");
		verf_porta_local(argc, argv);
	
	printf("\nCTLP>> inicia ctlp-server para a porta\n");
		inicia_ctlp_intges();
	
	/*printf("\nCTLP>> verifica se é possivel criar porta\n");	
		verf_porta_srv();		
	*/
		
	printf("\nCTLP>> executa threads\n");
	  	inicia_thread();
	
	printf("\nCTLP>> funcionamento da porta\n");
		func_porta(argc, argv);
	

return 0;
}
