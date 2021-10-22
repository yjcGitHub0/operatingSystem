#include "kernel/types.h"
#include "user.h"

int main(int argc,char* argv[]){
    int gdfa[2], gdch[2];
    int pid;
    if(pipe(gdfa) < 0 || pipe(gdch) < 0){
        printf("wrong\n");
        exit(0);
    }

    pid = fork();
    if(pid < 0){
        printf("wrong\n");
        exit(0);
    }
    if(pid == 0){
        close(gdch[0]);
        close(gdfa[1]);
        char ping[4];
        read(gdfa[0], ping, 4);
        printf("%d: received %s\n", getpid(), ping);
        write(gdch[1],"pong",4);
        close(gdch[1]);
        close(gdfa[0]);
    }
    else{
        close(gdch[1]);
        close(gdfa[0]);
        write(gdfa[1],"ping",4);
        char pong[4];
        read(gdch[0], pong, 4);
        printf("%d: received %s\n", getpid(), pong);
        close(gdch[0]);
        close(gdfa[1]);
    }

    exit(0); //确保进程退出
}
