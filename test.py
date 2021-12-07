import numpy as np
from ctypes import cdll
import ctypes
from numpy.ctypeslib import ndpointer

from signalprocessing import SignalProcessing

sp = SignalProcessing()

a = np.empty((0))
for i in range(10):
	a = np.append(a,i)
	
sp.imgscale(np.double(a),10,0,0,0)
print(a)