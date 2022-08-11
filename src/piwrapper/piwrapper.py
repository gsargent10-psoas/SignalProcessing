from ctypes import cdll
import ctypes
from numpy.ctypeslib import ndpointer

class PIWrapper:
    def __init__(self,lib_path='./libpiwrapper.so'):
        self.lib = cdll.LoadLibrary(lib_path)
        self.openpiwriterdouble = self.lib.openpiwriterdouble
        self.openpiwriterfloat = self.lib.openpiwriterfloat
        self.writepidouble = self.lib.writepidouble
        self.writepifloat = self.lib.writepifloat
        self.closewriter = self.lib.closewriter
        self.openpiread = self.lib.openpiread
        self.getwidth = self.lib.getwidth
        self.getheight = self.lib.getheight
        self.readpidouble = self.lib.readpidouble
        self.readpifloat = self.lib.readpifloat
        self.closereader = self.lib.closereader
        '''
        enum PI_PRODUCT_TYPE{
            UNKNOWN_PRODUCT_TYPE = -1,
            INTENSITY = 100,
            SPATIALLY_MODULATED_INTENSITY = 101,
            S0 = 0,
            S1 = 1,
            S2 = 2,
            LINEAR_STOKES = 5,
            LINEAR_STOKES_DOLP_AOP = 6,
            DOLP = 10,
            AOP = 11,
            PCM = 12,
            TYOCOLOR = 20,
            FLATFIELD = 200,
            FLATFIELD_AVERAGE = 201,
            BAD_PIXEL_MAP = 301
        };
        '''
        self.PI_PRODUCT_TYPE = {'UNKNOWN_PRODUCT_TYPE':-1,'INTENSITY':100,'SPATIALLY_MODULATED_INTENSITY':101,
        'S0':0,'S1':1,'S2':2,'LINEAR_STOKES':5,'LINEAR_STOKES_DOLP_AOP':6,'DOLP':10,
        'AOP':11,'PCM':12,'TYOCOLOR':20,'FLATFIELD':200,'FLATFIELD_AVERAGE':201,'BAD_PIXEL_MAP':301}
        print(self.lib)
        
        # bool openpiwriterdouble(const char *filename, int width, int height, int type, bool overwrite);
        self.openpiwriterdouble.restype = ctypes.c_int
        self.openpiwriterdouble.argtypes = [ctypes.c_char_p,ctypes.c_int,ctypes.c_int,ctypes.c_int,ctypes.c_bool]

        # bool openpiwriterfloat(const char *filename, int width, int height, int type, bool overwrite);
        self.openpiwriterdouble.restype = ctypes.c_int
        self.openpiwriterdouble.argtypes = [ctypes.c_char_p,ctypes.c_int,ctypes.c_int,ctypes.c_int,ctypes.c_bool]
        
        # int writepi(float* data, int num_frames); 
        self.writepidouble.restype = ctypes.c_int
        self.writepidouble.argtypes = [ndpointer(ctypes.c_double, flags="C_Contiguous"),ctypes.c_int]

        # int writepi(double* data, int num_frames);
        self.writepifloat.restype = ctypes.c_int
        self.writepifloat.argtypes = [ndpointer(ctypes.c_float, flags="C_Contiguous"),ctypes.c_int]
        
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
        
        # int int readpidouble(double *out, int width, int height, int index);
        self.readpidouble.restype = ctypes.c_int
        self.readpidouble.argtypes = [ndpointer(ctypes.c_double, flags="C_Contiguous"),ctypes.c_int,ctypes.c_int,ctypes.c_int]

        # int int readpifloat(double *out, int width, int height, int index);
        self.readpifloat.restype = ctypes.c_int
        self.readpifloat.argtypes = [ndpointer(ctypes.c_float, flags="C_Contiguous"),ctypes.c_int,ctypes.c_int,ctypes.c_int]
        
        # int closereader();
        self.closereader.restype = ctypes.c_int
        self.closereader.argtypes = None
