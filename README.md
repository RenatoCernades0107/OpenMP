# OpenMP

In order to make the compiling and running easy. We can create a command following these steps:

1. Run `sudo vim ~/.bashrc`
2. Write somewhere: 
    
    ```bash
    omp-run() {
        export OMP_NUM_THREADS=$2;
        g++ -fopenmp $1 -o $1.out
        ./$1.out $@
    }
3. Save the file



Example of running matrix_multiplication: `omp-run main.cpp 4` or `omp-run main.cpp 4 10 7 22`
