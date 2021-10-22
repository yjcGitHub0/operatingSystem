#include "kernel/types.h"
#include "user.h"

void itoa(int x, char ch[], int d){
    if(x>=10){
        ch[0]=(char)(x/10+'0');
        ch[1]=(char)(x%10+'0');
    }
    else{
        ch[0]=(char)(x+'0');
    }
}

void MyRead(int num[], int n, int pip){
    char ch[100];
    int x = 0, k = 0;
    read(pip, ch, n);
    for(int i = 0; i < n; i++){
        if(ch[i] >= '0' && ch[i] <= '9'){
            x = x * 10 + (ch[i] - '0');
        }
        else{
            if(x == 0) continue;
            num[k] = x;
            k++;
            x = 0;
        }
    }
    num[k] = -1;
}

void get_num(int n, int pip_fa, int num[]){
    int t = 0;
    int pnum[35];

    MyRead(num, n, pip_fa);
    close(pip_fa);

    printf("prime %d\n", num[0]);
    for(int i = 1; num[i] != -1; i++){
        if(num[i] % num[0] == 0)continue;
        pnum[t++] = num[i];
    }

    for(int i = 0; i < t; i++){
        num[i] = pnum[i];
    }
    num[t] = -1;
}

void slove(int n, int pip_fa[], int num[]){
    get_num(n, pip_fa[0], num);

    if(num[0] == -1){
        exit(0);
    }
    else{
        n = 0;
        int pip[2];
        pipe(pip);
        for(int i = 0; num[i] != -1; i++){
            char ch[2];
            itoa(num[i], ch, 10);
            n += write(pip[1], ch, num[i] < 10 ? 1 : 2);
            n += write(pip[1], " ", 1);
        }
        close(pip[1]);
        
        int pid = fork();
        if(pid < 0){
            printf("error\n");
            exit(0);
        }
        else if(pid > 0){
            int sta;
            wait(&sta);
            exit(0);
        }
        else {
            slove(n, pip, num);
        }
        
    }
    exit(0);
}

int main(int argc,char* argv[]){
    
    int pip[2];
    int n = 0;

    if(pipe(pip) < 0){
        printf("error\n");
        exit(0);
    }
    for(int i = 2; i <= 35; i++){
        char ch[2];
        itoa(i, ch, 10);
        n += write(pip[1], ch, i < 10 ? 1 : 2);
        n += write(pip[1], " ", 1);
    }
    close(pip[1]);
    int num1[35];
    
    slove(n, pip, num1);
    

    exit(0); //确保进程退出
}
