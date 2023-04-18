#include "signature.h"

Signature* init_signature(long* content, int size){

	if(content == NULL){
		printf("tableau content NULL en entree (init_signature)\n");
		return NULL;
	}

    Signature* sgn = (Signature*)malloc(sizeof(Signature));
    
    if(sgn == NULL){
    	printf("Probleme allocation de sgn (init_signature)\n");
    	return NULL;
    }
    
    sgn->content = (long*)malloc(sizeof(long)*size);
    
    if(sgn->content == NULL){
    	printf("Probleme allocation memoire de content(init_signature)\n");
    	free(sgn);
    	return NULL;
    }
    
    sgn->size = size;
    
    memcpy(sgn->content,content,sizeof(long)*size);//on recopie la memoire du tableau content
    free(content);//on libere le tableau content (eviter les fuites memoires venant de la fonction str_to_signature donnée dans l'énoncé
    
    return sgn;
}

Signature* sign(char* mess, Key* sKey) {
    
    if(mess == NULL || sKey == NULL){
    	printf("Erreur mess ou sKey NULL en entree(sign)\n");
    	return NULL;
    }
    
    long* tab_encrypt = encrypt(mess,sKey->val,sKey->n);
    
    Signature* sgn = init_signature(tab_encrypt,strlen(mess));
    
    return sgn;
}

char* signature_to_str(Signature* sgn){
	
	if(sgn == NULL){
		printf("Erreur sgn NULL en entree(signature_to_str)\n");
		return NULL;
	}
	
	char* result = malloc(10*sgn->size*sizeof(char));
	
	if(result == NULL){
		printf("Erreur allocation result (signature_to_str)\n");
		return NULL;
	}
	
	result[0] = '#';
	int pos = 1;
	char buffer[156];
	
	for(int i=0;i<sgn->size;i++){
	
		sprintf(buffer,"%lx",sgn->content[i]);
		
		for(int j=0;j < strlen(buffer);j++){
			
			result[pos] = buffer[j];
			pos = pos+1;
		}
		
		result[pos] = '#';
		pos = pos+1;
		
	}
	
	result[pos] = '\0';
	result = realloc(result, (pos+1)*sizeof(char));
	
	return result;
}

Signature* str_to_signature(char* str){

	if(str == NULL){
		printf("Erreur str NULL en entree(str_to_signature)\n");
		return NULL;
	}

	int len = strlen(str);
	long* content = (long*)malloc(sizeof(long)*len);
	
	if(content == NULL){
		printf("Probleme allocation content(str_to_signature)\n");
		return NULL;
	}
	
	int num=0;
	char buffer[256];
	int pos = 0;
	
	for(int i=0;i < len;i++){
		
		if(str[i] != '#'){
			
			buffer[pos] = str[i];
			pos= pos+1;
		}else{
			
			if(pos != 0){
			
				buffer[pos] = '\0';
				sscanf(buffer, "%lx", &(content[num]));
				num = num+1;
				pos = 0;
			}
		}
	}
	
	content = realloc(content,num*sizeof(long));
	
	return init_signature(content,num);
}


Protected* init_protected(Key* pKey,char* mess,Signature* sgn){
	
	if(pKey == NULL || mess == NULL || sgn == NULL){
		printf("Erreur pKey, mess ou sgn NULL en entree(init_protected)\n");
		return NULL;
	}
	
	Protected* p = (Protected*)malloc(sizeof(Protected));
	
	if(p == NULL){
		printf("Probleme allocation de p(init_protected)\n");
		return NULL;
	}
	
	p->pKey = (Key*)malloc(sizeof(Key));
	
	if(p->pKey == NULL){
		printf("Probleme allocation de pKey(init_protected)\n");
		free(p);
		return NULL;
	}
	memcpy(p->pKey,pKey,sizeof(Key));//on copie la memoire de la clé
	
	p->mess = strdup(mess);
	
	if(p->mess == NULL){
		printf("Probleme allocation de mess(init_protected)\n");
		free(p->pKey);
		free(p);
		return NULL;
	}
	
	long* content = (long*)malloc((sgn->size)*sizeof(long));
	memcpy(content,sgn->content,(sgn->size)*sizeof(long));//on copie la memoire du tableau content
	
	//on realise des copies et non des affections pour eviter lors d'un free en externe d'affecter les structures et vider les contenues sans le vouloir
	
	p->sgn = init_signature(content,sgn->size);
	
	if(p->sgn == NULL){
		printf("Probleme allocation de sgn(init_protected)\n");
		free(p->mess);
		free(p->pKey);
		free(p);
		return NULL;
	}
	
	return p;
}

