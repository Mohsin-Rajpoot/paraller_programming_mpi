#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    const int array_size = 20;
    const int key = 7;

    const int local_size = array_size / world_size;
    int local_array[local_size];
    for (int i = 0; i < local_size; i++)
    {
        local_array[i] = world_rank * local_size + i;
    }

    int local_count = 0;
    int local_index = -1;
    for (int i = 0; i < local_size; i++)
    {
        if (local_array[i] == key)
        {
            local_count++;
            local_index = world_rank * local_size + i;
        }
    }

    int receiving_counts[world_size];
    int displacement_array[world_size];

    MPI_Gather(&local_count, 1, MPI_INT, receiving_counts, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (world_rank == 0)
    {
        displacement_array[0] = 0;
        for (int i = 1; i < world_size; ++i)
        {
            displacement_array[i] = displacement_array[i - 1] + receiving_counts[i - 1];
        }
    }

    int receiving_indices[displacement_array[world_size - 1] + receiving_counts[world_size - 1]];

    MPI_Gatherv(&local_index, 1, MPI_INT, receiving_indices, receiving_counts, displacement_array, MPI_INT, 0, MPI_COMM_WORLD);

    if (world_rank == 0)
    {
        for (int i = 0; i < world_size; i++)
        {
            printf("%d:Total Counts=%d At Index: ", i, receiving_counts[i]);
            for (int j = displacement_array[i]; j < displacement_array[i] + receiving_counts[i]; j++)
            {
                printf("%d, ", receiving_indices[j]);
            }
            printf("\n");
        }
    }

    MPI_Finalize();
    return 0;
}
