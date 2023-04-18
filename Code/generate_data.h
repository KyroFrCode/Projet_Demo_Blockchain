#ifndef GENERATE_DATA_H
#define GENERATE_DATA_H

#include <stdio.h>
#include "primalite.h"
#include "generate.h"
#include "manipulation_cle.h"
#include "signature.h"

void generate_random_data(int nv, int nc);

/*
genere nv citoyens et nv candidates parmi les citoyens et les fichiers "keys.txt","candidates.txt" et "declarations.txt"

- "keys.txt" : contient les nv couples de clés différents des citoyens (un couple par ligne)

- "candidates.txt" : contient les clés publiques de tous les candidats choisis aléatoirement parmi les citoyens (une clé publique par ligne)

- "declarations.txt" : contient les declarations signées de chaques citoyens (une declaration par ligne)

*/

#endif
