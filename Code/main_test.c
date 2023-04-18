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

int main(void){
	
	srand(time(NULL));	
	
	printf("is_prime_native : %d\n",is_prime_naive(5504));
	printf("is_prime_native : %d\n",is_prime_naive(5431));
	
    printf("modpow_naive : %ld\n",modpow_naive(2,7,5));
    
    printf("modpow : %d\n",modpow(2,7,5));
    
    printf("rand_long : %ld\n",rand_long(3,10000));
    
    long p = random_prime_number(2,7,5000);
    long q = random_prime_number(2,7,5000);
    long n;
    long s;
    long u;
    
    printf("p:%ld q:%ld\n",p,q);
    
    generate_key_values(p,q,&n,&s,&u);
    
    printf("p:%ld q:%ld n:%ld s:%ld u:%ld\n",p,q,n,s,u);
    
    char* test = "test";
    
    long* tab = encrypt(test,s,n);
    
    
    for(int z=0;z<strlen(test);z++){
    	printf("tab[%d] = %ld\n",z,tab[z]);
    }
   	
   	
	char* final = decrypt(tab,strlen(test),u,n);
    
    printf("decrypt: %s\n",final);
    
    free(tab);
    free(final);
    
    Key* pKey = (Key*)malloc(sizeof(Key));
    Key* sKey = (Key*)malloc(sizeof(Key));
    
    init_pair_keys(pKey,sKey,1,7);
    
    printf("pKey: (%ld,%ld) sKey: (%ld,%ld)\n",pKey->val,pKey->n,sKey->val,sKey->n);
    
    char* key_pKey = key_to_str(pKey);
    char* key_sKey = key_to_str(sKey);
    
    printf("pKey: %s sKey: %s\n",key_pKey,key_sKey);
   
    Key* pKey1 = str_to_key(key_pKey);
    Key* sKey1 = str_to_key(key_sKey);
    
    printf("pKey: (%ld,%ld) sKey: (%ld,%ld)\n",pKey1->val,pKey1->n,sKey1->val,sKey1->n);
    
    free(pKey);
    free(sKey);
    free(key_pKey);
    free(key_sKey);
    free(pKey1);
    free(sKey1);
    
    generate_random_data(12,6);
    
    CellKey* pKeys = read_public_keys("keys.txt");
    CellKey* pKeys_c = read_public_keys("candidates.txt");
    
    printf("clé public citoyen:\n");
    print_list_keys(pKeys);
    printf("clé public candidates:\n");
	print_list_keys(pKeys_c);
	
	delete_list_keys(pKeys);
	delete_list_keys(pKeys_c);
	
	printf("declarations:\n");
	CellProtected* cp = read_protected("declarations.txt");
	print_list_protected(cp);
	
	printf("verification:\n");
	cp = verify_declarations(cp);
	print_list_protected(cp);
	delete_list_protected(cp);
	
	pKeys = read_public_keys("keys.txt");
	
	HashTable* t = create_hashtable(pKeys,20);
	delete_hashtable(t);
	delete_list_keys(pKeys);
	
	CellProtected* decl = read_protected("declarations.txt");
	CellKey* candidates = read_public_keys("candidates.txt");
	CellKey* voters = read_public_keys("keys.txt");
	
	Key* k = compute_winner(decl,candidates,voters,25,25);
	
	char* winner_key = key_to_str(k);
	
	printf("Winner: %s\n",winner_key);
	
	free(k);
	free(winner_key);
	
	delete_list_keys(voters);
	delete_list_keys(candidates);
	delete_list_protected(decl);

	decl = read_protected("declarations.txt");

	Block* b = (Block*)malloc(sizeof(Block));
	b->author = (Key*)malloc(sizeof(Key));
	b->author->val = 545486;
	b->author->n = 5806;
	b->votes = decl;
	b->hash = crypt_SHA256("previous");
	b->previous_hash = crypt_SHA256("test");
	b->nonce = 0;

	write_block(b,"block#1_test.txt");
	
	delete_full_block(b);
	
	Block* b2 = read_block("block#1_test.txt");
	write_block(b2,"block#2_test.txt");
	
	char* test_block = block_to_str(b2);
	
	printf("test_block:\n %s\n",test_block);
	
	free(test_block);
	
	delete_full_block(b2);
	
	test = "Rosetta code";
	
	unsigned char* str_test = crypt_SHA256(test);
	
	printf("crypt:\n");
	
	for(int i = 0;i < SHA256_DIGEST_LENGTH;i++)
		printf("%02x",str_test[i]);
	putchar('\n');
	
	decl = read_protected("declarations.txt");
	
	Block* b3 = (Block*)malloc(sizeof(Block));
	b3->author = (Key*)malloc(sizeof(Key));
	b3->author->val = 545486;
	b3->author->n = 5806;
	b3->votes = decl;
	b3->hash = NULL;
	b3->previous_hash = crypt_SHA256("958948646");
	b3->nonce = 0;
	
	compute_proof_of_work(b3,1);
	
	printf("final hash value exit: \n");
	
	for(int i = 0;i < SHA256_DIGEST_LENGTH;i++)
		printf("%02x",b3->hash[i]);
	putchar('\n');
	
	printf("Nonce exit: %d \n",b3->nonce);
	
	printf("verify block: %d\n",verify_block(b3,2));
	
	printf("verify block: %d\n",verify_block(b3,7));
	
	delete_full_block(b3);
	
	CellProtected* votes_blocks1 = read_protected("declarations_test.txt");
	CellProtected* votes_blocks2 = read_protected("declarations_test.txt");
	CellProtected* votes_blocks3 = read_protected("declarations_test.txt");
	CellProtected* votes_blocks4 = read_protected("declarations_test.txt");
	CellProtected* votes_blocks5 = read_protected("declarations_test.txt");
	
	Block* b4 = (Block*)malloc(sizeof(Block));
	b4->author = (Key*)malloc(sizeof(Key));
	b4->author->val = 545486;
	b4->author->n = 5806;
	b4->votes = votes_blocks1;
	b4->hash = NULL;
	b4->previous_hash = crypt_SHA256("0");
	b4->nonce = 0;
	
	compute_proof_of_work(b4,1);
	
	Block* b5 = (Block*)malloc(sizeof(Block));
	b5->author = (Key*)malloc(sizeof(Key));
	b5->author->val = 8786476;
	b5->author->n = 97476;
	b5->votes = votes_blocks2;
	b5->hash = NULL;
	b5->previous_hash = b4->hash;
	b5->nonce = 0;
	
	compute_proof_of_work(b5,1);
	
	Block* b6 = (Block*)malloc(sizeof(Block));
	b6->author = (Key*)malloc(sizeof(Key));
	b6->author->val = 9631845;
	b6->author->n = 863322457;
	b6->votes = votes_blocks3;
	b6->hash = NULL;
	b6->previous_hash = b5->hash;
	b6->nonce = 0;
	
	compute_proof_of_work(b6,1);
	
	Block* b7 = (Block*)malloc(sizeof(Block));
	b7->author = (Key*)malloc(sizeof(Key));
	b7->author->val = 864256;
	b7->author->n = 8652145;
	b7->votes = votes_blocks4;
	b7->hash = NULL;
	b7->previous_hash = b5->hash;
	b7->nonce = 0;
	
	compute_proof_of_work(b7,1);
	
	Block* b8 = (Block*)malloc(sizeof(Block));
	b8->author = (Key*)malloc(sizeof(Key));
	b8->author->val = 456329;
	b8->author->n = 19428;
	b8->votes = votes_blocks5;
	b8->hash = NULL;
	b8->previous_hash = b7->hash;
	b8->nonce = 0;
	
	compute_proof_of_work(b8,1);
	
	CellTree* racine = create_node(b4);
	CellTree* fils1 = create_node(b5);
	CellTree* fils2 = create_node(b6);
	CellTree* fils3 = create_node(b7);
	CellTree* fils4 = create_node(b8);
	
	add_child(racine,fils1);
	add_child(fils1,fils2);
	add_child(fils1,fils3);
	add_child(fils3,fils4);
	
	printf("racine hauteur: %d\n",racine->height);
	printf("fils1 hauteur: %d\n",fils1->height);
	printf("fils2 hauteur: %d\n",fils2->height);
	printf("fils3 hauteur: %d\n",fils3->height);
	printf("fils4 hauteur: %d\n",fils4->height);
	
	print_tree(racine);

	CellTree* ln = last_node(racine);
	
	printf("ln : %p\n",ln);
	printf("%p\n",fils4);
	
	//printf("declaration fusion print: \n");
	//CellProtected* fdecl = fusion_highest_chain(racine);
	//print_list_protected(fdecl);
	
	free(b4->author);
	free(b5->author);
	free(b6->author);
	free(b7->author);
	free(b8->author);
	
	libere_contenue_cellProtected_list(b4->votes);
	libere_contenue_cellProtected_list(b5->votes);
	libere_contenue_cellProtected_list(b6->votes);
	libere_contenue_cellProtected_list(b7->votes);
	libere_contenue_cellProtected_list(b8->votes);
	
	delete_tree(racine);
	
	CellProtected* sub = read_protected("declarations_test.txt");
	
	CellProtected* tmp = sub;
	
	while(tmp != NULL){
		submit_vote(tmp->data);
		tmp = tmp->next;
	}
	
	delete_list_protected(sub);
	
	CellTree* tree = NULL;
	Key* author = (Key*)malloc(sizeof(Key));
	author->val = 28511;
	author-> n = 6541546;
	
	create_block(tree,author,1);
	
	add_block(1,"test_block");
	
	//CellTree* tr = read_tree();
	
	//print_tree(tr);
	
	candidates = read_public_keys("candidates.txt");
	voters = read_public_keys("keys.txt");
	
	Key* winner = compute_winner_BT(tree,candidates,voters,20,20);
	
	char* key_winner = key_to_str(winner);
	
	printf("gagnant election test : %s\n",key_winner);
	
	delete_tree(tree);
	free(author);
	//delete_tree(tr);
	delete_list_keys(voters);
	delete_list_keys(candidates);
	free(key_winner);

	return 0;
}
