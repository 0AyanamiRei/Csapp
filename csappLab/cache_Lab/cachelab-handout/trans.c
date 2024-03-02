/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 * 你的转置函数将在一个大小位1KB，block size等于32bytes的direct-mapped cache中验证性能
 */ 
#include <stdio.h>
#include "cachelab.h"

/*cache 配置
* 32个cache block，每个cache block可以容纳8个int类型变量
* cache总共可以同时存放256个int类型变量
* 分块的大小等于8x8=64个int类型变量, A和B同时存放即128个int类型变量
*/



int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 *     transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */

void trans_for_32x32(int M, int N, int A[N][M], int B[M][N]);
void trans_for_64x64(int M, int N, int A[N][M], int B[M][N]);
void trans_for_61x67(int M, int N, int A[N][M], int B[M][N]);


char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N]){
    if(M==32){
        trans_for_32x32(M,N,A,B);
    }

    else if(M==64){
        trans_for_64x64(M,N,A,B);
    }

    else{
        trans_for_61x67(M,N,A,B);
    }
}

char trans_blocking_0[]="block size = 8, k=8 M=N=32";
void trans_for_32x32(int M, int N, int A[N][M], int B[M][N]){
		int i, j, k, v1, v2, v3, v4, v5, v6, v7, v8;
		for (i = 0; i < 32; i += 8)
			for(j = 0; j < 32; j += 8)
				for(k = i; k < i + 8; k++)
				{
					v1 = A[k][j];
					v2 = A[k][j+1];
					v3 = A[k][j+2];
					v4 = A[k][j+3];
					v5 = A[k][j+4];
					v6 = A[k][j+5];
					v7 = A[k][j+6];			
					v8 = A[k][j+7];
					B[j][k] = v1;
					B[j+1][k] = v2;
					B[j+2][k] = v3;
					B[j+3][k] = v4;
					B[j+4][k] = v5;
					B[j+5][k] = v6;
					B[j+6][k] = v7;
					B[j+7][k] = v8;
				}
}

char trans_blocking_1[]="block size = 8, k=8 M=N=64";
void trans_for_64x64(int M, int N, int A[N][M], int B[M][N]){
    int i, j, a;
    int t1, t2, t3, t4, t5, t6, t7, t8;
    for (i = 0; i < N; i+=8) {
        for (j = 0; j < M; j+=8) {
        /* B x B mini matrix transpos */
            /* 通过修改数组B中caching但未被使用过的内存来当本地变量 */
            for(a = 0; a < 4; a ++){
                t1 = A[i+a][j];
                t2 = A[i+a][j+1];
                t3 = A[i+a][j+2];
                t4 = A[i+a][j+3];
                t5 = A[i+a][j+4];
                t6 = A[i+a][j+5];
                t7 = A[i+a][j+6];
                t8 = A[i+a][j+7];

                B[j][i+a] = t1;
                B[j+1][i+a] = t2;
                B[j+2][i+a] = t3;
                B[j+3][i+a] = t4;

                B[j][i+a+4] = t5;
                B[j+1][i+a+4] = t6;
                B[j+2][i+a+4] = t7;
                B[j+3][i+a+4] = t8;
            }

            for(a = 0; a < 4; a ++){
                t1 = A[i+4][j+a];
                t2 = A[i+5][j+a];
                t3 = A[i+6][j+a];
                t4 = A[i+7][j+a];

                t5 = B[j+a][i+4];
                t6 = B[j+a][i+5];
                t7 = B[j+a][i+6];
                t8 = B[j+a][i+7];

                B[j+a][i+4] = t1;
                B[j+a][i+5] = t2;
                B[j+a][i+6] = t3;
                B[j+a][i+7] = t4;

                B[j+a+4][i] = t5;
                B[j+a+4][i+1] = t6;
                B[j+a+4][i+2] = t7;
                B[j+a+4][i+3] = t8;
                }

            for(a = 4; a < 8; a ++){
                t1 = A[i+a][j+4];
                t2 = A[i+a][j+5];
                t3 = A[i+a][j+6];
                t4 = A[i+a][j+7];

                B[j+4][i+a] = t1;
                B[j+5][i+a] = t2;
                B[j+6][i+a] = t3;
                B[j+7][i+a] = t4;
            }
        }
    }
}

