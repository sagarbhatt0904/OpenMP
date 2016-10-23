/* 
Computing Pi using midpoint rule by computing the following integration:
pi=int{0,1}(sqrt(1-x^2))
int{a,b}(f(x))=((b-a)/N)(sum{1,N}(f(a+(k-0.5)((b-a)/N))))
*/

#include <iostream>
#include <fstream>
#include "timer.h"
#include "papi.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <math.h>
#include <iomanip>

using namespace std;
int main()
{
    float real_time, proc_time,mflops;
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



    double N;
    double p,x;
    double sum=0;
    double a=0, b=1;
    timespec before, after, time_diff;
    
    ifstream fin("input.txt");
    fin>>N;
    fin.close();


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