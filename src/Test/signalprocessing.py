from ctypes import cdll
import ctypes
from tokenize import Double
from numpy.ctypeslib import ndpointer
import numpy as np

class SignalProcessing:
    def __init__(self,lib_path='./libsignalprocessing.so'):
        self.lib = cdll.LoadLibrary(lib_path)
        self._imgscale_double = self.lib.imgscale_double
        self._imgscale_float = self.lib.imgscale_float
        self._kmeans_double = self.lib.kmeans_double
        self._kmeans_float = self.lib.kmeans_float
        
        # void imgscale(double* img, int size, double param1, double param2, int type);
        # type: 0=minmax; 1=statistical; 2=absolute.
        self._imgscale_double.restype = ctypes.c_int
        self._imgscale_double.argtypes = [ndpointer(np.double, flags="C_Contiguous"),ctypes.c_int,ctypes.c_double,ctypes.c_int]
        self._imgscale_float.restype = ctypes.c_int
        self._imgscale_float.argtypes = [ndpointer(np.float32, flags="C_Contiguous"),ctypes.c_int,ctypes.c_double,ctypes.c_int]
        
        # size = x+y, where in is a 2d array of points and out is the classification for each point.
        # return: 0=no error; 1=k invalid; 2=invalid iterations;
        # int kmeans(double* in, int* out, int size, int k, int iterations);
        self._kmeans_double.restype = ctypes.c_int
        self._kmeans_double.argtypes = [ndpointer(np.double, flags="C_Contiguous"),ndpointer(ctypes.c_int, flags="C_Contiguous"),ctypes.c_int,ctypes.c_int]
        self._kmeans_float.restype = ctypes.c_int
        self._kmeans_float.argtypes = [ndpointer(np.float32, flags="C_Contiguous"),ndpointer(ctypes.c_int, flags="C_Contiguous"),ctypes.c_int,ctypes.c_int]

    def imgscale(self,img:np.double,p1:Double=1.0,p2:Double=1.0,type:int=0):
        h,w=img.shape
        return self._imgscale_double(np.ascontiguousarray(img, dtype=np.double),h*w,p1,p2,type)
    def imgscale(self,img:np.float32,p1:Double=1.0,p2:Double=1.0,type:int=0):
        h,w=img.shape
        return self._imgscale_float(np.ascontiguousarray(img, dtype=np.float32),h*w,p1,p2,type)