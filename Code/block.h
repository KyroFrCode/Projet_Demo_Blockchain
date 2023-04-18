#ifndef BLOCK_H
#define BLOCK_H

#include <openssl/sha.h>
#include "manipulation_cle.h"
#include "signature.h"
#include "import_data.h"
#include "primalite.h"
#include "generate.h"
#include "generate_data.h"
#include "hash.h"

typedef struct block{
	
	Key* author; //clé de l'auteur du bloc
	CellProtected* votes; //contient les votes des citoyens
	unsigned char* hash; //valeur haché du bloc (identifiant unique d'un bloc)
	unsigned char* previous_hash; //valeur haché du bloc précédent
	int nonce; // valeur unique (de communication) indiquant la preuve de calcul pour trouver la valeur haché du bloc
}Block;

void write_block(Block* b,char *filename);//ecrit un bloc dans un fichier *filename*

Block* read_block(char* filename);//lis un fichier *filename* contenant un bloc et retourne ce bloc

int length_block(Block* b);//retourne la taille totale des données des blocs sous la forme d'une chaine de caractere (pour block_to_str)

char* block_to_str(Block* block);//retourne une chaine de caractere contenant tous les informations du bloc (utilisé pour creer la valeur haché du bloc)

unsigned char* crypt_SHA256(char* str);//retourne une valeur haché de la chaine de caractere donnée en entrée grace a la technologie SHA256

void compute_proof_of_work(Block *B, int d);//realise le calcul de la preuve du travail (cela consiste a obtenir une valeur haché avec d zeros sucessives au debut)

int verify_block(Block* b,int d);//permet de verifier que le bloc est bien valide (verifie que le bloc a une valeur haché correct avec d zeros successives)

void delete_block(Block* b);//supprime la structure bloc en gardant la clé de l'auteur et les données des declarations signées(supprime seulement les cellprotected et garde les protected)

void delete_full_block(Block* b);//supprimer un bloc en entier (y compris la clé de l'auteur et les données des declarations)

#endif
