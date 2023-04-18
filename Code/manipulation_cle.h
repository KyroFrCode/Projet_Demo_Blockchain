#ifndef MANIPULATION_CLE_H
#define MANIPULATION_CLE_H

typedef struct Key{//structure clé
	long val;
	long n;
	
}Key;

void init_key(Key* key,long val, long n);//initialise la clé avec les valeurs val et n (clé deja alloué)

void init_pair_keys(Key* pKey, Key* sKey, long low_size, long up_size);//initialise les paires de clés avec le protocole RSA (paires clés deja alloué)

char* key_to_str(Key* key);//retourne une chaine de caractere representant la clé (format string : "(%lx,%lx)")

Key* str_to_key(char* str);//retourne la clé representé dans la chaine de caractere


#endif
