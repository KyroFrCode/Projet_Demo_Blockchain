#include "block_tree.h"

CellTree* create_node(Block* b){
	
	CellTree* t = (CellTree*)malloc(sizeof(CellTree));
	
	t->block = b;
	t->father = NULL;
	t->firstChild = NULL;
	t->nextBro = NULL;
	t->height = 0;
	
	return t; 
}

int max(int n1,int n2){

	if(n1>n2)
		return n1;
	else
		return n2;
}

int update_height(CellTree* father, CellTree* child){

	int h = max(father->height,child->height+1);
	
	//si la hauteur a changé
	if(h != father->height){
	
		father->height = h;
		return 1;
	}
	
	return 0;	
}

void add_child(CellTree* father, CellTree* child){

	
	if(father->firstChild == NULL){
	
		father->firstChild = child;
		child->father = father;
		
	}else{
		
		CellTree* t = father->firstChild;
		
		while(t->nextBro != NULL){
		
			t = t->nextBro;
		}
		
		t->nextBro = child;
		child->father = father;
	}
	
	//actualisation des hauteurs des noeuds
	while(father != NULL){
	
		update_height(father,child);
		child = father;
		father = father->father;
	}
}

void print_tree(CellTree* tree){
	
	if(tree==NULL){return;}
	
	printf("hauteur : %d id: ",tree->height);
			
	for(int i = 0;i < SHA256_DIGEST_LENGTH;i++)
		printf("%02x",tree->block->hash[i]);
	
	putchar('\n');
	
	print_tree(tree->firstChild);
	print_tree(tree->nextBro);
}

void delete_node(CellTree* node){
	
	if(node == NULL){return;}
	
	delete_block(node->block);
	free(node);
	node = NULL;
}

void delete_tree(CellTree* tree){
	
	if(tree == NULL){return;}
	
	delete_tree(tree->nextBro);
	delete_tree(tree->firstChild);
	
	delete_node(tree);
}

CellTree* highest_child(CellTree* cell){

	CellTree* t = cell->firstChild->nextBro;
	CellTree* hc = cell->firstChild;
	
	while(t != NULL){
		
		//comparaison des hauteurs parmi tous les freres
		if(t->height > hc->height){
			hc = t;
		}
		
		t = t->nextBro;
	}
	return hc;
}

CellTree* last_node(CellTree* tree){

	if(tree == NULL){
		return NULL;
	}

	CellTree* t = tree;
	
	while(t->firstChild != NULL){
		
		t = highest_child(t);
	}
	
	return t;	
}

CellProtected* fusion_declarations(CellProtected* l1, CellProtected* l2){

	CellProtected* tete = l1;//recupere l'en-tete de la 1er liste
	
	while(l1->next != NULL){
		l1 = l1->next;
	}
	l1->next = l2; //ajout de la seconde liste a la premiere
	
	return tete;
}

CellProtected* fusion_highest_chain(CellTree* tree){
	
	CellProtected* cp = tree->block->votes;
	CellTree* t = highest_child(tree);
	
	//recupere le noeud le plus haut et fusionne sa liste avec les autres noeuds hauts
	while(t->firstChild != NULL){
		
		cp = fusion_declarations(cp,t->block->votes);
		t = highest_child(t);
	}
	
	cp = fusion_declarations(cp,t->block-> votes);//fusionne la derniere liste du dernier noeud
	
	return cp;
}
