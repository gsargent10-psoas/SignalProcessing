#include "signalprocessing.h"
#include<math.h>
#include<stdlib.h>

/*** Global Variables ***/


/*** Internal Functions ***/

/* Min value of an array. */
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

/* Max value of an array. */
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

/* Mean value of an array. */
double mean(double* img, int size)
{
	double total = 0.0;
	for(int k=0;k<size;k++)
	{
		total = total + img[k];
	}
	return total/size;
}

/* Standard deviation of an array. */
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

/*** External Functions ***/ 

/* Scale in array in the range of [0,1].
type: 0=minmax; 1=statistical; 2=absolute.
return: 0=no error; 1=type not found; */
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

/* Perform kmeans clustering with k++ initialization on an array.
size = x+y, where in is a 2d array of points and out is the classification for each point.
return: 0>=number of iterations; -1=k invalid; -2=invalid iterations; */
int kmeans(double* in, int* out, int size, int k, int iterations)
{
	// Step-1: Select value of k, to decide number of clusters to be formed.
	if (k < 1 || k > size/2)
	{
		// number of clusters needs to be greater than zero and less than total size
		return -1;
	}
	if (iterations < 1)
	{
		// number of iterations needs to be greater than zero
		return -2;
	}
	
	// Step-2: Initialize k clusters using kmeans++ algorithm for optimal performance.
	int num_points = size/2;
	double* centroids = (double*) malloc(k*2*sizeof(double));
	double* distances = (double*) malloc(num_points*sizeof(double));
	int* centroid_count = (int*) malloc(k*sizeof(int));
	double max_distance = 0.0;
	double min_distance = pow(10,6);
	double distance = 0.0;
	int nearest_centroid = 0;
	int index_max_distance = 0;
	centroids[0] = in[0]; // initialize first centroid
	centroids[0+num_points] = in[0+num_points]; // initialize first centroid
	for (int index_centroid=1;index_centroid<k;index_centroid++)
	{
		for (int z=0;z<num_points;z++) // associate every point with each nearest centroid
		{
			distance = 0.0; min_distance = pow(10,6);
			for (int c=0;c<index_centroid;c++)
			{
				distance = sqrt((pow((centroids[c]-in[z]),2)+pow((centroids[c+k]-in[z+num_points]),2)));
				if (distance < min_distance)
				{
					min_distance = distance;
					distances[z] = min_distance;
				}
			}
			if (distances[z] > max_distance) // choose point that is farthest away from closest centroid
			{
				max_distance = distances[z];
				index_max_distance = z;
			}
		}
		centroids[index_centroid] = in[index_max_distance]; // assign new centroid
		centroids[index_centroid+k] = in[index_max_distance+num_points]; // assign new centroid
	}
	
	
	// Step 3: Assign each data point, based on their distance from the centroids,
	//		   to the nearest/closest centroid.
	for (int z=0;z<num_points;z++) // associate every point with each nearest centroid
	{
		distance = 0.0; min_distance = pow(10,6);
		for (int c=0;c<k;c++)
		{
			distance = sqrt((pow((centroids[c]-in[z]),2)+pow((centroids[c+k]-in[z+num_points]),2)));
			if (distance < min_distance)
			{
				min_distance = distance;
				nearest_centroid = c;
			}
		}
		out[z] = nearest_centroid; // assign closest centroid
	}
	
	int it=0;
	for (;it<iterations;it++)
	{
		// Step-4: Recompute centroids.
		for (int c=0;c<k;c++) // initialize centroid count to zero
		{
			centroid_count[c] = 0;
		}
		for (int z=0;z<num_points;z++) // add all points in the same cluster together
		{
			centroids[out[z]] = centroids[out[z]] + in[z];
			centroids[out[z]+k] = centroids[out[z]+k] + in[z+num_points];
			centroid_count[out[z]] = centroid_count[out[z]] + 1;
		} 
		for (int c=0;c<k;c++) // compute new centroids
		{
			centroids[c] = centroids[c]/(centroid_count[c]+1); // prevent divide by zero by including original centroid
			centroids[c+k] = centroids[c+k]/(centroid_count[c]+1); // prevent divide by zero by including original centroid
		}
		
		// Step-5: Reassign each data point.
		// End: If no reassignment occurs or max number of iterations reached, end.
		int reassigned = 0;
		int prev_c;
		for (int z=0;z<num_points;z++)
		{
			prev_c = out[z];
			distance = 0.0; min_distance = pow(10,6);
			for (int c=0;c<k;c++)
			{
				distance = sqrt((pow((centroids[c]-in[z]),2)+pow((centroids[c+k]-in[z+num_points]),2)));
				if (distance < min_distance)
				{
					min_distance = distance;
					nearest_centroid = c;
				}
			}
			out[z] = nearest_centroid; // assign closest centroid
			if (prev_c != nearest_centroid)
			{
				reassigned = reassigned + 1;
			}
		}
		if (reassigned == 0)
		{
			break;
		}
	}
	
	free(centroids);
	free(distances);
	free(centroid_count);
	return it;
}
