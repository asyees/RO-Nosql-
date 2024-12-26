#include <stdio.h>
#include <stdlib.h>
#include <chrono> // For high-resolution timing
#include <time.h>

// Function for simple initialization of matrix elements
void DummyDataInitialization (double* pAMatrix, double* pBMatrix, int Size) {
    for (int i = 0; i < Size; i++) {
        for (int j = 0; j < Size; j++) {
            pAMatrix[i * Size + j] = 1;
            pBMatrix[i * Size + j] = 1;
        }
    }
}

// Function for random initialization of matrix elements
void RandomDataInitialization (double* pAMatrix, double* pBMatrix, int Size) {
    for (int i = 0; i < Size; i++) {
        for (int j = 0; j < Size; j++) {
            pAMatrix[i * Size + j] = rand() / (double)RAND_MAX * 100.0; 
            pBMatrix[i * Size + j] = rand() / (double)RAND_MAX * 100.0; 
        }
    }
}

// Function for memory allocation and initialization of matrix elements
void ProcessInitialization (double* &pAMatrix, double* &pBMatrix,
                            double* &pCMatrix, int &Size) {
    // Setting the size of matrices
    do {
        printf("\nEnter the size of matrices: ");
        scanf("%d", &Size);
        printf("\nChosen matrices' size = %d\n", Size);
        if (Size <= 0)
            printf("\nSize of objects must be greater than 0!\n");
    } while (Size <= 0);

    // Memory allocation
    pAMatrix = new double[Size * Size];
    pBMatrix = new double[Size * Size];
    pCMatrix = new double[Size * Size];

    // Initialization of matrix elements
    RandomDataInitialization(pAMatrix, pBMatrix, Size);
    for (int i = 0; i < Size * Size; i++) {
        pCMatrix[i] = 0;
    }
}

// Function for formatted matrix output
void PrintMatrix (double* pMatrix, int RowCount, int ColCount) {
    for (int i = 0; i < RowCount; i++) {
        for (int j = 0; j < ColCount; j++) {
            printf("%7.4f ", pMatrix[i * ColCount + j]);
        }
        printf("\n");
    }
}

// Function for matrix multiplication
void SerialResultCalculation(double* pAMatrix, double* pBMatrix,
                             double* pCMatrix, int Size) {
    for (int i = 0; i < Size; i++) {
        for (int j = 0; j < Size; j++) {
            for (int k = 0; k < Size; k++) {
                pCMatrix[i * Size + j] += pAMatrix[i * Size + k] * pBMatrix[k * Size + j];
            }
        }
    }
}

// Function for computational process termination
void ProcessTermination (double* pAMatrix, double* pBMatrix, double* pCMatrix) {
    delete[] pAMatrix;
    delete[] pBMatrix;
    delete[] pCMatrix;
}

int main() {
    double* pAMatrix; // First argument of matrix multiplication
    double* pBMatrix; // Second argument of matrix multiplication
    double* pCMatrix; // Result matrix
    int Size;         // Size of matrices

    printf("Serial matrix multiplication program\n");

    srand(unsigned(time(0)));

    // Memory allocation and initialization of matrix elements
    ProcessInitialization(pAMatrix, pBMatrix, pCMatrix, Size);

    // Start time measurement using chrono
    auto start = std::chrono::high_resolution_clock::now();
    
    // Matrix multiplication
    SerialResultCalculation(pAMatrix, pBMatrix, pCMatrix, Size);

    // End time measurement
    auto finish = std::chrono::high_resolution_clock::now();

    // Calculate duration
    std::chrono::duration<double> duration = finish - start;

    // // Matrix output based on size condition
    // if (Size <= 10) {
    //     printf("Initial A Matrix \n");
    //     PrintMatrix(pAMatrix, Size, Size);
    //     printf("Initial B Matrix \n");
    //     PrintMatrix(pBMatrix, Size, Size);
    //     printf("\n Result Matrix: \n");
    //     PrintMatrix(pCMatrix, Size, Size);
    // } else {
    //     printf("Matrix size is greater than 10, skipping matrix output.\n");
    // }

    // Printing the time spent by matrix multiplication
    printf("\n Time of execution: %f seconds\n", duration.count());

    // Computational process termination
    ProcessTermination(pAMatrix, pBMatrix, pCMatrix);
}
