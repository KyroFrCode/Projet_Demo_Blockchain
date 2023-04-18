#ifndef GENERATE_H
#define GENERATE_H

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "primalite.h"

long extended_gcd(long s, long t, long *u, long *v);//fonction representant l'algorithme d'euclide(PGCD)

void generate_key_values(long p, long q, long *n, long *s, long *u);//genere les valeurs d'une clé

long* encrypt(char* chaine, long s, long n);//chiffre une chaine de caractere avec la clé secrete et retourne un tableau de long

char* decrypt(long* crypted, int size, long u, long n);//decrypte un tableau de long avec la clé publique

#endif
