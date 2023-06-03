/***************************************************************************
| File: monitor.c
|
| Autor: Carlos Almeida (IST), from work by Jose Rufino (IST/INESC), 
|        from an original by Leendert Van Doorn
| Data:  Nov 2002
***************************************************************************/

#ifndef CTRL_ACESS_H_INCLUDED
#define CTRL_ACESS_H_INCLUDED
/*

Includes

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/mman.h>
#include <errno.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>


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

#define ARGVECSIZE 7
#define MAX_LINE   50
#define SERVS "/tmp/SERV" /* nome do servidor (socket) */
#define CLINAME "/tmp/CLI"
#define TSER "tser"
#define UMAX 20 /* número máximo de utilizadores */
#define FUTI "UTILIZADORES.DAT" /* ficheiro relativo aos utilizadores */
#define NREG 150  /* número máximo de registos no ficheiro */
#define FREG "REGISTOS.LOG" /* ficheiro com histórico dos acessos */



/*------------------------------------------------------------------------------
Define's - Controlador
------------------------------------------------------------------------------*/

#define NPOR 3 /* número de portas a controlar (ABC...) */
#define TFCH 5 /* tempo (segundos) para fecho automático porta */
#define SERVQ "/SERV" /* nome do servidor (queue) */
// ou #define SERVQ 0x53 /* ('S´) nome (chave) do servidor (msg) */
#define CTLA "/CTLA" /* nome do controlador A (queue) */
// ou #define CTLA 0x4341 /* ('C''A´) nome (chave) do controlador A (msg) */
#define CTLB "/CTLB" /* nome do controlador B (queue) */
// ou #define CTLB 0x4342 /* ('C''B´) nome (chave) do controlador B (msg) */
#define CTLC "/CTLC" /* nome do controlador C (queue) */
// ou #define CTLC 0x4343 /* ('C''C´) nome (chave) do controlador C (msg) */
#define TXTSIZE 10


/*------------------------------------------------------------------------------
................................Funcoes.........................................
------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------
................................Estruturas......................................
--------------------------------------------------------------------------*/

typedef struct cmd_integes {
  char id[10];
  char cmd[10];
  char portas[NPOR+1];
  char estado[3];
  char nome[100];
  char tempo1[100];
  char tempo2[100];
}linha_cmd;

typedef struct uti_s { /* estrutura de um registo utilizador */
	char id[NDIG+1]; /* identificador do utilizador */
	char nome [MAXN]; /* nome do utilizador */
	unsigned char port [NPOR]; /* portas acessíveis ao utilizador: 1-acesso O-não */
} uti_t;

typedef struct reg_s { /* estrutura de um registo histórico */
	struct timespec t; /* estampilha temporal */
	char p; /* identificador da porta: A,B,C */
	char id [NDIG+1];  /* identificador do utilizador */
} reg_t;

typedef struct 	Utilizador {
  char cmd[5];
  char id[3];
  char nome[100];
  char portas[3];
}utilizador;




#endif
