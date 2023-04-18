#include "generate.h"

long extended_gcd(long s, long t, long *u, long *v){
    
    if(s == 0){
        
        *u = 0;
        *v = 1;
        return t;
    }
    
    long uPrim, vPrim;
    long gcd = extended_gcd(t%s, s, &uPrim, &vPrim);
    *u = vPrim -(t/s)*uPrim;
    *v = uPrim;
    return gcd;
    
}

void generate_key_values(long p, long q, long *n, long *s, long *u){
	
	*n = p*q;
	long t = (p-1)*(q-1);
	long v = 0;
	long sp = 0;
	
	do{
		sp = rand_long(1,t);
		
	}while(!(extended_gcd(sp,t,u,&v) == 1 && (sp<t) && (*u > 0) && (sp*(*u))%t == 1));
	
	*s = sp;
}


long* encrypt(char* chaine, long s, long n){
	
	if(chaine == NULL){
		printf("Erreur chaine NULL(encrypt)\n");
		return NULL;
	}
	
	int len = strlen(chaine);
	long* encrypt_tab = malloc(sizeof(long)*len);
	
	if(encrypt_tab == NULL){
		printf("Erreur d'allocation du tableau encrypt_tab(encrypt)\n");
		return NULL;
	}
	//initialise le tableau a 0
	for(int j=0;j<len;j++){
		encrypt_tab[j] = 0;
	}
	
	//chiffre le message
	for(int i=0;i<len;i++){

		encrypt_tab[i] = modpow((int)chaine[i],s,n);
	}
	
	return encrypt_tab;
}

char* decrypt(long* crypted, int size, long u, long n){
	
	if(crypted == NULL){
		printf("Tableau crypted en entree NULL(decrypt)\n");
		return NULL;
	}
	
	char* message = (char*)malloc(sizeof(char)*(size+1));
	
	if(message == NULL){
		printf("Probleme allocation de message(decrypt)\n");
		return NULL;
	}
	
	int i;
	for(i=0;i<size;i++){
		message[i] = modpow(crypted[i],u,n);
	}

	message[i] = '\0';
	
	return message;
}


