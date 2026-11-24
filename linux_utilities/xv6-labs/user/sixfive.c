#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"
int main(int argc, char *argv[])
{
    int fd; //file descriptor
    char buff[1]; //character buffer to read one character at a time
    int n;
    int num=0; //current number that we are constructing
    int read_num=0; //tells if we have started reading the digits or not
    if(argc<2){
        fprintf(2, "argument count is wrong\n");
        exit(1);
    }
    for(int file=1; file<argc; file++){
        //resetting everyhting for a new file
        num=0;
        read_num=0;
        fd=open(argv[file], O_RDONLY); //we open the file in read only mode
        if(fd<0){
            fprintf(2, "cannot open the file");
            exit(1);
        }
        while((n=read(fd, buff, 1))>0){
            //read character from file into the buffer
            char c=buff[0]; 
            if(c>='0' && c<='9'){
                //if the character is a digit
                num=num*10+(c-'0'); // we construct the number
                read_num=1; // tells that we are currently reading a number
            }
            else if(strchr(" -\r\t\n./,", c)!=0){
                //check if the character is a separator
                if(read_num){
                    // if we had read a number and not something else 
                    //check for divisibilty by 5 and 6
                    if(num%5==0 || num%6==0){
                        printf("%d\n", num);
                    }
                    num=0;
                    read_num=0; //resetting again to build a new number
                    
                }
            }
        }
        //if the EOF is a number and nothing else so we have to process that
        if(read_num){
            if(num%5==0 || num%6==0){
                printf("%d\n", num);
            }
        }
        close(fd);
    }
    exit(0);

}