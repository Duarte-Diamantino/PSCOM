/* Exemplo de utilização do "mmap" para mapeamento de um ficheiro 
em memória (ou criação de uma zona de memória partilhada) -- CRA
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
 
#define FICH "FICHEIRO.DAT"
#define MSIZE 100

int main()
{
  int i, mfd;
  char *pa;

  if ((mfd=open(FICH, O_RDWR|O_CREAT, 0666 )) < 0) {  /* abrir / criar ficheiro */
    perror("Erro a criar ficheiro");
    exit(-1);
  }
  else {
    if (ftruncate(mfd, MSIZE) < 0) {                  /* definir tamanho do ficheiro */
      perror("Erro no ftruncate");
      exit(-1);
    }
  }
  /* mapear ficheiro */
  if ((pa=mmap(NULL, MSIZE, PROT_READ|PROT_WRITE, MAP_SHARED, 
	       mfd, 0)) < (char *)0) {
    perror("Erro em mmap");
    exit(-1);
  }

  /* aceder ao ficheiro através da memória */
  for (i=0; i<MSIZE; i++) pa[i]='A';

  munmap(pa, MSIZE);
  close(mfd);


  return 0;
}
