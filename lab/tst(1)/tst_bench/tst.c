/* Exemplo básico de programa ("Ola' Mundo"). -- CRA
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*-----------------------------------------------------------------------------
Define's - Interfeca de gestao (INTGES)
-----------------------------------------------------------------------------*/

#define NDIG 3 /* número de digitos do identificador */
#define IDX "999" /* identificador especial de acesso */ 
#define MAXN 40  /* dimensão máxima do nome do utilizador */
#define MSIZE 100
//#define MSG "Servidor responde"

/*------------------------------------------------------------------------------
Define's - Servidor <-> Cliente
------------------------------------------------------------------------------*/

#define ARGVECSIZE 5
#define MAX_LINE   50
#define SERVS "/tmp/SERV" /* nome do servidor (socket) */
#define CLINAME "/tmp/CLI"
#define TSER "tser"
#define UMAX 20 /* número máximo de utilizadores */
#define FUTI "UTILIZADORES.DAT" /* ficheiro relativo aos utilizadores */
#define NREG 150  /* número máximo de registos no ficheiro */
#define FREG "REGISTOS.LOG" /* ficheiro com histórico dos acessos */


/*
struct 	command_d {
  char*	cmd_name;
  char*	cmd_arg1;
  char*	cmd_arg2;
  char*	cmd_arg3;
  char*	cmd_help;
  int argc_def;
} const commands[] = {
	{"cuti","<id>"," <nome>"," <[portas]>", "- comando de criar",4},
	{"luti","<id>","\n","\n", "- comando de listar",2},
	{"euti","<id>", "\n", "\n" , "- comando de eliminar",2},
	{"muti","<portas>", "\n", "\n", "- comando de modificar", 2},	
  	{"lac","<portas>", "<id>" ,"<tempos>", "- comando de listar",3},	
  	{"tser","\n", "\n", "\n", "- comando de terminar",1},
  	{"cep","<portas>", "\n", "\n", "- comando de consultar estado",2},
  	{"mep","<portas>", "<estado>" ,"\n", "- comando de modificar estado",3},
	{"tctl","<portas>", "\n", "\n", "- comando de controlador",1}, 
};
*/

struct 	command_d {
  char*	cmd;
  char*	argv_integes[3];
  char*	help;
  int argc_def;
} const commands[] = {
	{"cuti","<id>","<nome>","<portas>", "- comando de criar",4},
	{"luti","<id>","\n","\n", "- comando de listar",2},
	{"euti","<id>", "\n", "\n" , "- comando de eliminar",2},
	{"muti","<portas>", "\n", "\n", "- comando de modificar", 2},	
  	{"lac","<portas>", "<id>" ,"<tempos>", "- comando de listar",3},	
  	{"tser","\n", "\n", "\n", "- comando de terminar",1},
  	{"cep","<portas>", "\n", "\n", "- comando de consultar estado",2},
  	{"mep","<portas>", "<estado>" ,"\n", "- comando de modificar estado",3},
	{"tctl","<portas>", "\n", "\n", "- comando de controlador",1} 
};


typedef struct cmd_integes {
  char cmd[5];
  char id[3];
  char portas[3];
  char estado[3];
  char nome[100];
  char tempos[100];
}linha_cmd;

linha_cmd u = {"\0","\0","\0","\0","\0","\0"};

/*-------------------------------------------------------------------------+
| Function: cmd_mep - mofificar estado
+-------------------------------------------------------------------------*/
void testa_arg (int argc, char** argv, int i){
	strcpy(u.cmd, commands[i].cmd);		
	for (int j= 0; j<=commands[i].argc_def-1; j++){
		if(strcmp(commands[i].argv_integes[j], "<id>") == 0){
			if(strlen(argv[j]) > 3){
				printf("id demasiado grande\n");
				return;
			}
			printf("id casa %d\n",j);
			strcpy(u.id, argv[j]);		
		}	
		if(strcmp(commands[i].argv_integes[j], "<nome>") == 0){
			printf("nome casa  %d\n",j);
			strcpy(u.nome, argv[j]);	
		}
		if(strcmp(commands[i].argv_integes[j], "<portas>") == 0){
			printf("portas casa  %d\n",j);
			if (((u.portas[j] <= 'A' ) & (u.portas[j] >= 'C' )) || (u.portas[j] <= 'a' ) & (u.portas[j] >= 'c')){
				printf("portas erradas\n");
				return;
			}
			strcpy(u.portas, argv[j]);	
		}
		if(strcmp(commands[i].argv_integes[j], "<estado>") == 0){
			if (((u.portas[j] != 'A' ) ||  (u.portas[j] != 'F' )) || (u.portas[j] != 'N' ) || (u.portas[j] != 'a') || (u.portas[j] != 'f' ) || (u.portas[j] != 'n' )){
				printf("estado errado: diferente de A N F\n");
				return;		
			}		
			printf("estado casa  %d\n",j);
			strcpy(u.nome, argv[j]);	
		}
		if(strcmp(commands[i].argv_integes[j], "<tempos>") == 0){
			printf("tempos casa  %d\n",j);
			strcpy(u.tempos, argv[j]);	
		}
	}
	printf("%s %s %s %s\n", u.cmd, u.nome, u.portas, u.id);
	

}

/*-------------------------------------------------------------------------+
| Function: cmd_sair - termina a aplicacao
+-------------------------------------------------------------------------*/ 
void cmd_sair (void)
{
  exit(0);
}


/*-------------------------------------------------------------------------+
| Function: cmd_sos - provides a rudimentary help
+--------------------------------------------------------------------------*/ 
void cmd_sos (void)
{
  int i;
  printf("sos\n");
  for (i=0; i<9; i++)
    printf("%s %s\n", commands[i].cmd, commands[i].help);
}
/*-------------------------------------------------------------------------+
| Function: my_getline        (called from monitor) 
+--------------------------------------------------------------------------*/ 
int my_getline (char** argv, int argvsize)
{
  static char line[MAX_LINE];
  char *p;
  int argc;

  fgets(line, MAX_LINE, stdin);

  /* Break command line into an o.s. like argument vector,
     i.e. compliant with the (int argc, char **argv) specification --------*/

  for (argc=0,p=line; (*line != '\0') && (argc < argvsize); p=NULL,argc++) {
    p = strtok(p, " \t\n");
    argv[argc] = p;
    if (p == NULL) return argc;
  }
  argv[argc] = p;
  return argc;
}


void monitor (void)
{
	static char *argv[ARGVECSIZE+1], *p;
	int argc, i;

	printf("Type sos for help\n");
	for (;;) {
    	printf("\nCmd> ");
    	/* Reading and parsing command line  ----------------------------------*/
		if ((argc = my_getline(argv, ARGVECSIZE)) > 0) {
			for (p=argv[0]; *p != '\0'; *p=tolower(*p), p++);
    		for (i = 0; i < 9; i++){ 
				if (strcmp(argv[0], commands[i].cmd) == 0){ 
					break;
				}
			}
 			if (strcmp(argv[0], "sos") == 0)
 				cmd_sos();
 			if (strcmp(argv[0], "sair") == 0)
				cmd_sair();
    	  	if (i < 9){
				if(commands[i].argc_def == argc){
    	  			testa_arg( argc, argv, i);			
    	  		}else{  
					printf("deu coco\n");
				}
    		}
  		} 
	}
}



int main()
{
 	monitor();
 	
   
  

  return 0;
}
