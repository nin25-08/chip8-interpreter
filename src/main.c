#include <stdio.h>
#include <string.h>
#include "chip8.h"
#include "display.h"

void loadRom(chip8 *cpu,const char* filedir){
    FILE *fp = fopen(filedir,"r");
    if(fp==NULL){
        printf("cannot open fp!");
        return;
    }
    fseek(fp,0,SEEK_END);
    long size = ftell(fp);
    if(size>MEM_SIZE-MEM_RESERVED){
        printf("fp too big!");
        fclose(fp);
        return;
    }
    rewind(fp);

    fread(&cpu->ram[0x200],1,size,fp);
    fclose(fp);
}
int main(int argc,int**argv){

}