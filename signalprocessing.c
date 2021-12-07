#include "signalprocessing.h"
#include<math.h>

/* Internal Functions */
double min(double* img, int size)
{
	double current=img[0];
	double min = current;
	if (size == 1)
	{
		return min;
	}
	for(int k=1;k<size;k++)
	{
		current=img[k];
		min = min < current ? min : current;
	}
	return min;
}

double max(double* img, int size)
{
	double current=img[0];
	double max = current;
	if (size == 1)
	{
		return max;
	}
	for(int k=1;k<size;k++)
	{
		current=img[k];
		max = max > current ? max : current;
	}
	return max;
}

double mean(double* img, int size)
{
	double total = 0.0;
	for(int k=0;k<size;k++)
	{
		total = total + img[k];
	}
	return total/size;
}

double std(double* img, int size)
{
	double mu = mean(img,size);
	double sum = 0.0;
	for(int k=0;k<size;k++)
	{
		sum = sum + pow(fabs((img[k]-mu)),2);
	}
	return sqrt(sum/size);
}

/* External Functions */ 
// type: 0=minmax; 1=statistical; 2=absolute.
void imgscale(double* img, int size, double param1, double param2, int type)
{
	double alpha_l, alpha_h, mu, sd;
	if (type==0)
	{
		alpha_l = min(img,size);
		alpha_h = max(img,size);
	}
	else if (type==1)
	{
		mu = mean(img,size);
		sd = std(img,size);
		alpha_l = mu - param1*sd;
		alpha_h = mu + param2*sd;
	}
	else
	{
		alpha_l = param1;
		alpha_h = param2;
	}
	double div = (alpha_h-alpha_l);
	div = div == 0.0 ? 0.0001 : div;
	for(int k=0;k<size;k++)
	{
		img[k] = img[k] < alpha_l ? alpha_l : img[k];
		img[k] = img[k] > alpha_h ? alpha_h : img[k];
		img[k] = (img[k] - alpha_l)/(div); // [0,1]
	}
}

