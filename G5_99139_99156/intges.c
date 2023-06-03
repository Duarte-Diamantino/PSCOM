/***************************************************************************
| File: intges.c
|
| Autor: Duarte Diamantino e Francisca Rodrigues 
| Data:  Abril 2023
***************************************************************************/
#include "ctrl_acess.h"


#define NCOMMANDS  (sizeof(command_integes)/sizeof(struct command_integes))

void inicia_cli_srv (void);

struct 	command_integes {
  char*	cmd;
  char*	argv_integes[4];
  char*	help;
  int argc_def;
} const command_integes[] = {
	{"cuti",{"<id>","<nome>","<portas>", "\n"}, "- comando de criar",4},
	{"luti",{"<id>","\n","\n"}, "- comando de listar",2},
	{"euti",{"<id>", "\n", "\n"}, "- comando de eliminar",2},
	{"muti",{"<id>","<portas>", "\n", "\n"}, "- comando de modificar", 3},	
  	{"lac",{"<portas>", "<id>" ,"<tempo1>", "<tempo2>"}, "- comando de listar",7},	
  	{"tser",{"\n", "\n", "\n"}, "- comando de terminar",1},
  	{"cep",{"<portas>", "\n", "\n"}, "- comando de consultar estado",2},
  	{"mep",{"<portas>", "<estado>" ,"\n"}, "- comando de modificar estado",3},
	{"tctl",{"<portas>", "\n", "\n"}, "- comando de controlador",2} 
};


linha_cmd u;



const char TitleMsg[] = "\n Application Control Monitor\n";
const char InvalMsg[] = "\nInvalid command!";


struct sockaddr_un my_addr;
socklen_t addrlen;
struct sockaddr_un to;
socklen_t tolen;
int sd;

int n_cuti = 0;
char MSG[100];	
char nome[10];
 


