#include "/home/refrain/csapp/Csapp/code/mycsapp.c"

int main()
{   
    int *p1, *p2, *p3;

    p1 = malloc(4*sizeof(int));
    p2 = malloc(5*sizeof(int));
    p3 = malloc(6*sizeof(int));

    printf("p2-p1 = %p, p3-p2 = %p...", p2-p1, p3-p2);
/*  
    p1              p2                  p3
    ↓---------------↓-------------------↓-----------------------+
    |xxx|xxx|xxx|xxx|xxx|xxx|xxx|xxx|xxx|xxx|xxx|xxx|xxx|xxx|xxx|
    +-----------------------------------------------------------+
实际上:
    p1                              p2                              p3
    ↓-------------------------------↓-------------------------------↓-------------------------------+
    |xxx|xxx|xxx|xxx|000|000|000|000|xxx|xxx|xxx|xxx|xxx|000|000|000|xxx|xxx|xxx|xxx|xxx|xxx|000|000|
    +-----------------------------------------------------------------------------------------------+
    
    

*/
    exit(0);
}