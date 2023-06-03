#include "empresas.h"

void troca_dados (linha_n v[], int x, int y){
    linha_n temp = v[x];
    v[x] = v[y];
    v[y] = temp;
}

void quicksort (linha_n v[],  int e, int d){
	int i;

    	if (d <= e) return;

    	i = particao(v, e, d);
   	quicksort(v, e, i - 1);
    	quicksort(v, i + 1, d);
}

int particao (linha_n v[], int e, int d){
    int j, i;

    	/*pivot = v[d].fat;*/
    	i = e;

    for (j = e; j < d; j++) {
        if (v[j].fat >= v[d].fat) {
            troca_dados(v, i, j);
            i++;
        }
    }
    troca_dados(v, i, d);

    return i;
}
/*---------------------------------------------------------------------------*/
void troca_empresas (linha_n v[], int x, int y){
    linha_n temp = v[x];
    v[x] = v[y];
    v[y] = temp;
    
}

void quicksort_alfabetico (linha_n v[],  int e, int d){
	int i;

    if (d <= e) return;

    i = compara_empresas(v, e, d);
    quicksort_alfabetico(v, e, i - 1);
    quicksort_alfabetico(v, i + 1, d);
}

int compara_empresas(linha_n v[], int e, int d){
 
    int j, i;
    i = e;

    for (j = e; j < d; j++) {
        if (strcmp(v[j].empresa, v[d].empresa ) < 0) {
            troca_empresas(v, i, j);
            i++;
        }
    }
    troca_empresas(v, i, d);

    return i;
}