struct sockaddr_un my_addr;
socklen_t addrlen;
struct sockaddr_un from;
socklen_t fromlen;
/*-------------------------------------------------------------------------+
* Funcao: inicia_cli_srv -> establece ligação entre o servidor e o integes
+-------------------------------------------------------------------------*/
void inicia_cli_srv(void){
/*atribui o endereço para o socket*/		
	my_addr.sun_family = AF_UNIX;
	memset(my_addr.sun_path, 0, sizeof(my_addr.sun_path));
/*cria o socket*/	  
	  	 if ((sd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0 ) {
	    perror("Erro a criar socket"); exit(-1);
	  }
/*cria um nome para o cliente - permite varios intges*/	   
	  sprintf(nome, "%s%d", CLINAME ,getpid());
/*mete o cliente no caminho*/	  
	  strcpy(my_addr.sun_path, nome); 
	  addrlen = sizeof(my_addr.sun_family) + strlen(my_addr.sun_path);
/*teste o bind*/	
	  if (bind(sd, (struct sockaddr *)&my_addr, addrlen) < 0 ) {
	    perror("Erro no bind"); exit(-1);
	  }
	  to.sun_family = AF_UNIX;
	  memset(to.sun_path, 0, sizeof(to.sun_path));
/*mete o SERV no caminho*/	  
	  strcpy(to.sun_path, SERVS);
	  tolen = sizeof(my_addr.sun_family) + strlen(to.sun_path);
		
}


/*-------------------------------------------------------------------------+
| Function: Testa a qualidade dos comandos antes de enviar
+-------------------------------------------------------------------------*/
int testa_arg (int argc, char** argv, int i){

/*Variaveis*/	
	int x = command_integes[i].argc_def;
	int y = 1;
/*limpa a estrura para começar o teste dos argumentos*/	
	strcpy(u.cmd, command_integes[i].cmd);
	strcpy(u.id, "");
	strcpy(u.nome, "");
	strcpy(u.tempo1, "");
	strcpy(u.tempo2, "");
	strcpy(u.estado, "");	
	strcpy(u.portas, "");
/*corre o vetor dos argumentos e testa a integridade dos argumentos*/
	printf("TESTARG:------------------------------------\n");	
	for (int j = 0; j< x ; j++){
		/*Verifica se o comando tem "ID"*/
			if(strcmp(command_integes[i].argv_integes[j], "<id>") == 0){	
				printf("|Int.ges> O comando tem <id>\n");
				if(strlen(argv[j+1]) > 3){
					printf("|Int.ges>  erro: id demasiado grande %s\n", argv[j]);
					return 1;
				}
				if((strcmp(argv[j+1], "0") == 0) && (strcmp(u.cmd, "cuti") == 0) ){
					printf("|Int.ges> erro: id n pode ser 0 - pois 0 é usado para todos %s\n", argv[j]);
					return 1;
				}
				if((strcmp(argv[j+1], "999") == 0) && (strcmp(u.cmd, "cuti") == 0) ){
					printf("|Int.ges> erro: id n pode ser 999 - pois 999 é um id especial %s\n", argv[j]);
					return 1;
				}
				strncpy(u.id, argv[j+1],3);
				printf("|Int.ges> id:%s\n", u.id);
			}				
		/*Verifica se o comando tem "NOME"*/	
			if(strcmp(command_integes[i].argv_integes[j], "<nome>") == 0){
				printf("|Int.ges> o comando tem nome\n");
					strcpy(u.nome, argv[j+1]);
					while(1){	
						y =0;
						if (argv[j+2] == NULL){
							u.portas[0] = 'A';
							u.portas[1] = 'B';
							u.portas[2] = 'C';
							
							printf("|Int.ges> nome: %s\n", u.nome);
							printf("|Int.ges> portas não encontradas;; permissao total: %s\n", u.portas);
							return 0;
						}
						if ((argv[j+2][0] == 'A' ) || (argv[j+2][0] == 'a')){
							y--;
						}
						if ((argv[j+2][1] == 'B' ) || (argv[j+2][1] == 'b' )){
							y--;
						}
						if ((argv[j+2][2] == 'C' ) || (argv[j+2][2] == 'c' )){
							y--;
						}						
					
						if(y == -3){
							break;
						}	
						
						strcat(u.nome, "_");		
						strcat(u.nome, argv[j+2]);
						j++;
					}
				
						strcpy(u.portas, argv[j+2]);				
						printf("|Int.ges> nome: %s\n", u.nome);
						printf("|Int.ges> portas: %s\n", u.portas);				

					return 0;
							
			}
	
		/*Verifica se o comando tem "PORTAS"*/
			if(strcmp(command_integes[i].argv_integes[j], "<portas>") == 0){
				printf("|Int.ges> tem portas\n");
				
				if (strcmp("0",argv[j+1]) == 0){
					strcpy(u.portas, "0");
					printf("|Int.ges> ira ler todas as portas\n");	
				}else{		
				strcpy(u.portas, argv[j+1]);
					if ((u.portas[0] != 'A' ) && (u.portas[0] != 'a')){
						printf("|Int.ges> erro: na porta A/a\n");
					}
					if ((u.portas[1] != 'B' ) && (u.portas[1] != 'b' )){
						printf("> Erro na porta B/b\n");
					}
					if ((u.portas[2] != 'C' ) && (u.portas[2] != 'c' )){
						printf("> Erro na porta C/c\n");
					}						
				printf("|Int.ges> portas: %s\n", u.portas);				
			}}
		
		/*Verifica se o comando tem "ESTADO"*/
			if(strcmp(command_integes[i].argv_integes[j], "<estado>") == 0){
				printf("|Int.ges> comando tem estado\n");
				strcpy(u.id, argv[j+1]);
				if((u.id[0] != 'A' ) &&  (u.id[0] != 'F' ) && (u.id[0] != 'N' )){
					if((u.id[0] != 'a') && (u.id[0] != 'f' ) && (u.id[0] != 'n' )){
						printf("|Int.ges> erro estado diferente de A N F\n");
						return 1;
					}
				}	
				strcpy(u.estado, argv[j+1]);
				printf("|Int.ges> estado: %s\n", u.estado);	
			}
		
		/*Verifica se o comando tem <tempo 1>*/
			if(strcmp(command_integes[i].argv_integes[j], "<tempo1>") == 0){
                printf("|Int.ges> tem <tempo1>\n");
                if (argv[j+1] == NULL){
                    strcpy(u.tempo1, "0");
                    x--;
                }else{
                    strcpy(u.tempo1, argv[j+1]);
                    strcat(u.tempo1, " ");
                    strcat(u.tempo1, argv[j+2]);
                    printf("|Int.ges> tempo1: %s\n", u.tempo1);
                    x--;
                }
            }
		/*Verifica se o comando tem <tempo 2>*/
			if(strcmp(command_integes[i].argv_integes[j], "<tempo2>") == 0){
                printf("|Int.ges> tem <tempo2>\n");
                if (argv[j+1] == NULL){
                    strcpy(u.tempo2, "0");
                    x--;
                }else{
                    strcpy(u.tempo2, argv[j+1]);
                    strcat(u.tempo2, " ");
                    strcat(u.tempo2, argv[j+2]);
                    printf("|Int.ges> tempo2: %s\n", u.tempo2);
                    x--;
                }
            }
	}
return 0;
}

/*-------------------------------------------------------------------------+
	Funcao: envi_srv
+-------------------------------------------------------------------------*/
void env_srv (int argc, char** argv){	
	printf("ENVSERV----------------------------------------\n");	
	printf("|Int.ges> vai enviar:\n|cmd:%s\n|id: %s\n|nome:%s\n|portas:%s\n",u.cmd, u.id, u.nome, u.portas);
	printf("------------------------------------------\n");
		if (sendto(sd, &u, sizeof(u), 0, (struct sockaddr *)&to, tolen) < 0) {
		    	perror("CLI: Erro no sendto");
	  	}
}

/*-------------------------------------------------------------------------+
* Funcao: recebe_srv
*
+-------------------------------------------------------------------------*/
void recebe_srv (void){
	printf("RCBSERV----------------------------------------\n");			

	strcpy(u.id, "");
	strcpy(u.nome, "");
	strcpy(u.tempo1, "");
	strcpy(u.tempo2, "");
	strcpy(u.estado, "");	
	strcpy(u.portas, "");

	while(1){	
		if (recvfrom(sd,  &u, sizeof(u), 0, (struct sockaddr *)&to, &tolen) < 0) {
	      			perror("CLI: Erro no recvfrom");
		}
		
		if(strcmp(u.cmd, "erro") == 0){
			printf("ERRO\n");
			return ;
		}
		
		if(strcmp(u.cmd, "comando executado com sucesso") == 0){
			printf("\n->%s\n",u.cmd);
			return ;
		}else {
			printf("|Int.ges> %s %s %s\n", u.id, u.nome, u.portas);
    	}	
	}
}


/*-------------------------------------------------------------------------+
| Function: cmd_sos - provides a rudimentary help
+--------------------------------------------------------------------------*/ 
void cmd_sos (void){
  int i;
  printf("sos\n");
  for (i=0; i<9; i++)
    printf("%s %s\n", command_integes[i].cmd, command_integes[i].help);
}
/*-------------------------------------------------------------------------+
* Funcao: my_getline        (chamado do monitor) 
+--------------------------------------------------------------------------*/ 
int my_getline (char** argv, int argvsize){
  static char line[MAX_LINE];
  char *p;
  int argc;

/*fica a ler do teclado*/
  fgets(line, MAX_LINE, stdin);
/*descobre o argc atraves de um for que corre a string*/
  for (argc=0,p=line; (*line != '\0') && (argc < argvsize); p=NULL,argc++) {
  /*divide a line em argv[] diferentes*/  
    p = strtok(p, " \t\n");
    argv[argc] = p;
    if (p == NULL) return argc;
  }
  argv[argc] = p;
  return argc;
}

/*-------------------------------------------------------------------------+
* Funcao: monitor       (chamado da main) 
+--------------------------------------------------------------------------*/
void monitor (void){
	static char *argv[ARGVECSIZE+1], *p;
	int argc, i, erro;
	
	printf("Escreva <sos> para ver a lista de comandos\n");
	for (;;) {
    	printf("\nInt.ges> ");
    	/* Reading and parsing command line  ----------------------------------*/
		if ((argc = my_getline(argv, ARGVECSIZE)) > 0) {
			for (p=argv[0]; *p != '\0'; *p=tolower(*p), p++);
    		for (i = 0; i < 9; i++){ 
				if (strcmp(argv[0], command_integes[i].cmd) == 0){ 
					break;
				}
			}
 			if (strcmp(argv[0], "sos") == 0)
 				cmd_sos();
 			if (strcmp(argv[0], "sair") == 0)
				return;
    	  	if (i < 9){
    	  			erro = testa_arg( argc, argv, i);
    	  			if(erro == 0){
    	  				env_srv(argc, argv);
    	  				recebe_srv();			    	  			
    		}else{	
					printf("Int.ges> ERRO;\n");
	    		}
	  		} 
		}
	}
}

/*--------------------------------------------------------------------------
* Funcao: main       	 
---------------------------------------------------------------------------*/

int main(){

	printf("Int.ges> Inicia cliente-servidor;\n");
	inicia_cli_srv();
	printf("Int.ges> Inicia monitor;\n");
 	monitor();
  return 0;
}

