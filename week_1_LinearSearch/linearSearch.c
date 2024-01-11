#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int world_size; // total processes
    int world_rank; // process number
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    // printf("World size: %d, World rank: %d\n", world_size, world_rank);
    int size = 20;
    int array[20] = {2, 3, 430, 5, 1, 41, 52, 620, 1, 5, 20, 41, 401, 13, 20, 20, 21, 430, 56, 26};
    int key = 430;
    int element_per_process = size / world_size;

    int start = world_rank * element_per_process;
    int end = start + element_per_process - 1;
    int count = 0;

    for (int i = start; i <= end; i++)
    {
        if (array[i] == key)
        {
            count++;
        }
    }

    if (world_rank != 0)
    {
        MPI_Send(&count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        // int MPI_Send(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);
    }

    if (world_rank == 0)
    {
        for (int i = 1; i < world_size; i++)
        {
            int received_count;
            MPI_Recv(&received_count, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            // printf("rank : %d, count:%d\n", i, received_count);
            // int MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status);
            count += received_count;
        }
        printf("Total rank is %d,count %d\n", world_rank, count);
    }

    MPI_Finalize();
    return 0;
}

// buf: Pointer to the data you want to send.
// count: Number of elements in the send buffer.
// datatype: MPI datatype of the elements.
// dest: Rank of the destination process.
// tag: Message tag, which is an integer identifier for the message.
// comm: MPI communicator.
// status: Pointer to an MPI status structure that will be filled with information about the received message.