#include <cstdio>
#include <algorithm>
using namespace std;
#include "ParallelFloyd.h"
#include "ParallelFloydTest.h"
int Min(int A, int B);

// Function for copying the matrix
void CopyMatrix(int *pMatrix, int Size, int *pMatrixCopy) {
copy(pMatrix, pMatrix + Size * Size, pMatrixCopy);
}

// Function for the serial Floyd algorithm
void SerialFloyd(int *pMatrix, int Size) {
int t1, t2;
for(int k = 0; k < Size; k++)
for(int i = 0; i < Size; i++)
for(int j = 0; j < Size; j++)
if((pMatrix[i * Size + k] != -1) &&
(pMatrix[k * Size + j] != -1)) {
t1 = pMatrix[i * Size + j];
t2 = pMatrix[i * Size + k] + pMatrix[k * Size + j];
pMatrix[i * Size + j] = Min(t1, t2);
}
}
// Function for formatted matrix output
void PrintMatrix(int *pMatrix, int RowCount, int ColCount) {
for(int i = 0; i < RowCount; i++) {
for(int j = 0; j < ColCount; j++) {
printf("%7d", pMatrix[i * ColCount + j]);
fflush(stdout);
}
printf("\n");
fflush(stdout);
}
}