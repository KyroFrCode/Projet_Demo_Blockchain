#include "block.h"

void write_block(Block* b,char *filename){
	
	if(b == NULL){
		printf("Erreur bloc null (write_block)\n");
		return;
	}
	
	FILE* f = fopen(filename,"w");
	
	if(f == NULL){
		printf("Erreur ouverture fichier (write_block)\n");
		return;
	}
	
	char* author_str = key_to_str(b->author);
	CellProtected* votes = NULL;
	CellProtected* tete_votes = NULL;//permet de garder l'en-tete de la liste de declarations
	Protected* pr = NULL;
	CellProtected* tmp = b->votes;
	char* pr_str = NULL;	
	char hash_value[256];
	char previous_hash_value[256];
	
	//boucle evitant l'inversion de l'ecriture des declarations dans le fichier (evite d'avoir un bloc avec des declarations inverser en ordre lors de la lecture)
	while(tmp != NULL){
		
		pr = init_protected(tmp->data->pKey,tmp->data->mess,tmp->data->sgn);
		
		votes = inserer_tete_protected(votes,pr);
		
		tmp = tmp->next;
	}
	
	//recupere le 1er element de la liste de protected(pour la suppression a la fin)
	tete_votes = votes;
	
	//valeur haché par default si aucune valeur
	if(b->hash == NULL){
		b->hash = crypt_SHA256("0"); //cette valeur peut etre changé plus tard mais necessaire pour eviter des erreurs
	}
	
	//hv et phv pointeur char* temporaire necessaire pour sprintf() (se referer a la documentation sprintf())
	char* hv = hash_value; 
	char* phv = previous_hash_value;
	
	// ecriture des valeurs hachés en hexadecimal dans des chaines de caracteres (pour l'ecriture dans un fichier)
	for(int i = 0;i < SHA256_DIGEST_LENGTH;i++){
		hv += sprintf(hv,"%02x",b->hash[i]);
		phv += sprintf(phv,"%02x",b->previous_hash[i]);
	}
	
	//ecriture du bloc dans le fichier
	fprintf(f,"%s %s %s %d\n",author_str,hash_value,previous_hash_value,b->nonce);
	
	while(votes != NULL){
		
		pr_str = protected_to_str(votes->data);
		
		fprintf(f,"%s\n",pr_str);
		free(pr_str);
		
		votes = votes->next;
	}
	
	delete_list_protected(tete_votes);
	free(author_str);
	fclose(f);
}

Block* read_block(char* filename){

	FILE* f = fopen(filename,"r");
	
	if(f == NULL){
		printf("Erreur ouverture fichier (write_block)\n");
		return NULL;
	}
	
	char* buffer = (char*)malloc(256*sizeof(char));//buffer pour recuperer les lignes du fichier via fgets (eviter les problemes de type buffer overflow avec les fscanf)
	
	if(buffer == NULL){
		printf("Erreur allocation buffer (read_block)\n");
		return NULL;
	}
	
	Block* b = (Block*)malloc(sizeof(Block));
	
	if(b == NULL){
		printf("Erreur allocation block (read_block)\n");
		free(buffer);
		return NULL;
	}
	
	//recupere la 1er ligne du fichier contenant la clé de l'auteur les valeurs haché et le nonce et reconstruit le bloc (les declarations sont recuperer plus tard via un autre fgets)
	if(fgets(buffer,256,f) != NULL){
	
		char* author_str = (char*)malloc(256*sizeof(char));
		
		if(author_str == NULL){
			printf("Erreur allocation author_str (read_block)\n");
			free(buffer);
			free(b);
			return NULL;
		}
		
		unsigned char hash[SHA256_DIGEST_LENGTH];
		unsigned char previous_hash[SHA256_DIGEST_LENGTH];
		char* hash_value = (char*)malloc(256*sizeof(char));//cette chaine recuperer les valeurs haché en hexadecimal
		
		if(hash_value == NULL){
			printf("Erreur allocation author_str (read_block)\n");
			free(buffer);
			free(b);
			free(author_str);
			return NULL;
		}
		
		char* previous_hash_value = (char*)malloc(256*sizeof(char));//cette chaine recuperer les valeurs haché en hexadecimal
		
		if(previous_hash_value == NULL){
			printf("Erreur allocation author_str (read_block)\n");
			free(buffer);
			free(b);
			free(author_str);
			free(hash_value);
			return NULL;
		}
		
		int nonce = 0;
		CellProtected* votes = NULL;
		
		//on stock chaque elements dans leur chaine respectif pour les traiter si necessaire (comme les clé publique etc)
		if(sscanf(buffer,"%s %s %s %d",author_str,hash_value,previous_hash_value,&nonce) == 4 ){
			
			char* hv = hash_value;
			char* phv = previous_hash_value;
			
			//passage de hexadecimal a des caracteres non signés des valeurs haché (hexa -> unsigned char)
			for(int i = 0;i < SHA256_DIGEST_LENGTH;i++){
				sscanf(hv,"%02hhx",&hash[i]);
				sscanf(phv,"%02hhx",&previous_hash[i]);
				hv += 2; //+2 car chaque valeur en hexa sous 2 caracteres : %02x dans le fichier
				phv += 2;
			}
			
			b->author = str_to_key(author_str);
			b->hash = hash;
			b->previous_hash = previous_hash;
			b->nonce = nonce;
			
			//on recupere les declarations de votes du fichier
			while(fgets(buffer,256,f) != NULL){
				votes = inserer_tete_protected(votes,str_to_protected(buffer));
			}
			
			b->votes = votes;
			
			//on libere les chaines de caracteres temporaires
			free(author_str);
			free(hash_value);
			free(previous_hash_value);
			free(buffer);
			fclose(f);
			
			return b;
		
		//si le format du fichier est incorrect, manque d'un element on non conforme
		}else{
		
			printf("Erreur probleme de format fichier(read_block_file)\n");
			free(author_str);
			free(hash_value);
			free(previous_hash_value);
			free(buffer);
			fclose(f);
			return NULL;
		}
	
	//probleme fichier vide
	}else{
		printf("Erreur fichier vide(read_block_file)\n");
		free(buffer);
		fclose(f);
		return NULL;
	}
}

