#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
int main(int argc, char *argv[])
{
    int i;
    for (i = 1; i < argc; i++)
    {
        if(argc !=2){
            fprintf(2, "number of sleep ticks arguments not sufficient\n");
            exit(1);
          }
    }
    int noOfTicks=atoi(argv[1]); //number of ticks after which the system has to sleep
    if(pause(noOfTicks<0)){
        fprintf(2,"sleep failed as number of ticks cannot be less than 0");
        exit(1);
    }
    exit(0);
}