#include <omp.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <cstdlib> 
#include <ctime>    
using namespace std;

/*
Compute matrix multiplication (A x B)
Parameters:
    - A: input matrix n x m
    - B: input matrix m x k
    - n: rows of A
    - m: colums of A and rows of B
    - k: colums of B
    - C: output matrix n x k 
*/
void mult(double** A, double** B, int n, int m, int k, double** C) { 
    int i, j, h;
    
    #pragma omp for schedule(static) private(j, h)
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            for (h = 0; h < k; ++h)
            {
                C[i][h] += A[i][j] * B[j][h];
            }
        }
    }
}

void generate_random_matrices(double **&A, double **&B, double **&C, const int& n, const int& m, const int& k) {
    A = new double*[n];
    B = new double*[m];
    C = new double*[n];
    
    for (int i=0; i<n; ++i) {
        A[i] = new double[m];
        C[i] = new double[k];
    }

    for (int j=0; j<m; ++j) {
        B[j] = new double[k];
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            A[i][j] = static_cast<double>(rand()) / RAND_MAX; // número aleatorio entre 0 y 1
        }
    }

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < k; j++) {
            B[i][j] = static_cast<double>(rand()) / RAND_MAX; // número aleatorio entre 0 y 1
        }
    }
}

void read_input(double **&A, double **&B, double **&C, int& n, int& m, int& k) {
    
    std::ifstream file("input.txt");
    if (!file.is_open()) {
        throw runtime_error("Cannot open file input.txt");
    }
    int a, b, c;
    file >> a >> b >> c;
    n = a; m = b; k = c;

    A = new double*[n];
    B = new double*[m];
    C = new double*[n];
    
    for (int i=0; i<n; ++i) {
        A[i] = new double[m];
        C[i] = new double[k];
    }

    for (int j=0; j<m; ++j) {
        B[j] = new double[k];
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            file >> A[i][j];
        }
    }

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < k; j++)
        {
            file >> B[i][j];
        }
    }

    file.close();
}

void write_output(double **C, int n, int k) {
    ofstream file("output.txt");
    for (size_t i = 0; i < n; i++)
    {   
        for (size_t j = 0; j < k; j++)
        {
            file << C[i][j] << " ";
        }
        file << endl;
    }
    file.close();
}

void print_matrices(double **A, double **B, const int n, const int m, const int k) {
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            cout << A[i][j] << " ";
        }
        cout << endl;
    }

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < k; j++)
        {
             cout << B[i][j] << " ";
        }
        cout << endl;
    }
}


/*
    To run using a file as an input run the following command:
        
        omp-run example.cpp number_of_threads

        The file structure is the following: 
            n m k

            matrix A

            matrix B

        As an example:
            3 4 3

            4 5 6 1
            2 6 3 2
            7 3 1 1

            5 2 4
            2 1 2
            4 5 6
            1 2 1

    To run using two random matrix run the following command:
        
        omp-run example.cpp number_of_threads n m k
        
        where: 
            n is the number of rows of matrix A
            m is the number of colum of matrix A (and rows of B)
            k is the number of columns of matrix B
*/
int main(int argc, char const *argv[])
{
    int n, m, k;
    double **A, **B, **C;
    
    if (argc == 3) {
        read_input(A, B, C, n, m, k);
    }
    else if (argc == 6) {
        k = atoi(argv[argc-1]);
        m = atoi(argv[argc-2]);
        n = atoi(argv[argc-3]);
        generate_random_matrices(A, B, C, n, m, k);
    }
    else {
        throw std::runtime_error("Wrong number of arguments, read the documentation above main function");
    }

    // print_matrices(A, B, n, m, k);
   
    auto start = std::chrono::system_clock::now();
    mult(A, B, n, m, k, C);
    auto end = std::chrono::system_clock::now();

    write_output(C, n, k);

    auto time = end - start;
    cout << "Computed time: " << time.count()/10e9 << " seg. Result in output.txt" << endl;

    for (int i = 0; i < n; i++)
    {
        delete [] A[i];
        delete [] C[i];
    }

    for (int i = 0; i < m; i++)
    {
        delete [] B[i];
    }

    delete [] A; 
    delete [] B;
    delete [] C;
    
    return 0;
}
