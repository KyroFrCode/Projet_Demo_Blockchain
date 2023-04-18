#ifndef PROCESSUS_VOTE_H
#define PROCESSUS_VOTE_H

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include "manipulation_cle.h"
#include "signature.h"
#include "import_data.h"
#include "hash.h"
#include "block.h"
#include "block_tree.h"

void submit_vote(Protected* p);//permet a un citoyen de soumettre un vote, ajoute son vote a la fin du fichier "Pending_votes.txt"

void create_block(CellTree* tree, Key* author, int d);//creer un bloc valide contenant les votes en attentes depuis le fichier "Pending_votes.txt" et ecrit se bloc dans le fichier "Pending_block" ("Pending_votes.txt supprimé après la creation du bloc)

void add_block(int d, char* name);//verifie que le bloc du fichier Pending_block est valide et ajoute le bloc dans le repertoire Blockchain sous le nom *name* (Pending_block" supprimé après l'appel de la fonction)

CellTree* read_tree();//lis les fichiers dans le repertoire Blockchain et construit l'arbre correspondant

Key* compute_winner_BT(CellTree* tree, CellKey* candidates,CellKey* votes,int sizeC,int sizeV);//retourne la clé publique du vainqueur de l'election en realisant le calcule a partir de la long chaine de noeuds de l'arbre et des listes de clés des candidats et voteurs (sizeC et sizeV les tailles des tables de hachage)

#endif
