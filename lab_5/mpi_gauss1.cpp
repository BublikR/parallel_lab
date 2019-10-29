#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>
using namespace std;

const int n = 4;
double a0[n][n+1]; // b0 = a[][n+1]
double a[n][n+1];
//double b0[n+1];
double x0[n];
double x;
double xx;

void print (int i){
	int j ;
	
	sleep(i);
	cout <<i<<" "<< endl;
        for(j=0;j<n+1;j++){
                cout << a[i][j]<<" ";
        }
	cout <<endl;
	MPI_Barrier(MPI_COMM_WORLD);
}

int main( int argc, char** argv ) {

	int ThreadNumber;
	int ThreadsAmount;
	MPI_Status Status;
	MPI_Init (&argc, &argv);
	MPI_Comm_rank ( MPI_COMM_WORLD, & ThreadNumber );
	MPI_Comm_size ( MPI_COMM_WORLD, & ThreadsAmount );

	long i = ThreadNumber;
	long j;
	long k;
	
	if( ThreadsAmount != n )
		return -1;
	
    for(j=0;j<n+1;j++){
        //a0[i][j] = a[i][j] = ( 1.0 * rand() ) / (RAND_MAX ) + i;
		if (i == j) a0[i][j] = a[i][j] = 2;
		else
			a0[i][j] = a[i][j] = 1;
	}
	MPI_Gather(a0[i], n+1, MPI_DOUBLE, a0 , n+1,MPI_DOUBLE , 0, MPI_COMM_WORLD);	
	//////////////////////////////////////////
	MPI_Barrier(MPI_COMM_WORLD);	

	sleep(i);
	/*cout <<i<<" "<< endl;
	for(j=0;j<n+1;j++){
		cout << a0[i][j]<<" ";
	}
	cout <<endl;

	MPI_Barrier(MPI_COMM_WORLD);*/
	if(ThreadNumber == 0){
	for(i=0;i<n;i++){
		for(j=0;j<n+1;j++){
        		cout << a0[i][j] <<" ";
        	}
		cout << endl;
	}
	}
	//////////////////////////////////////////
	for(j=0;(j<n-1)&&(j<i);j++){
		
		MPI_Recv( &a[j][j], n+1-j, MPI_DOUBLE, i-1, 111, MPI_COMM_WORLD, &Status );	
		if(i<n-1)
			MPI_Send( &a[j][j], n+1-j, MPI_DOUBLE, i+1, 111, MPI_COMM_WORLD );
	
		for(k = i; k<n+1; k++) {
			a[i][k]-=a[j][k]*a[i][j]/a[j][j];
			//a[i][k] = (a[i][k]*a[j][j] - a[j][k]*a[i][j])/(a[i][j]*a[j][j]);
		}
	}
	if(i<n-1)
		MPI_Send( &a[i][i], n+1-i, MPI_DOUBLE, i+1, 111, MPI_COMM_WORLD);
	////////////////////////////////////////// back
	for(j=n-1;j>i;j--) {
                MPI_Recv( &xx, 1, MPI_DOUBLE, i+1, 111, MPI_COMM_WORLD, &Status );
                if( i > 0 )
                        MPI_Send( &xx, 1, MPI_DOUBLE, i-1, 111, MPI_COMM_WORLD );
                a[i][n]-=xx*a[i][j];
        }
        x = a[i][n]/a[i][i];
        if( i > 0 )
                MPI_Send( &x, 1, MPI_DOUBLE, i-1, 111, MPI_COMM_WORLD );
	///////////////////////////////////////////
	MPI_Gather(&x, 1, MPI_DOUBLE, x0 , 1, MPI_DOUBLE , 0, MPI_COMM_WORLD);
	
	///////////////////////////////////////////
	if( ThreadNumber == 0 ) {
	for(i=0;i<n;i++) {
              double res = - a0[i][n];
              for(j=0;j<n;j++)
                    res += x0[j] * a0[i][j];
              cout<< i << " : " << x0[i] << " | " << res << endl;
        }
	}
	///////////////////////////////////////////
	MPI_Finalize ();
	return 0;
}
