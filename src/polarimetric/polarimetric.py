from ctypes import cdll
import ctypes
from tokenize import Double
from numpy.ctypeslib import ndpointer
from ctypes import *
import numpy as np

class Polarimetric:
    def __init__(self,lib_path='./libpolarimetric.so'):
        self.lib = cdll.LoadLibrary(lib_path)
        self._formugrid = self.lib.formugrid
        self._computeintensity = self.lib.computeintensity
        self._getNumberSubImages22 = self.lib.getNumberSubImages22
        self._Stitching = self.lib.Stitching
        self._formSubImage22 = self.lib.formSubImage22

        # int formugrid(double *s0, double *s1, double *s2, double *out_data, int width, int height, int modulation);
        # input: modulation={0,1,2,3} (2x2a,2x2b,3x2,4x2); 
        self._formugrid.restype = ctypes.c_int
        self._formugrid.argtypes = [ndpointer(dtype=np.double, flags="C_Contiguous"),ndpointer(ctypes.c_double, flags="C_Contiguous"),ndpointer(ctypes.c_double, flags="C_Contiguous"),ndpointer(ctypes.c_double, flags="C_Contiguous"),ctypes.c_int,ctypes.c_int,ctypes.c_int]
		# void computeintensity(double *s0,double *s1, double *s2, double *out_data, int width, int height, double angle);
        self._computeintensity.restype = None 
        self._computeintensity.argtypes = [ndpointer(dtype=np.double, flags="C_Contiguous"),ndpointer(ctypes.c_double, flags="C_Contiguous"),ndpointer(ctypes.c_double, flags="C_Contiguous"),ndpointer(ctypes.c_double, flags="C_Contiguous"),ctypes.c_int,ctypes.c_int,ctypes.c_double]

        #int getNumberSubImages22(int image_y, int image_x, int sub_y, int sub_x, int overlap);
        self._getNumberSubImages22.restype = ctypes.c_int
        self._getNumberSubImages22.argtypes = [ctypes.c_int,ctypes.c_int,ctypes.c_int, ctypes.c_int,ctypes.c_int]

        #int Stitching(double* image, int image_rows, int image_cols, double* sub_images, int sub_rows, int sub_cols, int overlap, int num_sub);
        self._Stitching.restype = ctypes.c_int
        self._Stitching.argtypes = [ndpointer(dtype=np.double, flags="C_Contiguous"),ctypes.c_int, ctypes.c_int, ndpointer(ctypes.c_double, flags="C_Contiguous"),ctypes.c_int,ctypes.c_int,ctypes.c_int, ctypes.c_int]

        #int formSubImage22(double* image, int image_rows, int image_cols, double* sub_images, int sub_rows, int sub_cols, int overlap, int num_sub);
        self._formSubImage22.restype = ctypes.c_int
        self._formSubImage22.argtypes = [ndpointer(dtype=np.double, ndim=2, flags="C_Contiguous"),ctypes.c_int, ctypes.c_int, ndpointer(dtype=ctypes.c_double, ndim=3, flags="C_Contiguous"),ctypes.c_int,ctypes.c_int,ctypes.c_int, ctypes.c_int]

    # int formugrid(double *s0, double *s1, double *s2, double *out_data, int width, int height, int modulation);
    def formugrid(self,s0:np.double,s1:np.double,s2:np.double,data:np.double,modulation:int)->int:
        h,w=data.shape
        return self._formugrid(np.ascontiguousarray(s0, dtype=np.double),np.ascontiguousarray(s1, dtype=np.double),
        np.ascontiguousarray(s2, dtype=np.double),np.ascontiguousarray(data, dtype=np.double),w,h,modulation)

    # void computeintensity(double *s0,double *s1, double *s2, double *out_data, int width, int height, double angle);
    def computeintensity(self,s0:np.double,s1:np.double,s2:np.double,data:np.double,angle:Double)->int:
        h,w=s0.shape
        self._computeintensity(np.ascontiguousarray(s0, dtype=np.double),np.ascontiguousarray(s1, dtype=np.double),
        np.ascontiguousarray(s2, dtype=np.double),np.ascontiguousarray(data, dtype=np.double),w,h,angle)

    # int getNumberSubImages22(int image_y, int image_x, int sub_y, int sub_x, int overlap);
    def getNumberSubImages22(self,image_y:int,image_x:int,sub_y:int,sub_x:int,overlap:int)->int:
        return self._getNumberSubImages22(image_y,image_x,sub_y,sub_x,overlap)

    #int Stitching(double* image, int image_rows, int image_cols, double* sub_images, int sub_rows, int sub_cols, int overlap, int num_sub);
    def stitching(self,image:np.double,sub_image:np.double,overlap:int)->int:
        iy,ix=image.shape
        sy,sx,num=sub_image.shape
        return self._Stitching(np.ascontiguousarray(image, dtype=np.double),iy,ix,
        np.ascontiguousarray(sub_image, dtype=np.double),sy,sx,overlap,num)

    #int formSubImage22(double* image, int image_rows, int image_cols, double* sub_images, int sub_rows, int sub_cols, int overlap, int num_sub);
    def formSubImage22(self,image:np.double,sub_image:np.double,overlap:int)->int:
        iy,ix=image.shape
        sy,sx,num=sub_image.shape
        return self._formSubImage22(np.ascontiguousarray(image, dtype=np.double),iy,ix,
        np.ascontiguousarray(sub_image, dtype=np.double),sy,sx,overlap,num)