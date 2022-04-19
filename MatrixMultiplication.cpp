#include <chrono>
#include <cmath>
#include<fstream>
#include <iostream>
#include <vector>

using namespace std;
using namespace std::chrono;

static const char filenameMatrixA[] = "Matrix_A.txt";
static const char filenameMatrixB[] = "Matrix_B.txt";
static const char filenameMatrixC_MultiThread[] = "Matrix_C_Multi_Thread.txt";
static const char filenameMatrixC_SingleThread[] = "Matrix_C_Single_Thread.txt";

static void ParallelizedMatrixMultiplication(const vector<vector<int>> &A,
                                    const vector<vector<int>> &B,
                                    vector<vector<int>> &C,
                                    const int n) {
    #pragma omp parallel for
	for (int i=0; i<n; i++) {
		for (int k=0; k<n; k++) {
			for (int j=0; j<n; j++) {
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}
}

static void UnParallelizedMatrixMultiplication(const vector<vector<int>> &A,
                                    const vector<vector<int>> &B,
                                    vector<vector<int>> &C,
                                    const int n) {
	for (int i=0; i<n; i++) {
		for (int k=0; k<n; k++) {
			for (int j=0; j<n; j++) {
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}
}

static const void SaveMatrixInTextFile(const char filename[], const vector<vector<int>>& matrix) { 
    fstream file(filename, fstream::out);
    for (auto i=0; i<matrix.size(); i++) {
        for (auto j=0; j<matrix[i].size(); j++) {
            file << matrix[i][j] << "\t";
        }

        file << endl;
    }

    file.close();
}

int main()
{
    int n;
    cout << "Enter Matrix Dimension:" << endl;
    cin >> n;
    cout << endl;

    vector<vector<int>> A(n , vector<int> (n, 0));
    vector<vector<int>> B(n , vector<int> (n, 0));
    vector<vector<int>> C(n , vector<int> (n, 0));

    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            A[i][j] = rand() % 100;
            B[i][j] = rand() % 100;
        }
    }

    SaveMatrixInTextFile(filenameMatrixA, A);
    cout << "Input matrix A is initialized and saved in file: " << filenameMatrixA << endl;
    SaveMatrixInTextFile(filenameMatrixB, B);
    cout << "Input matrix B is initialized and saved in file: " << filenameMatrixB << endl << endl;

    auto startWithParallelization = high_resolution_clock::now();
    ParallelizedMatrixMultiplication(A, B, C, n);
    auto stopWithParallelization = high_resolution_clock::now();

    long durationWithParallelization = (duration_cast<microseconds>(stopWithParallelization - startWithParallelization)).count();
    cout << "Time taken for multiplication of arrays with parallelization(in s): " << durationWithParallelization/pow(10.0, 6.0) << endl;

    SaveMatrixInTextFile(filenameMatrixC_MultiThread, C);
    cout << "Output matrix C computed with multithreading is saved in file: " << filenameMatrixC_MultiThread << endl << endl;
    
    C = vector<vector<int>>(n , vector<int> (n, 0));
    auto startWithoutParallelization = high_resolution_clock::now();
    UnParallelizedMatrixMultiplication(A, B, C, n);
    auto stopWithoutParallelization = high_resolution_clock::now();

    long durationWithoutParallelization = (duration_cast<microseconds>(stopWithoutParallelization - startWithoutParallelization)).count();
    cout << "Time taken for multiplication of arrays without parallelization(in s): " << durationWithoutParallelization/pow(10.0, 6.0) << endl;

    SaveMatrixInTextFile(filenameMatrixC_SingleThread, C);
    cout << "Output matrix C computed without multithreading is saved in file: " << filenameMatrixC_SingleThread << endl << endl;

    return 0;
}