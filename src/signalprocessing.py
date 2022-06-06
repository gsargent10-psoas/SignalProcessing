from ctypes import cdll
import ctypes
from numpy.ctypeslib import ndpointer

class SignalProcessing:
    def __init__(self,lib_path='./libsignalprocessing.so'):
        self.lib = cdll.LoadLibrary(lib_path)
        self.imgscale = self.lib.imgscale
        self.kmeans = self.lib.kmeans
        
        # void imgscale(double* img, int size, double param1, double param2, int type);
        self.imgscale.restype = ctypes.c_int
        self.imgscale.argtypes = [ndpointer(ctypes.c_double, flags="C_Contiguous"),ctypes.c_int,ctypes.c_double,ctypes.c_int]
        
        # size = x+y, where in is a 2d array of points and out is the classification for each point.
        # return: 0=no error; 1=k invalid; 2=invalid iterations;
        # int kmeans(double* in, int* out, int size, int k, int iterations);
        self.kmeans.restype = ctypes.c_int
        self.kmeans.argtypes = [ndpointer(ctypes.c_double, flags="C_Contiguous"),ndpointer(ctypes.c_int, flags="C_Contiguous"),ctypes.c_int,ctypes.c_int]
