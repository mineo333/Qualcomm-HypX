#include <sys/mman.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>


#include "PropsInfo.h"
#include "ghidra_helpers.h"

/**
 * You should probably only use this on machines with 0x1000 page size. Otherwise you're not going to have a good time
*/

#define DEVCFG_BASE 0x805FD000
#define DEVCFG_SIZE 0xC3C
#define DEVCFG_OFF 0xC000
#define BASEADDR 0x805fd040 //baseAddr is 



void insanity(void *ptr,long base,ulong ptr_size);

int devcfg_fd = -1;
char* devcfg_base = 0x0;

int load_devcfg(){
    devcfg_fd = open("resources/devcfg.mbn", O_RDONLY);

    if(devcfg_fd < 0){
        perror("Error: ");
        return -1;
    }

    devcfg_base = mmap((void*)DEVCFG_BASE, DEVCFG_SIZE, PROT_READ, MAP_FIXED | MAP_PRIVATE, devcfg_fd, DEVCFG_OFF);

    if(devcfg_base == (char*)-1){
        perror("Error: ");
        return -1;
    }
    //let the fuckery begin    
    return 0;
}




int main(int argc, char* argv[]){

    if(argc < 3){
        printf("Usage: ./insanity [base addr of PropsInfo] [ptr_size]\n");
        return -1;
    }

    PropsInfo* props_info = (PropsInfo*)strtol(argv[1], NULL, 0);
    unsigned long ptr_size = atol(argv[2]);

   

    

    printf("props_info: %p\n", props_info);
    printf("ptr_size: %lx\n", ptr_size);

    if(load_devcfg()){
        close(devcfg_fd);
        return -1;
    }

     PropsInfo* baseProps = (PropsInfo*)BASEADDR;
    unsigned long baseAddr = baseProps->base;

    printf("baseAddr: %lx\n", baseAddr);

    

    if((unsigned long)props_info < DEVCFG_BASE || (unsigned long)props_info >= DEVCFG_BASE + DEVCFG_SIZE){ //sanity check
        printf("Invalid base address");
        return -1;
    }

    printf("Name sanity check: %s\n", props_info->base);
    printf("pDalPropsDirDevicepos: %x\n", props_info->pDalPropsDirDevicepos);

    void* ptr = 0;

    if(posix_memalign(&ptr, 16, ptr_size)){ //forcibly 16 byte align this shit
        printf("Failed to allocate memory necessary for ptr\n");
        return -1;
    }
    
    unsigned long base = baseAddr + props_info->pDalPropsDirDevicepos;
    
    

    insanity(ptr, base, ptr_size);

    for(int i = 0; i<ptr_size; i++){
        printf("%x\n", ((char*)ptr)[i] & 0xff);
    }

    printf("%x\n", *(int*)ptr);

    int param4 = 0;

    insanity(&param4, base+4, ptr_size);

    printf("%x\n", param4);
    
    
    return 0;
}




/*
I want to kill myself
*/
void insanity(void *ptr,long base,ulong ptr_size)

