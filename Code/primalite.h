#ifndef PRIMALITE_H
#define PRIMALITE_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int is_prime_naive(long p);//verifie si le nombre est premier ou non

long modpow_naive(long a, long m, long n);//modulation exponentiel naive (a^b%n)

int modpow(long a,long m,long n);//modulation exponentiel améliorer par succession elevation au carré

int witness(long a, long b, long d, long p);//test si a est un temoin de Miller pour p

long rand_long(long low, long up);//retourne un long compris entre low et up inclus

int is_prime_miller(long p, int k);//realise le test de Miller-Rabin

long random_prime_number(int low_size, int up_size, int k);//retourne un nombre premier de taille comprise entre low_size et up_size

#endif
