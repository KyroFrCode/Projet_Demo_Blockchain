#ifndef SIGNATURE_H
#define SIGNATURE_H

#include "manipulation_cle.h"
#include "generate.h"

typedef struct signature{//signature du vote (permet de verifier la validité du vote)
   
	long* content;//tableau de long contenant le message chiffre
	int size;//taille du tableau content

}Signature;

typedef struct protected{//structure contenant la clé publique de dechiffrement de la signature et le message permettant de verifier que le message dechiffrer est bien valide

	Key* pKey;//clé publique de l'auteur de la signature
	char* mess;//message a comparer avec le message dechiffrer pour la validité
	Signature* sgn;//signature déposer par l'auteur en chiffrant le message avec sa clé privé

}Protected;

Signature* init_signature(long* content, int size);//créer et initialise une signature avec le tableau contenant le message deja chiffre et la taille du tableau

Signature* sign(char* mess, Key* sKey);//permet de chiffrer le message avec la clé secrete et de retourne une signature initialisé

char* signature_to_str(Signature* sgn);//retourne une chaine de caractere representant la signature (format string : "#x0#x1#...#xn#" avec xi le ieme entier du tableau)

Signature* str_to_signature(char* str);//retourne la signature representé par la chaine de caractere

Protected* init_protected(Key* pKey,char* mess,Signature* sgn);//creer et initialise une declaration signé a partir d'une clé publique,d'un message et d'une signature

int verify(Protected* pr);//verifie que la declaration est bien valide

char* protected_to_str(Protected* p);//retourne une chaine de caractere representant la declaration (format string : "(%lx,%lx) (%lx,%lx) #x0#x1#...#xn#" avec la clé publique,le message et la signature) 

Protected* str_to_protected(char* str);//retourne une declaration representé par la chaine de caractere

void libere_signature(Signature* sgn);//libere la signature en memoire

void libere_protected(Protected* pr);//libere la declaration signées en memoire

#endif
