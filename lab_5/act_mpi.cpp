#include <iostream>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
using namespace std;

int  main(int argc, char* argv[]) {
  int size, rank, i, j, k;
  const int n = 4; // Размерность матрицы
  double a[n][n+1], x[n];

  MPI_Status status;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  for(int i = rank*n/size; i < n*(rank+1)/size; i++){
	  for(int j = 0; j < n + 1; j++){
	    if (i == j) a[i][j] = 2;
		  else
			  a[i][j] = 1;
	  }
  }

  MPI_Gather(a[rank*n/size], (n+1)*n/size, MPI_DOUBLE, a[rank*n/size], (n+1)*n/size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);
  
  //Вывод матрицы
  if(rank == 0){
    for(int i=0;i<n;i++){
      for(int j=0;j<n+1;j++){
        cout << a[i][j] <<" ";
      }
      cout << endl;
    }
  }//
  
  if(rank == 0) MPI_Send( &a[0][0], n+1, MPI_DOUBLE, rank+1, 1, MPI_COMM_WORLD );
  
  //Прямой ход
  for(i=0; i<n-1; i++){
    if(i<rank*n/size) {
      MPI_Recv( &a[i][i], n+1-i, MPI_DOUBLE, rank-1, 1, MPI_COMM_WORLD, &status );	  
		  if(rank<n-1){
        MPI_Send( &a[i][i], n+1-i, MPI_DOUBLE, rank+1, 1, MPI_COMM_WORLD );
      }
	  }
	  for(j=((i > rank)?i:((rank == 0)?(i+1):rank*n/size)); j<(rank+1)*n/size; j++){
		  for (k = i+1; k<n+1; k++)
		    a[j][k] -=a[j][i]*a[i][k]/a[i][i];
      if(rank<n-1)
 		    MPI_Send( &a[j][i+1], n-i+1, MPI_DOUBLE, rank+1, 1, MPI_COMM_WORLD);
      // print
      if(rank == 2){
        for(int i=0;i<n;i++){
          for(int j=0;j<n+1;j++){
            cout << a[i][j] <<" ";
          }
          cout << endl;
      }
  }// finish print
    }
  }
  
#if 1  
  //Вывод матрицы
  if(rank == 2){
    for(int i=0;i<n;i++){
      for(int j=0;j<n+1;j++){
        cout << a[i][j] <<" ";
      }
      cout << endl;
    }
  }//
#endif

#if 0
  if(rank == n-1){
    for(i=n-1;i>=0;i--) {
      x[i]= a[i][n]/a[i][i];
      for(j=0;j<i;j++)
        a[j][n]-=x[i]*a[j][i];
        }
  for(i=0;i<n;i++) {
		cout<< i << " : " << x[i] << endl;
  }
  }
#endif
  
  MPI_Finalize ();
  return 0;
}
