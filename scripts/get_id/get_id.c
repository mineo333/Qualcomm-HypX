/**
 * The purpose of this script is to get the id from a string. This id is seen in the GetProps array and is used for identification.
 * 
 * This calculation is a cleaned up version of the decompilation from __path_resolution
 * 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//char HypX[] = "HypX";


int main(int argc, char** argv){
	if(argc < 2){
		printf("Usage ./get_id [string]\n");
		exit(-1);
	}
	char* str = argv[1];
	int id = 0x1505;

	for(int i = 0; i<strlen(str); i++){
		id = id*0x21 + str[i];
	}
	printf("0x%x\n", id);
	
}