char trans_blocking_2[]="block size = 8, k=8 M=61,N=67";
void trans_for_61x67(int M, int N, int A[N][M], int B[M][N]) {
    int i, j, t1,t2,t3,t4,t5,t6,t7,t8;
    int n = N / 8 * 8;
    int m = M / 8 * 8;
    for (j = 0; j < m; j += 8)
        for (i = 0; i < n; ++i)
        {
                t1 = A[i][j];
				t2 = A[i][j+1];
				t3 = A[i][j+2];
				t4 = A[i][j+3];
				t5 = A[i][j+4];
				t6 = A[i][j+5];
				t7 = A[i][j+6];
				t8 = A[i][j+7];
				
				B[j][i] = t1;
				B[j+1][i] = t2;
				B[j+2][i] = t3;
				B[j+3][i] = t4;
				B[j+4][i] = t5;
				B[j+5][i] = t6;
				B[j+6][i] = t7;
				B[j+7][i] = t8;
        }
    for (i = n; i < N; ++i)
        for (j = m; j < M; ++j)
        {
            t1 = A[i][j];
            B[j][i] = t1;
        }
    for (i = 0; i < N; ++i)
        for (j = m; j < M; ++j)
        {
            t1 = A[i][j];
            B[j][i] = t1;
        }
    for (i = n; i < N; ++i)
        for (j = 0; j < M; ++j)
        {
            t1 = A[i][j];
            B[j][i] = t1;
        }
}



/*
    如果采用k=8(加上循环下表刚好12个变量占用完)的话可以能够保证从A中读取数据
    全是1次cold miss加7次hit的组合
    
    但是检查数组B的load操作后发现(以第一个block块为例子):
    cache block一直在来回抖动(书上的概念),比如set 6存储的数据
    tag一直从000->010->000->010 不停的切换,导致cache miss

    策略是把t1~t4存储完了之后先不急着把t5~t8存储到B(这样会刷新掉t1~t4的caching)
    而是等下一轮t1~t4存储完了之后,把还没更新的t5~t8存储到上一轮j1对应的B后
    再更新t5~t8,重复这个阶段即可

    问题的关键是怎么样才能保存第几轮这个信息? 目前我们循环下标+t1~t8已经将12个变量
    的限制用完了, 那就是利用B数组的值我们可以修改这个规则
*/




/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 
}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}



