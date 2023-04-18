#ifndef BLOCK_TREE_H
#define BLOCK_TREE_H

#include "block.h"
#include "import_data.h"
#include <stdio.h>

typedef struct block_tree_cell{ //noeud d'un arbre de blocs

	Block* block;//bloc du noeud
	struct block_tree_cell* father; //noeud père (null si racine)
	struct block_tree_cell* firstChild;//noeud 1er fils
	struct block_tree_cell* nextBro; //noeud frere
	int height;//hauteur du noeud

}CellTree;

CellTree* create_node(Block* b);//creer un noeud et initialise le noeud

int update_height(CellTree* father, CellTree* child);//met a jour la hauteur de father si child a changé de hauteur

void add_child(CellTree* father, CellTree* child);//ajoute un noeud fils au père et met a jour les hauteurs ascendants

void print_tree(CellTree* tree);//affiche les noeuds de l'arbre

void delete_node(CellTree* node);//supprime un noeud

void delete_tree(CellTree* tree);//supprime l'arbre

CellTree* highest_child(CellTree* cell);//retourne le fils avec la hauteur la plus grande du noeud cell

CellTree* last_node(CellTree* tree);//retourne le dernier noeud de la plus grande chaine de noeuds de l'arbre

CellProtected* fusion_declarations(CellProtected* l1, CellProtected* l2);//fusionne deux listes de declarations signées

CellProtected* fusion_highest_chain(CellTree* tree);//fusionne tous les declarations signées de la plus longue chaine de noeuds de l'arbre

#endif
