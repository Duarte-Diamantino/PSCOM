#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define M 4

int linha[M][M];
int n_linha;

int main(){
	int i = 0;
	ler_linha();
	int j = 0;
	for(j = 0; j < M){
		for(i = 0; i < M){
    			printf("%d", M[j][i]);
    		}
    	}



return 0;
}

int linha()
	int x, i = 0;
	int j = 0;
	for(j = 0; j < M){
		for(i = 0; i < M){
    			scanf("%d", &x);
    			M[j][i] = x; 
    		}
    	}
return 0;
}
