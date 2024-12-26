#include <iostream>
#include <mpi.h>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

// Функція для ініціалізації матриці та вектора
void InitializeMatrixAndVector(vector<vector<double>>& matrix, vector<double>& vec, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = (i == j) ? 1.0 : 1.0 / (i + j + 1); // Симетрична матриця
        }
        vec[i] = 1.0; // Вектор правих частин
    }
}

// Функція для перевірки результатів
void PrintVector(const vector<double>& vec) {
    for (double val : vec) {
        cout << val << " ";
    }
    cout << endl;
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int ProcNum, ProcRank;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    int Size;
    if (ProcRank == 0) {
        cout << "Parallel Gauss algorithm for solving linear systems" << endl;
        cout << "Enter the size of the matrix and the vector: ";
        cin >> Size;

        if (Size < 1) {
            cerr << "Error: Matrix size must be at least 1." << endl;
            MPI_Abort(MPI_COMM_WORLD, -1);
        }

        if (ProcNum > Size) {
            cerr << "Error: Number of processes cannot exceed matrix size." << endl;
            MPI_Abort(MPI_COMM_WORLD, -1);
        }
    }

    MPI_Bcast(&Size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Ініціалізація матриці та вектора
    vector<vector<double>> matrix(Size, vector<double>(Size));
    vector<double> vec(Size);
    vector<double> result(Size, 0.0);

    if (ProcRank == 0) {
        InitializeMatrixAndVector(matrix, vec, Size);
    }

    // Передача матриці та вектора
    for (int i = 0; i < Size; i++) {
        MPI_Bcast(matrix[i].data(), Size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }
    MPI_Bcast(vec.data(), Size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Початок вимірювання часу
    double start_time = MPI_Wtime();

    // Алгоритм Гаусса
    for (int step = 0; step < Size; step++) {
        // Вибір півотного рядка
        double pivot = matrix[step][step];
        if (fabs(pivot) < 1e-10) {
            if (ProcRank == 0) {
                cerr << "Error: Zero pivot at step " << step << endl;
            }
            MPI_Abort(MPI_COMM_WORLD, -1);
        }

        // Нормалізація півотного рядка
        for (int j = step; j < Size; j++) {
            matrix[step][j] /= pivot;
        }
        vec[step] /= pivot;

        // Розсилка нормалізованого рядка
        MPI_Bcast(matrix[step].data(), Size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(&vec[step], 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        // Модифікація інших рядків
        for (int i = ProcRank; i < Size; i += ProcNum) {
            if (i != step) {
                double factor = matrix[i][step];
                for (int j = step; j < Size; j++) {
                    matrix[i][j] -= factor * matrix[step][j];
                }
                vec[i] -= factor * vec[step];
            }
        }
    }

    // Збір результатів
    MPI_Reduce(vec.data(), result.data(), Size, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // Кінець вимірювання часу
    double end_time = MPI_Wtime();
    double time_taken = end_time - start_time;

    // Виведення результатів
    if (ProcRank == 0) {
        cout << "Result vector: ";
        //PrintVector(result);
        cout << "Time of execution: " << time_taken << " seconds" << endl;
    }

    MPI_Finalize();
    return 0;
}
