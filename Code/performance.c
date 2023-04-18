#include "primalite.h"

#define PUISSANCE 10000

int main(){
	
	//Calcule de performance et comparaisons des fonctions modpow_naive & modpow
	
	clock_t temps_initial;
	clock_t temps_final;
	double temps_cpu;
	int m = PUISSANCE;
	srand(time(NULL));
	
	FILE* f = fopen("modpow_naive.txt","w");
	FILE* f2 = fopen("modpow.txt","w");
	
	for(int i=0;i<m;i+=100){
	    
		//modpow_naive
    	temps_initial = clock();
    	modpow_naive(2,i,5);
    	temps_final = clock();
    	temps_cpu = ((double)(temps_final - temps_initial))/CLOCKS_PER_SEC;
    	fprintf(f,"%d %f \n",i,temps_cpu);
        
        //modpow
        temps_initial = clock();
    	modpow(2,i,5);
    	temps_final = clock();
    	temps_cpu = ((double)(temps_final - temps_initial))/CLOCKS_PER_SEC;
    	fprintf(f2,"%d %f \n",i,temps_cpu);
    	
	}
	
	fclose(f);
	fclose(f2);
	return 1;
}
