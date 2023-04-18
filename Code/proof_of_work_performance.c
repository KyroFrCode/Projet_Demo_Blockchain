#include "block.h"
#include "manipulation_cle.h"
#include "signature.h"
#include "import_data.h"
#include "primalite.h"
#include "generate.h"
#include "generate_data.h"
#include "hash.h"
#include <time.h>

#define LOW_SIZE 3
#define UP_SIZE 7
#define ZEROS 4

int main(){

	//Calcule et compare la performance de la fonction proof_of_work pour trouver une valeur haché avec d zeros successives
	
	//creation des clés pour le test
	Key* pKey = (Key*)malloc(sizeof(Key));
	Key* sKey = (Key*)malloc(sizeof(Key));

	init_pair_keys(pKey,sKey,LOW_SIZE,UP_SIZE);
	
	//creation du bloc test
	Block* b = (Block*)malloc(sizeof(Block));
	b->author = pKey;
	b->votes = read_protected("declarations.txt");
	b->hash = NULL;
	b->previous_hash = crypt_SHA256("Rosetta code");
	b->nonce = 0;

	clock_t temps_initial;
	clock_t temps_final;
	double temps_cpu;
	int d = ZEROS;
	srand(time(NULL));
	
	FILE* f = fopen("POW_performance.txt","w");
	
	for(int i=0;i<d;i++){
	    
    	temps_initial = clock();
    	compute_proof_of_work(b,i);
    	temps_final = clock();
    	
    	temps_cpu = ((double)(temps_final - temps_initial))/CLOCKS_PER_SEC;
    	
    	fprintf(f,"%d %f \n",i,temps_cpu);
    	
    	printf("i : %d\n",i);
		for(int i = 0;i < SHA256_DIGEST_LENGTH;i++)
			printf("%02x",b->hash[i]);
		putchar('\n');
		printf("Nonce: %d\n",b->nonce);

    	
	}
	
	free(sKey);
	free(b->author);
	delete_list_protected(b->votes);
	free(b);
	fclose(f);
	
	return 0;
}