int length_block(Block* b){
	
	int size = 0;
	char* author_str = key_to_str(b->author);
	char* pr_str = NULL;
	CellProtected* tmp = b->votes;
	
	size = strlen(author_str)+strlen(b->previous_hash)+(b->nonce)%10+10;
	
	while(tmp!=NULL){
	
		pr_str = protected_to_str(tmp->data);
		size += strlen(pr_str)+5;
		free(pr_str);
		tmp = tmp->next;
	}
	
	free(author_str);
	return size;
}

char* block_to_str(Block* b){
	
	int size_str = length_block(b);//recupere la taille du bloc sous forme de chaine de caractere (pour allocation de la chaine de caractere)
	char* author_str = key_to_str(b->author);
	char* pr_str = NULL;
	char* block_str = (char*)malloc(size_str*sizeof(char));
	
	if(block_str == NULL){
		printf("Erreur d'allocation block_str(block_to_str)\n");
		return NULL;
	}
	
	char* previous_hash_value = (char*)malloc(256*sizeof(char));
	
	if(previous_hash_value == NULL){
		printf("Erreur d'allocation previous_hash_value(block_to_str)\n");
		free(block_str);
		return NULL;
	}
	
	char* phv = previous_hash_value;
	
	//recupere la valeur haché sous forme hexadecimal %02x
	for(int i = 0;i < SHA256_DIGEST_LENGTH;i++){
		phv += sprintf(phv,"%02x",b->previous_hash[i]);	
	}
	
	sprintf(block_str,"%s%s%d",author_str,previous_hash_value,b->nonce);
	
	CellProtected* tmp = b->votes;
	
	while(tmp!=NULL){
		
		pr_str = protected_to_str(tmp->data);
		strncat(block_str,pr_str,strlen(pr_str)+5);//concatenation des declarations a la chaine de caractere finale
		free(pr_str);
		
		tmp = tmp->next;
	}
	
	free(author_str);
	free(previous_hash_value);
	
	return block_str;
}

unsigned char* crypt_SHA256(char* str){

	unsigned char* d = SHA256(str,strlen(str),0);
	
	return d;
}

void compute_proof_of_work(Block *B, int d){
	
	B->nonce = 0;
	unsigned char* hash_value = NULL;
	char* block_str = NULL;
	int proof = 0;//variable temporaire permettant de verfier la validité du bloc
	
	while(!proof){
	
		B->nonce++;
		block_str = block_to_str(B);//recupere la chaine de caractere representant le bloc
		hash_value = crypt_SHA256(block_str);
		free(block_str);
		
		proof = 1;
		
		for(int i=0;i<d;i++){
			
			//verification de la validité du bloc (d zeros sucessives)
			if(hash_value[i] != 0){
			
				proof = 0;
				break;			
			}
		}
		
		//affichage de la valeur haché
		/*
		for(int i = 0;i < SHA256_DIGEST_LENGTH;i++)
			printf("%02x",hash_value[i]);
		putchar('\n');
		printf("Nonce: %d\n",B->nonce);
		*/
	}
	
	B->hash = hash_value;
}

int verify_block(Block* b,int d){
	
	for(int i=0;i<d;i++)
			if(b->hash[i] != 0)
				return 0;
	return 1;
}

void delete_block(Block* b){

	if(b->votes == NULL){
		free(b);
		return;
	}

	CellProtected* tmp = b->votes;
	CellProtected* tmp2 = NULL;
	
	//supprime seulement les cellules de cellProtected(les données protected ne sont pas supprimer des cellules!)
	while(tmp != NULL){
	
		tmp2 = tmp->next;
		free(tmp);
		tmp = tmp2;
	}
	free(b);
}

void delete_full_block(Block* b){
	
	free(b->author);
	delete_list_protected(b->votes);
	free(b);
}

