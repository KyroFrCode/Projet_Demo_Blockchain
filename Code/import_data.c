#include "import_data.h"

CellKey* create_cell_key(Key* key){
	
	if(key == NULL){
		
		printf("clé valeur NULL(create_cell_key)\n");
		return NULL;
	}
	
	CellKey* ck = (CellKey*)malloc(sizeof(CellKey));
	
	if(ck == NULL){
		printf("Erreur d'allocation CellKey(create_cell_key)\n");
		return NULL;
	}
	
	ck->data = key;
	ck->next = NULL;
	
	return ck;	
}

CellKey* inserer_tete_key(CellKey* list_key, Key* key){

		if(key == NULL){
			printf("clé valeur NULL(inserer_tete_key)\n");
		}

        if(list_key == NULL){
        
            list_key = create_cell_key(key);
            return list_key;
        }

		CellKey* k = create_cell_key(key);

		k->next = list_key;
		return k;
}

CellKey* read_public_keys(char* pKeys_file){

	char* buffer = (char*)malloc(sizeof(char)*256);
	
	if(buffer == NULL){
		printf("Erreur allocation buffer(read_public_keys)\n");
		return NULL;
	}
	
	char* str_key = (char*)malloc(sizeof(char)*256);
	
	if(str_key == NULL){
		printf("Erreur allocation str_key(read_public_keys)\n");
		free(buffer);
		return NULL;
	}
	
	CellKey* ck = NULL;
	
	FILE* f = fopen(pKeys_file,"r");
	
	if(f == NULL){
		
		printf("Erreur ouverture du fichier\n");
		free(buffer);
		free(str_key);
		return NULL;
	}
	//recupere les clés du fichier
	while(fgets(buffer,256,f) != NULL){
		
		sscanf(buffer,"%s",str_key);
		ck = inserer_tete_key(ck,str_to_key(str_key));
	}
	
	fclose(f);
	free(buffer);
	free(str_key);
	
	return ck;
}

void print_list_keys(CellKey* LCK){
    
	if(LCK == NULL){
		return;	
	}

	CellKey* tmp = LCK;
	char* key_str = NULL;
	
	while(tmp != NULL){
	
		key_str = key_to_str(tmp->data);
		
		printf("%s\n",key_str);
		free(key_str);
		
		tmp = tmp->next;
		
	}
}

void delete_cell_key(CellKey* c){
	
	if(c != NULL){
		free(c->data);
		free(c);
	}
}

void delete_list_keys(CellKey* l){

	if(l==NULL){
		return;
	}

	CellKey* tmp_l = NULL;
	
	while(l != NULL){
		
		tmp_l = l->next; 
		delete_cell_key(l);
		l = tmp_l;
		
	}
}

CellProtected* create_cell_protected(Protected* pr){

	CellProtected* cp = (CellProtected*)malloc(sizeof(CellProtected));
	
	if(cp == NULL){
		printf("Erreur allocation cellprotected(create_cell_protected)\n");
		return NULL;
	}
	
	cp->data = pr;
	cp->next = NULL;
	
	return cp;
}

CellProtected* inserer_tete_protected(CellProtected* cl, Protected* pr){

	if(cl == NULL){
		cl = create_cell_protected(pr);
		return cl;
	}

	CellProtected* c = create_cell_protected(pr);

	c->next = cl;
	return c;
}

CellProtected* read_protected(char* declarations_file){

	char* buffer = (char*)malloc(sizeof(char)*256);
	
	if(buffer == NULL){
		printf("Erreur allocation buffer(read_public_keys)\n");
		return NULL;
	}
	
	CellProtected* cp = NULL;
	
	FILE* f = fopen(declarations_file,"r");
	
	if(f == NULL){
		
		printf("Erreur ouverture du fichier\n");
		free(buffer);
		return NULL;
	}
	
	while(fgets(buffer,256,f) != NULL){
		cp = inserer_tete_protected(cp,str_to_protected(buffer));
	}
	
	fclose(f);
	free(buffer);
	
	return cp;
}

void print_list_protected(CellProtected* c){

	if(c == NULL){
		return;	
	}

	CellProtected* tmp = c;
	char* pr_str = NULL;
	
	while(tmp != NULL){
		
		pr_str = protected_to_str(tmp->data);
		
		printf("%s\n",pr_str);
		free(pr_str);
		
		tmp = tmp->next;
	}
}

void delete_cell_protected(CellProtected* c){

	if(c != NULL){
		libere_protected(c->data);
		free(c);
	}
}

void delete_list_protected(CellProtected* cl){

	if(cl==NULL){
		return;
	}

	CellProtected* tmp_cl = NULL;
	
	while(cl != NULL){
		
		tmp_cl = cl->next; 
		delete_cell_protected(cl);
		cl = NULL;
		cl = tmp_cl;
		
	}
}

CellProtected* verify_declarations(CellProtected* cp){

	CellProtected* tmp = cp;
	CellProtected* pred = NULL;
	
	while(tmp!=NULL){
		
		//si declaration fausse on supprime de la liste
		if(verify(tmp->data) == 0){
			if(pred == NULL){
			
				cp = cp->next;
				delete_cell_protected(tmp);
				tmp = NULL;
				pred = tmp;
				tmp = cp;
			
			}else{
				pred->next = tmp->next;
				delete_cell_protected(tmp);
				tmp = pred->next;
			}
		}
		//sinon on passe au suivant
		else{
		    pred = tmp;
		    tmp = tmp->next;
		}
	}
	return cp;
}

void libere_contenue_cellProtected_list(CellProtected* cp){
	
	CellProtected* tmp = cp;
	
	while(tmp!=NULL){
		libere_protected(tmp->data);
		tmp = tmp->next;
	}
}