int verify(Protected* pr){

	if(pr == NULL || pr->pKey == NULL || pr->sgn == NULL){
		printf("Erreur pr (ou sous pointeur) NULL en entrée(verify)\n");
		return 0;
	}
	
	Signature* sgn = pr->sgn;
	Key* pKey = pr->pKey;
	char* message_decrypt;
	
	message_decrypt = decrypt(sgn->content,sgn->size,pKey->val,pKey->n);
	
	//si la signature est bien correct (message dechiffrer correspond au message) on retourne 1
	if(strcmp(message_decrypt,pr->mess) == 0){
		free(message_decrypt);
		return 1;
	}
	
	//sinon on retourne 0
	free(message_decrypt);
	return 0;
}

char* protected_to_str(Protected* p){

	if(p == NULL ||p->pKey == NULL || p->sgn == NULL){
		printf("Erreur p (ou sous pointeur) NULL en entree(protected_to_str)\n");
		return NULL;
	}

	char* str = (char*)malloc(sizeof(char)*256);
	
	if(str == NULL){
		printf("Probleme allocation de str(protected_to_str)\n");
		return NULL;	
	}
	
	char* key_str = key_to_str(p->pKey);
	char* signature_str = signature_to_str(p->sgn);
	
	sprintf(str,"%s %s %s",key_str,p->mess,signature_str);
	
	free(key_str);
	free(signature_str);
	return str;
}

Protected* str_to_protected(char* str){

	if(str == NULL){
		printf("Erreur str NULL en entree(str_to_protected)\n");
		return NULL;
	}

	Protected* p = (Protected*)malloc(sizeof(Protected));
	
	if(p == NULL){
		printf("Probleme allocation de p(str_to_protected)\n");
		return NULL;
	}
	
	char* str_pKey = (char*)malloc(sizeof(char)*256);
	
	if(str_pKey == NULL){
		printf("Probleme allocation de str_pKey(str_to_protected)\n");
		free(p);
		return NULL;
	}
	
	char* str_mess = (char*)malloc(sizeof(char)*256);
	
	if(str_mess == NULL){
		printf("Probleme allocation de str_mess(str_to_protected)\n");
		free(str_pKey);
		free(p);
		return NULL;
	}
	
	char* str_sgn = (char*)malloc(sizeof(char)*256);
	
	if(str_sgn == NULL){
		printf("Probleme allocation de str_sgn(str_to_protected)\n");
		free(str_mess);
		free(str_pKey);
		free(p);
		return NULL;
	}
	
	sscanf(str,"%s %s %s ",str_pKey,str_mess,str_sgn);
	
	p->pKey = str_to_key(str_pKey);
	p->sgn = str_to_signature(str_sgn);
	p->mess = (char*)malloc(sizeof(char)*((p->sgn->size)+1));
	
	if(p->mess == NULL){
	
		printf("Probleme allocation de mess(str_to_protected)\n");
		
		free(str_mess);
		
		if(p->pKey != NULL){
			free(p->pKey);
		}
		if(p->sgn != NULL){
			free(p->sgn->content);
			free(p->sgn);
		}
		free(p);
	}
	
	strncpy(p->mess,str_mess,(p->sgn->size)+1);//on copie le nombre de caractère exacte de la chaine de caractere de la signature dans la structure
	
	free(str_mess);
	free(str_pKey);
	free(str_sgn);

	return p;
}

void libere_signature(Signature* sgn){

	free(sgn->content);
	free(sgn);
}

void libere_protected(Protected* pr){
	
	free(pr->pKey);
	free(pr->mess);
	libere_signature(pr->sgn);
	free(pr);
}
