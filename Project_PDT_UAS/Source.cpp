# include <mpi.h>
# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# include <math.h>
# include <string.h>

#define TIME_SIZE 40
#define MAX_SIZE 1000

int main(int argc, char *argv[]) {
	long int N = 1000000000; // target number
	int my_id; // Process ID
	int numprocs; // Number of processes
	int master = 0;
	long long local_sum = 0, global_sum = 0;
	MPI_Status status;
	int i;
	double part = ceil(sqrt(N));
	double counter = 0.0;
	/*
	  Initialize MPI.
	*/
	MPI_Init(&argc, &argv);
	/*
	  Get the number of processes.
	*/
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	/*
	  Determine the rank of this process.
	*/
	MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

	// Init Master


	counter -= MPI_Wtime();

	int interval = ceil(part / numprocs);
	int low = (numprocs - my_id - 1) * interval + 1;
	int high = (numprocs - my_id) * interval;
	printf("%d %d %d %d\n",my_id, interval, low, high);
	for (low; low < high; low++) {
		if (N % low == 0) {
			if (N / low == low) {
				local_sum += low;
				printf("P %d Found The Factor %d\n",my_id, low);
			}
			else {

				local_sum += low + N / low;
				printf("P %d Found The Factor %d and %d\n",my_id, low, N / low);
			}
			//printf("\nThe Local Sum is %lld\n",local_sum);
		}
		
	}

	MPI_Barrier(MPI_COMM_WORLD);
	if (my_id != master) {
		MPI_Send(&local_sum, 1, MPI_DOUBLE, master, 1, MPI_COMM_WORLD);
	}
	else {
		global_sum = local_sum;
		for (i = 1; i < numprocs; i++) {
			MPI_Recv(&local_sum, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
			global_sum = global_sum + local_sum;
		}
	}
	MPI_Barrier(MPI_COMM_WORLD);
	counter += MPI_Wtime();
	
	
	if(my_id == master){
		printf("\nTotal Worktime is %f\n=============", counter);
		printf("\nTotal Sum is %lld\n", global_sum);
	}
	
	
	/*
	  Terminate MPI.
	*/
	MPI_Finalize();
	/*
	  Terminate.
	*/
	

	

	return 0;



}
