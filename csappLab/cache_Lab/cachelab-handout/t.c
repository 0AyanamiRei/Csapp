#include<stdio.h>

const int N = 5;
const int M = 7;

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

void trans_1(int M, int N, int A[N][M], int B[M][N]){
    int i, j, i1, j1;
    int b = M - M % 2;   // 67-(67%8)=64
    int a = N - N % 2;   // 61-(61%8)=56

    for (i = 0; i < a; i+=2) {

        for (j = 0; j < b; j+=2) {

        /* B x B mini matrix transpos */
            for(i1 = i; i1 < i+2; i1 ++)
                for(j1 = j; j1 < j+2; j1++){
                    B[j1][i1] = A[i1][j1];
                }
        }
    }
    /* A[][] 剩余右边的空白 */
    for(i = 0; i < a; i ++)
        for(j = b; j < M; j ++){
            B[j][i] = A[i][j];
        }
    /* A[][]剩余的下边的空白 */
    for(i = a; i < N; i ++)
        for(j = 0; j < b; j ++){
            B[j][i] = A[i][j];
        }
    B[M-1][N-1] = A[N-1][M-1];
}

void show_Matrix(int N, int M, int m[N][M])
{
    for(int i = 0; i < N; i ++){
        for(int j = 0 ; j < M; j ++)
            printf("%3d ", m[i][j]);
        printf("\n");
    }
}

int main()
{
    int A[5][7]={0}, B[7][5]={0};
    for(int i = 1; i <= N; i ++)
        for(int j = 1; j <= M; j ++)
            A[i-1][j-1] = i * j;
    show_Matrix(N, M, A);
    printf("\n");
    trans_1(M, N, A, B);
    show_Matrix(M, N, B);
    printf("\n %d", is_transpose(M, N, A, B));
    return 0;
}