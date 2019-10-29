#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>
#include <math.h>
using namespace std;

int  main( int argc, char** argv ) {
  int size, rank, *ranks, i, j, k;
  const int n = 4; // количество уравнений
  double a[n][n+1];
  
  MPI_Status status;
  //MPI_Comm subComm;
  //MPI_Group group, subgroup;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  //Генерация строк матрицы
  k = rank; 
  for(int i = 0; i < ceil((double)n/size); i++){
    //if((i == n/size) && (rank > n%size)) break;
    for(j=0;j<n+1;j++)
      a[k][j] = rank;
    k+=size;
  }
  
  //Пересылка сгенерированых строк на root 
  for (int i = 0, j = rank; i < ceil((double)n/size); i++, j+=size){
   /* if((i == n/size) && (rank < n%size)){
      ranks = new int[n%size];
      for(int k = 0; k<n%size; k++) ranks[k] = k;
      MPI_Group_excl(group, n%size, ranks, &subgroup);
      MPI_Comm_create(MPI_COMM_WORLD, subgroup, &subComm);
      MPI_Gather(a[j], n+1, MPI_DOUBLE, a[j], n+1, MPI_DOUBLE, 0, subComm);
      break;
    }*/
    MPI_Gather(a[j], n+1, MPI_DOUBLE, a[j], n+1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  }
  MPI_Barrier(MPI_COMM_WORLD);

  //Вывод матрицы
  if(rank == 0){
    for(i=0;i<n;i++){
      for(j=0;j<n+1;j++){
      	cout << a[i][j] <<" ";
      }
      cout << endl;
    }
  }

#if 0
for(i=0; i<(r)*(n+1)/p; i++){
	if(i< r*n/p) {only recv  
		if(need send) send
	}
	for(j=i; j<=n; j++){
		for (k)
		a[j == rank][k] -=a[j][i]*a[i][k]/a[i][i];
		send
	}	
}
	
}



#endif
#if 0
  for(int k = 0; k < rank; k++){
    MPI_Recv(&a[k][k], n+1-k, MPI_DOUBLE, rank-1, 1, MPI_COMM_WORLD, &status);
    if(rank<n-1)
      MPI_Send(&a[k][k], n+1-k, MPI_DOUBLE, rank+1, 1, MPI_COMM_WORLD);
    for(int j = 0; j < n; j++){
      for(int i = 0; i < n+1; i++){
        a[rank][i] -= a[rank][i]*a[k][i]/a[k][k];
      }
      MPI_Send(&a[k][k], n+1-k, MPI_DOUBLE, rank+1, 1, MPI_COMM_WORLD);
    }
  }
  if(rank == 0){
    /*for(int g = 0; g < n+1; g++){
      a[rank][g] = a[rank][g]/a[rank][rank];
    }*/
    MPI_Send(&a[rank][rank], n+1, MPI_DOUBLE, rank+1, 1, MPI_COMM_WORLD);
    
  }
#endif

 
 
  //Вывод матрицы
  if(rank == 0){
    for(i=0;i<n;i++){
      for(j=0;j<n+1;j++){
        cout << a[i][j] <<" ";
      }
      cout << endl;
    }
  }
  //
    
  MPI_Finalize ();
  return 0;
}
