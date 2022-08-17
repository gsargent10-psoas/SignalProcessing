from ctypes import cdll
import ctypes
from numpy.ctypeslib import ndpointer
from ctypes import *

class Polarimetric:
    def __init__(self,lib_path='./libpolarimetric.so'):
        self.lib = cdll.LoadLibrary(lib_path)
        self.formugrid = self.lib.formugrid
        self.computeintensity = self.lib.computeintensity
        self.getNumberSubImages22 = self.lib.getNumberSubImages22
        self.Stitching = self.lib.Stitching
        self.formSubImage22 = self.lib.formSubImage22

        # int formugrid(double *s0, double *s1, double *s2, double *out_data, int width, int height, int modulation);
        # input: modulation={0,1,2,3} (2x2a,2x2b,3x2,4x2); 
        self.formugrid.restype = ctypes.c_int
        self.formugrid.argtypes = [ndpointer(ctypes.c_double, flags="C_Contiguous"),ndpointer(ctypes.c_double, flags="C_Contiguous"),ndpointer(ctypes.c_double, flags="C_Contiguous"),ndpointer(ctypes.c_double, flags="C_Contiguous"),ctypes.c_int,ctypes.c_int,ctypes.c_int]
		# void computeintensity(double *s0,double *s1, double *s2, double *out_data, int width, int height, double angle);
        self.computeintensity.restype = None 
        self.computeintensity.argtypes = [ndpointer(ctypes.c_double, flags="C_Contiguous"),ndpointer(ctypes.c_double, flags="C_Contiguous"),ndpointer(ctypes.c_double, flags="C_Contiguous"),ndpointer(ctypes.c_double, flags="C_Contiguous"),ctypes.c_int,ctypes.c_int,ctypes.c_double]

        #int getNumberSubImages22(int image_y, int image_x, int sub_y, int sub_x, int overlap);
        self.getNumberSubImages22.restype = ctypes.c_int
        self.getNumberSubImages22.argtypes = [ctypes.c_int,ctypes.c_int,ctypes.c_int, ctypes.c_int,ctypes.c_int]

        #int Stitching(double* image, int image_rows, int image_cols, double* sub_images, int sub_rows, int sub_cols, int overlap, int num_sub, int* error);
        self.Stitching.restype = ctypes.c_int
        self.Stitching.argtypes = [ndpointer(ctypes.c_double, flags="C_Contiguous"),ctypes.c_int, ctypes.c_int, ndpointer(ctypes.c_double, flags="C_Contiguous"),ctypes.c_int,ctypes.c_int,ctypes.c_int, ctypes.c_int]

        #int formSubImage22(double* image, int image_rows, int image_cols, double* sub_images, int sub_rows, int sub_cols, int overlap, int num_sub, int* error);
        self.formSubImage22.restype = ctypes.c_int
        self.formSubImage22.argtypes = [ndpointer(dtype=ctypes.c_double, ndim=2, flags="C_Contiguous"),ctypes.c_int, ctypes.c_int, ndpointer(dtype=ctypes.c_double, ndim=3, flags="C_Contiguous"),ctypes.c_int,ctypes.c_int,ctypes.c_int, ctypes.c_int]