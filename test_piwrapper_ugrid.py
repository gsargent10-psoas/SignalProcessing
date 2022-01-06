import os
import numpy as np
import sys
import glob
import matplotlib.pyplot as plt
import matplotlib
import math as m
#sys.path.insert(1, '../SignalProcessing')
from piwrapper import PIWrapper
from signalprocessing import SignalProcessing
from polarimetric import Polarimetric
import ctypes

def imgscale(img,stype="minmax",param1=0,param2=1) :
    np.seterr(divide='ignore',invalid='ignore')
    [M,N] = img.shape
    img = img.astype(np.float64)

    if(stype=="minmax") :
        alpha_l = img[:].min()
        alpha_h = img[:].max()
    elif(stype=="statistical") :
        mu = img[:].mean()
        sd = img[:].std()
        alpha_l = mu - param1*sd
        alpha_h = mu + param2*sd
    elif(stype=="absolute") :
        alpha_l = param1
        alpha_h = param2

    # Saturate all values of in < alpha_l = alpha_l
    img[img[:]<alpha_l] = alpha_l

    # Saturate all values of in > alpha_h = alpha_h
    img[img[:]>alpha_h] = alpha_h

    # Linearly scale data in the range of [alpha_l,alpha_h] --> [0,1]
    out = np.nan_to_num((img[:] - alpha_l)/(alpha_h - alpha_l))
    out = np.reshape(out,(M,N))

    return out

def imread(path,img_name) :
    return np.array(list(sio.loadmat(path,verify_compressed_data_integrity=False).values())[3]).astype(np.float64)
    #return np.array(sio.loadmat(path,variable_names=img_name,verify_compressed_data_integrity=False).get(img_name)).astype(np.double)

def getUgridImage(path,img_name,var_name) :
    print(path,img_name)
    img = imread(path+img_name,var_name)
    return img 

def getStokesImage(path,img_name,var_name) :
    print(path,img_name)
    img = imread(path+img_name,var_name)
    [M,N,Z] = img.shape
    img_out = img
    return img_out 


if __name__ == '__main__' :
    plt.ion()
    filename = '../../Data/Ground_truth_Stokes_001.pi'
    filename = filename.encode('utf-8')
    pw = PIWrapper(lib_path='./libpiwrapper.so')
    sp = SignalProcessing(lib_path='./libsignalprocessing.so')
    p = Polarimetric(lib_path='./libpolarimetric.so')
    ret = pw.openpiread(ctypes.c_char_p(filename))
    print('openpiread() return:',ret)
    print('getwidth() return:',pw.getwidth())
    print('getheight() return:',pw.getheight())
    s0 = np.empty([pw.getheight(),pw.getwidth()])
    s1 = np.empty([pw.getheight(),pw.getwidth()])
    s2 = np.empty([pw.getheight(),pw.getwidth()])
    ugrid = np.empty([pw.getheight(),pw.getwidth()])
    I45 = np.empty([pw.getheight(),pw.getwidth()])
    h,w = s0.shape
    ret = pw.readpi(s0,w,h,0);
    print('readpi() return:',ret)
    ret = pw.readpi(s1,w,h,1);
    print('readpi() return:',ret)
    ret = pw.readpi(s2,w,h,2);
    print('readpi() return:',ret)
    ret = pw.closereader()
    print('closereader() return:',ret)
    ret = p.formugrid(s0,s1,s2,ugrid,w,h,3)
    print('formugrid() return:',ret)
    p.computeintensity(s0,s1,s2,I45,w,h,45.0)
    
    print('max s0',s0.max(),'min s0',s0.min())
    print('max ugrid',ugrid.max(),'min ugrid',ugrid.min())
    print('max I45',I45.max(),'min I45',I45.min())
    sp.imgscale(np.double(s0),h*w,0,0,0)
    sp.imgscale(np.double(ugrid),h*w,0,0,0)
    sp.imgscale(np.double(I45),h*w,0,0,0)
    print('max s0 scaled',s0.max(),'min s0 scaled',s0.min())
    print('max ugrid scaled',ugrid.max(),'min ugrid scaled',ugrid.min())
    print('max I45 scaled',I45.max(),'min I45 scaled',I45.min())
    s0_fig = plt.figure()
    plt.imshow(s0,cmap='gray')
    plt.title("s0")
    s0_fig.canvas.flush_events()
    ugrid_fig = plt.figure()
    plt.imshow(ugrid,cmap='gray')
    plt.title("ugrid")
    ugrid_fig.canvas.flush_events()
    I45_fig = plt.figure()
    plt.imshow(I45,cmap='gray')
    plt.title("I45")
    I45_fig.canvas.flush_events()

    a = np.zeros([2,2])
    b = np.ones([2,2])
    e = np.ones([2,2])*2
    c = np.stack((a,b,e),-1)
    print(c[:,:,0])
    print(c[:,:,1])
    print(c[:,:,2])
    print(c.shape)


    input("press any key to end script")










