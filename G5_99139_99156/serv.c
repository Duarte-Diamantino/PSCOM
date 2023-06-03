/***************************************************************************
| File: sdsrv.c
|
| Autor: Duarte Diamantino e Francisca Rodrigues 
| Data:  Abril 2023
***************************************************************************/
#include "ctrl_acess.h"

#define NCOMMANDS  (sizeof(commands)/sizeof(struct command_d))

/*Incializacoes*/
void cria_ficheiro(void);
void inicia_srv_cli(void);
void env_serv (int, char** );


/*Comandos a executar - interface*/
void cmd_cuti (void );
void cmd_luti (void );
void cmd_euti (void );
void cmd_muti (void );
void cmd_lac (void);
void cmd_tser (void );
void cmd_cep (void );
void cmd_mep (void );
void cmd_tctl (void );
void cmd_sos  (void); 
void cmd_sair (void);


char portas_abertas[3];

int portas_A_abertas = 0;
int portas_B_abertas = 0;
int portas_C_abertas = 0;

/***************************************

Variaveis da queue para abrir porta

***************************************/

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

int mqids2, mqidc2, j;
struct mq_attr ma2;
struct {
    char    id[TXTSIZE];
    char    mtext[10];   
  } msg2;
char queue1_id[10];


/***************************************

Estrutura usada para os comandos sobre o intges

***************************************/

struct 	command_d {
  void  (*cmd_fnct)(void);
  char*	cmd_name;
} const commands[] = {
	{cmd_cuti, "cuti"},
	{cmd_luti, "luti"},
	{cmd_euti, "euti"},
	{cmd_muti, "muti"},	
  	{cmd_lac, "lac"},	
  	{cmd_tser, "tser"},
  	{cmd_cep, "cep"},
  	{cmd_mep, "mep"},
	{cmd_tctl, "tctl"}
};

linha_cmd u1;

pthread_t  thread_integes;

struct sockaddr_un my_addr;
socklen_t addrlen;
struct sockaddr_un from;
socklen_t fromlen;

struct sockaddr_un my_addr;
socklen_t addrlen;
struct sockaddr_un to;
socklen_t tolen;


uti_t u, u_v[UMAX];
uti_t *pa;
int sd;
int mfd;
int linha_ficheiro = 0;


reg_t reg;
reg_t *pa2; 
int sd_reg;
int mfd_reg;
int linha_ficheiro_reg = 0;

pthread_mutex_t mux;

/*-------------------------------------------------------------------------+
* 
* Funcao: cria_ficheiro
*
+-------------------------------------------------------------------------*/
void cria_ficheiro(void){
	linha_ficheiro = 0;

/* abrir/criar ficheiro */
	if ((mfd=open(FUTI, O_RDWR|O_CREAT, 0666 )) < 0) {  
	    perror("Erro a criar ficheiro");
	    exit(-1);
	}else {
/* definir tamanho do ficheiro */
		if (ftruncate(mfd, UMAX*sizeof(u)) < 0) {  
      		perror("Erro no ftruncate");
      		exit(-1);
    	}
  	}
/* mapear ficheiro */
  	if ((pa=mmap(NULL, sizeof(u), PROT_READ|PROT_WRITE, MAP_SHARED, mfd, 0)) < (uti_t *)0) {
    	perror("Erro em mmap");
    	exit(-1);
  	}
/*Mostra conteudo no ficheiro*/  	
  	while((strcmp(pa[linha_ficheiro].id, "") != 0)){
		printf("%d	",linha_ficheiro);
		printf( "%s	",pa[linha_ficheiro].id);
		printf( "%s	",pa[linha_ficheiro].nome);
		printf( "%s\n",pa[linha_ficheiro].port);
		linha_ficheiro++;
	} 
	printf("	linhas do ficheiro: %d\n", linha_ficheiro);	


}

