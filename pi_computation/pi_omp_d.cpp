/* 
Computing Pi using midpoint rule by computing the following integration:
pi=int{0,1}(sqrt(1-x^2))
int{a,b}(f(x))=((b-a)/N)(sum{1,N}(f(a+(k-0.5)((b-a)/N))))
*/
#include <iostream>
#include <math.h>
#include <omp.h>
#include <fstream>
#include "timer.h"
#include <iomanip>

using namespace std;
int main()
{
    int N;
    double p,x;
    double sum;
    double a=0, b=1;
    timespec before, after, time_diff;
    
    ifstream fin("input.txt");
    fin>>N;
    fin.close();

    int numthd[12];
    double time_s[12], speed[12], serial_frac[12];
    int k;
    
    for (int proc = 0; proc < 12; ++proc)
    {   
        get_time(&before);
        numthd[proc]=proc+1;
        sum=0;
        #pragma omp parallel for schedule(dynamic, 10000) reduction(+:sum)     
        for (int k = 0; k < N; ++k)
        {
        	x=a+((k-0.5)*(b-a)/N);    	
        	sum+=sqrt(1-pow(x,2));
        }
        p=4*((b-a)/N)*sum;
        get_time(&after);

        diff(&before,&after,&time_diff);
    	time_s[proc] = time_diff.tv_sec + (double)(time_diff.tv_nsec)/1.0e9;
        if (proc==11)
        {
            cout<<"Pi="<<setprecision(18)<<p<<endl<<"Time taken with dynamic scheduling on "<<numthd[proc]<<" threads: "<<time_s[proc]<<endl;
        }
    }
    ofstream fout("pi_perform_d.csv");
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