#include <iostream>
#include <mpi.h>
#include <fstream>
#include <ctime>
#include <cmath>
#include <algorithm>

using namespace std;

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int arrSize = 12;
    int cap = arrSize / size;

    int myArray[arrSize];

    if (rank == 0)
    {
        ifstream inputFile("data2.txt");

        if (!inputFile.is_open())
        {
            cerr << "Error: Unable to open the file." << endl;
            return 1;
        }

        for (int i = 0; i < arrSize; ++i)
        {
            inputFile >> myArray[i];
        }

        inputFile.close();
    }

    int *localArr = new int[cap];
    MPI_Scatter(myArray, cap, MPI_INT, localArr, cap, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    int localMin = localArr[0];
    for (int i = 1; i < cap; ++i)
    {
        if (localArr[i] < localMin)
        {
            localMin = localArr[i];
        }
    }

    int globalMin;
    MPI_Allreduce(&localMin, &globalMin, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);

    int maxNum = myArray[0];
    for (int i = 1; i < arrSize; ++i)
    {
        if (myArray[i] > maxNum)
        {
            maxNum = myArray[i];
        }
    }

    int sqrtMaxNum = static_cast<int>(sqrt(maxNum));

    for (int i = globalMin; i <= sqrtMaxNum; i++)
    {

        for (int j = 0; j < cap; j++)
        {
            if (localArr[j] % i == 0 && localArr[j] != i)
            {
                localArr[j] = -1;
            }
        }
    }
    int newArr[arrSize];
    MPI_Gather(localArr, cap, MPI_INT, newArr, cap, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        cout << "Prime numbers: ";
        for (int i = 0; i < arrSize; i++)
        {
            if (newArr[i] != -1)
            {
                cout << newArr[i] << " ";
            }
        }
        cout << endl;
    }

    MPI_Finalize();
    return 0;
}
// #include <stdio.h>
// #include <stdlib.h>
// #include <mpi.h>
// #include <stdbool.h>
// #include <string.h>

// void SieveAlgo(int n, int *local_primes, int local_size, int process_rank, int total_processes)
// {
//     bool prime[n + 1];
//     memset(prime, true, sizeof(prime));

//     for (int p = 2; p * p <= n; p++)
//     {
//         if (prime[p] == true)
//         {
//             for (int i = p * p; i <= n; i += p)
//                 prime[i] = false;
//         }
//     }

//     int count = 0;
//     for (int p = 2; p <= n && count < local_size; p++)
//     {
//         if (prime[p])
//         {
//             local_primes[count] = p;
//             count++;
//         }
//     }
// }

// int main(int argc, char *argv[])
// {
//     MPI_Init(&argc, &argv);

//     int process_rank, total_processes;
//     MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
//     MPI_Comm_size(MPI_COMM_WORLD, &total_processes);

//     int n = 30; // Set a larger value to ensure we find enough primes
//     int local_size = n / total_processes;
//     int *local_primes = (int *)malloc(local_size * sizeof(int));

//     // Apply the Sieve algorithm
//     SieveOfEratosthenes(n, local_primes, local_size, process_rank, total_processes);

//     // Gather all primes to process 0
//     int *all_primes = NULL;

//     if (process_rank == 0)
//     {
//         all_primes = (int *)malloc(n * sizeof(int));
//     }

//     MPI_Gather(local_primes, local_size, MPI_INT, all_primes, local_size, MPI_INT, 0, MPI_COMM_WORLD);

//     // Print the primes on process 0
//     if (process_rank == 0)
//     {
//         printf("Prime numbers: ");
//         for (int i = 0; i < n; i++)
//         {
//             if (all_primes[i] != 0)
//             {
//                 printf("%d ", all_primes[i]);
//             }
//         }
//         printf("\n");
//     }

//     free(local_primes);
//     if (process_rank == 0)
//     {
//         free(all_primes);
//     }

//     MPI_Finalize();

//     return 0;
// }
