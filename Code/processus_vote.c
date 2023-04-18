#include "processus_vote.h"

void submit_vote(Protected* p){

	FILE* f = fopen("Pending_votes.txt","a");
	char* pr_str = protected_to_str(p);
	
	fprintf(f,"%s\n",pr_str);
	
	free(pr_str);
	fclose(f);
}

void create_block(CellTree* tree, Key* author, int d){
	
	Block* b = (Block*)malloc(sizeof(Block));
	
	if(b == NULL){
		return;
	}
	
	b->author = author;
	b->votes = read_protected("Pending_votes.txt");
	b->hash = NULL;
	
	if(remove("Pending_votes.txt") != 0){
	
		printf("Erreur suppression du fichier Pending_votes.txt(create_block)\n");
		delete_list_protected(b->votes);
		free(b);
		return;
	}
	
	CellTree* lnode = last_node(tree);
	
	if(lnode == NULL){
		
		b->previous_hash = crypt_SHA256("0"); //valeur haché du bloc de la racine (probleme Seg Fault avec previous_hash NULL)
	}
	else{
		b->previous_hash = lnode->block->hash;
	}
	
	compute_proof_of_work(b,d);
	
	write_block(b,"Pending_block");
	
	libere_contenue_cellProtected_list(b->votes);
	
	delete_block(b);
	
}

void add_block(int d, char* name){

	Block* b = read_block("Pending_block");
	
	if(verify_block(b,d) == 1){
	
		char directory[256] = "./Blockchain/";
		
		if(strlen(directory)+strlen(name)>256){
		
			printf("Erreur nom de fichier en entree trop long(add_block)");
			free(b->author);
			libere_contenue_cellProtected_list(b->votes);
			delete_block(b);
			return;
		}
		
		strcat(directory,name);
		strcat(directory,".txt");
		
		write_block(b,directory);
	}
	
	remove("Pending_block");
	
	libere_contenue_cellProtected_list(b->votes);
	free(b->author);
	delete_block(b);
}

CellTree* read_tree(){

	CellTree** T = (CellTree**)malloc(256*sizeof(CellTree*));
	CellTree* noeud = NULL;
	Block* b = NULL;
	int size = 0;

	DIR* rep = opendir("./Blockchain/");
	
	if(rep != NULL){
	
		struct dirent* dir;
		
		while(dir = readdir(rep)){
		
			if(strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..")!=0){
				
				printf("Chemin du fichier : ./Blockchain/%s \n",dir->d_name);
				
				char directory_file[256] = "./Blockchain/";
				strcat(directory_file,dir->d_name);
				
				b = read_block(directory_file);
				noeud = create_node(b);
				size++;
				T[size-1] = noeud;
			}
		}
		
		closedir(rep);
		
		for(int i=0;i<size;i++)
			for(int j=0;j<size;j++)
				if(strcmp(T[i]->block->hash,T[j]->block->previous_hash) == 0)
					add_child(T[i],T[j]);
					
		
		for(int k=0;k<size;k++)
			if(T[k]->father == NULL)
				noeud = T[k];
		
		return noeud;
	
	}
	return NULL;
}

Key* compute_winner_BT(CellTree* tree, CellKey* candidates,CellKey* voters,int sizeC,int sizeV){

	CellProtected* declarations = NULL;
    CellTree* child = NULL;
    Key* winner = NULL;
    
    if(tree != NULL){
        declarations = tree->block->votes;
        child = highest_child(tree);
    }
    
    while(tree != NULL && child != NULL){
    
        declarations = fusion_declarations(declarations, child->block->votes);
        tree = child;
        child = highest_child(tree);
    }
 
    verify_declarations(declarations);

	winner = compute_winner(declarations, candidates, voters, sizeC, sizeV);

	delete_list_protected(declarations);

    return winner;
}
