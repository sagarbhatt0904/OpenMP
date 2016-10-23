/* 
Computing Pi using midpoint rule by computing the following integration:
pi=int{0,1}(sqrt(1-x^2))
int{a,b}(f(x))=((b-a)/N)(sum{1,N}(f(a+(k-0.5)((b-a)/N))))
*/
#include <iostream>
#include <fstream>
#include "timer.h"
#include <math.h>
#include "papi.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <iomanip>

#define NUM_EVENTS 3
#define ERROR_RETURN(retval) { fprintf(stderr, "Error %d %s:line %d: \n",retval,__FILE__,__LINE__); exit(retval); }

using namespace std;
int main()
{
    int Events[3] = {PAPI_TOT_INS, PAPI_TOT_CYC,PAPI_L1_DCM};
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

    double N;
    double p,x;
    double sum=0;
    double a=0, b=1;
    timespec before, after, time_diff;
   
    ifstream fin("input.txt");
    fin>>N;
    fin.close();
    
    /*Counter Starts*/

    if ( (retval = PAPI_start_counters(Events, NUM_EVENTS)) != PAPI_OK)
    ERROR_RETURN(retval);
    printf("\nCounter Started: \n");


    get_time(&before);
    for (int k = 0; k < N; ++k)
    {
    	x=a+((k-0.5)*(b-a)/N);    	
    	sum+=sqrt(1-pow(x,2));
    }
    p=4*((b-a)/N)*sum;
    get_time(&after);

    diff(&before,&after,&time_diff);
	double time_s = time_diff.tv_sec + (double)(time_diff.tv_nsec)/1.0e9;
    cout<<"Pi="<<setprecision(18)<<p<<endl<<"Time taken:"<<time_s<<endl;

    /*Computation Ends*/
    /*Counter Ends*/

    if ( (retval=PAPI_read_counters(values, NUM_EVENTS)) != PAPI_OK)
    ERROR_RETURN(retval);
    printf("Read successfully\n");
    printf("The total instructions executed for addition are %lld \n",values[0]);
    printf("The total cycles used are %lld \n", values[1] );
    printf("The total cache misses are %lld \n", values[2] );    

    return 0;
}