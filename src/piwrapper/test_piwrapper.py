import numpy as np
from ctypes import cdll
import ctypes
from numpy.ctypeslib import ndpointer

from piwrapper import PIWrapper

pw = PIWrapper()

a = np.empty([10,10])
bb = np.empty([10,10],np.float32)
num = 0
for x in range(10):
    for y in range(10):
        a[x,y] = num
        bb[x,y] = num-1
        num = num + 1
filenamedouble = './testdouble.pi'
filenamedouble = filenamedouble.encode('utf-8')
filenamefloat = './testfloat.pi'
filenamefloat = filenamefloat.encode('utf-8')
ret = pw.openpiwriterdouble(ctypes.c_char_p(filenamedouble),10,10,4,True)
print('openpiwriter() return:',ret)
ret = pw.writepidouble(np.double(a),1)
ret = pw.writepidouble(np.double(a),1)
ret = pw.writepidouble(np.double(a),1)
print('writepi() return:',ret)
ret = pw.closewriter()
print('closewriter() return:',ret)
ret = pw.openpiwriterfloat(ctypes.c_char_p(filenamefloat),10,10,4,True)
print('openpiwriter() return:',ret)
ret = pw.writepifloat(np.float32(bb),1)
ret = pw.writepifloat(np.float32(bb),1)
ret = pw.writepifloat(np.float32(bb),1)
print('writepi() return:',ret)
ret = pw.closewriter()
print('closewriter() return:',ret)

ret = pw.openpiread(ctypes.c_char_p(filenamefloat))
print('openpiread() return:',ret)
print('getwidth() return:',pw.getwidth())
print('getheight() return:',pw.getheight())
l = np.empty([pw.getheight(),pw.getwidth()],np.float32)
m = np.empty([pw.getheight(),pw.getwidth()],np.float32)
n = np.empty([pw.getheight(),pw.getwidth()],np.float32)
h,w = l.shape
ret = pw.readpifloat(l,w,h,0)
print('readpi() return:',ret)
ret = pw.readpifloat(m,w,h,1)
print('readpi() return:',ret)
ret = pw.readpifloat(n,w,h,2)
print('readpi() return:',ret)
e=np.stack((l,m,n))
#ret = pw.closereader()
#print('closereader() return:',ret)
print(e.shape)
print('max',e.max(),'min',e.min())


#filename = './Ground_truth_Stokes_005.pi'
#filename = filename.encode('utf-8')
ret = pw.openpiread(ctypes.c_char_p(filenamedouble))
print('openpiread() return:',ret)
print('getwidth() return:',pw.getwidth())
print('getheight() return:',pw.getheight())
b = np.empty([pw.getheight(),pw.getwidth()])
c = np.empty([pw.getheight(),pw.getwidth()])
d = np.empty([pw.getheight(),pw.getwidth()])
h,w = b.shape
ret = pw.readpidouble(b,w,h,0)
print('readpi() return:',ret)
ret = pw.readpidouble(c,w,h,1)
print('readpi() return:',ret)
ret = pw.readpidouble(d,w,h,2)
print('readpi() return:',ret)
e=np.stack((b,c,d))
#ret = pw.closereader()
#print('closereader() return:',ret)
print(e.shape)
print('max',e.max(),'min',e.min())

