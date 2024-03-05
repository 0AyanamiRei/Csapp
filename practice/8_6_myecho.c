/* 暂缓  */
#include "/home/refrain/csapp/Csapp/code/mycsapp.c"

int main(int argc, char **argv, char **envp)
{  
    int i;
    printf("Command-line arguements:\n");
    for(i = 0; argv[i]!=NULL; ++i){
        printf("argv[%2d]: %s\n", i, argv[i]);
    }
    for(i = 0; envp[i] != NULL; ++i){
        printf("envp[%2d], %s\n", i, envp[i]);
    }
    return 0;
}