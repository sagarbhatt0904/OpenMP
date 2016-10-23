#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include "timer.h"
#include <vector>
using namespace std;

int main ()
{	timespec before1, after1, time_diff1;
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
	for (int i=0; i<m; i++)
	for (int j=0; j<m; j++)
	{			
		mat3[i][j]=(mat1[i][j]+mat2[i][j]);
	}
	get_time( &after1 );
	diff(&before1,&after1,&time_diff1);
	
	double time_s1 = time_diff1.tv_sec + (double)(time_diff1.tv_nsec)/1.0e9;

	cout<<"Time taken when looping over rows first:"<<time_s1<<"\n";
	return 0;
}
