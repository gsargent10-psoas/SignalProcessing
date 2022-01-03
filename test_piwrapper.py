import numpy as np
from ctypes import cdll
import ctypes
from numpy.ctypeslib import ndpointer

from piwrapper import PIWrapper

pw = PIWrapper()

a = np.empty((0))
for i in range(10*10):
	a = np.append(a,i)
filename = './test.pi'
filename = filename.encode('utf-8')
ret = pw.openpiwriter(ctypes.c_char_p(filename),10,10,4,True)
print('openpiwriter() return:')
print(ret)
ret = pw.writepi(np.double(a),1)
ret = pw.writepi(np.double(a),1)
ret = pw.writepi(np.double(a),1)
print('writepi() return:')
print(ret)
ret = pw.closewriter()
print('closewriter() return:')
print(ret)

#filename = './Ground_truth_Stokes_005.pi'
#filename = filename.encode('utf-8')
ret = pw.openpiread(ctypes.c_char_p(filename))
print('openpiread() return:')
print(ret)
print('getwidth() return:')
print(pw.getwidth())
print('getheight() return:')
print(pw.getheight())
ret = pw.closereader()
print('closereader() return:')
print(ret)
