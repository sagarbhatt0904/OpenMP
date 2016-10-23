#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include "timer.h"
#include <vector>
#include "papi.h"

#define NUM_EVENTS 4
#define ERROR_RETURN(retval) { fprintf(stderr, "Error %d %s:line %d: \n",retval,__FILE__,__LINE__); exit(retval); }
#define N 10000
#define TOT_FLOPS (N*N)

using namespace std;

int main ()
{	timespec before1, after1, time_diff1;
	srand(time(NULL));
	int Events[4] = {PAPI_TOT_INS, PAPI_TOT_CYC,PAPI_L1_DCM, PAPI_FP_OPS};
	int num_hwcntrs = 0;
    int retval;
    char errstring[PAPI_MAX_STR_LEN];
    long_long values[NUM_EVENTS];
    if((retval = PAPI_library_init(PAPI_VER_CURRENT)) != PAPI_VER_CURRENT ) 
    {
        fprintf(stderr, "Error: %d %s\n",retval, errstring);
        exit(1);
    }
     if ((num_hwcntrs = PAPI_num_counters()) < PAPI_OK) {
    printf("There are no counters available. \n");
    exit(1);
    }
    printf("There are %d counters in this system\n",num_hwcntrs);

    /*Computation Starts here*/

	vector<vector<double> > mat1(N,vector<double> (N,0));
	vector<vector<double> > mat2(N,vector<double> (N,0));
	vector<vector<double> > mat3(N,vector<double> (N,0));	

	for (int i=0; i<N; i++)		
	for (int j=0; j<N; j++)
	{	
			mat1[i][j]=rand()%10;
			mat2[i][j]=rand()%10;

	}
	
	
	get_time( &before1 );

	if ( (retval = PAPI_start_counters(Events, NUM_EVENTS)) != PAPI_OK)
    ERROR_RETURN(retval);
    printf("\nCounter Started: \n");
	
	for (int j=0; j<N; j++)
	for (int i=0; i<N; i++)
	{			
		mat3[i][j]=(mat1[i][j]+mat2[i][j]);
	}
	get_time( &after1 );
	diff(&before1,&after1,&time_diff1);
	
	double time_s1 = time_diff1.tv_sec + (double)(time_diff1.tv_nsec)/1.0e9;

	cout<<"Time taken when looping over columns first:"<<time_s1<<"\n";

	/*Computation Ends*/

	if ( (retval=PAPI_read_counters(values, NUM_EVENTS)) != PAPI_OK)
    ERROR_RETURN(retval);
    printf("Read successfully\n");
    printf("The total instructions executed for addition are %lld \n",values[0]);
    printf("The total cycles used are %lld \n", values[1] );
    printf("The total cache misses are %lld \n", values[2] );
    printf("MFlop/s = %f\n", (float)(TOT_FLOPS/1000000)/(time_s1));
    printf("The flops: %d \n", values[3]);


	return 0;
}
