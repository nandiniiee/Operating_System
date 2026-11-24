#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
void find(char *curr_path, char *target_dir, int argc, char *argv[]){
    //curr_path is where are we searching currently
    //target_ir is the directory we are looking for
    char buff[512]; 
    int fd; //file descriptor
    struct dirent de; //stores one dir entry at a time
    struct stat st; // stores info about the dir
    fd=open(curr_path, O_RDONLY); //open the current path
    if(fd<0){
        fprintf(2, "cannot open the file\n");
        return;
    }
    if(fstat(fd, &st)<0){
        //checking if it is a directory or not
        fprintf(2, "cannot stat\n");
        close(fd);
        return;
    }
    if(st.type!=T_DIR){
        //if not a directory then there is nothing to search
        close(fd);
        return;
    }
    //read directory one entry at a time
    while(read(fd, &de, sizeof(de))==sizeof(de)){
        if(de.inum==0){
            continue; // if imum=0 means that the dir is empty
        }
        if(strcmp(de.name, ".")==0){
            continue; // skip "." 
        }
        if(strcmp(de.name, "..")==0){
            continue; //skip ".."
        }
        //making path
        strcpy(buff, curr_path);
        char *ptr=buff+strlen(buff); // moving ptr to end of current path
        *ptr= '/'; //adding '/' after current path
        ptr++;
        
        memmove(ptr, de.name, DIRSIZ); // copying the name of the dir after the slash
        ptr[DIRSIZ]='\0';
        if(strcmp(de.name, target_dir)==0){
            if(argc==3){
                //means no exec
                printf("%s\n", buff);

            }else if(strcmp(argv[3], "-exec")==0){
                int pid=fork();
                if(pid==0){
                    char *new_args[10];
                    int i;
                    for(i=4; i<argc;i++){
                        new_args[i-4]=argv[i];
                    }
                    new_args[i-4]=buff; //adding a file that we found
                    new_args[i-3]=0; //end of arguments
                    exec(new_args[0], new_args);
                    exit(1);
                }
                wait(0); //parent waits for the child process to finish
            }

        }
        if(stat(buff, &st)>=0 && st.type==T_DIR){
            find(buff, target_dir, argc, argv);
        }
    }
    close(fd); 
}
int main(int argc, char *argv[]){
    if(argc<3){
        fprintf(2, "number of arguments are not enough\n");
        exit(1);
    }
    find(argv[1], argv[2], argc, argv); //argv[1] has our current path and argv[2] will have our target dir
    exit(0);
}
