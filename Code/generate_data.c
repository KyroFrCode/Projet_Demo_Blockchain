#include "generate_data.h"

#define LOW_SIZE 3
#define UP_SIZE 7

void generate_random_data(int nv, int nc){

	FILE* fkey = fopen("keys.txt","w");
	FILE* fcandidates = fopen("candidates.txt","w");
	FILE* fdeclarations = fopen("declarations.txt","w");
	
	Key** tab_pKeys = (Key**)malloc(sizeof(Key*)*nv);
	
	if(tab_pKeys == NULL){
		printf("Erreur allocation tab_pKeys(generate_random_data)\n");
		return;
	}
	
	Key** tab_sKeys = (Key**)malloc(sizeof(Key*)*nv);
	
	if(tab_sKeys == NULL){
		printf("Erreur allocation tab_sKeys(generate_random_data)\n");
		free(tab_pKeys);
		return;
	}
	
	Key** tab_pKeys_candidates = (Key**)malloc(sizeof(Key*)*nc);
	
	if(tab_pKeys_candidates == NULL){
		printf("Erreur allocation tab_pKeys_candidates(generate_random_data)\n");
		free(tab_pKeys);
		free(tab_sKeys);
		return;
	}
	
	int tab_b[nv];//tableau de booléen utilisé pour eviter les doublons lors des choix des candidats
	int r_pKey = 0;
	char* pKey_str = NULL;
	char* sKey_str = NULL;
	char* pr_str = NULL;
	Signature* sgn = NULL;
	Protected* pr = NULL;
	
	//initialisation du tableau booléen a 0
	for(int l=0;l<nv;l++){
		tab_b[l] = 0;
	}
	
	//allocation des cases des tableaux
	for(int i=0;i<nv;i++){
		tab_pKeys[i] = (Key*)malloc(sizeof(Key));
		tab_sKeys[i] = (Key*)malloc(sizeof(Key));
	}
	//creation et ecriture des clés dans le fichiers "keys.txt"
	for(int j=0;j<nv;j++){
	
		init_pair_keys(tab_pKeys[j],tab_sKeys[j],LOW_SIZE,UP_SIZE);
		pKey_str = key_to_str(tab_pKeys[j]);
		sKey_str = key_to_str(tab_sKeys[j]);
		fprintf(fkey,"%s %s\n",pKey_str,sKey_str);
		
		free(pKey_str);
		free(sKey_str);
	}
	
	fclose(fkey);
	
	//recupere et ecrit les clés publiques des candidates dans le fichier "candidates.txt"
	r_pKey = rand()%nv;//retourne un nombre aléatoire entre 0 et nv (nv representant le nombre de citoyen créer precedement)
	tab_b[r_pKey] = 1;
	tab_pKeys_candidates[0] = tab_pKeys[r_pKey];
	pKey_str = key_to_str(tab_pKeys_candidates[0]);
	fprintf(fcandidates,"%s\n",pKey_str);//ecrit le 1er candidat dans le fichier
	
	free(pKey_str);
	
	//inscription des restes des candidates dans le fichier (nc represente le nombre de candidates selectionné);
	for(int k=1;k<nc;k++){
	
		while(tab_b[r_pKey]){
			r_pKey = rand()%nv;
		}
		
		tab_b[r_pKey] = 1;
		tab_pKeys_candidates[k] = tab_pKeys[r_pKey];
		pKey_str = key_to_str(tab_pKeys_candidates[k]);
		fprintf(fcandidates,"%s\n",pKey_str);
		
		free(pKey_str);
	}
	
	fclose(fcandidates);
	
	//ecriture des declarations signées dans le fichier "declarations.txt"
	for(int v=0;v<nv;v++){
		
		pKey_str = key_to_str(tab_pKeys_candidates[rand()%nc]);
		sgn = sign(pKey_str,tab_sKeys[v]);
		pr = init_protected(tab_pKeys[v],pKey_str,sgn);
		
		free(pKey_str);
		libere_signature(sgn);
		
		//verifie si la declaration est bien valide avant de l'ecrire dans le fichier
		if(verify(pr)){
		
			pr_str = protected_to_str(pr);
			fprintf(fdeclarations,"%s\n",pr_str);
			
			free(pr_str);
		}
		
		libere_protected(pr);
	}

	fclose(fdeclarations);
	
	//libere les tableaux et leur cases
	for(int x=0;x<nv;x++){
	
		free(tab_pKeys[x]);
		free(tab_sKeys[x]);
	}
	
	free(tab_pKeys);
	free(tab_sKeys);
	free(tab_pKeys_candidates);
}
