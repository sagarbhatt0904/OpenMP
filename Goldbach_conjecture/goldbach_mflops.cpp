/*GOLDBACH conjecture= : Every even integer greater than 2 can be expressed as the sum of two primes.*/

#include <iostream>
#include <fstream>
#include "timer.h"
#include <vector>
#include "papi.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>

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