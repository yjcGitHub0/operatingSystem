#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"


int main(int argc,char* argv[]){
    if(argc < 2){
        printf("illegal\n");
         exit(0);
    }
    
    char ch[32];
    char * other[32];

    for(int i=1;i<argc;i++){
        other[i-1]=argv[i];
    }
    other[argc] = '\0';

    for(int n=0;1;n=0){
        //n=read(0,ch,sizeof(ch));
        int m=1;
        for(n=0;m>0&&ch[n-1]!='\n';n++){
            m=read(0, &ch[n], 1);
        }
        n--;
        if(n==0)break;
        ch[n]='\0';
        other[argc-1]=ch;
        if(fork()==0){
            exec(other[0], other);
            exit(0);
        }
        else{
            wait(0);
        }
    }

    exit(0); //确保进程退出
}
