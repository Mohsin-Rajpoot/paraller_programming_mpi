// #include <stdio.h>
// #include <mpi.h>

// int main()
// {
//     MPI_Init(NULL, NULL);

//     int rank;
//     MPI_Comm_rank(MPI_COMM_WORLD, &rank);

//     int key;

//     if (rank == 0)
//     {
//         key = 5
//     }
//     // Broadcast the key from process 0 to all other processes
//     MPI_Bcast(&key, 1, MPI_INT, 0, MPI_COMM_WORLD);

//     // Now all processes, including process 0, can use the local_key variable
//     printf("Process %d: Received key = %d\n", rank, local_key);

//     MPI_Finalize();
//     return 0;
// }

#include <iostream>
#include <mpi.h>
#include <fstream>
#include <ctime>

using namespace std;

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int arrSize = 500;
    int cap = arrSize / size;

    int myArray[arrSize];

    if (rank == 0)
    {
        ifstream inputFile("data.txt");

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

    int key;
    if (rank == 0)
    {
        srand(time(NULL));
        key = rand() % 100;
    }

    MPI_Bcast(&key, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int localCount = 0;
    for (int i = 0; i < cap; ++i)
    {
        if (localArr[i] == key)
        {
            localCount++;
        }
    }

    int globalCount;
    MPI_Reduce(&localCount, &globalCount, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        cout << "Your key " << key << " appears " << globalCount << " times" << endl;
    }

    MPI_Finalize();
    ����return�0;
}

// int MPI_Scatter(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
//                 void *recvbuf, int recvcount, MPI_Datatype recvtype,
//                 int root, MPI_Comm comm);

// sendbuf: Pointer to the send buffer (data to be scattered). It is significant only at the root process.
// sendcount: Number of elements in the send buffer per process.
// sendtype: MPI datatype of the elements in the send buffer.
// recvbuf: Pointer to the receive buffer (portion of the scattered data that each process will receive).
// recvcount: Number of elements in the receive buffer per process.
// recvtype: MPI datatype of the elements in the receive buffer.
// root: Rank of the root process (the process that is scattering the data).
// comm: MPI communicator.
