#include "primalite.h"
#include "generate.h"
#include "manipulation_cle.h"
#include "signature.h"

void print_long_vector(long *result, int size){
	
	printf("Vector: [");
	
	for(int i=0;i<size;i++){
		
		printf("%lx \t",result[i]);
	}
	
	printf("]\n");
}

int main(void){

	srand(time(NULL));
	
	//Testing Init Keys
	Key* pKey = malloc(sizeof(Key));
	Key* sKey = malloc(sizeof(Key));
	
	init_pair_keys(pKey,sKey,3,7);
	
	printf("pKey: %lx, %lx\n",pKey->val,pKey->n);
	printf("sKey: %lx, %lx\n",sKey->val,sKey->n);
	
	//Testing Key Serialisation
	char* chaine = key_to_str(pKey);
	
	printf("key_to_str: %s \n",chaine);
	
	Key* k = str_to_key(chaine);
	
	printf("str_to_key: %lx, %lx \n",k->val,k->n);

    free(chaine);
	free(k);

	//Testing signature
	
	//Candidate keys:
	Key* pKeyC = malloc(sizeof(Key));
	Key* sKeyC = malloc(sizeof(Key));
	
	init_pair_keys(pKeyC,sKeyC,3,7);
	
	//Declaration:
	char* mess = key_to_str(pKeyC);
	char* pKey_str = key_to_str(pKey);
	
	printf("%s vote pour %s\n",pKey_str,mess);
	
	Signature* sgn = sign(mess, sKey);
	
	printf("signature: ");
	print_long_vector(sgn->content,sgn->size);
	
	chaine = signature_to_str(sgn);
	printf("signature_to_str: %s \n", chaine);

	libere_signature(sgn);

	sgn = str_to_signature(chaine);
	printf("str_to_signature: ");
	print_long_vector(sgn->content,sgn->size);
	
	free(pKey_str);
	free(chaine);
	
	//Testing protected:
	Protected* pr = init_protected(pKey,mess,sgn);
	
	free(mess);
	libere_signature(sgn);
	
	
	//Verification:
	if(verify(pr)){
	
		printf("Signature valide \n");
	}else{
		
		printf("Signature non valide\n");
	}
	
	chaine = protected_to_str(pr);
	printf("protected_to_str: %s\n", chaine);
	
	libere_protected(pr);
	
	pr = str_to_protected(chaine);
	
	free(chaine);
	
	pKey_str = key_to_str(pr->pKey);
	char* sgn_str = signature_to_str(pr->sgn);
	
	printf("str_to_protected: %s %s %s\n",pKey_str,pr->mess,sgn_str);
	
	libere_protected(pr);
	free(pKey_str);
	free(sgn_str);
	
	free(pKey);
	free(sKey);
	free(pKeyC);
	free(sKeyC);
	
	return 0;
}
	
