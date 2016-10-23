/* Mandlebrot Set

Z_{i+1} = (Z_{i})^2 + c

|Z|<=2 (for mandlebrot set, the value goes out of bound for |Z| greater than 2)
=> sqrt(x^2 +y^2)<=2
*/

#include <iostream>
#include <fstream>
#include <omp.h>
#include <vector>
#include "timer.h"

using namespace std;

int main()
{		
	int N; /*Resolution of mandlbrot plot is to be made*/
	int iter, max_iter;
	
	timespec before, after, time_diff;
	double x_max, x_min, y_max, y_min; /*iteration countert and max iterations*/
	// double** c_real; double** c_img; int** plot;
	
	ifstream fin("input.txt");
	fin>>N>>max_iter>>x_max>>x_min>>y_max>>y_min;;
	fin.close();
	
	std::vector<std::vector<double> > c_real (N,std::vector<double>(N, 0)); /*Real component of c*/
	std::vector<std::vector<double> > c_img (N,std::vector<double>(N, 0)); /*Imaginary component of c*/
	std::vector<std::vector<int> > plot (N,std::vector<int>(N, 0));
	
	int numthd[12]; /*Number of threads*/
	double time_s[12];
	double speed[12];
	double serial_frac[12];

	for (int k = 0; k < 12; ++k)
	{
		numthd[k]=k+1;
		double area=0;	/*Initializing area*/
	get_time(&before);

	#pragma omp parallel for schedule(runtime) num_threads(numthd[k])
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

	#pragma omp parallel for reduction(+:area) schedule(runtime) num_threads(numthd[k])
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
	// ofstream fout("mand_omp.vtk");
	// fout<<"# vtk DataFile Version 2.0\nVTK from matlab\n"<<"ASCII\n"<<"DATASET STRUCTURED_POINTS\n"<<"DIMENSIONS "<<N<<" "<<N<<" "<<1<<"\nSPACING 1 1 1 \n"<<"ORIGIN 0 0 0\n"<<"POINT_DATA "<<N*N<<"\nSCALARS yvel float 1\n"<<"LOOKUP_TABLE default\n";
	// for (int i = 0; i < N; ++i)
	// {
	// 	for (int j = 0; j < N; ++j)
	// 	{
	// 		fout<<plot[i][j]<<" ";
	// 	}
	// }
	// fout.close();
	time_s[k] = time_diff.tv_sec + (double)(time_diff.tv_nsec)/1.0e9;
	// cout<<"\nFor "<<numthd[k]<<" thread(s):"<<endl;
	// cout<<endl<<"Time with runtime omp: "<<time_s[k]<<endl<<"Area of the mandlebrot set ="<<area<<endl;
}

ofstream fout("mand_perform_r.csv");
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