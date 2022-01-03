import numpy as np
from ctypes import cdll
import ctypes
from numpy.ctypeslib import ndpointer

from piwrapper import PIWrapper

pw = PIWrapper()

a = np.empty([10,10])
num = 0
for x in range(10):
    for y in range(10):
        a[x,y] = num
        num = num + 1
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
print('openpiread() return:',ret)
print('getwidth() return:',pw.getwidth())
print('getheight() return:',pw.getheight())
b = np.empty([pw.getheight(),pw.getwidth()])
c = np.empty([pw.getheight(),pw.getwidth()])
d = np.empty([pw.getheight(),pw.getwidth()])
h,w = b.shape
ret = pw.readpi(b,w,h,0);
print('readpi() return:',ret)
ret = pw.readpi(c,w,h,1);
print('readpi() return:',ret)
ret = pw.readpi(d,w,h,2);
print('readpi() return:',ret)
e=np.stack((b,c,d))
ret = pw.closereader()
print('closereader() return:',ret)
print(e.shape)
print('max',e.max(),'min',e.min())
