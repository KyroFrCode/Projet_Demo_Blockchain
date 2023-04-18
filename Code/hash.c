#include "hash.h"

HashCell* create_hashcell(Key* key){

    HashCell* hc = (HashCell*)malloc(sizeof(HashCell));
    
    if(hc == NULL){
    	printf("Erreur allocation hashcell(create_hashcell)\n");
    	return NULL;
    }
    
    hc->key = (Key*)malloc(sizeof(Key));
    
    if(hc->key == NULL){
    	printf("Erreur allocation key(create_hashcell)\n");
    	return NULL;
    }
    
    init_key(hc->key,key->val,key->n);
    hc->val = 0;
    
    return hc;
}

int hash_function(Key* key, int size){

    return (key->val)%size;
}

int find_position(HashTable* t, Key* key){

    int i = hash_function(key,t->size);
    int j = i;
    int cpt = 0;
    
    HashCell* cell = t->tab[i];//recupere emplacement de la clé donnée par hash_function
    
    //s'il n'y aucune clé ou si la clé est bien a cette position on retourne la position(table de hachage fonctionne sous probing lineaire)
    if( cell == NULL || ((cell->key->val == key->val) && (cell->key->n == key->n))){
        return i;
    }
    //on cherche sur les cases suivantes si la clé est bien la
    else{
    
        while(cpt < t->size){ 
        
        	if(cell == NULL || ((cell->key->val == key->val) && (cell->key->n = key->n))){
        	return j;
        	}
            j = (j+1)%t->size; //si j>size on repart au debut table de hachage avec "%t->size"
            cpt++;
            cell = t->tab[j];
        }
        
        //si la clé n'est pas trouvé on retourne la position ou elle devait etre
        return i;
    }
}

HashTable* create_hashtable(CellKey* keys, int size){
    
    HashTable* t = (HashTable*)malloc(sizeof(HashTable));
    
    if(t == NULL){
    	printf("Erreur d'allocation structure hashtable(create_hashtable)\n");
    	return NULL;
    }
    
    t->tab = (HashCell**)malloc(sizeof(HashCell*)*size);
    
    if(t->tab == NULL){
    	printf("Erreur d'allocation hashtable(create_hashtable)\n");
    	free(t);
    	return NULL;
    }
    
    t->size = size;
    
    CellKey* tmp = keys;
    HashCell* hc = NULL;
    
    //initialise le tableau a NULL
    for(int i=0;i<size;i++){
        t->tab[i] = NULL;
    }
    
    int k = 0;
    int j = 0;
    int cpt = 0;
    
    //on place les clés dans la table de hachage selon la methode du probing lineaire
    while(tmp!=NULL){
        
        k = hash_function(tmp->data,size);
        
        if(t->tab[k] == NULL){
            
            hc = create_hashcell(tmp->data);
            t->tab[k] = hc;
        }
        else{
            
            j=k;
            
            while(t->tab[j] != NULL && cpt<t->size){
				
				cpt++;
                j = (j+1)%t->size;
            }
            
            hc = create_hashcell(tmp->data);
            t->tab[j] = hc;
        }
        
        tmp = tmp->next;
    }
    
    return t;
}

void delete_hashtable(HashTable* t){
    
   HashCell** tab = t->tab;
   
   for(int i=0;i<t->size;i++){
   
   	if(tab[i] != NULL){
   	
   		free(tab[i]->key);
   		free(tab[i]);
   	}
   }
   free(t->tab);
   free(t);
}

Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizeV){

	HashTable* hc = create_hashtable(candidates,sizeC);
	HashTable* hv = create_hashtable(voters,sizeV);
	
	CellProtected* d = decl;
	int kv = 0;
	int kc = 0;
	Key* pKeyc = NULL;
	char* pKeyc_str = NULL;
	HashCell* winner = NULL;
	
	//verification et comptabilisation des votes
	while(d != NULL){
		
		kv = find_position(hv,d->data->pKey);
		pKeyc = str_to_key(d->data->mess);
		kc = find_position(hc,pKeyc);
		
		if((hc->tab)[kc] != NULL){
			
			pKeyc_str = key_to_str((hc->tab)[kc]->key);
			
			if((!(hv->tab)[kv]->val) && (strcmp(pKeyc_str,d->data->mess) == 0)){
				(hv->tab)[kv]->val = 1;
				(hc->tab)[kc]->val += 1;
			}
			
			free(pKeyc_str);
		}
		
		free(pKeyc);
		pKeyc = NULL;
		d = d->next;
	}
	
	//Compare le nombre de vote des candidates et recupere la clé publique du gagnant
	for(int i=0;i<sizeC;i++){
		
		if(winner == NULL && ((hc->tab)[i]!= NULL)){
			
			winner = (HashCell*)malloc(sizeof(HashCell));
			winner->key = (Key*)malloc(sizeof(Key));
			winner->key->val = (hc->tab)[i]->key->val;
			winner->key->n = (hc->tab)[i]->key->n;
			winner->val = (hc->tab)[i]->val;
			
		}else if(((hc->tab)[i] != NULL) && (winner->val<(hc->tab[i]->val))){
			
			winner->key->val = (hc->tab)[i]->key->val;
			winner->key->n = (hc->tab)[i]->key->n;
			winner->val = (hc->tab)[i]->val;
			
		}
	}
	
	pKeyc = winner->key;
	free(winner);
	
	//suppression des hashtables
	delete_hashtable(hc);
	delete_hashtable(hv);
	
	return pKeyc;
}
