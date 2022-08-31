from ctypes import cdll
import ctypes
from tokenize import Double
from numpy.ctypeslib import ndpointer
from ctypes import *
import numpy as np

class Polarimetric:
    def __init__(self,lib_path='./libpolarimetric.so'):
        self.lib = cdll.LoadLibrary(lib_path)
        self._formugrid_double = self.lib.formugrid_double
        self._formugrid_float = self.lib.formugrid_float
        self._computeintensity_double = self.lib.computeintensity_double
        self._computeintensity_float = self.lib.computeintensity_float
        self._getNumberSubImages22 = self.lib.getNumberSubImages22
        self._stitchsubimages_double = self.lib.stitchsubimages_double
        self._stitchsubimages_float = self.lib.stitchsubimages_float
        self._formSubImage22_double = self.lib.formSubImage22_double
        self._formSubImage22_float = self.lib.formSubImage22_float

        # int formugrid(double *s0, double *s1, double *s2, double *out_data, int width, int height, int modulation);
        # input: modulation={0,1,2,3} (2x2a,2x2b,3x2,4x2); 
        self._formugrid_double.restype = ctypes.c_int
        self._formugrid_double.argtypes = [ndpointer(dtype=np.double, flags="C_Contiguous"),ndpointer(dtype=np.double, flags="C_Contiguous"),
        ndpointer(dtype=np.double, flags="C_Contiguous"),ndpointer(dtype=np.double, flags="C_Contiguous"),ctypes.c_int,ctypes.c_int,ctypes.c_int]
        self._formugrid_float.restype = ctypes.c_int
        self._formugrid_float.argtypes = [ndpointer(dtype=np.float32, flags="C_Contiguous"),ndpointer(dtype=np.float32, flags="C_Contiguous"),
        ndpointer(dtype=np.float32, flags="C_Contiguous"),ndpointer(dtype=np.float32, flags="C_Contiguous"),ctypes.c_int,ctypes.c_int,ctypes.c_int]

		# void computeintensity(double *s0,double *s1, double *s2, double *out_data, int width, int height, double angle);
        self._computeintensity_double.restype = None 
        self._computeintensity_double.argtypes = [ndpointer(dtype=np.double, flags="C_Contiguous"),ndpointer(dtype=np.double, flags="C_Contiguous"),
        ndpointer(dtype=np.double, flags="C_Contiguous"),ndpointer(dtype=np.double, flags="C_Contiguous"),ctypes.c_int,ctypes.c_int,ctypes.c_double]
        self._computeintensity_float.restype = None 
        self._computeintensity_float.argtypes = [ndpointer(dtype=np.float32, flags="C_Contiguous"),ndpointer(dtype=np.float32, flags="C_Contiguous"),
        ndpointer(dtype=np.float32, flags="C_Contiguous"),ndpointer(dtype=np.float32, flags="C_Contiguous"),ctypes.c_int,ctypes.c_int,ctypes.c_double]

        #int getNumberSubImages22(int image_y, int image_x, int sub_y, int sub_x, int overlap);
        self._getNumberSubImages22.restype = ctypes.c_int
        self._getNumberSubImages22.argtypes = [ctypes.c_int,ctypes.c_int,ctypes.c_int, ctypes.c_int,ctypes.c_int]

        #int Stitching(double* image, int image_rows, int image_cols, double* sub_images, int sub_rows, int sub_cols, int overlap, int num_sub);
        self._stitchsubimages_double.restype = ctypes.c_int
        self._stitchsubimages_double.argtypes = [ndpointer(dtype=np.double, flags="C_Contiguous"),ctypes.c_int, ctypes.c_int, 
        ndpointer(dtype=np.double, flags="C_Contiguous"),ctypes.c_int,ctypes.c_int,ctypes.c_int, ctypes.c_int]
        self._stitchsubimages_float.restype = ctypes.c_int
        self._stitchsubimages_float.argtypes = [ndpointer(dtype=np.float32, flags="C_Contiguous"),ctypes.c_int, ctypes.c_int, 
        ndpointer(dtype=np.float32, flags="C_Contiguous"),ctypes.c_int,ctypes.c_int,ctypes.c_int, ctypes.c_int]

        #int formSubImage22(double* image, int image_rows, int image_cols, double* sub_images, int sub_rows, int sub_cols, int overlap, int num_sub);
        self._formSubImage22_double.restype = ctypes.c_int
        self._formSubImage22_double.argtypes = [ndpointer(dtype=np.double, flags="C_Contiguous"),ctypes.c_int, ctypes.c_int, 
        ndpointer(dtype=np.double, ndim=3, flags="C_Contiguous"),ctypes.c_int,ctypes.c_int,ctypes.c_int, ctypes.c_int]
        self._formSubImage22_float.restype = ctypes.c_int
        self._formSubImage22_float.argtypes = [ndpointer(dtype=np.float32, flags="C_Contiguous"),ctypes.c_int, ctypes.c_int, 
        ndpointer(dtype=np.float32, flags="C_Contiguous"),ctypes.c_int,ctypes.c_int,ctypes.c_int, ctypes.c_int]

    # int formugrid(double *s0, double *s1, double *s2, double *out_data, int width, int height, int modulation);
    def formugrid(self,s0:np.double,s1:np.double,s2:np.double,data:np.double,modulation:int)->int:
        h,w=data.shape
        return self._formugrid_double(np.ascontiguousarray(s0, dtype=np.double),np.ascontiguousarray(s1, dtype=np.double),
        np.ascontiguousarray(s2, dtype=np.double),np.ascontiguousarray(data, dtype=np.double),w,h,modulation)
    def formugrid(self,s0:np.float32,s1:np.float32,s2:np.float32,data:np.float32,modulation:int)->int:
        h,w=data.shape
        return self._formugrid_float(np.ascontiguousarray(s0, dtype=np.float32),np.ascontiguousarray(s1, dtype=np.float32),
        np.ascontiguousarray(s2, dtype=np.float32),np.ascontiguousarray(data, dtype=np.float32),w,h,modulation)

    # void computeintensity(double *s0,double *s1, double *s2, double *out_data, int width, int height, double angle);
    def computeintensity(self,s0:np.double,s1:np.double,s2:np.double,data:np.double,angle:Double)->int:
        h,w=s0.shape
        self._computeintensity_double(np.ascontiguousarray(s0, dtype=np.double),np.ascontiguousarray(s1, dtype=np.double),
        np.ascontiguousarray(s2, dtype=np.double),np.ascontiguousarray(data, dtype=np.double),w,h,angle)
    def computeintensity(self,s0:np.float32,s1:np.float32,s2:np.float32,data:np.float32,angle:Double)->int:
        h,w=s0.shape
        self._computeintensity_float(np.ascontiguousarray(s0, dtype=np.float32),np.ascontiguousarray(s1, dtype=np.float32),
        np.ascontiguousarray(s2, dtype=np.float32),np.ascontiguousarray(data, dtype=np.float32),w,h,angle)

    # int getNumberSubImages22(int image_y, int image_x, int sub_y, int sub_x, int overlap);
    def getNumberSubImages22(self,image_y:int,image_x:int,sub_y:int,sub_x:int,overlap:int)->int:
        return self._getNumberSubImages22(image_y,image_x,sub_y,sub_x,overlap)

    #int stitchsubimages(double* image, int image_rows, int image_cols, double* sub_images, int sub_rows, int sub_cols, int overlap, int num_sub);
    def stitchsubimages(self,image:np.double,sub_image:np.double,overlap:int)->int:
        iy,ix=image.shape
        sy,sx,num=sub_image.shape
        return self._stitchsubimages_double(np.ascontiguousarray(image, dtype=np.double),iy,ix,
        np.ascontiguousarray(sub_image, dtype=np.double),sy,sx,overlap,num)
    def stitchsubimages(self,image:np.float32,sub_image:np.float32,overlap:int)->int:
        iy,ix=image.shape
        sy,sx,num=sub_image.shape
        return self._stitchsubimages_float(np.ascontiguousarray(image, dtype=np.float32),iy,ix,
        np.ascontiguousarray(sub_image, dtype=np.float32),sy,sx,overlap,num)

    #int formSubImage22(double* image, int image_rows, int image_cols, double* sub_images, int sub_rows, int sub_cols, int overlap, int num_sub);
    def formSubImage22(self,image:np.double,sub_image:np.double,overlap:int)->int:
        iy,ix=image.shape
        sy,sx,num=sub_image.shape
        return self._formSubImage22_double(np.ascontiguousarray(image, dtype=np.double),iy,ix,
        np.ascontiguousarray(sub_image, dtype=np.double),sy,sx,overlap,num)
    def formSubImage22(self,image:np.float32,sub_image:np.float32,overlap:int)->int:
        iy,ix=image.shape
        sy,sx,num=sub_image.shape
        return self._formSubImage22_float(np.ascontiguousarray(image, dtype=np.float32),iy,ix,
        np.ascontiguousarray(sub_image, dtype=np.float32),sy,sx,overlap,num)
        