#include "primalite.h"
#include <math.h>

//complexite en O(p)
int is_prime_naive(long p){ 
    
   for(int i=3;i<p-1;i++){
   
   		if(p%i==0){
   			return 0;
   		}
	}
	return 1;
}

//complexite en O(a^m)
long modpow_naive(long a, long m, long n){
	
	long res = a;
	
	for(int i=0;i<m-1;i++){
		
		res = res*a;
		res = res%n;
	}
	
	return res;
}
//complexite en O(log2(m))
int modpow(long a,long m,long n){
    
    long res = 1;	

    while(m > 0){
    
        if(m%2==1){
            res = (a*res)%n;
        }
        
        m = m/2;
        a = (a*a)%n;
    }
    
    return res;
}

int witness(long a, long b, long d, long p){
	
	long x = modpow(a,d,p);
	
	if(x == 1){
	
		return 0;
	}
	for(long i = 0;i<b;i++){
	
		if(x == p-1){
		
			return 0;
		}
		
		x = modpow(x,2,p);
	}
	
	return 1;
}

long rand_long(long low, long up){
	return rand() % (up-low+1)+low;
}

int is_prime_miller(long p, int k){
	
	if(p == 2){
	
		return 1;
	}
	if (!(p & 1) || p <= 1){
	
		return 0;
	}
	
	long b = 0;
	long d = p-1;
	
	while(!(d & 1)){
	
		d = d/2;
		b = b+1;
	}
	
	long a;
	int i;
	for(i =0;i<k;i++){
	
		a = rand_long(2,p-1);
		if(witness(a,b,d,p)){
		
			return 0;
		}
	}
	
	return 1;
}

long random_prime_number(int low_size, int up_size, int k){
    
    long entier = 0;
    
    do{
    	entier = rand_long(pow(2,(low_size-1)),pow(2,up_size)-1);
    	
    }while(is_prime_miller(entier,k) == 0);
    
    return entier;
}

