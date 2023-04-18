#include <openssl/sha.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
	
	//fichier test de la technologie SHA-256
		
	const char *s = "Rosetta code";
	unsigned char *d = SHA256(s,strlen(s),0);
	int i;
	for(i = 0;i < SHA256_DIGEST_LENGTH;i++)
		printf("%02x",d[i]);
	putchar('\n');
	
}
