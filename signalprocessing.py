from ctypes import cdll
import ctypes
from numpy.ctypeslib import ndpointer

class SignalProcessing:
    def __init__(self,lib_path='./libsignalprocessing.so'):
        self.lib = cdll.LoadLibrary(lib_path)
        self.imgscale = self.lib.imgscale
        self.imgscale.restype = None
        self.imgscale.argtypes = [ndpointer(ctypes.c_double, flags="C_Contiguous"),ctypes.c_int,ctypes.c_double,ctypes.c_int]
        
        # void imgscale(double* img, int size, double param1, double param2, int type);