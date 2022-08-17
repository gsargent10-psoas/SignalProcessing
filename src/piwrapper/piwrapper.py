from ctypes import cdll
import ctypes
from numpy.ctypeslib import ndpointer
import numpy as np

class PIWrapper:
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
    PI_PRODUCT_TYPE = {'UNKNOWN_PRODUCT_TYPE':-1,'INTENSITY':100,'SPATIALLY_MODULATED_INTENSITY':101,
    'S0':0,'S1':1,'S2':2,'LINEAR_STOKES':5,'LINEAR_STOKES_DOLP_AOP':6,'DOLP':10,
    'AOP':11,'PCM':12,'TYOCOLOR':20,'FLATFIELD':200,'FLATFIELD_AVERAGE':201,'BAD_PIXEL_MAP':301}
    '''
    enum ENVI_DATA_TYPE{
        UNKNOWN_DATA_TYPE = -1,
        ENVI_CHAR = 1,
        ENVI_SIGNED_SHORT = 2,
        ENVI_INTEGER = 3,
        ENVI_FLOAT = 4,
        ENVI_DOUBLE = 5,
        ENVI_UNSIGNED_SHORT = 12,
        ENVI_UNSIGNED_INT = 13,
        ENVI_I64 = 14,
        ENVI_UNSIGNED_I64 = 15
    };
    '''
    ENVI_DATA_TYPE = {'UNKNOWN_DATA_TYPE':-1,'ENVI_CHAR':1,'ENVI_SIGNED_SHORT':2,
    'ENVI_INTEGER':3,'ENVI_FLOAT':4,'ENVI_DOUBLE':5,'LINEAR_STOKES':5,'ENVI_UNSIGNED_SHORT':12,'ENVI_UNSIGNED_INT':13,
    'ENVI_I64':14,'ENVI_UNSIGNED_I64':15}
    '''
    enum PI_SENSOR_TYPE{
        UNKNOWN_SENSOR_TYPE = -1,
        DIVISION_OF_FOCAL_PLANE = 0,
        DIVISION_OF_TIME = 1,
        DIVISION_OF_AMPLITUDE = 2,
        DIVISION_OF_APERTURE = 3,
        NON_POLARIMETRIC_SENSOR = 10,
    };
    '''
    PI_SENSOR_TYPE = {'UNKNOWN_SENSOR_TYPE':-1,'DIVISION_OF_FOCAL_PLANE':0,'DIVISION_OF_TIME':1,
    'DIVISION_OF_AMPLITUDE':2,'DIVISION_OF_APERTURE':3,'NON_POLARIMETRIC_SENSOR':10}
    '''
    enum PI_SENSOR_NAME {
        UNKNOWN_SENSOR_NAME = -1,
        PIRATE_I = 0,
        PIRATE_II = 1,
        TYO_LWIRuG = 10,
        PYXIS_LWIR_MONO_UGRID = 20,
        CORVIS_LWIR_MONO_DOT = 21,
        BLACKFLY_VISIBLE_MONO_UGRID = 31,
        BLACKFLY_VISIBLE_MONO_DOT = 32,
        BLACKFLY_VISIBLE_RGB_UGRID = 33,
    };
    '''
    PI_SENSOR_NAME = {'UNKNOWN_SENSOR_NAME':-1,'PIRATE_I':0,'PIRATE_II':1,
    'TYO_LWIRuG':10,'PYXIS_LWIR_MONO_UGRID':20,'CORVIS_LWIR_MONO_DOT':21,'BLACKFLY_VISIBLE_MONO_UGRID':31,
    'BLACKFLY_VISIBLE_MONO_DOT':32,'BLACKFLY_VISIBLE_RGB_UGRID':33}
    '''
    enum PI_FILE_FORMAT{
        UNKNOWN_FILE_TYPE = -1,
        PIFILE = 0,
        AFRL_PIRATE = 1,
        POLARIS_PYXIS = 10,
    };
    '''
    PI_FILE_FORMAT = {'UNKNOWN_FILE_TYPE':-1,'PIFILE':0,'AFRL_PIRATE':1,
    'POLARIS_PYXIS':10}
    '''
    enum RADIOMETRIC_UNIT{
        UNKNOWN_RADIOMETRIC_UNIT = -1,
        COUNTS = 0,
        KELVIN = 1,
        CELCIUS = 2,
        FAHRENHEIT = 3,
        PHOTONS = 4,
        MICROFLICKS = 5,
        RADIANCE = 6,
    };
    '''
    RADIOMETRIC_UNIT = {'UNKNOWN_RADIOMETRIC_UNIT':-1,'COUNTS':0,'KELVIN':1,
    'CELCIUS':2,'FAHRENHEIT':3,'PHOTONS':4,'MICROFLICKS':5,
    'RADIANCE':6}
    '''
    enum WAVELENGTH_UNIT{
        UNKNOWN_WAVELENGTH_UNIT = -1,
        METERS = 0,
        CENTIMETERS = 1,
        MILLIMETERS = 2,
        MICROMETERS = 3,
        NANOMETERS = 4,
        MICRONS = 5,
    };
    '''
    WAVELENGTH_UNIT = {'UNKNOWN_WAVELENGTH_UNIT':-1,'METERS':0,'CENTIMETERS':1,
    'MILLIMETERS':2,'MICROMETERS':3,'NANOMETERS':4,'MICRONS':5}
    '''
    enum SCALING_TYPE{
        ABS = 0,
        STATISTICAL = 1,
    };
    '''
    SCALING_TYPE = {'ABS':0,'STATISTICAL':1}
    '''
    enum DEMOSAIC_TYPE{
        DEMOSAICING_NOT_APPLICABLE = -1, //Means sensor does not require demosaicing, such as a DoT sensor
        NLPN = 0, //Nearest Like-Polarization Neighbor
        OLF = 1, //Optimal Linear Filtering
        LABF = 2, //Locally-adaptive Bilateral-Filter
    };
    '''
    DEMOSAIC_TYPE = {'DEMOSAICING_NOT_APPLICABLE':-1,'NLPN':0,'OLF':1,'LABF':2}

    def __init__(self,lib_path='./libpiwrapper.so'):
        # Initialize library and grab all the functions. Initialize their argument list.
        self.lib = cdll.LoadLibrary(lib_path)
        self._openpiwriterdouble = self.lib.openpiwriterdouble
        self._openpiwriterfloat = self.lib.openpiwriterfloat
        self._writepidouble = self.lib.writepidouble
        self._writepifloat = self.lib.writepifloat
        self._closewriter = self.lib.closewriter
        self._openpiread = self.lib.openpiread
        self._getwidth = self.lib.getwidth
        self._getheight = self.lib.getheight
        self._readpidouble = self.lib.readpidouble
        self._readpifloat = self.lib.readpifloat
        self._closereader = self.lib.closereader
        #print(self.lib)
        
        # bool openpiwriterdouble(const char *filename, int width, int height, int _PI_PRODUCT_TYPE, bool overwrite);
        self._openpiwriterdouble.restype = ctypes.c_int
        self._openpiwriterdouble.argtypes = [ctypes.c_char_p,ctypes.c_int,ctypes.c_int,ctypes.c_int,ctypes.c_bool]

        # bool openpiwriterfloat(const char *filename, int width, int height, int _PI_PRODUCT_TYPE, bool overwrite);
        self._openpiwriterfloat.restype = ctypes.c_int
        self._openpiwriterfloat.argtypes = [ctypes.c_char_p,ctypes.c_int,ctypes.c_int,ctypes.c_int,ctypes.c_bool]
        
        # int writepi(double* data, int num_frames); 
        self._writepidouble.restype = ctypes.c_int
        self._writepidouble.argtypes = [ndpointer(ctypes.c_double, flags="C_Contiguous"),ctypes.c_int]

        # int writepi(float* data, int num_frames);
        self._writepifloat.restype = ctypes.c_int
        self._writepifloat.argtypes = [ndpointer(ctypes.c_float, flags="C_Contiguous"),ctypes.c_int]
        
        # int closewriter();
        self._closewriter.restype = ctypes.c_int
        self._closewriter.argtypes = None
        
        # bool openpiread(const char *filename);
        self._openpiread.restype = ctypes.c_bool
        self._openpiread.argtypes = [ctypes.c_char_p]
        
        # int getwidth();
        self._getwidth.restype = ctypes.c_int
        self._getwidth.argtypes = None
        
        # int getheight();
        self._getheight.restype = ctypes.c_int
        self._getheight.argtypes = None
        
        # int readpidouble(double *out, int width, int height, int index);
        self._readpidouble.restype = ctypes.c_int
        self._readpidouble.argtypes = [ndpointer(ctypes.c_double, flags="C_Contiguous"),ctypes.c_int,ctypes.c_int,ctypes.c_int]

        # int int readpifloat(double *out, int width, int height, int index);
        self._readpifloat.restype = ctypes.c_int
        self._readpifloat.argtypes = [ndpointer(ctypes.c_float, flags="C_Contiguous"),ctypes.c_int,ctypes.c_int,ctypes.c_int]
        
        # int closereader();
        self._closereader.restype = ctypes.c_int
        self._closereader.argtypes = None

    # int openpiwriterdouble(const char *filename, int width, int height, int _PI_PRODUCT_TYPE, bool overwrite);
    def openpiwriterdouble(self,filename:str,height:int,width:int,_PI_PRODUCT_TYPE:int,overwrite:bool)->bool:
        filename = filename.encode('utf-8')
        ret = self._openpiwriterdouble(ctypes.c_char_p(filename),width,height,_PI_PRODUCT_TYPE,overwrite)
        if ret == 0:
            self._writepidouble.restype = ctypes.c_int
            self._writepidouble.argtypes = [ndpointer(dtype=np.double,shape=(height,width), flags="C_Contiguous"),ctypes.c_int]
        return ret
        
    # int openpiwriterfloat(const char *filename, int width, int height, int _PI_PRODUCT_TYPE, bool overwrite);
    def openpiwriterfloat(self,filename:str,height:int,width:int,_PI_PRODUCT_TYPE:int,overwrite:bool)->bool:
        filename = filename.encode('utf-8')
        ret = self._openpiwriterfloat(ctypes.c_char_p(filename),width,height,_PI_PRODUCT_TYPE,overwrite)
        if ret == 0:
            self._writepifloat.restype = ctypes.c_int
            self._writepifloat.argtypes = [ndpointer(dtype=np.float32,shape=(height,width), flags="C_Contiguous"),ctypes.c_int]
        return ret

    # int writepi(double* data, int num_frames); 
    def writepidouble(self,data:np.double,frames:int)->int:
        if frames > 1:
            for f in range(frames):
                ret = self._writepidouble(np.ascontiguousarray(data[:,:,f], dtype=np.double),1)
                if ret != 0:
                    return ret
        else:
            return self._writepidouble(np.ascontiguousarray(data, dtype=np.double),1)

    # int writepi(float* data, int num_frames); 
    def writepifloat(self,data:np.float32,frames:int)->int:
        if frames > 1:
            for f in range(frames):
                ret = self._writepifloat(np.ascontiguousarray(data[:,:,f], dtype=np.float32),1)
                if ret != 0:
                    return ret
        else:
            return self._writepifloat(np.ascontiguousarray(data, dtype=np.float32),1)

    # int closewriter();
    def closewriter(self,)->int:
        return self._closewriter()

    # bool openpiread(const char *filename);
    def openpiread(self,filename:str)->bool:
        filename = filename.encode('utf-8')
        ret = self._openpiread(ctypes.c_char_p(filename))
        if ret == True:
            h=self._getheight()
            w=self._getwidth()
            self._readpidouble.restype = ctypes.c_int
            self._readpidouble.argtypes = [ndpointer(dtype=np.double,shape=(h,w),flags="C_Contiguous"),ctypes.c_int,ctypes.c_int,ctypes.c_int]
            self._readpifloat.restype = ctypes.c_int
            self._readpifloat.argtypes = [ndpointer(dtype=np.float32,shape=(h,w),flags="C_Contiguous"),ctypes.c_int,ctypes.c_int,ctypes.c_int]
        return ret

    # int getwidth();
    def getwidth(self)->int:
        return self._getwidth()

    # int getheight();
    def getheight(self)->int:
        return self._getheight()

    # int readpidouble(double *out, int width, int height, int index);
    def readpidouble(self,data:np.double,index:int)->int:
        h,w = data.shape
        return self._readpidouble(np.ascontiguousarray(data, dtype=np.double),w,h,index)

    # int readpifloat(float *out, int width, int height, int index);
    def readpifloat(self,data:np.float32,index:int)->int:
        h,w = data.shape
        return self._readpifloat(np.ascontiguousarray(data, dtype=np.float32),w,h,index)

    # int closereader();
    def closereader(self)->int:
        return self._closereader()