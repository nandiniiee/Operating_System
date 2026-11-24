#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"

#define PAGE_SIZE 4096 //page size
int main(int argc, char *argv[])
{
 char *mem= (char *)sbrk(PAGE_SIZE); //one page gets 4KB size
 for(int i=0;i<PAGE_SIZE-1;i++){
  if((mem[i]>='0' && mem[i]<='9')|| (mem[i]>='A' && mem[i]<='Z') || (mem[i]>='a' && mem[i]<='z')){
    printf("%s\n", &mem[i]);
    
  }
 }
  exit(0);
  /*char *p=sbrk(8*4096);
  for(int i = 0; i < 8 * 4096 - 16; i++) {
    if(p[i] == 'T' &&
      p[i+1] == 'h' &&
      p[i+2] == 'i' &&
      p[i+3] == 's' &&
      p[i+4] == ' ' &&
      p[i+5] == 'm' &&
      p[i+6] == 'a' &&
      p[i+7] == 'y' &&
      p[i+8] == ' ' &&
      p[i+9] == 'h' &&
      p[i+10] == 'e' &&
      p[i+11] == 'l' &&
      p[i+12] == 'p' &&
      p[i+13] == '.') {
      printf("%s\n", p + i + 16);
      break;
    }
  }
  exit(1);*/
}
