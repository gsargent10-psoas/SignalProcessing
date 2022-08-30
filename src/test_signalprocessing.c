#include<stdio.h>
#include "signalprocessing.h"

int main()
{
	float array[10];
	float* ptr = &array[0];
	for (int k=0;k<10;k++)
	{
		array[k]=k;
	}
	float min,max,mu,sd;
	imgscale_float(ptr,10,0,0,0);
	for (int k=0;k<10;k++)
	{
		fprintf(stdout,"array[k]: %f\n",array[k]);
	}
	return 0;
}
