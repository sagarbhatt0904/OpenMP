/*GOLDBACH conjecture= : Every even integer greater than 2 can be expressed as the sum of two primes.*/

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

bool isPrime(int num)
{   
    bool prime;
    if (num==1)
    {
        prime = false;
        return prime;
    }
    if (num==2)
    {
        prime = true;
    }
    else
    {
        for(int i = 3; i <= num; i++)
        {
            prime = true;
            for(int n = 2; n < i ; n++)
            {
                if(i % n == 0)
                {
                    prime = false;
                }           
            }
        }
    }
    return prime;   
}

    
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



    int N,N1;
    timespec before, after, time_diff;
    ifstream fin("input.txt");
    fin >> N;
    fin.close();
    N1=N;
    if(N < 4)
    return 0;

    int i;
    get_time(&before);
    int abort;

    /*Counter Starts*/

    if ( (retval = PAPI_start_counters(Events, NUM_EVENTS)) != PAPI_OK)
    ERROR_RETURN(retval);
    printf("\nCounter Started: \n");

    for(int k=4; k<=N1; k++ )
    { 
        N=k; 
        abort=0;
        
        for(int i=0; i<=N/2; i++)
        {   abort=0;
            if(isPrime(i))
            {                   
                for(int j = N-1; j >= N/2; j--)
                {
                    if(isPrime(j) && i+j == N)
                    {                                           
                       abort=1;
                       break;                       
                    }
                    
                }
            }
        }
    }
    get_time(&after);
    diff(&before,&after,&time_diff);
    double time_s = time_diff.tv_sec + (double)(time_diff.tv_nsec)/1.0e9;
    cout<<endl<<"Elapsed Time: "<<time_s;
    if (abort=1)
    {
        cout<<" \nGoldbach conjecture was verified for all even numbers less than "<<N1<<endl;
    }
    else
    {
        cout<<"\nGoldbach conjecture could not be verified! \n";
    }

    /*Computation Ends*/
    /*Counter Ends*/

    if ( (retval=PAPI_read_counters(values, NUM_EVENTS)) != PAPI_OK)
    ERROR_RETURN(retval);
    printf("Read successfully\n");
    printf("The total instructions executed for addition are %lld \n",values[0]);
    printf("The total cycles used are %lld \n", values[1] );
    printf("The total cache misses are %lld \n", values[2] );
    printf("The flops: %d \n", values[3]);
    
    return 0; 
}