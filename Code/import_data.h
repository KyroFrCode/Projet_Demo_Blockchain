#ifndef IMPORT_DATA_H
#define IMPORT_DATA_H

#include <stdio.h>
#include <string.h>
#include "primalite.h"
#include "generate.h"
#include "manipulation_cle.h"
#include "signature.h"

typedef struct cellKey{//structure representant une liste de clés (cellule clé)

	Key* data;// data contient les données de la clé publique
	struct cellKey* next;//clé suivante
	
}CellKey;

typedef struct cellProtected{//structure representant une liste de declarations signées (cellule declaration)

	Protected* data;//data contient les données de la déclarations signées
	struct cellProtected* next;//déclarations suivante

}CellProtected;

CellKey* create_cell_key(Key* key);//creer et initialise une cellule clé avec la clé donnée en argument 

CellKey* inserer_tete_key(CellKey* list_key, Key* key);//insere une clé en tete de liste

CellKey* read_public_keys(char* pKeys_file);//lis le fichier "pKeys_file" contenant des clés et retourne la liste des clés

void print_list_keys(CellKey* LCK);//affiche la liste des clés

void delete_cell_key(CellKey* c);//supprime une cellule clé

void delete_list_keys(CellKey* l);//supprime la liste de clés

CellProtected* create_cell_protected(Protected* pr);//creer et initialise une cellule declaration avec la declaration signée donnée en argument

CellProtected* inserer_tete_protected(CellProtected* cl, Protected* pr);//insere une declaration en tete de liste

CellProtected* read_protected(char* declarations_file);//lis le fichier "declarations_file" contenant les declarations et retourne la liste des declarations

void print_list_protected(CellProtected* c);//affiche la liste des declarations

void delete_cell_protected(CellProtected* c);//supprime une cellule declarations

void delete_list_protected(CellProtected* cl);//supprime la liste de declarations

CellProtected* verify_declarations(CellProtected* cp);//verifie les declarations et supprime les declarations fausses de la liste

void libere_contenue_cellProtected_list(CellProtected* cp);//libere les contenues de cellProtected

#endif
