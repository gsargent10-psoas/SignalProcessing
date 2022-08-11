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
        self.ENVI_DATA_TYPE = {'UNKNOWN_DATA_TYPE':-1,'ENVI_CHAR':1,'ENVI_SIGNED_SHORT':2,
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
        self.PI_SENSOR_TYPE = {'UNKNOWN_SENSOR_TYPE':-1,'DIVISION_OF_FOCAL_PLANE':0,'DIVISION_OF_TIME':1,
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
        self.PI_SENSOR_NAME = {'UNKNOWN_SENSOR_NAME':-1,'PIRATE_I':0,'PIRATE_II':1,
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
        self.PI_FILE_FORMAT = {'UNKNOWN_FILE_TYPE':-1,'PIFILE':0,'AFRL_PIRATE':1,
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
        self.RADIOMETRIC_UNIT = {'UNKNOWN_RADIOMETRIC_UNIT':-1,'COUNTS':0,'KELVIN':1,
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
        self.WAVELENGTH_UNIT = {'UNKNOWN_WAVELENGTH_UNIT':-1,'METERS':0,'CENTIMETERS':1,
        'MILLIMETERS':2,'MICROMETERS':3,'NANOMETERS':4,'MICRONS':5}
        '''
        enum SCALING_TYPE{
            ABS = 0,
            STATISTICAL = 1,
        };
        '''
        self.SCALING_TYPE = {'ABS':0,'STATISTICAL':1}
        '''
        enum DEMOSAIC_TYPE{
            DEMOSAICING_NOT_APPLICABLE = -1, //Means sensor does not require demosaicing, such as a DoT sensor
            NLPN = 0, //Nearest Like-Polarization Neighbor
            OLF = 1, //Optimal Linear Filtering
            LABF = 2, //Locally-adaptive Bilateral-Filter
        };
        '''
        self.DEMOSAIC_TYPE = {'DEMOSAICING_NOT_APPLICABLE':-1,'NLPN':0,'OLF':1,'LABF':2}
        print(self.lib)
        
        # bool openpiwriterdouble(const char *filename, int width, int height, int _PI_PRODUCT_TYPE, bool overwrite);
        self.openpiwriterdouble.restype = ctypes.c_int
        self.openpiwriterdouble.argtypes = [ctypes.c_char_p,ctypes.c_int,ctypes.c_int,ctypes.c_int,ctypes.c_bool]

        # bool openpiwriterfloat(const char *filename, int width, int height, int _PI_PRODUCT_TYPE, bool overwrite);
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
