#include <iostream>
#include <fstream>
#include "timer.h"
#include "papi.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <math.h>
#include <vector>

using namespace std;

int main ()
{	float real_time, proc_time,mflops;
    long_long flpops;
    float ireal_time, iproc_time, imflops;
    long_long iflpops;
    int retval;  
    
    if((retval=PAPI_flops(&ireal_time,&iproc_time,&iflpops,&imflops)) < PAPI_OK)
    {
        printf("Could not initialise PAPI_flops \n");
        printf("Your platform may not support floating point operation event.\n");
        printf("retval: %d\n", retval);
        exit(1);
    }

    /*Computation Starts here*/

	timespec before1, after1, time_diff1;
	srand(time(NULL));
	int m;
	
	//cout<<"Enter n for n x n matrices:";
	//cin>>m;
	m=10000;
	
	vector<vector<double> > mat1(m,vector<double> (m,0));
	vector<vector<double> > mat2(m,vector<double> (m,0));
	vector<vector<double> > mat3(m,vector<double> (m,0));	

	for (int i=0; i<m; i++)		
	for (int j=0; j<m; j++)
	{	
			mat1[i][j]=rand()%10;
			mat2[i][j]=rand()%10;

	}
	
	
	get_time( &before1 );
	for (int j=0; j<m; j++)
	for (int i=0; i<m; i++)
	{			
		mat3[i][j]=(mat1[i][j]+mat2[i][j]);
	}
	get_time( &after1 );
	diff(&before1,&after1,&time_diff1);
	
	double time_s1 = time_diff1.tv_sec + (double)(time_diff1.tv_nsec)/1.0e9;

	cout<<"Time taken when looping over rows first:"<<time_s1<<"\n";

	 /*Computation Ends*/

    if((retval=PAPI_flops( &real_time, &proc_time, &flpops, &mflops))<PAPI_OK)
    {
    printf("retval: %d\n", retval);
    exit(1);
    }
    printf("Real_time: %f Proc_time: %f Total flpops: %lld MFLOPS: %f\n",
    real_time, proc_time,flpops,mflops);
    exit(0);
	return 0;
}
