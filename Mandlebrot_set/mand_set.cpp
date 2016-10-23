/* Mandlebrot Set

Z_{i+1} = (Z_{i})^2 + c

|Z|<=2 (for mandlebrot set, the value goes out of bound for |Z| greater than 2)
=> sqrt(x^2 +y^2)<=2
*/

#include <iostream>
#include <fstream>
#include "timer.h"
#include <vector>
#include "papi.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>


#define NUM_EVENTS 4
#define ERROR_RETURN(retval) { fprintf(stderr, "Error %d %s:line %d: \n",retval,__FILE__,__LINE__); exit(retval); }


using namespace std;

int main()
{	
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

	
	int N; /*Resolution of mandlbrot plot is to be made*/
	int iter, max_iter;
	
	timespec before, after, time_diff;
	double x_max, x_min, y_max, y_min; /*iteration countert and max iterations*/
	
	// double** c_real; double** c_img; int** plot;
	
	ifstream fin("input.txt");
	fin>>N>>max_iter>>x_max>>x_min>>y_max>>y_min;;
	fin.close();
	
	vector<vector<double> > c_real (N,vector<double>(N, 0)); /*Real component of c*/
	vector<vector<double> > c_img (N,vector<double>(N, 0)); /*Imaginary component of c*/
	vector<vector<int> > plot (N,vector<int>(N, 0));
	
	double TOT_FLOPS = N*N;
	double area=0; /*Initializing area*/

	/*Counter Starts*/

	if ( (retval = PAPI_start_counters(Events, NUM_EVENTS)) != PAPI_OK)
    ERROR_RETURN(retval);
    printf("\nCounter Started: \n");

	get_time(&before);

	 for(int i=0; i<N; i++)
	{
		for(int j=0; j<N; j++)
		{
			iter=0;
			double x=0,y=0, tmp;
			c_real[i][j] =(x_max*((double)(j))+x_min*((double)(N-j)))/((double)(N));
			c_img[i][j] =(y_max*((double)(i))+y_min*((double)(N-i)))/((double)(N));
			
			while(x*x +y*y < 4 && iter < max_iter)
			{
				tmp= x*x-y*y + c_real[i][j];
				y=2*x*y + c_img[i][j];
				x=tmp;				
				iter++;
			}
			
			plot[i][j]=iter;
		}			
	}

	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			if (plot[i][j]>max_iter/2)
			{
				area+=((x_max-x_min)/N)*((y_max-y_min)/N);
			}
		}
	}
		
	get_time(&after);
	diff(&before,&after,&time_diff);
	// ofstream fout("mand.vtk");
	// fout<<"# vtk DataFile Version 2.0\nVTK from matlab\n"<<"ASCII\n"<<"DATASET STRUCTURED_POINTS\n"<<"DIMENSIONS "<<N<<" "<<N<<" "<<1<<"\nSPACING 1 1 1 \n"<<"ORIGIN 0 0 0\n"<<"POINT_DATA "<<N*N<<"\nSCALARS yvel float 1\n"<<"LOOKUP_TABLE default\n";
	// for (int i = 0; i < N; ++i)
	// {
	// 	for (int j = 0; j < N; ++j)
	// 	{
	// 		fout<<plot[i][j]<<" ";
	// 	}
	// }
	// fout.close();
	double time_s = time_diff.tv_sec + (double)(time_diff.tv_nsec)/1.0e9;
	cout<<endl<<"Time without omp: "<<time_s<<endl<<"Area of the mandlebrot set ="<<area<<endl;

	/*Computation Ends*/
	/*Counter Ends*/
	if ( (retval=PAPI_read_counters(values, NUM_EVENTS)) != PAPI_OK)
    ERROR_RETURN(retval);
    printf("Read successfully\n");
    printf("The total instructions executed for addition are %lld \n",values[0]);
    printf("The total cycles used are %lld \n", values[1] );
    printf("The total cache misses are %lld \n", values[2] );
    printf("MFlop/s = %f\n", (float)(TOT_FLOPS/1000000)/(time_s));
    printf("The flops: %d \n", values[3]);
	return 0;
}