#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>
#include <math.h>
using namespace std;

void matrix(double **a, int n) // Вывод матрицы
{
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n+1; j++)
    {
      cout << a[i][j];
      if (j < n) cout << "\t";
    }
    cout << endl;
  }
}

int main( int argc, char** argv ) {
  int size, rank, i, j, k;
  int n; // количество уравнений
  double time;
  double **a, *y, *x;

  MPI_Status status;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  
  if (argc == 1) n = 10;
  else if (argc == 2) n = atoi(argv[1]);
  else {
    cout << "Error! Too many arguments." << endl;
    return 0;
  }
  
  a = new double*[n];
  j = rank;
  for (int i = 0; i < n; i++)
  {
    a[i] = new double[n+1];
	if(i == j){
	  for (int k = 0; j < n+1; k++)
	  {
	    a[i][k] = rand() % 10000;
	  }
	j+=(n/size);
	}
  }
  for (int i = 0, j = rank; i < ceil((double)n/size); i++, j+=(n/size))
    MPI_Gather(a[i], n+1, MPI_DOUBLE, a[j], n+1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  if(rank == 0)
    matrix(a, n);
  
  MPI_Finalize ();
  return 0;
}