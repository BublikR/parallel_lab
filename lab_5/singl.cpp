#include <iostream>
#include <stdlib.h>
#include <mpi.h>
using namespace std;

int main(int argc, char* argv[]) 
{
	const int n = 8; // Размерность матрицы
	int rank, size, i;
	double a[n][n+1], x[n], temp1, temp2;
	MPI_Status status;
	MPI_Init (&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	MPI_Comm_size (MPI_COMM_WORLD, &size);

	i = rank;
	
	/*if(size != n)
		return -1;*/
	// Заполнение матрицы
    for(int j = 0; j < n + 1; j++){
        if (i == j) a[i][j] = 2;
		else
			a[i][j] = 1;
	}
	MPI_Gather(a[i], n+1, MPI_DOUBLE, a, n+1, MPI_DOUBLE , 0, MPI_COMM_WORLD);	

	//Вывод матрицы
    if(rank == 0){
      for(int i = 0; i < n; i++){
        for(int j = 0;j < n + 1; j++){
          cout << a[i][j] << " ";
        }
        cout << endl;
      }
    }
  
	// Прямой ход
	for(int j = 0; j < n - 1; j++){	
		if(j < i){
			MPI_Recv(&a[j][j], n+1-j, MPI_DOUBLE, i-1, 1, MPI_COMM_WORLD, &status);	
			if( i < n - 1)
				MPI_Send(&a[j][j], n+1-j, MPI_DOUBLE, i+1, 1, MPI_COMM_WORLD);
			for(int k = j + 1;k < n + 1; k++){
				a[i][k] -= a[j][k] * a[i][j] / a[j][j];		
			}
		}
	}
    if(i < n - 1)
		MPI_Send(&a[i][i], n+1-i, MPI_DOUBLE, i+1, 1, MPI_COMM_WORLD);

	// Обратный ход	
	for(int j = n - 1; j > i; j--){
        MPI_Recv(&temp2, 1, MPI_DOUBLE, i+1, 1, MPI_COMM_WORLD, &status);
        if(i > 0)
            MPI_Send(&temp2, 1, MPI_DOUBLE, i-1, 1, MPI_COMM_WORLD);
		a[i][n] -= temp2 * a[i][j];
    }
    temp1 = a[i][n] / a[i][i];
    if(i > 0)
        MPI_Send(&temp1, 1, MPI_DOUBLE, i-1, 1, MPI_COMM_WORLD);

	MPI_Gather(&temp1, 1, MPI_DOUBLE, x, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);	

	// Вывод результата
	if(rank == 0){
		for(int i = 0; i < n; i++){
			cout << "x[" << i << "]" << " = " << x[i] << endl;
        }
	}

	MPI_Finalize ();
	return 0;
}

