#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include "timer.h"
#include <vector>

using namespace std;

int main ()
{	timespec before2, after2, time_diff2;
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
	
	get_time( &before2 );
	for (int j=0; j<m; j++)
	for (int i=0; i<m; i++)
	{			
		mat3[i][j]=(mat1[i][j]+mat2[i][j]);
	}
	get_time( &after2 );	
	diff(&before2,&after2,&time_diff2);
	
	double time_s2 = time_diff2.tv_sec + (double)(time_diff2.tv_nsec)/1.0e9;
	cout<<"Time taken when looping over columns first:"<<time_s2<<"\n";
	return 0;
}
