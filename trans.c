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

/* Function prototypes */
int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - final optimized transpose
 * 
 * This function performs a matrix transpose, optimizing for different sizes
 * of input matrices (32x32, 32x64, and 64x64). It employs various strategies
 * to minimize cache misses and improve performance.
 */
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    // Check if dimensions are valid for transpose
    if (M <= 0 || N <= 0) {
        fprintf(stderr, "Invalid matrix dimensions: M and N must be positive integers.\n");
        return;
    }

    // check for compatibility of input matrices
    if ((N == 32 || N == 64) && (M == 32)) {
        fprintf(stderr, "Input matrices must have square dimensions for transposition.\n");
        return;
    }

    // transposes
    int i, j, ii, jj;
    if(M == 32 && N == 32){
        for(i = 0; i < N; i += 8){ // iterate after 8 rows
            for(j = 0; j < M; j += 8){ //iterate after 8 cols
                for(ii = i; ii < i + 8; ii++){
                    for(jj = j; jj < j + 8; jj++){
                        if (!(ii == jj && i == j)){
                            B[jj][ii] = A[ii][jj];
                        } 
                    }
                    if(i == j){
                        B[ii][ii] = A[ii][ii];
                    }
                } 
            }
        }
    }

    if(N == 64 && M == 32){
        for (i = 0; i < N; i += 8) {
            for (j = 0; j < M; j += 4) {
                for (ii = i; ii < i + 8; ii++) { 
                    for (jj = j; jj < j + 4; jj++) { 
                        B[jj][ii] = A[ii][jj];
                    }
                }     
            }   
        }
    }

    if(M == 64 && N == 64){
        for(i = 0; i < N; i += 4){ // iterate after 8 rows
            for(j = 0; j < M; j += 4){ //iterate after 8 cols
                for(ii = i; ii < i + 4; ii++){
                    for(jj = j; jj < j + 4; jj++){
                        if (!(ii == jj && i == j)){
                            B[jj][ii] = A[ii][jj];
                        } 
                    }
                    if(i == j){
                        B[ii][ii] = A[ii][ii];
                    }
                } 
            }
        }
    }
}

/* 
 * trans_32x32 - Transpose function for the 32x32 matrix
 * 
 * This function transposes a 32x32 matrix by iterating over blocks of 
 * elements and employing techniques to reduce cache misses and improve 
 * performance.
 */
void trans_32x32(int M, int N, int A[N][M], int B[M][N])
{
    if(M == 32 && N == 32){
        int i, j, ii, jj;
        for(i = 0; i < N; i += 8){ // iterate +8 rows 0 -> 8 -> 16 -> 24
            for(j = 0; j < M; j += 8){ //iterate +8 cols 0 -> 8 -> 16 -> 24
                for(ii = i; ii < i + 8; ii++){ //iterate through each 8 columns (i.e 9, 10 ...)
                    for(jj = j; jj < j + 8; jj++){ //iterate through each 8 values
                        if (!(ii == jj && i == j)){ //skip diagonals
                            B[jj][ii] = A[ii][jj];
                        } 
                    }
                    if(i == j){ // check each row if a diagonal exists if so add diagonals back after already inserting everything 
                        B[ii][ii] = A[ii][ii];
                    }
                } 
            }
        }
    }
}

/* 
 * trans_32x64 - Transpose function for the 32x64 matrix
 * 
 * This function transposes a 32x64 matrix using a similar approach as 
 * trans_32x32, but optimized for the larger size.
 */
void trans_32x64(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, ii, jj;
    if(N == 64 && M == 32){
        for (i = 0; i < N; i += 8) {
            for (j = 0; j < M; j += 4) {
                for (ii = i; ii < i + 8; ii++) { //iterate through two rows
                    for (jj = j; jj < j + 4; jj++) { //iterate through 4 values
                        B[jj][ii] = A[ii][jj];
                    }
                }     
            }   
        }
    }
}

/* 
 * trans_64x64 - Transpose function for the 64x64 matrix
 * 
 * This function transposes a 64x64 matrix by dividing it into smaller 
 * blocks and applying optimization techniques to reduce cache misses.
 */
void trans_64x64(int M, int N, int A[N][M], int B[M][N])
{
    if(M == 64 && N == 64){
        int i, j, ii, jj;
        for(i = 0; i < N; i += 4){
            for(j = 0; j < M; j += 4){
                for(ii = i; ii < i + 4; ii++){
                    for(jj = j; jj < j + 4; jj++){
                        if (!(ii == jj && i == j)){
                            B[jj][ii] = A[ii][jj];
                        } 
                    }
                    if(i == j){
                        B[ii][ii] = A[ii][ii];
                    }
                } 
            }
        }
    }
}

/* 
 * trans - Simple baseline transpose function
 * 
 * This function performs a simple row-wise scan transpose, not optimized 
 * for cache performance. It serves as a baseline for comparison with 
 * optimized transpose functions.
 */
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
 * is_transpose - Check if B is the transpose of A
 * 
 * This helper function checks if matrix B is the transpose of matrix A 
 * by comparing corresponding elements. It returns 1 if B is the transpose 
 * of A, and 0 otherwise.
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

