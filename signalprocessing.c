#include "signalprocessing.h"
#include<math.h>
#include<stdlib.h>

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
// return: 0=no error; 1=type not found;
int imgscale(double* img, int size, double param1, double param2, int type)
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
	else if (type==2)
	{
		alpha_l = param1;
		alpha_h = param2;
	}
	else
	{
		// type not found
		return 1;
	}
	double div = (alpha_h-alpha_l);
	div = div == 0.0 ? 0.0001 : div;
	for(int k=0;k<size;k++)
	{
		img[k] = img[k] < alpha_l ? alpha_l : img[k];
		img[k] = img[k] > alpha_h ? alpha_h : img[k];
		img[k] = (img[k] - alpha_l)/(div); // [0,1]
	}
	return 0;
}

// size = x*y, where in is a 2d array of points and out is the classification for each point.
// return: 0=no error; 1=k invalid; 2=invalid iterations;
int kmeans(double* in, double* out, int size, int k, int iterations)
{
	// Step-1: Select value of k, to decide number of clusters to be formed.
	if (k < 1 || k > size/2)
	{
		// number of clusters needs to be greater than zero and less than total size
		return 1;
	}
	if (iterations < 1)
	{
		// number of iterations needs to be greater than zero
		return 2;
	}
	
	// Step-2: Select random k points, which will act as centroids.
	double* clusters = (double*) malloc(k*2*sizeof(double));
	int num_points = size/2;
	int delta = num_points/k;
	int index = 0;
	for (int z=0;z<k;z++)
	{
		clusters[index] = in[index];
		clusters[index+k] = in[index+num_points];
		index = index + delta;
	}
	
	// Step 3: Assign each data point, based on their distance from the centroids,
	//		   to the nearest/closest centroid.
	double in_x,in_y,c_x,c_y,new_cx,new_cy;
	for (int it=0;it<iterations;it++)
	{
		for (int z=0;z<num_points;z++)
		{
			in_x = in[z];
			in_y = in[z+num_points];
		}
	
		// Step-4: Recompute centroid of each cluster.
		for (int z=0;z<num_points;z++)
		{
			for (int c=0;c<k;c++)
			{
				if (out[z] == c)
				{
					
				}
			}
		}
		// Step-5: Reassign each data point.
		// End: If no reassignment occurs or max number of iterations reach, end.
	}
	free(clusters);
	return 0;
}

int knn(double* in, double* out, int size, int k)
{
	// future implementation
}