/* 优化M=N=64  blocksize=8
想要避免从conflict miss就要把B[4~7][0]的数据留存, 但是光存下本轮的数据就要消耗4个变量
而用于t1~t4是必须的, 再加上循环变量(即使全展开), 已经有4+2+4=10个变量了,我们需要等到
B[0~3][0]caching的位置全部填满，这需要(8-1)*4=28个额外变量>12, 所以不能再开变量
☆☆☆:但是我们B[0~3][0]caching的位置有空位呀！大概那么一想，我们通过t1~t4来往B[0~3][0]
中搬运变量, 同时又用t5~t7来利用B[0~3][0]尚未填满的cache block.设XX是tag为0的数据 YY是tag为1的数据
                    

      A   _j__j+1__j+2__j+3__j+4__j+5__j+6__j+7_|        B _i__i+1__i+2__i+3__i+4__i+5__i+6__i+7___|
      i   XX__XX___XX___XX___YY___YY___YY___YY__|          j XX__XX__XX___XX__YY__YY___YY___YY_____|
    i+1   XX__XX___XX___XX___YY___YY___YY___YY__|        j+1 XX__XX__XX___XX__YY__YY___YY___YY_____|
    i+2   XX__XX___XX___XX___YY___YY___YY___YY__|        j+2 XX__XX__XX___XX__YY__YY___YY___YY_____| 
    i+3   XX__XX___XX___XX___YY___YY___YY___YY__|  ->    j+3 XX__XX__XX___XX__YY__YY___YY___YY_____|
    i+4   ______________________________________|        j+4 ______________________________________|
    i+5   ______________________________________|        j+5 ______________________________________|
    i+6   ______________________________________|        j+6 ______________________________________|
    i+7   ______________________________________|        j+7 ______________________________________|                                    
    for(a = 0; a < 4; a ++){
        t1 = A[i+a][j];
        t2 = A[i+a][j+1];
        t3 = A[i+a][j+2];
        t4 = A[i+a][j+3];
        t5 = A[i+a][j+4];
        t6 = A[i+a][j+5];
        t7 = A[i+a][j+6];
        t8 = A[i+a][j+7];

        B[j][i+a] = t1;
        B[j+1][i+a] = t2;
        B[j+2][i+a] = t3;
        B[j+3][i+a] = t4;

        B[j+4][i+a] = t1;
        B[j+5][i+a] = t1;
        B[j+6][i+a] = t1;
        B[j+7][i+a] = t1;
    }

        
      A   _j__j+1__j+2__j+3__j+4__j+5__j+6__j+7_|        B _i__i+1__i+2__i+3__i+4__i+5__i+6__i+7___|
      i   XX__XX___XX___XX___YY___YY___YY___YY__|          j XX__XX__XX___XX__XX___XX___XX____XX___|
    i+1   XX__XX___XX___XX___YY___YY___YY___YY__|        j+1 XX__XX__XX___XX__XX___XX___XX____XX___|
    i+2   XX__XX___XX___XX___YY___YY___YY___YY__|        j+2 XX__XX__XX___XX__YY___YY___YY____YY___| 
    i+3   XX__XX___XX___XX___YY___YY___YY___YY__|  ->    j+3 XX__XX__XX___XX__YY___YY___YY____YY___|
    i+4   00__00________________________________|        j+4 YY__YY__YY___YY_______________________|
    i+5   00__00________________________________|        j+5 YY__YY__YY___YY_______________________|
    i+6   00__00________________________________|        j+6 ______________________________________|
    i+7   00__00________________________________|        j+7 ______________________________________| 
          00表示读出数据并替换B中的YY成XX
          相当于是把A数组第j列的i+4~i+7读出,让B数组第i行的YY回到原来的位置,用A数组中读出的数据替换掉
    for(a = 0; a < 4; a ++){
        t1 = A[i+4][j+a];
        t2 = A[i+5][j+a];
        t3 = A[i+6][j+a];
        t4 = A[i+7][j+a];

        t5 = B[j+a][i+4];
        t6 = B[j+a][i+5];
        t7 = B[j+a][i+6];
        t8 = B[j+a][i+7];

        B[j+4+a][i] = t5;
        B[j+4+a][i+1] = t5;
        B[j+4+a][i+2] = t5;
        B[j+4+a][i+3] = t5;

        B[j+a][i+4] = t1;
        B[j+a][i+5] = t2;
        B[j+a][i+6] = t3;
        B[j+a][i+7] = t4;
    }


      A   _j__j+1__j+2__j+3__j+4__j+5__j+6__j+7_|        B _i__i+1__i+2__i+3__i+4__i+5__i+6__i+7___|
      i   XX__XX___XX___XX___YY___YY___YY___YY__|          j XX__XX__XX___XX__XX___XX___XX____XX___|
    i+1   XX__XX___XX___XX___YY___YY___YY___YY__|        j+1 XX__XX__XX___XX__XX___XX___XX____XX___|
    i+2   XX__XX___XX___XX___YY___YY___YY___YY__|        j+2 XX__XX__XX___XX__XX___XX___XX____XX___| 
    i+3   XX__XX___XX___XX___YY___YY___YY___YY__|  ->    j+3 XX__XX__XX___XX__XX___XX___XX____XX___|
    i+4   00__00___00___00___00___00___00___00__|        j+4 YY__YY__YY___YY__YY___YY______________|
    i+5   00__00___00___00___00___00___00___00__|        j+5 YY__YY__YY___YY__YY___YY______________|
    i+6   00__00___00___00______________________|        j+6 YY__YY__YY___YY__YY___YY______________|
    i+7   00__00___00___00______________________|        j+7 YY__YY__YY___YY__YY___YY______________| 
          最后把A[i+4~i+7][j+4~j+7]的读给B[j+4~j+7][i+4~i+7]
    
    for(a = 0; a < 4 ++a){
        t1 = A[i+4+a][j+4];
        t1 = A[i+4+a][j+5];
        t1 = A[i+4+a][j+6];
        t1 = A[i+4+a][j+7];

        B[j+4][i+4+a] = t1;
        B[j+5][i+4+a] = t1;
        B[j+6][i+4+a] = t1;
        B[j+7][i+4+a] = t1;
    }




*/