#include "processus_vote.h"
#include "primalite.h"
#include "generate.h"
#include "manipulation_cle.h"
#include "signature.h"
#include "generate_data.h"
#include "import_data.h"
#include "hash.h"
#include "block.h"
#include "block_tree.h"
#include "processus_vote.h"

#define NB_CITOYENS 1000
#define NB_CANDIDATS 5

int main(){

	generate_random_data(NB_CITOYENS,NB_CANDIDATS);
	
	CellProtected* decl = read_protected("declarations.txt");
	CellKey* candidates = read_public_keys("candidates.txt");
	CellKey* voters = read_public_keys("keys.txt");
	
	CellTree* tree = NULL;
	int nb_votes = 0;
	
	CellProtected* tmp = decl;
	
	while(tmp != NULL){
	
		submit_vote(tmp->data);
		nb_votes++;
		
		if(nb_votes == 10){
		
			create_block(tree,tmp->data->pKey,1);
			char* key_str = key_to_str(tmp->data->pKey);
			add_block(1,key_str);
			free(key_str);
			nb_votes = 0;
		}
		
		tmp = tmp->next;
	}
	
	tree = read_tree();
	print_tree(tree);
	
	Key* k = compute_winner_BT(tree,candidates,voters,NB_CANDIDATS,NB_CITOYENS);
	
	char* winner_str = key_to_str(k);
	
	printf("Winner: %s\n",winner_str);
	
	delete_list_keys(voters);
	delete_list_keys(candidates);
	delete_list_protected(decl);
	delete_tree(tree);
	
	return 0;
}
