from ctypes import cdll
import ctypes
from numpy.ctypeslib import ndpointer

class Polarimetric:
    def __init__(self,lib_path='./libpolarimetric.so'):
        self.lib = cdll.LoadLibrary(lib_path)
        self.formugrid = self.lib.formugrid
        self.computeintensity = self.lib.computeintensity
        
        # int formugrid(double *s0, double *s1, double *s2, double *out_data, int width, int height, int modulation);
        # input: modulation={0,1,2,3} (2x2a,2x2b,3x2,4x2); 
        self.formugrid.restype = ctypes.c_int
        self.formugrid.argtypes = [ndpointer(ctypes.c_double, flags="C_Contiguous"),ndpointer(ctypes.c_double, flags="C_Contiguous"),ndpointer(ctypes.c_double, flags="C_Contiguous"),ndpointer(ctypes.c_double, flags="C_Contiguous"),ctypes.c_int,ctypes.c_int,ctypes.c_int]
		# void computeintensity(double *s0,double *s1, double *s2, double *out_data, int width, int height, double angle);
        self.computeintensity.restype = None 
        self.computeintensity.argtypes = [ndpointer(ctypes.c_double, flags="C_Contiguous"),ndpointer(ctypes.c_double, flags="C_Contiguous"),ndpointer(ctypes.c_double, flags="C_Contiguous"),ndpointer(ctypes.c_double, flags="C_Contiguous"),ctypes.c_int,ctypes.c_int,ctypes.c_double]
