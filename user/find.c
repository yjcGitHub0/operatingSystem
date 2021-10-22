#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char temp[505];

void getFileName(char *x){
    int n=strlen(x);
    int i;

    for(i=n-1;i>=0;i--){
        if(x[i]=='/')break;
    }
    int j=0;
    for(i=i+1;i<n;i++,j++){
        temp[j]=x[i];
    }
    temp[j]='\0';
}


int equal(char *x,char *y){
    int n=strlen(x);
    int m=strlen(y);
    if(n!=m)return 0;
    for(int i=0;i<n;i++){
        if(x[i]!=y[i])return 0;
    }
    return 1;
}

void Find(char * path, char * Name){
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        printf("cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        printf("cannot stat %s\n", path);
        close(fd);
        return;
    }
    switch(st.type){
    case T_FILE:
        getFileName(path);
        if(equal(temp,Name)) printf("%s\n", path);
        break;

    case T_DIR:
        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
            printf("path too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf+strlen(buf);
        *p++ = '/';
        while(read(fd, &de, sizeof(de)) == sizeof(de)){
			if(de.inum == 0 || de.inum == 1 || equal(de.name, ".") || equal(de.name, ".."))
				continue;	
            int n = strlen(de.name);
            for(int i=0;i<n;i++){
                p[i]=de.name[i];
            }			
			p[n] = 0;
			Find(buf, Name);
		}
		break;
  }
  close(fd);

}

int main(int argc,char* argv[]){
    
    if(argc != 3){
        printf("illegal\n");
         exit(0);
    }
    
    Find(argv[1],argv[2]);

    exit(0); //确保进程退出
}
