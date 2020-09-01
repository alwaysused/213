/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{ 
           int i1, j1,i, j,val1, val2, val3, val4, val5, val6, val7, val0;

    if (N==64){
    //i,i change order don't has effect
	for(i = 0; i < 64; i += 4)
		for(j = 0; j < 64; j += 4)
			for(i1 = i; i1 < i + 4; i1++){
                        	//for(j1 = j; j1 < j + 8; j1++){
                                val0 = A[i1][j];
				val1 = A[i1][j + 1];
				val2 = A[i1][j + 2];
				val3 = A[i1][j + 3];

				B[j][i1] = val0;
				B[j + 1][i1] = val1;
				B[j + 2][i1] = val2;
				B[j + 3][i1] = val3;


                            }
    }else if (N == 32){
        //int i1, i, j,val1, val2, val3, val4, val5, val6, val7, val0;
    //i,i change order don't has effect
	for(i = 0; i < 32; i += 8)
		for(j = 0; j < 32; j += 8)
			for(i1 = i; i1 < i + 8; i1++){
                        	//for(j1 = j; j1 < j + 8; j1++){
                                val0 = A[i1][j];
				val1 = A[i1][j + 1];
				val2 = A[i1][j + 2];
				val3 = A[i1][j + 3];
				val4 = A[i1][j + 4];
				val5 = A[i1][j + 5];
				val6 = A[i1][j + 6];
				val7 = A[i1][j + 7];
				B[j][i1] = val0;
				B[j + 1][i1] = val1;
				B[j + 2][i1] = val2;
				B[j + 3][i1] = val3;
				B[j + 4][i1] = val4;
				B[j + 5][i1] = val5;
				B[j + 6][i1] = val6;
				B[j + 7][i1] = val7;

                            }
    }else {
        //int i1, i, j,val1, val2, val3, val4, val5, val6, val7, val0;
    //i,i change order don't has effect
	for(i = 0; i + 8< N; i += 8)
		for(j = 0; j + 8< M; j += 8)
			for(i1 = i; i1 < i + 8; i1++){
                        	//for(j1 = j; j1 < j + 8; j1++){
                                val0 = A[i1][j];
				val1 = A[i1][j + 1];
				val2 = A[i1][j + 2];
				val3 = A[i1][j + 3];
				val4 = A[i1][j + 4];
				val5 = A[i1][j + 5];
				val6 = A[i1][j + 6];
				val7 = A[i1][j + 7];
				B[j][i1] = val0;
				B[j + 1][i1] = val1;
				B[j + 2][i1] = val2;
				B[j + 3][i1] = val3;
				B[j + 4][i1] = val4;
				B[j + 5][i1] = val5;
				B[j + 6][i1] = val6;
				B[j + 7][i1] = val7;

                            }

    for(i = i1; i < N; i++)
		for(j1 = 0; j1 < M; j++)
			B[j1][i] = A[i][j1];
	for(i = 0; i < i1; i++)
		for(j1 = j; j1 < M; j++)
			B[j1][i] = A[i][j1];
    }
	

 
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 
char transpose_submit61_desc[] = "Transpose61 submission";
void transpose_submit61(int M, int N, int A[N][M], int B[M][N]){
    int i,j,k,l;
    for (i = 0; i < N; i+=16)

    for (j = 0; j < M; j+=16)
    {
        for (k = i; k < i + 16 && k < N; k++)
        {
            for (l = j; l < j + 16 && l < M; l++)
            {
                B[l][k] = A[k][l];
            }
        }
    }
}


/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

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
        registerTransFunction(transpose_submit61, transpose_submit61_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

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

