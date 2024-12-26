#include <iostream>
#include <stdlib.h>
#include <chrono> // Для точного замера времени
#include <time.h>
void DummyDataInitialization(double * pMatrix, double * pVector, int Size) {
    int i, j; // Переменные цикла
    for (i = 0; i < Size; i++) {
        pVector[i] = 1;
        for (j = 0; j < Size; j++)
            pMatrix[i * Size + j] = i;
    }
}

void ProcessInitialization(double * & pMatrix, double * & pVector, double * & pResult, int & Size) {
    do {
        std::cout << "\nEnter the size of the initial objects: ";
        std::cin >> Size;
        std::cout << "\nChosen objects size = " << Size << "\n";
        if (Size <= 0)
            std::cout << "\nSize of objects must be greater than 0!\n";
    } while (Size <= 0);

    pMatrix = new double[Size * Size];
    pVector = new double[Size];
    pResult = new double[Size];
    DummyDataInitialization(pMatrix, pVector, Size);
}

void ResultCalculation(double * pMatrix, double * pVector, double * pResult, int Size) {
    int i, j; 
    for (i = 0; i < Size; i++) {
        pResult[i] = 0;
        for (j = 0; j < Size; j++)
            pResult[i] += pMatrix[i * Size + j] * pVector[j];
    }
}

void ProcessTermination(double * pMatrix, double * pVector, double * pResult) {
    delete[] pMatrix;
    delete[] pVector;
    delete[] pResult;
}

int main() {
    double* pMatrix; // First argument - initial matrix
    double* pVector; // Second argument - initial vector
    double* pResult; // Result vector for matrix-vector multiplication
    int Size; // Sizes of initial matrix and vector
    struct timespec start, finish;
    double duration;

    printf("Serial matrix-vector multiplication program\n");

    // Memory allocation and data initialization
    ProcessInitialization(pMatrix, pVector, pResult, Size);

    // Matrix and vector output
    printf("Initial Matrix \n");
    // PrintMatrix(pMatrix, Size, Size);
    printf("Initial Vector \n");
    // PrintVector(pVector, Size);

    // Matrix-vector multiplication
    clock_gettime(CLOCK_MONOTONIC, &start);
    ResultCalculation(pMatrix, pVector, pResult, Size);
    clock_gettime(CLOCK_MONOTONIC, &finish);

    // Высчитываем разницу во времени с точностью до наносекунд
    duration = (finish.tv_sec - start.tv_sec) + (finish.tv_nsec - start.tv_nsec) / 1e9;

    // Printing the result vector
    printf("\nResult Vector: \n");
    // PrintVector(pResult, Size);

    // Printing the time spent by matrix-vector multiplication
    printf("\nTime of execution: %f seconds\n", duration);

    // Computational process termination
    ProcessTermination(pMatrix, pVector, pResult);

    return 0;
}
