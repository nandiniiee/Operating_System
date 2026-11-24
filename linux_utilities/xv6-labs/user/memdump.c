#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

void memdump(char *fmt, char *data, int len);
// fmt tells how to interpret the data (how to consume the bytes)
// data points to the bytes
// len tells us how many bytes are valid
int
main(int argc, char *argv[])
{
  if(argc == 1){
    printf("Example 1:\n");
    int a[2] = { 61810, 2025 };
    memdump("ii", (char*) a, sizeof(a));

    printf("Example 2:\n");
    char *t = "a string";
    memdump("S", t, strlen(t));

    printf("Example 3:\n");
    char *s = "another";
    memdump("s", (char *) &s, sizeof(char *));

    struct sss {
      char *ptr;
      int num1;
      short num2;
      char byte;
      char bytes[8];
    } example;

    example.ptr = "hello";
    example.num1 = 1819438967;
    example.num2 = 100;
    example.byte = 'z';
    strcpy(example.bytes, "xyzzy");

    printf("Example 4:\n");
    memdump("pihcS", (char*) &example, sizeof(example));

    printf("Example 5:\n");
    memdump("sccccc", (char*) &example, sizeof(example));
  } else if(argc == 2){
    // format in argv[1], up to 512 bytes of data from standard input.
    char data[512];
    int n = 0;
    memset(data, '\0', sizeof(data));
    while(n < sizeof(data)){
      int nn = read(0, data + n, sizeof(data) - n);
      if(nn <= 0)
        break;
      n += nn;
    }
    memdump(argv[1], data, n);
  } else {
    printf("Usage: memdump [format]\n");
    exit(1);
  }
  exit(0);
}

void
memdump(char *fmt, char *data, int len)
{
  // Your code here.
  char *curr=data; //points to where we are currently reading
  int rem=len; //remianing bytes to read
  for(int i=0; fmt[i]!='\0';i++){
    if(fmt[i]=='i'){
      // i takes 4 bytes
      if(rem<4){
        printf("memdump: not enough data for 'i'\n");
        return;
      }
      printf("%d", *(int *)curr);
      curr+=4;
      rem-=4;
    }else if(fmt[i]=='p'){
      if(rem<8){
        printf("memdump: not enough data for 'p'\n");
        return;
      }
      printf("%lx", *(unsigned long*)curr);
      curr+=8;
      rem-=8;

    }else if(fmt[i]=='h'){
      if(rem<2){
        printf("memdump: not enough data for 'h'\n");
        return;
      }
      printf("%d", *(short *)curr);
      curr+=2;
      rem-=2;
    }else if(fmt[i]=='c'){
      if(rem<1){
        printf("memdump: not enough data for 'c'\n");
        return;
      }
      printf("%c", *curr);
      curr+=1;
      rem-=1;
    }else if(fmt[i]=='s'){
      if(rem<8){
        printf("memdump: not enough data for 's'\n");
        return;
      }
      printf("%s", *(char **)curr);
      curr+=8;
      rem-=8;
    }else if(fmt[i]=='S'){
      for(int j=0;j<rem;j++){
        if(curr[j]=='\0'){
          break;
        }
        printf("%c", curr[j]);
      }
      curr+=rem;
      rem=0;
    }
    printf("\n");
  }
  

}