{
  undefined8 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined uVar4;
  undefined uVar5;
  undefined uVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  undefined8 uVar9;
  bool bVar10;
  long lVar11;
  undefined uVar12;
  long lVar13;
  undefined4 *puVar14;
  undefined4 *puVar15;
  undefined4 *puVar16;
  undefined4 *puVar17;
  uint uVar18;
  undefined4 uVar19;
  ulong uVar20;
  long lVar21;
  ulong uVar22;
  ulong uVar23;
  long uVar24;
  long lVar24;
  long lVar25;
  ulong uVar26;
  
  uVar20 = 2 - ptr_size;
  lVar13 = 0;
  lVar21 = ptr_size + 0xd;
  uVar24 = -ptr_size;
  lVar25 = ptr_size + 0xe;
  uVar23 = -ptr_size;
  lVar24 = ptr_size + 0xf;
  uVar26 = ~ptr_size;
  lVar11 = ptr_size + 0x10;
  while( true ) {
    uVar24 = uVar24 + 1;
    puVar14 = (undefined4 *)(base + lVar13);
    uVar22 = ptr_size - lVar13;
    if ((uVar22 == 0) || (((ulong)puVar14 & 3) == 0)) break;
    uVar20 = uVar20 + 1;
    lVar21 = lVar21 + -1;
    lVar25 = lVar25 + -1;
    *(undefined *)((long)ptr + lVar13) = *(undefined *)puVar14;
    uVar23 = uVar23 + 1;
    lVar24 = lVar24 + -1;
    lVar13 = lVar13 + 1;
    uVar26 = uVar26 + 1;
    lVar11 = lVar11 + -1;
  }
  puVar17 = (undefined4 *)((long)ptr + lVar13);
                    /* if the address is 4 byte aligned */
  if (((ulong)puVar17 & 3) == 0) {
    uVar20 = uVar26;
    if (uVar26 < 0xfffffffffffffff1) {
      uVar20 = 0xfffffffffffffff0;
    }
    lVar21 = 0;
    puVar17 = (undefined4 *)((long)ptr + lVar13 + (uVar20 + lVar11 & 0xfffffffffffffff0));
    while (bVar10 = 0xf < uVar22, uVar22 = uVar22 - 0x10, bVar10) {
      puVar15 = (undefined4 *)((long)puVar14 + lVar21);
      lVar25 = base + lVar21 + lVar13;
      uVar19 = *(undefined4 *)(lVar25 + 4);
      puVar16 = (undefined4 *)((long)ptr + lVar13 + lVar21);
      uVar2 = *(undefined4 *)((long)puVar14 + lVar21 + 8);
      lVar21 = lVar21 + 0x10;
      uVar3 = *(undefined4 *)(lVar25 + 0xc);
      *puVar16 = *puVar15;
      puVar16[1] = uVar19;
      puVar16[2] = uVar2;
      puVar16[3] = uVar3;
    }
    uVar20 = uVar26;
    if (uVar26 < 0xfffffffffffffff1) {
      uVar20 = 0xfffffffffffffff0;
    }
    uVar18 = (uint)(ptr_size - lVar13);
    if ((uVar18 >> 3 & 1) == 0) {
      puVar14 = (undefined4 *)(base + (uVar20 + lVar11 & 0xfffffffffffffff0) + lVar13);
    }
    else {
      if (uVar26 < 0xfffffffffffffff1) {
        uVar26 = 0xfffffffffffffff0;
      }
      uVar20 = uVar26 + lVar11 & 0xfffffffffffffff0;
      puVar15 = (undefined4 *)(base + uVar20 + lVar13);
      puVar16 = (undefined4 *)((long)ptr + lVar13 + uVar20);
      uVar19 = puVar15[1];
      puVar17 = puVar16 + 2;
      puVar14 = puVar15 + 2;
      *puVar16 = *puVar15;
      puVar16[1] = uVar19;
    }
    if ((uVar18 >> 2 & 1) != 0) {
      *puVar17 = *puVar14;
      puVar14 = puVar14 + 1;
      puVar17 = puVar17 + 1;
    }
    if ((uVar18 >> 1 & 1) != 0) {
      uVar12 = *(undefined *)puVar14;
      uVar6 = *(undefined *)((long)puVar14 + 1);
      puVar14 = (undefined4 *)((long)puVar14 + 2);
      *(undefined *)puVar17 = uVar12;
      *(undefined *)((long)puVar17 + 1) = uVar6;
      puVar17 = (undefined4 *)((long)puVar17 + 2);
    }
    if ((ptr_size - lVar13 & 1) == 0) {
      return;
    }
    uVar12 = *(undefined *)puVar14;
    goto LAB_80094080;
  }
  if (uVar22 < 0x20) {
LAB_80093cc8:
    if (((uint)uVar22 >> 4 & 1) == 0) goto LAB_80093ccc;
LAB_80093f4c:
    uVar12 = *(undefined *)((long)puVar14 + 1);
    uVar6 = *(undefined *)((long)puVar14 + 2);
    uVar4 = *(undefined *)((long)puVar14 + 3);
    *(undefined *)puVar17 = *(undefined *)puVar14;
    uVar5 = *(undefined *)(puVar14 + 1);
    *(undefined *)((long)puVar17 + 1) = uVar12;
    *(undefined *)((long)puVar17 + 2) = uVar6;
    *(undefined *)((long)puVar17 + 3) = uVar4;
    uVar12 = *(undefined *)((long)puVar14 + 5);
    uVar6 = *(undefined *)((long)puVar14 + 6);
    uVar4 = *(undefined *)((long)puVar14 + 7);
    *(undefined *)(puVar17 + 1) = uVar5;
    uVar5 = *(undefined *)(puVar14 + 2);
    *(undefined *)((long)puVar17 + 5) = uVar12;
    *(undefined *)((long)puVar17 + 6) = uVar6;
    *(undefined *)((long)puVar17 + 7) = uVar4;
    uVar12 = *(undefined *)((long)puVar14 + 9);
    uVar6 = *(undefined *)((long)puVar14 + 10);
    uVar4 = *(undefined *)((long)puVar14 + 0xb);
    *(undefined *)(puVar17 + 2) = uVar5;
    uVar5 = *(undefined *)(puVar14 + 3);
    *(undefined *)((long)puVar17 + 9) = uVar12;
    *(undefined *)((long)puVar17 + 10) = uVar6;
    *(undefined *)((long)puVar17 + 0xb) = uVar4;
    uVar12 = *(undefined *)((long)puVar14 + 0xd);
    uVar6 = *(undefined *)((long)puVar14 + 0xe);
    uVar4 = *(undefined *)((long)puVar14 + 0xf);
    puVar14 = puVar14 + 4;
    *(undefined *)(puVar17 + 3) = uVar5;
    puVar15 = puVar17 + 4;
    *(undefined *)((long)puVar17 + 0xd) = uVar12;
    *(undefined *)((long)puVar17 + 0xe) = uVar6;
    *(undefined *)((long)puVar17 + 0xf) = uVar4;
    if (((uint)uVar22 >> 3 & 1) == 0) goto LAB_80093cd0;
LAB_80093fdc:
    uVar12 = *(undefined *)((long)puVar14 + 1);
    uVar6 = *(undefined *)((long)puVar14 + 2);
    uVar4 = *(undefined *)((long)puVar14 + 3);
    *(undefined *)puVar15 = *(undefined *)puVar14;
    uVar5 = *(undefined *)(puVar14 + 1);
    *(undefined *)((long)puVar15 + 1) = uVar12;
    *(undefined *)((long)puVar15 + 2) = uVar6;
    *(undefined *)((long)puVar15 + 3) = uVar4;
    uVar12 = *(undefined *)((long)puVar14 + 5);
    uVar6 = *(undefined *)((long)puVar14 + 6);
    uVar4 = *(undefined *)((long)puVar14 + 7);
    puVar14 = puVar14 + 2;
    *(undefined *)(puVar15 + 1) = uVar5;
    puVar16 = puVar15 + 2;
    *(undefined *)((long)puVar15 + 5) = uVar12;
    *(undefined *)((long)puVar15 + 6) = uVar6;
    *(undefined *)((long)puVar15 + 7) = uVar4;
    if (((uint)uVar22 >> 2 & 1) == 0) goto LAB_80093cd4;
LAB_8009402c:
    uVar12 = *(undefined *)puVar14;
    uVar6 = *(undefined *)((long)puVar14 + 1);
    uVar4 = *(undefined *)((long)puVar14 + 2);
    uVar5 = *(undefined *)((long)puVar14 + 3);
    puVar14 = puVar14 + 1;
    *(undefined *)puVar16 = uVar12;
    puVar17 = puVar16 + 1;
    *(undefined *)((long)puVar16 + 1) = uVar6;
    *(undefined *)((long)puVar16 + 2) = uVar4;
    *(undefined *)((long)puVar16 + 3) = uVar5;
    uVar18 = (uint)uVar22;
  }
  else {
    uVar18 = (uint)puVar17 & 3;
    if (uVar18 == 1) {
      uVar12 = *(undefined *)((long)puVar14 + 2);
      uVar23 = uVar20;
      if (uVar20 < 0xfffffffffffffff0) {
        uVar23 = 0xffffffffffffffef;
      }
      uVar19 = *puVar14;
      *(undefined *)((long)puVar17 + 1) = *(undefined *)((long)puVar14 + 1);
      *(undefined *)((long)puVar17 + 2) = uVar12;
      *(char *)puVar17 = (char)uVar19;
      lVar24 = lVar13 + (uVar23 + lVar21 & 0xfffffffffffffff0) + 3;
      lVar25 = 0;
      for (uVar22 = uVar22 - 3; 0x10 < uVar22; uVar22 = uVar22 - 0x10) {
        lVar11 = base + lVar25 + lVar13;
        uVar7 = *(undefined8 *)(lVar11 + 4);
        uVar1 = CONCAT44(*(undefined4 *)(undefined8 *)(lVar11 + 4),uVar19);
        uVar8 = *(undefined8 *)(lVar11 + 8);
        uVar19 = *(undefined4 *)(lVar11 + 0x10);
        uVar9 = *(undefined8 *)(lVar11 + 0xc);
        lVar11 = lVar13 + lVar25;
        *(int *)((long)ptr + lVar11 + 3) = (int)((ulong)uVar1 >> 0x18);
        *(int *)((long)ptr + lVar11 + 7) = (int)((ulong)uVar7 >> 0x18);
        *(int *)((long)ptr + lVar11 + 0xb) = (int)((ulong)uVar8 >> 0x18);
        *(int *)((long)ptr + lVar11 + 0xf) = (int)((ulong)uVar9 >> 0x18);
        lVar25 = lVar25 + 0x10;
      }
      if (uVar20 < 0xfffffffffffffff0) {
        uVar20 = 0xffffffffffffffef;
      }
      uVar20 = uVar20 + lVar21 & 0xfffffffffffffff0;
      uVar22 = ((ptr_size - 3) - uVar20) - lVar13;
      puVar14 = (undefined4 *)(base + uVar20 + lVar13 + 3);
      uVar18 = (uint)uVar22;
    }
    else {
      if (uVar18 != 2) {
        if (uVar18 == 3) {
          uVar22 = uVar22 - 1;
          uVar19 = *(undefined4 *)(base + lVar13);
          uVar20 = uVar23;
          if (uVar23 < 0xffffffffffffffee) {
            uVar20 = 0xffffffffffffffed;
          }
          *(char *)((long)ptr + lVar13) = (char)uVar19;
          puVar17 = (undefined4 *)((long)ptr + lVar13 + (uVar20 + lVar24 & 0xfffffffffffffff0) + 1);
          lVar21 = 0;
          for (; 0x12 < uVar22; uVar22 = uVar22 - 0x10) {
            lVar25 = base + lVar21 + lVar13;
            uVar7 = *(undefined8 *)(lVar25 + 4);
            uVar1 = CONCAT44(*(undefined4 *)(undefined8 *)(lVar25 + 4),uVar19);
            uVar8 = *(undefined8 *)(lVar25 + 8);
            uVar19 = *(undefined4 *)(lVar25 + 0x10);
            uVar9 = *(undefined8 *)(lVar25 + 0xc);
            lVar25 = lVar13 + lVar21;
            *(int *)((long)ptr + lVar25 + 1) = (int)((ulong)uVar1 >> 8);
            *(int *)((long)ptr + lVar25 + 5) = (int)((ulong)uVar7 >> 8);
            *(int *)((long)ptr + lVar25 + 9) = (int)((ulong)uVar8 >> 8);
            *(int *)((long)ptr + lVar25 + 0xd) = (int)((ulong)uVar9 >> 8);
            lVar21 = lVar21 + 0x10;
          }
          if (uVar23 < 0xffffffffffffffee) {
            uVar23 = 0xffffffffffffffed;
          }
          uVar20 = uVar23 + lVar24 & 0xfffffffffffffff0;
          uVar22 = ((ptr_size - 1) - uVar20) - lVar13;
          puVar14 = (undefined4 *)(base + uVar20 + lVar13 + 1);
        }
        goto LAB_80093cc8;
      }
      uVar22 = uVar22 - 2;
      uVar19 = *puVar14;
      uVar12 = *(undefined *)((long)puVar14 + 1);
      lVar21 = uVar24;
      if ((ulong)uVar24 < 0xffffffffffffffef) {
        lVar21 = -0x12;
      }
      *(char *)puVar17 = (char)uVar19;
      *(undefined *)((long)puVar17 + 1) = uVar12;
      lVar24 = lVar13 + (lVar21 + lVar25 & 0xfffffffffffffff0U) + 2;
      lVar21 = 0;
      for (; 0x11 < uVar22; uVar22 = uVar22 - 0x10) {
        lVar11 = base + lVar21 + lVar13;
        uVar7 = *(undefined8 *)(lVar11 + 4);
        uVar1 = CONCAT44(*(undefined4 *)(undefined8 *)(lVar11 + 4),uVar19);
        uVar8 = *(undefined8 *)(lVar11 + 8);
        uVar19 = *(undefined4 *)(lVar11 + 0x10);
        uVar9 = *(undefined8 *)(lVar11 + 0xc);
        lVar11 = lVar13 + lVar21;
        *(int *)((long)ptr + lVar11 + 2) = (int)((ulong)uVar1 >> 0x10);
        *(int *)((long)ptr + lVar11 + 6) = (int)((ulong)uVar7 >> 0x10);
        *(int *)((long)ptr + lVar11 + 10) = (int)((ulong)uVar8 >> 0x10);
        *(int *)((long)ptr + lVar11 + 0xe) = (int)((ulong)uVar9 >> 0x10);
        lVar21 = lVar21 + 0x10;
      }
      if ((ulong)uVar24 < 0xffffffffffffffef) {
        uVar24 = -0x12;
      }
      uVar20 = uVar24 + lVar25 & 0xfffffffffffffff0;
      uVar22 = ((ptr_size - 2) - uVar20) - lVar13;
      puVar14 = (undefined4 *)(base + uVar20 + lVar13 + 2);
      uVar18 = (uint)uVar22;
    }
    puVar17 = (undefined4 *)((long)ptr + lVar24);
    if ((uVar18 >> 4 & 1) != 0) goto LAB_80093f4c;
LAB_80093ccc:
    puVar15 = puVar17;
    if (((uint)uVar22 >> 3 & 1) != 0) goto LAB_80093fdc;
LAB_80093cd0:
    puVar16 = puVar15;
    if (((uint)uVar22 >> 2 & 1) != 0) goto LAB_8009402c;
LAB_80093cd4:
    uVar18 = (uint)uVar22;
    puVar17 = puVar16;
  }
  if ((uVar18 >> 1 & 1) != 0) {
    uVar12 = *(undefined *)puVar14;
    uVar6 = *(undefined *)((long)puVar14 + 1);
    puVar14 = (undefined4 *)((long)puVar14 + 2);
    *(undefined *)puVar17 = uVar12;
    *(undefined *)((long)puVar17 + 1) = uVar6;
    puVar17 = (undefined4 *)((long)puVar17 + 2);
  }
  if ((uVar22 & 1) == 0) {
    return;
  }
  uVar12 = *(undefined *)puVar14;
LAB_80094080:
  *(undefined *)puVar17 = uVar12;
  return;
}