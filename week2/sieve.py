from mpi4py import MPI
import numpy as np


def main():
    comm = MPI.COMM_WORLD
    rank = comm.Get_rank()
    size = comm.Get_size()
    limit = 38
    primes = []

    numbers = np.array(list(range(2, limit)))
    numbers_array = np.array(numbers)
    data = np.empty(len(numbers) // size, dtype=int)
    reshaped_numbers = numbers_array.reshape((size, len(numbers) // size))

    comm.Scatter(reshaped_numbers, data, root=0)

    # Find local minimum
    local_min = np.min(data)

    # Use MPI_Allreduce to find the global minimum
    global_min = comm.allreduce(local_min, op=MPI.MIN)
    p = global_min
    print(data, "before")

    for p in range(global_min, len(data) + 1):
        if p:
            for i in range(len(data)):
                if data[i] != p:
                    if data[i] % p == 0:
                        data[i] = False
    local_min = np.min(data)
    global_min = comm.allreduce(local_min, op=MPI.MIN)
    p = global_min

    for i in range(len(data)):
        if data[i]:
            primes.append(data[i])

    # Gather primes on node 0
    all_primes = comm.gather(primes, root=0)

    if rank == 0:
        # Flatten the list of primes from all nodes
        all_primes_flat = [
            prime for sublist in all_primes for prime in sublist]
        print(all_primes_flat)


main()
