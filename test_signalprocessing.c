#include<stdio.h>
#include "signalprocessing.h"

int main()
{
	double array[10];
	double* ptr = &array[0];
	for (int k=0;k<10;k++)
	{
		array[k]=k;
	}
	double min,max,mu,sd;
	imgscale(ptr,10,0,0,0);
	for (int k=0;k<10;k++)
	{
		fprintf(stdout,"array[k]: %f\n",array[k]);
	}
	return 0;
}