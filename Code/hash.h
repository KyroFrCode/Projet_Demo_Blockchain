#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include "primalite.h"
#include "generate.h"
#include "manipulation_cle.h"
#include "signature.h"
#include "import_data.h"

typedef struct hashcell{
    
    Key* key;//clé publique du citoyen
    int val; //valeur permettant de savoir si le citoyen a voter ou non
   
}HashCell;

typedef struct hashtable{
    
    HashCell** tab;//tableau d'hachage contenant les cellules de clés (probing lineaire)
    int size;//taille du tableau d'hachage

}HashTable;


HashCell* create_hashcell(Key* key);//creer et initialise une HashCell

int hash_function(Key* key, int size);//fonction hachage retournant la position de la clé dans la table de hachage

int find_position(HashTable* t, Key* key);//retrouve la position de la clé dans la table de hachage si existe sinon la position a laquelle elle devrait etre (probing lineaire)

HashTable* create_hashtable(CellKey* keys, int size);//creer et initiale la table de hachage avec sa taille "size" et la liste des clés donnée en entrée

void delete_hashtable(HashTable* t);//supprime la table de hachage

Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizeV);//retourne la clé publique du gagnant de l'election a partir des déclaration signées ,des clés publiques des candidats et des voteurs donnée en entrée (sizeC et SizeV taille des tables de hachages pour candidates et citoyens(=voters))

#endif