/*-------------------------------------------------------------------------+
* 
* Funcao: cria_ficheiro_reg
*
+-------------------------------------------------------------------------*/
void cria_ficheiro_reg(void){
	
	time_t t1;
    time_t t2; 
    double diff;
    struct tm tm1;
    struct tm tm2;
    char time1[50];
    char time2[50];	

	linha_ficheiro_reg = 0;
	
	/* abrir / criar ficheiro */
	if ((mfd_reg=open(FREG, O_RDWR|O_CREAT, 0666 )) < 0) {  
	    perror("Erro a criar ficheiro");
	    exit(-1);
	}else {
		/* definir tamanho do ficheiro */
    	 if (ftruncate(mfd_reg, NREG*sizeof(reg)) < 0) {  
      		perror("Erro no ftruncate");
      		exit(-1);
    	}
  	}
  	/* mapear ficheiro */
  	if ((pa2=mmap(NULL, sizeof(reg), PROT_READ|PROT_WRITE, MAP_SHARED, mfd_reg, 0)) < (reg_t *)0) {
    	perror("Erro em mmap");
    	exit(-1);
  	}

	/*Mostra conteudo no ficheiro*/
           while((strcmp(pa2[linha_ficheiro_reg].id, "") != 0)){
            printf("%d    ",linha_ficheiro);
            printf("%s    ",pa2[linha_ficheiro_reg].id);
            printf("%c\n",pa2[linha_ficheiro_reg].p);
            linha_ficheiro_reg++;
    }

	if(linha_ficheiro_reg==NREG){
		while((strcmp(pa2[linha_ficheiro_reg].id, "") != 0)){
            printf("%d    ",linha_ficheiro);
            printf("%s    ",pa2[linha_ficheiro_reg].id);
            printf("%c\n",pa2[linha_ficheiro_reg].p); 

              t1 = pa2[linha_ficheiro_reg].t.tv_sec;
              memset(&tm1, 0, sizeof(struct tm));
              localtime_r (&t1, &tm1);
              strftime(time1, sizeof(time1), "%d/%m/%Y %H:%M:%S", &tm1);
              printf("Data:%s\n", time1);

            t2 = pa2[linha_ficheiro_reg+1].t.tv_sec;
              memset(&tm2, 0, sizeof(struct tm));
              localtime_r (&t2, &tm2);
              strftime(time2, sizeof(time2), "%d/%m/%Y %H:%M:%S", &tm2);
              printf("Data:%s\n", time2);

/*Ajustar os anos e os meses para o formato esperado pela struct tm*/
            tm1.tm_year -= 1900; 
            tm1.tm_mon -= 1;    
            
            tm2.tm_year -= 1900; 
            tm2.tm_mon -= 1;     

/*Converter as estruturas em valores de tempo em segundos*/
            t1 = mktime(&tm1);
            t2 = mktime(&tm2);

/* Calcular a diferença em segundos */
            diff = difftime(t2, t1);
            
/*  */
    	if (diff < 0){
            printf("encontrada evidencia de buffer circular\n");
            printf("linhas do ficheiro: %d\n", linha_ficheiro_reg);
            return;
        	}
            linha_ficheiro_reg++;
   	 	} 
	}

	printf("	linhas do ficheiro: %d\n", linha_ficheiro_reg);

}
  	 
