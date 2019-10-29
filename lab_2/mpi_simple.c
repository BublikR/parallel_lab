#include <stdio.h>
#include <mpi.h>
#include <unistd.h>
int main(int argc, char* argv[]){

        int my_number; 	//Мой номер процесса
        int proc_num; 	//общее количество процессов
        
        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
        MPI_Comm_rank(MPI_COMM_WORLD, &my_number);

        printf("I an process %d from %d\n", my_number,proc_num);
	
	MPI_Barrier(MPI_COMM_WORLD);

        MPI_Finalize();
        return 0;
}
