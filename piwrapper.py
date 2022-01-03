from ctypes import cdll
import ctypes
from numpy.ctypeslib import ndpointer

class PIWrapper:
    def __init__(self,lib_path='./libpiwrapper.so'):
        self.lib = cdll.LoadLibrary(lib_path)
        self.openpiwriter = self.lib.openpiwriter
        self.writepi = self.lib.writepi
        self.closewriter = self.lib.closewriter
        self.openpiread = self.lib.openpiread
        self.getwidth = self.lib.getwidth
        self.getheight = self.lib.getheight
        self.readpi = self.lib.readpi
        self.closereader = self.lib.closereader
        print(self.lib)
        
        # bool openpiwriter(const char *filename, int width, int height, int type, bool overwrite);
        self.openpiwriter.restype = ctypes.c_int
        self.openpiwriter.argtypes = [ctypes.c_char_p,ctypes.c_int,ctypes.c_int,ctypes.c_int,ctypes.c_bool]
        
        # int writepi(double* data, int num_frames);
        self.writepi.restype = ctypes.c_int
        self.writepi.argtypes = [ndpointer(ctypes.c_double, flags="C_Contiguous"),ctypes.c_int]
        
        # int closewriter();
        self.closewriter.restype = ctypes.c_int
        self.closewriter.argtypes = None
        
        # bool openpiread(const char *filename);
        self.openpiread.restype = ctypes.c_bool
        self.openpiread.argtypes = [ctypes.c_char_p]
        
        # int getwidth();
        self.getwidth.restype = ctypes.c_int
        self.getwidth.argtypes = None
        
        # int getheight();
        self.getheight.restype = ctypes.c_int
        self.getheight.argtypes = None
        
        # int int readpi(double *out, int width, int height, int index);
        self.readpi.restype = ctypes.c_int
        self.readpi.argtypes = [ndpointer(ctypes.c_double, flags="C_Contiguous"),ctypes.c_int,ctypes.c_int,ctypes.c_int]
        
        # int closereader();
        self.closereader.restype = ctypes.c_int
        self.closereader.argtypes = None