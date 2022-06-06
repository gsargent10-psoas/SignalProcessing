import numpy as np
from ctypes import cdll
import ctypes
from numpy.ctypeslib import ndpointer
import pandas as pd

from signalprocessing import SignalProcessing

sp = SignalProcessing()

a = np.empty((0))
for i in range(10):
	a = np.append(a,i)
	
sp.imgscale(np.double(a),10,0,0,0)
print(a)

data = pd.read_csv('./test.csv')
data = data.transpose().to_numpy(np.double)
labels = np.zeros([1,788],np.intc)
k=7
iterations = 100
#print(data)
#print(data.shape)
print(labels)
print(labels.shape)

print('Running kmeans')
num_iterations = sp.kmeans(data,labels,788*2,k,iterations)
print('Finished kmeans')
print(labels)
print(num_iterations)
