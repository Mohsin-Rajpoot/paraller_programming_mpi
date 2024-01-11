
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int arraySize = 200; // Adjust this based on the size of your array

    int *localArray = (int *)malloc(sizeof(int) * (arraySize / size));
    int *globalArray = NULL;

    // Read array from file on process 0 and scatter to all processes
    if (rank == 0)
    {
        FILE *arrayFile = fopen("data.txt", "r");
        if (arrayFile == NULL)
        {
            fprintf(stderr, "Error: Unable to open array file.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        int *fullArray = (int *)malloc(sizeof(int) * arraySize);
        for (int i = 0; i < arraySize; ++i)
        {
            fscanf(arrayFile, "%d", &fullArray[i]);
        }

        fclose(arrayFile);

        globalArray = (int *)malloc(sizeof(int) * arraySize);
        MPI_Scatter(fullArray, arraySize / size, MPI_INT,
                    localArray, arraySize / size, MPI_INT, 0, MPI_COMM_WORLD);

        MPI_Bcast(globalArray, arraySize, MPI_INT, 0, MPI_COMM_WORLD);

        free(fullArray);
    }
    else
    {
        MPI_Scatter(NULL, arraySize / size, MPI_INT,
                    localArray, arraySize / size, MPI_INT, 0, MPI_COMM_WORLD);

        globalArray = (int *)malloc(sizeof(int) * arraySize);
        MPI_Bcast(globalArray, arraySize, MPI_INT, 0, MPI_COMM_WORLD);
    }

    // Read key from file or command-line argument on process 0
    int key;
    if (rank == 0)
    {
        srand(time(NULL));
        key = rand() % 100;
    }

    // Broadcast the key to all processes
    MPI_Bcast(&key, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Perform linear search and count occurrences
    int localCount = 0;
    for (int i = 0; i < arraySize / size; ++i)
    {
        if (localArray[i] == key)
        {
            localCount++;
        }
    }

    // Reduce the counts from all processes using MPI_Reduce
    int globalCount;
    MPI_Reduce(&localCount, &globalCount, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Print the result on process 0
    if (rank == 0)
    {
        printf("Key %d appears %d times in the array.\n", key, globalCount);
    }

    free(localArray);
    if (rank == 0)
    {
        free(globalArray);
    }
    MPI_Finalize();
    return 0;
}