/*-------------------------------------------------------------------------+
* 
* Funcao: inicia_srv_intges
*
+-------------------------------------------------------------------------*/
void inicia_srv_intges(void){
	if ((sd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0 ) {
    		perror("Erro a criar socket"); exit(-1);
  	}    
  	
  	my_addr.sun_family = AF_UNIX;
  	memset(my_addr.sun_path, 0, sizeof(my_addr.sun_path));
  	strcpy(my_addr.sun_path, SERVS);
  	addrlen = sizeof(my_addr.sun_family) + strlen(my_addr.sun_path);

  	if (bind(sd, (struct sockaddr *)&my_addr, addrlen) < 0 ) {
    		perror("Erro no bind"); exit(-1);
  	}
  
  	fromlen = sizeof(from);
  	if (recvfrom(sd, &u1, sizeof(u1), 0, (struct sockaddr *)&from, &fromlen) < 0) {
    		perror("Erro no recvfrom");
  	}
  	printf("RCBINTG----------------------------------------\n");
	printf("\nSRV> recebeu de int.ges\n");
	printf("comando: %s\nid: %s\nnome: %s\nportas: %s\n", u1.cmd, u1.id, u1.nome, u1.portas);
  	
}

/*-------------------------------------------------------------------------+
* 
* Funcao: env_ctlp
*
+-------------------------------------------------------------------------*/
void env_ctlp(void){
	printf("\nENVCTLP----------------------------------------\n");	
	printf("SRV> id:%s\n", msg.id);
	printf("SRV> porta:%c\n", msg.porta);	
	printf("SRV> text:%s\n", msg.mtext);
	
    if ((mqidc=mq_open(msg.id, O_RDWR)) < 0) {
      perror("Servidor: Erro a associar a queue cliente");
    }
    else {
     	if (mq_send(mqidc, (char *)&msg, sizeof(msg), 0) < 0) {
			perror("Servidor: erro a enviar mensagem");
     	}
    	strcpy(msg.mtext,"");
    
    }
}

/*-------------------------------------------------------------------------+
* 
* Funcao: rcb_ctlp
*
+-------------------------------------------------------------------------*/
void rcb_ctlp(void){
	printf("\nRCBCTLP----------------------------------------\n");	
	
    if ((mqids=mq_open(SERVQ, O_RDWR|O_CREAT, 0666, &ma)) < 0) {
    	perror("Servidor: Erro a criar queue servidor");
    	exit(-1);
  	}
  	if (mq_receive(mqids, (char *)&msg, sizeof(msg), NULL) < 0) {
    	perror("Servidor: erro a receber mensagem");
  	}else{      	
    	printf("SRV> id:%s\n", msg.id);
		printf("SRV> porta:%c\n", msg.porta);	
		printf("SRV> text:%s\n", msg.mtext);
	}
}

/*-------------------------------------------------------------------------+
* 
* Funcao: srv_vrfporta1
*
+-------------------------------------------------------------------------*/
void srv_vrfporta1(void){
	
	printf("SRV> Verfica se porta pode ser criada srv 1\n");
	rcb_ctlp();
	for(int i = 0 ; i < 3; i++){
		if(portas_abertas[i] == msg.porta){
			printf("SRV> erro porta ja existe\n");
			strcpy(msg.mtext, "erro");
			printf("\nENVCTLP----------------------------------------\n");	
			printf("SRV> id:%s\n", msg.id);
			printf("SRV> porta:%c\n", msg.porta);	
			printf("SRV> text:%s\n", msg.mtext);
	
    		if ((mqidc=mq_open(msg.id, O_RDWR)) < 0) {
      			perror("Servidor: Erro a associar a queue cliente");
    		}else{
      			if (mq_send(mqidc, (char *)&msg, sizeof(msg), 0) < 0) {
					perror("Servidor: erro a enviar mensagem");
      			}
    		}
		}	
	} 
		if(msg.porta == 'A'){
			portas_abertas[0] = 'A';
		}
	
		if(msg.porta == 'B'){
			portas_abertas[1] = 'B';
		}
	
		if(msg.porta == 'C'){
			portas_abertas[2] = 'C';
		}
		portas_abertas[3]= ' ';
	printf("Porta %c criada com sucesso :: portas %s existe,\n", msg.porta, portas_abertas);
	
	strcpy(msg.mtext, "porta");
	env_ctlp();
}


/*-------------------------------------------------------------------------+
* 
* Funcao: srv_vrfporta2
*
+-------------------------------------------------------------------------*/
void srv_vrfporta2(void){
	time_t t;
  	struct tm tm;
  	char time [50];


/*comeca a verificacao do id*/	
	rcb_ctlp();
	pthread_mutex_lock(&mux);
	for(i = 0; i< linha_ficheiro; i++){
		/*Testa se o id existe*/
		if(strcmp(msg.mtext, pa[i].id) == 0){				
			printf("\nSRV> <id> encontrado -> pode abrir a porta\n");
			strcpy(reg.id, msg.mtext);
			
			if(msg.porta == 'A'){
				if(pa[i].port[0] == '0'){
					strcpy(msg.mtext, "nao abre");
					pthread_mutex_unlock(&mux);
					break;
				}
			}
			
			if(msg.porta == 'B'){
				if(pa[i].port[1] == '0'){
					strcpy(msg.mtext, "nao abre");
					pthread_mutex_unlock(&mux);
					break;
				}
			}
			
			if(msg.porta == 'C'){
				if(pa[i].port[2] == '0'){
					strcpy(msg.mtext, "nao abre");
					pthread_mutex_unlock(&mux);
					break;
				}
			}			
			
/*carrega na queue o "abre" e regista a porta na estrura do ficheiro*/		
			strcpy(msg.mtext, "abre");
  			reg.p = msg.porta; 
/*coloca a hora no ficheiro dos registos*/			
  			clock_gettime(CLOCK_REALTIME, &reg.t);			
  			t = reg.t.tv_sec;
  			memset(&tm, 0, sizeof(struct tm));
  			localtime_r (&t, &tm);
  			strftime(time, sizeof(time), "%d/%m/%Y %H:%M:%S", &tm);
  			printf ("Data:%s %ld \n", time,  reg.t.tv_sec);
/*guarda a estrutura no ficheiro*/
  			pa2[linha_ficheiro_reg]= reg;
  			reg = pa2[linha_ficheiro_reg];

/*em caso de ultima linha, a proxima sera a primeira (buffer circular)*/
			if(linha_ficheiro_reg == NREG-1){
                linha_ficheiro_reg = 0;
                printf("linha de reg: %d\n",linha_ficheiro_reg);
				pthread_mutex_unlock(&mux);
                break;
        	}

  			linha_ficheiro_reg++;	
  			printf("linha de reg: %d\n",linha_ficheiro_reg);
			pthread_mutex_unlock(&mux);

    		break;
		}

		pthread_mutex_unlock(&mux);	
	}
	
	env_ctlp();

}

/*-------------------------------------------------------------------------+
* 
* Funcao: inicia_srv_ctlp1
*
+-------------------------------------------------------------------------*/	
void inicia_srv_ctlp1(){
	printf("SRV> porcesso das portas:\n");
/*inicia a queue*/
	ma.mq_flags = 0;
	ma.mq_maxmsg = 3;
	ma.mq_msgsize = sizeof(msg);
/*atribui o id do cliente*/	
	strcpy (msg.id, CTLA);	 

	printf("verifica porta\n");
	srv_vrfporta1();

	while(1){
		srv_vrfporta2();	
  	}	

  
  if (mq_unlink(SERVQ) < 0) {
    perror("Servidor: Erro a eliminar queue servidor");
  }
}

/*-------------------------------------------------------------------------+
* 
* Funcao: cmd_cuti - cria utilizador
*
+-------------------------------------------------------------------------*/
void cmd_cuti (void){
	int i;
	printf("\nSRV> executa <cuti>;; ja existem %d utilizadores\n", linha_ficheiro);
	
/*bloqueia o mutex*/
		pthread_mutex_lock(&mux);

/*Testa se o id existe*/	
	for(i = 0; i<= linha_ficheiro; i++){
		if(strcmp(u1.id, pa[i].id) == 0){				
			printf("\nSRV> ja existe o <id>\n");
			strcpy(u1.cmd, "erro");
			if (sendto(sd, &u1, sizeof(u1), 0, (struct sockaddr *)&from, fromlen) < 0) {
      			perror("Erro no sendto");
    		}
			printf("\nSRV> desbloqueia o mutex\n");
				pthread_mutex_unlock(&mux);
    		return;
		}
	}
/*corre as portas e se tem acesso mete 1, caso nao tem mete 0*/	
		for(int j = 0; j < 3; j++){
  				if ((u1.portas[j] == 'A' ) ||  (u1.portas[j] == 'B' ) || (u1.portas[j] == 'C' )){
						u.port[j] = '1';
					}else{
						u.port[j] = '0';
					}
			}
/*mete os argumentos que vieram no integes para a estutura de uti. do registo*/			
			strcpy( u.id , u1.id);
			strcpy( u.nome , u1.nome);
			printf("%s\n%s\n%s\n", u.id, u.nome, u.port);
/*escreve no ficheiro e incrementa linha*/	
  			pa[linha_ficheiro]= u;
			linha_ficheiro++;
/*da ordem para acabar o comando no intges*/			
			printf("SRV> comando executado com sucesso\n");
			strcpy(u1.cmd, "comando executado com sucesso"); 
			if (sendto(sd, &u1, sizeof(u1), 0, (struct sockaddr *)&from, fromlen) < 0) {
      			perror("Erro no sendto");
    		}

/*desbloqueia o mutex*/
	pthread_mutex_unlock(&mux);   		
    		
return;
}
/*-------------------------------------------------------------------------+
* Function: cmd_luti - lista o ficheiro utilizadores.dat
+-------------------------------------------------------------------------*/ 
void cmd_luti (void){
	int i=0;
		
	printf("\nSRV> executa <luti>\n");
/*bloqueia o mutex*/
		pthread_mutex_lock(&mux);
	
/*executa a lista completa em caso de luti 0*/	
	if(strcmp(u1.id, "0") == 0){
		for(i = 0; i< linha_ficheiro; i++){	
			printf("%s %s %s\n", pa[i].id,  pa[i].nome,  pa[i].port);
			strcpy(u1.id, pa[i].id);
			strcpy(u1.nome, pa[i].nome);
			strcpy(u1.tempo1, "");
			strcpy(u1.tempo2, "");
			strcpy(u1.estado, "");
/*converte o 1/0 do ficheiro para portas em letras*/			
			if (pa[i].port[0] == '1'){
				u1.portas[0] = 'A'; 
			}else{
				u1.portas[0] = 'a'; 
				}		
			if (pa[i].port[1] == '1'){
				u1.portas[1] = 'B'; 
			}else{
				u1.portas[1] = 'b'; 
			}
			if (pa[i].port[2] == '1' ){
				u1.portas[2] = 'C'; 
			}else{
				u1.portas[2] = 'c'; 
			}		
			u1.portas[3] = ' ';
/*envia a estrutura para o intges*/
			if (sendto(sd, &u1, sizeof(u1), 0, (struct sockaddr *)&from, fromlen) < 0) {
      			perror("Erro no sendto");
    		}
		}
/*da ordem para acabar o comando no intges*/			
		strcpy(u1.cmd, "comando executado com sucesso"); 
		if (sendto(sd, &u1, sizeof(u1), 0, (struct sockaddr *)&from, fromlen) < 0) {
      		perror("Erro no sendto");
    	}	
		
	}else{
/*executa a lista completa em caso de luti <id>*/	
		for(i = 0; i<linha_ficheiro; i++){
			if(strcmp(pa[i].id, u1.id) == 0){				
				printf("%s %s %s\n", pa[i].id,  pa[i].nome,  pa[i].port);
				strcpy(u1.id, pa[i].id);
				strcpy(u1.nome, pa[i].nome);
				strcpy(u1.tempo1, "");
				strcpy(u1.tempo2, "");
				strcpy(u1.estado, "");
/*converte o 1/0 do ficheiro para portas em letras*/			
				if (pa[i].port[0] == '1'){
					u1.portas[0] = 'A'; 
				}else{
					u1.portas[0] = 'a'; 
				}		
				if (pa[i].port[1] == '1'){
					u1.portas[1] = 'B'; 
				}else{
					u1.portas[1] = 'b'; 
				}
				if (pa[i].port[2] == '1' ){
					u1.portas[2] = 'C'; 
				}else{
					u1.portas[2] = 'c'; 
				}	
/*envia a estrutura para o intges*/
				if (sendto(sd, &u1, sizeof(u1), 0, (struct sockaddr *)&from, fromlen) < 0) {
      			perror("Erro no sendto");
    			}
			}	
		}
/*da ordem para acabar o comando no intges*/	
		strcpy(u1.cmd, "comando executado com sucesso"); 
		if (sendto(sd, &u1, sizeof(u1), 0, (struct sockaddr *)&from, fromlen) < 0) {
      		perror("Erro no sendto");
    	}
	}

/*desbloqueia o mutex*/
		pthread_mutex_unlock(&mux);

}
/*-------------------------------------------------------------------------+
* Function: cmd_euti - elimina utilizador
+-------------------------------------------------------------------------*/ 
void cmd_euti (void){
	int i, apagou_id = 0;
	printf("\nSRV>> executa <euti>\n");
/*bloqueia o mutex*/
		pthread_mutex_lock(&mux);
		
/*apaga todos os utilizadores no caso do euti 0*/
	if(strcmp(u1.id, "0") == 0){
		
		printf("\nSRV> apaga o ficheiro\n");	
			memset(u.id , '\0', sizeof(u.id));
			memset(u.nome , '\0', sizeof(u.nome));
			memset(u.port, 0, sizeof(u.port));
		
    		linha_ficheiro = 0;	
	}else{
/*apaga o utilizadores associado ao id*/	
		printf("\nSRV> percorre o ficheiro\n");
		for(i = 0; i<linha_ficheiro; i++){
			if(strcmp(pa[i].id, u1.id) == 0){				
				printf("\nSRV> apaga o utilizador com o id\n");
					memset(u.id , '\0', sizeof(u.id));
					memset(u.nome , '\0', sizeof(u.nome));
					memset(u.port, 0, sizeof(u.port));	
					pa[i]= u;
					apagou_id = 1;
			}	
/*puxa as linhas do ficheiro para cima*/			
			if(apagou_id ==1){
				pa[i] = pa[i+1];
			}		
		}
    	linha_ficheiro--;
	}
/*da ordem para acabar o comando no intges*/	
	strcpy(u1.cmd, "comando executado com sucesso"); 
		if (sendto(sd, &u1, sizeof(u1), 0, (struct sockaddr *)&from, fromlen) < 0) {
      		perror("Erro no sendto");
    	}

/*desbloqueia o mutex*/
		pthread_mutex_unlock(&mux);
 
}
/*-------------------------------------------------------------------------+
| Function: cmd_muti - modifica as portas do utilizador
+-------------------------------------------------------------------------*/ 
void cmd_muti (void)
{
	printf("\nSRV> executa <muti>\n");
/*bloqueia o mutex*/	
		pthread_mutex_lock(&mux);
		
/*procura no ficheiro o id*/		
		for(i = 0; i<linha_ficheiro; i++){
			if(strcmp(pa[i].id, u1.id) == 0){				
/*atraves das portas escreve diretamente no ficheiro*/				
				for(int j = 0; j < 3; j++){
  					if ((u1.portas[j] == 'A' ) ||  (u1.portas[j] == 'B' ) || (u1.portas[j] == 'C' )){
						pa[i].port[j] = '1';
					}else{
						pa[i].port[j] = '0';

					}
				}
				printf("SRV > muda para portas para%s\n", pa[i].port);
				if (sendto(sd, &u1, sizeof(u1), 0, (struct sockaddr *)&from, fromlen) < 0) {
      				perror("Erro no sendto");
    			}
    		}
		}
	strcpy(u1.cmd, "comando executado com sucesso"); 
		if (sendto(sd, &u1, sizeof(u1), 0, (struct sockaddr *)&from, fromlen) < 0) {
      		perror("Erro no sendto");
    	}	
	

/*desbloqueia o mutex*/
		pthread_mutex_unlock(&mux);	
}
/*-------------------------------------------------------------------------+
| Function: cmd_lac - lista os acessos às portas
+-------------------------------------------------------------------------*/ 
void cmd_lac (void){
	time_t t;
	time_t time1;
    time_t time2;
    time_t time3; 
	double diff1, diff2;
	struct tm t1 = {0}; // Inicializar a estrutura com zeros
	struct tm t2 = {0}; // Inicializar a estrutura com zeros
  	struct tm t3 = {0}; // Inicializar a estrutura com zeros
  	struct tm tm;
  	char time [50];
  	char portas[4];
  	char id[4];
	
	printf("\nSRV> executa <lac>\n");	
	
	printf("%s\n", u1.tempo1);
	printf("%s\n", u1.tempo2);
	
	
	strcpy(id, u1.id);
	strcpy(portas, u1.portas);

	for(i = 0; i < linha_ficheiro_reg; i++){
		reg = pa2[i];
	
		printf("id:%s	", reg.id);
  		printf("porta:%c	", reg.p);
  		
  
  		t = reg.t.tv_sec;
  		memset(&tm, 0, sizeof(struct tm));
  		localtime_r (&t, &tm);
  		strftime(time, sizeof(time), "%d/%m/%Y %H:%M:%S", &tm);
  		printf("Data:%s\n", time);
	
		
		printf("id:%s || portas:%s\n", portas, id);
		
		printf("reg.id:%s || reg.portas:%c\n", reg.id, reg.p);
		
		

    // Converter a hora para a struct tm
    sscanf(u1.tempo1, "%d/%d/%d %d:%d:%d", &t1.tm_mday, &t1.tm_mon, &t1.tm_year, &t1.tm_hour, &t1.tm_min, &t1.tm_sec);
	sscanf(u1.tempo2, "%d/%d/%d %d:%d:%d", &t2.tm_mday, &t2.tm_mon, &t2.tm_year, &t2.tm_hour, &t2.tm_min, &t2.tm_sec);
	sscanf(time, "%d/%d/%d %d:%d:%d", &t3.tm_mday, &t3.tm_mon, &t3.tm_year, &t3.tm_hour, &t3.tm_min, &t3.tm_sec);

    // Imprimir a struct tm
   	
    t1.tm_year -= 1900; // Ajustar o ano para o formato esperado pela struct tm
    t1.tm_mon -= 1;     // Ajustar o mês para o formato esperado pela struct tm

	t2.tm_year -= 1900; // Ajustar o ano para o formato esperado pela struct tm
    t2.tm_mon -= 1;     // Ajustar o mês para o formato esperado pela struct tm
		
		
	// Converter as estruturas em valores de tempo em segundos
    	time1 = mktime(&t1);
    	time2 = mktime(&t2);
    	time3 = mktime(&tm);

    // Calcular a diferença em segundos
    diff1 = difftime(time3, time1);	
	diff2 = difftime(time2, time3);
		
	if (diff1 > 0){
        	printf("t3 é maior que t1 por %.0f segundos\n", diff1);
    		strcpy(u1.tempo1, "0");
    	}else{ 
    		if (diff1 < 0){
        		printf("t1 é maior que t3 por %.0f segundos\n", -diff1);
    		}else{
        		printf("t1 e t3 são iguais\n");
    		}
		}

	
	if (diff2 > 0){
     		printf("t2 é maior que t3 por %.0f segundos\n", diff2);
    		strcpy(u1.tempo2, "0");	
    	}else{
    		if(diff2 < 0) {
        		printf("t2 é maior que t3 por %.0f segundos\n", -diff2);
    		}else{	
    		    printf("t2 e t3 são iguais\n");
    		}
    	}	
	
		printf("tempo1: %s tempo2: %s\n", u1.tempo1 ,u1.tempo2);
		
		if((strcmp(reg.id, id) == 0) || (strcmp( "0", id) == 0)){
			if((strchr(portas, reg.p) != NULL) || (strcmp( "0", portas) == 0)){
				printf("ola %s ola:%s\n", u1.tempo1, u1.tempo2);
				if((strcmp("0", u1.tempo1) == 0) && (strcmp("0", u1.tempo2) == 0)){
		/*Atribui var*/		
					printf("manda\n");
					strcpy(u1.portas,"");
					u1.portas[0] = reg.p;
					strcpy(u1.id, reg.id);
					strcpy(u1.nome, time);
					strcpy(u1.cmd, "");
			/*manda para o integes*/	
					if (sendto(sd, &u1, sizeof(u1), 0, (struct sockaddr *)&from, fromlen) < 0) {
      					perror("Erro no sendto");
    				}		
				}
			}		
		}
	}

	strcpy(u1.cmd, "comando executado com sucesso"); 
	printf("manda: %s\n", u1.cmd );	
		if (sendto(sd, &u1, sizeof(u1), 0, (struct sockaddr *)&from, fromlen) < 0) {
      		perror("Erro no sendto");
    	}
	


}


/*-------------------------------------------------------------------------+
* Function: cmd_tser - termina servidor 
+-------------------------------------------------------------------------*/ 
void cmd_tser (void){
	printf("\nSRV> executa <tser>\n");
	strcpy(u1.cmd, "comando executado com sucesso"); 
	if (sendto(sd, &u1, sizeof(u1), 0, (struct sockaddr *)&from, fromlen) < 0) {
    	perror("Erro no sendto");
    }
	close(sd);
	unlink(SERVS);
	exit (0);
}
void inicia_queue2_cli(void){
	/*Atribui as variaveis  para a queue*/	
	ma2.mq_flags = 0;
	ma2.mq_maxmsg = 2;
	ma2.mq_msgsize = sizeof(msg2);
	
  	if(strcmp(u1.portas, "A") == 0){
  		strcpy(queue1_id, "/CLSERVA");  	
  	}else{
  		if(strcmp(u1.portas, "B") == 0){
  			strcpy(queue1_id, "/CLSERVB");  	
  		}else{
  			if(strcmp(u1.portas, "C") == 0){
  				strcpy(queue1_id, "/CLSERVC"); 	
  			}else{
  				printf("ERRO A ASSOCIAR PORTA\n");
  			}
  		}
  	}	
	

/*copia as informações para a queue*/		
	strcpy (msg2.id, queue1_id);	 
	
/*associa a queen do lado do cli e do srv*/		 	  	
  	if ((mqidc2=mq_open("/CLSERV", O_RDWR|O_CREAT, 0666, &ma2)) < 0) {
  		perror("Cliente: Erro a criar queue cliente");
  		exit(-1);
  	}
  	if ((mqids2=mq_open(msg2.id, O_RDWR)) < 0) {
    	perror("Cliente: Erro a associar a queue servidor");
    	exit(-1);
  	}

}


/*-------------------------------------------------------------------------+
* Function: cmd_cep - consulta o estado da porta 
+-------------------------------------------------------------------------*/ 
void cmd_cep (void){
	printf("SRV> executa cep\n");	

	inicia_queue2_cli();
	
	strcpy(msg2.mtext, "cep\0");	

	
	
/*envia a informação para a queue*/	
      if (mq_send(mqids2, (char *)&msg2, sizeof(msg2), 0) < 0) {
	perror("Servidor: erro a enviar mensagem");
      }
/*recebe a informação para a queue*/      
      if (mq_receive(mqidc2, (char *)&msg2, sizeof(msg2), NULL) < 0) {
    			perror("Cliente: erro a receber mensagem");
    		}else {
				printf("SRV> estado: %s\n", msg2.mtext);
    		}
    
		
/*recebe da queue e mete na estrutura do socket*/	 
	strcpy(u1.nome, msg2.mtext); 
	printf("SRV> envia o estado %s (%s)\n", msg2.mtext, u1.cmd);
      	
	if (sendto(sd, &u1, sizeof(u1), 0, (struct sockaddr *)&from, fromlen) < 0) {
    	perror("Erro no sendto");
    }
	    
    strcpy(u1.cmd, "comando executado com sucesso");
    
	if (sendto(sd, &u1, sizeof(u1), 0, (struct sockaddr *)&from, fromlen) < 0) {
    	perror("Erro no sendto");
    }

}
/*-------------------------------------------------------------------------+
| Function: cmd_mep - mofificar estado da porta
+-------------------------------------------------------------------------*/ 
void cmd_mep (void)
{
	printf("SRV> executa mep\n");	

/*Atribui as variaveis  para a queue*/	
	ma2.mq_flags = 0;
	ma2.mq_maxmsg = 2;
	ma2.mq_msgsize = sizeof(msg2);
	
	printf("id: %s\n", u1.portas);
	
  	if(strcmp(u1.portas, "A") == 0){
  		strcpy(queue1_id, "/CLSERVA");  	
  	}else{
  		if(strcmp(u1.portas, "B") == 0){
  			strcpy(queue1_id, "/CLSERVB");  	
  		}else{
  			if(strcmp(u1.portas, "C") == 0){
  				strcpy(queue1_id, "/CLSERVC"); 	
  			}else{
  				printf("ERRO A ASSOCIAR PORTA\n");
  			}
  		}
  	}	
	
/*mostra o id da porta a usar*/	
	printf("id: %s\n", queue1_id);

/*copia as informações para a queue*/		
	strcpy (msg2.id, queue1_id);	 
	sprintf(msg2.mtext, "mep %s", u1.estado);	
	
/*associa a queen do lado do cli e do srv*/		 	  	
  	if ((mqidc2=mq_open("/CLSERV", O_RDWR|O_CREAT, 0666, &ma2)) < 0) {
  		perror("Cliente: Erro a criar queue cliente");
  		exit(-1);
  	}
  	if ((mqids2=mq_open(msg2.id, O_RDWR)) < 0) {
    	perror("Cliente: Erro a associar a queue servidor");
    	exit(-1);
  	}
	
/*envia a informação para a queue*/	
    	if (mq_send(mqids2, (char *)&msg2, sizeof(msg2), 0) < 0) {
			perror("Servidor: erro a enviar mensagem");
     	}
/*recebe a informação para a queue*/      
      	if (mq_receive(mqidc2, (char *)&msg2, sizeof(msg2), NULL) < 0) {
    			perror("Cliente: erro a receber mensagem");
    	}
		
/*recebe da queue e mete na estrutura do socket*/	 
	strcpy(u1.nome, msg2.mtext); 
	printf("SRV> envia o estado %s (%s)\n", msg2.mtext, u1.cmd);
      	
	if (sendto(sd, &u1, sizeof(u1), 0, (struct sockaddr *)&from, fromlen) < 0) {
    	perror("Erro no sendto");
    }
	    
    strcpy(u1.cmd, "comando executado com sucesso");
    
	if (sendto(sd, &u1, sizeof(u1), 0, (struct sockaddr *)&from, fromlen) < 0) {
    	perror("Erro no sendto");
    }

}
/*-------------------------------------------------------------------------+
| Funcao: cmd_tctl - terminar controlador
+-------------------------------------------------------------------------*/ 
void cmd_tctl (void){
	printf("\nSRV> executa <tctl>");

/*Atribui as variaveis  para a queue*/	
	ma2.mq_flags = 0;
	ma2.mq_maxmsg = 2;
	ma2.mq_msgsize = sizeof(msg2);
	
	printf("id: %s\n", u1.portas);
	
  	if(strcmp(u1.portas, "A") == 0){
  		strcpy(queue1_id, "/CLSERVA");  	
  	}else{
  		if(strcmp(u1.portas, "B") == 0){
  			strcpy(queue1_id, "/CLSERVB");  	
  		}else{
  			if(strcmp(u1.portas, "C") == 0){
  				strcpy(queue1_id, "/CLSERVC"); 	
  			}else{
  				printf("ERRO A ASSOCIAR PORTA\n");
  			}
  		}
  	}	
	
/*mostra o id da porta a usar*/	
	printf("id: %s\n", queue1_id);

/*copia as informações para a queue*/		
	strcpy (msg2.id, queue1_id);	
	sprintf(msg2.mtext, "tctl");	
	
/*associa a queen do lado do cli e do srv*/		 	  	
  	if ((mqids2=mq_open(msg2.id, O_RDWR)) < 0) {
    	perror("Cliente: Erro a associar a queue servidor");
    	exit(-1);
  	}
	
/*envia a informação para a queue*/	
      printf("Servidor vai enviar\n");
      if (mq_send(mqids2, (char *)&msg2, sizeof(msg2), 0) < 0) {
	perror("Servidor: erro a enviar mensagem");
      }
    
		
/*recebe da queue e mete na estrutura do socket*/	 
	strcpy(u1.nome, msg2.mtext); 
	printf("SRV> envia o estado %s (%s)\n", msg2.mtext, u1.cmd);
      	
	if (sendto(sd, &u1, sizeof(u1), 0, (struct sockaddr *)&from, fromlen) < 0) {
    	perror("Erro no sendto");
    }
	    
    strcpy(u1.cmd, "comando executado com sucesso");
    
	if (sendto(sd, &u1, sizeof(u1), 0, (struct sockaddr *)&from, fromlen) < 0) {
    	perror("Erro no sendto");
    }

}
/*-------------------------------------------------------------------------+
* Funcao: monitor
+-------------------------------------------------------------------------*/ 
void monitor (void){

/*----------------- Procura comandos---------------------------*/
	int i;
	for (i = 0; i < NCOMMANDS; i++){ 
		if (strcmp(u1.cmd, commands[i].cmd_name) == 0) 
	  		break;
	}	  

/*----------------- Executa comandos---------------------------*/
	if (i < NCOMMANDS){
		commands[i].cmd_fnct ();
		close(sd);
		unlink(SERVS);
  		close(mfd);
    } 
}

void inicia_queue2_srv(){
/*Atribui as variaveis  para a queue*/	
	ma2.mq_flags = 0;
	ma2.mq_maxmsg = 2;
	ma2.mq_msgsize = sizeof(msg2);
  		
	if ((mqidc2=mq_open("/CLSERV", O_RDWR|O_CREAT, 0666, &ma2)) < 0) {
  		perror("Cliente: Erro a criar queue cliente");
  		exit(-1);
  	}
	
}


/*-------------------------------------------------------------------------+
* Funcao: integesth 
+-------------------------------------------------------------------------*/ 

void *integesth(void *){
			
	while(1){
			inicia_queue2_srv();
		printf("\nSRV> inicia server-int.ges.\n");	
			inicia_srv_intges();
		printf("\nSRV> executa o comando\n");		
				monitor();
	}
			
  
  return 0;
}

void inicia_thread(void){

	if (pthread_create(&thread_integes, NULL, integesth, NULL) != 0) {
		printf("Erro a criar thread_integes\n");
   	}
    	
}


/*-------------------------------------------------------------------------+
| Function: termina
+-------------------------------------------------------------------------*/ 
void termina (int sn){
	munmap(pa, sizeof(uti_t));
	close(mfd);
	munmap(pa2, sizeof(reg_t));
	close (mfd);
	mq_unlink(SERVQ);
	mq_unlink(SERVQ);
	close(sd);
	unlink(SERVS);
}

/*-------------------------------------------------------------------------+
| Function: main 
+-------------------------------------------------------------------------*/ 
int main(){
	  	
  	printf("\nSRV>> cria ficheiro dos utilizadores\n");
		cria_ficheiro();
	
  	printf("\nSRV>> cria ficheiro dos registos\n");
		cria_ficheiro_reg();	
		
	printf("\nSRV>> cria mecaismos de sincronização\n");
		if (pthread_mutex_init(&mux, NULL) != 0) {
      		printf("Erro a inicializar mutex\n");
      		return -1;
  		}
  	
  	
  	
  		
  	
		
	printf("\nSRV>> executa threads\n");
		inicia_thread();
    
    printf("\nSRV>> executa threads\n");
		signal(SIGKILL, termina);
    	
	printf("\nSRV>> inicia server-ctlp\n");
		inicia_srv_ctlp1();
	
	printf("\nSRV>> encerra threads\n");
		pthread_join(thread_integes, NULL);

return 0;
}
