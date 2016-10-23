/*GOLDBACH conjecture= : Every even integer greater than 2 can be expressed as the sum of two primes.*/

#include <iostream>
#include "timer.h"
#include <fstream>
#include <omp.h>


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
    int N,N1;
    timespec before, after, time_diff;

    ifstream fin("input.txt");
    fin >> N;
    fin.close();
    N1=N;
    if(N < 4)
    return 0;
    
    int i;
    
    int abort, numthd[12];
    double time_s[12], speed[12], serial_frac[12];
    for (int proc = 0; proc < 12; ++proc)
    {   get_time(&before);
        numthd[proc]=proc+1;
        
        for(int k=4; k<=N1; k++ )
        { 
            N=k; 
            abort=0;
            #pragma omp parallel for schedule(runtime) num_threads(numthd[proc]) private(before, after, time_diff)    
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
        time_s[proc] = time_diff.tv_sec + (double)(time_diff.tv_nsec)/1.0e9;
        // cout<<endl<<"Elapsed Time: "<<time_s[proc];
        // if (abort=1)
        // {
        //     cout<<" \nGoldbach conjecture was verified for all even numbers less than "<<N1<<" using "<<numthd[proc]<<" threads"<<endl;
        // }
        // else
        // {
        //     cout<<"\nGoldbach conjecture could not be verified! \n";
        // }
    }
    
    ofstream fout("goldbach_perform_r1.csv");
    for (int i = 0; i < 12; ++i)
    {   
        speed[i]= time_s[0]/time_s[i];
        fout<<numthd[i]<<","<<speed[i]<<endl;
    }
    fout<<endl<<endl<<endl;
    for (int i = 0; i < 12; ++i)
    {
        serial_frac[i]=((1/speed[i])-(1/numthd[i]))/(1-(1/numthd[i]));
        fout<<numthd[i]<<","<<serial_frac[i]<<endl;
    }
    fout.close();
    return 0; 
}