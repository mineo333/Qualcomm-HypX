#include <stdio.h>
#include <stdlib.h>

int uh_app_id = 0xc300c002;
typedef unsigned long ulong;
typedef unsigned int uint;


int main(){
  uint uVar1;
  
  if ((uh_app_id != 0x80008000) &&
     ((uVar1 = (uh_app_id & 0x3fffffff) + 0xfc000000, 0x21 < uVar1 ||
      ((1L << ((ulong)uVar1 & 0x3f) & 0x200001c0fU) == 0)))) {
    	printf("0");
	return 0;
  }
  printf("1");
	

}
