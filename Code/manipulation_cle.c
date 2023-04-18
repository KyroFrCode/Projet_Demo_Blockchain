#include "manipulation_cle.h"
#include "primalite.h"
#include "generate.h"

#define MILLER_NB 5000

void init_key(Key* key, long val, long n){
	
	if(key == NULL){
		printf("Erreur key NULL(init_key)\n");
		return;
	}
	
	key->val = val;
	key->n = n;
}

void init_pair_keys(Key* pKey, Key* sKey, long low_size, long up_size){
	
	if(pKey == NULL || sKey == NULL){
		printf("Erreur pKey ou sKey NULL(init_pair_keys)\n");
		return;
	}
	
	long p = random_prime_number(low_size,up_size,MILLER_NB);
	long q = random_prime_number(low_size,up_size,MILLER_NB);
	
	while(p==q){
		q = random_prime_number(low_size,up_size,MILLER_NB);
	}
	
	long n,s,u;
	
	generate_key_values(p,q,&n,&s,&u);
	
	if(u<0){
	
		long t = (p-1)*(q-1);
		u = u*t;
	}
	
	init_key(pKey,s,n);
	init_key(sKey,u,n);
}

char* key_to_str(Key* key){
	
	if(key == NULL){
		printf("Erreur key NULL(key_to_str)\n");
		return NULL;
	}
	
	char* key_str = (char*)malloc(sizeof(char)*256);
	
	if(key_str == NULL){
		printf("Probleme allocation memoire de key_str(key_to_str)\n");
		return NULL;
	}
	
	sprintf(key_str,"(%lx,%lx)",key->val,key->n);
	
	return key_str;
}

Key* str_to_key(char* str){
	
	if(str == NULL){
		printf("Erreur chaine de caracteres str en entree NULL(str_to_key)\n");
		return NULL;
	}
	
	Key* k = (Key*)malloc(sizeof(Key));
	
	if(k == NULL){
		printf("Probleme allocation memoire de k(str_to_key)\n");
		return NULL;
	}
	
	long val;
	long n;
	
	sscanf(str,"(%lx,%lx)",&val,&n);

	k->val = val;
	k->n = n;
	
	return k;
}

