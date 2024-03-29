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
filenamedouble = "./testdouble.pi"
filenamefloat = './testfloat.pi'
ret = pw.openpiwriterdouble(filenamedouble,10,10,pw.PI_PRODUCT_TYPE['INTENSITY'],True)
print('openpiwriter() return:',ret)
ret = pw.writepidouble(np.double(a),1)
ret = pw.writepidouble(np.double(a),1)
ret = pw.writepidouble(np.double(a),1)
print('writepi() return:',ret)
ret = pw.closewriter()
print('closewriter() return:',ret)
ret = pw.openpiwriterfloat(filenamefloat,10,10,pw.PI_PRODUCT_TYPE['LINEAR_STOKES'],True)
print('openpiwriter() return:',ret)
ret = pw.writepifloat(np.float32(bb),1)
ret = pw.writepifloat(np.float32(bb),1)
ret = pw.writepifloat(np.float32(bb),1)
print('writepi() return:',ret)
ret = pw.closewriter()
print('closewriter() return:',ret)

ret = pw.openpiread(filenamefloat)
print('openpiread() return:',ret)
print('getwidth() return:',pw.getwidth())
print('getheight() return:',pw.getheight())
l = np.empty([pw.getheight(),pw.getwidth()],np.float32)
m = np.empty([pw.getheight(),pw.getwidth()],np.float32)
n = np.empty([pw.getheight(),pw.getwidth()],np.float32)
h,w = l.shape
ret = pw.readpifloat(l,0)
print('readpi() return:',ret)
ret = pw.readpifloat(m,1)
print('readpi() return:',ret)
ret = pw.readpifloat(n,2)
print('readpi() return:',ret)
e=np.stack((l,m,n))
ret = pw.closereader()
print('closereader() return:',ret)
print(e.shape)
print('max',e.max(),'min',e.min())


#filename = './Ground_truth_Stokes_005.pi'
#filename = filename.encode('utf-8')
ret = pw.openpiread(filenamedouble)
print('openpiread() return:',ret)
print('getwidth() return:',pw.getwidth())
print('getheight() return:',pw.getheight())
b = np.empty([pw.getheight(),pw.getwidth()])
c = np.empty([pw.getheight(),pw.getwidth()])
d = np.empty([pw.getheight(),pw.getwidth()])
h,w = b.shape
ret = pw.readpidouble(b,0)
print('readpi() return:',ret)
ret = pw.readpidouble(c,1)
print('readpi() return:',ret)
ret = pw.readpidouble(d,2)
print('readpi() return:',ret)
e=np.stack((b,c,d))
ret = pw.closereader()
print('closereader() return:',ret)
print(e.shape)
print('max',e.max(),'min',e.min())
print()
print()

"""
filenamerealdata = './Ground_truth_Stokes_002.pi'
ret = pw.openpiread(ctypes.c_char_p(filenamerealdata))
print('openpiread() return:',ret)
print('getwidth() return:',pw.getwidth())
print('getheight() return:',pw.getheight())
s0 = np.empty([pw.getheight(),pw.getwidth()],np.double)
s1 = np.empty([pw.getheight(),pw.getwidth()],np.double)
s2 = np.empty([pw.getheight(),pw.getwidth()],np.double)
print("h is ", h, "w is " , w)
h,w = s0.shape
ret = pw.readpidouble(s0,w,h,0)
print('readpi() return:',ret)
ret = pw.readpidouble(s1,w,h,1)
print('readpi() return:',ret)
ret = pw.readpidouble(s2,w,h,2)
print('readpi() return:',ret)
e=np.stack((s0,s1,s2))
ret = pw.closereader()
print('closereader() return:',ret)
print(e.shape)
print('max',e.max(),'min',e.min())
"""

