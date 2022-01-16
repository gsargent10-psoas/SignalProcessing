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
    filename = '../../Data/Ground_truth_Stokes_020.pi'
    filename = filename.encode('utf-8')
    pw = PIWrapper(lib_path='./libpiwrapper.so')
    sp = SignalProcessing(lib_path='./libsignalprocessing.so')
    p = Polarimetric(lib_path='./libpolarimetric.so')
    ret = pw.openpiread(ctypes.c_char_p(filename))
    print('openpiread() return:',ret)
    print('getwidth() return:',pw.getwidth())
    print('getheight() return:',pw.getheight())
    s0 = np.empty([pw.getheight(),pw.getwidth()],np.double)
    s1 = np.empty([pw.getheight(),pw.getwidth()],np.double)
    s2 = np.empty([pw.getheight(),pw.getwidth()],np.double)
    ugrid = np.empty([pw.getheight(),pw.getwidth()],np.double)
    I0 = np.empty([pw.getheight(),pw.getwidth()],np.double)
    I30 = np.empty([pw.getheight(),pw.getwidth()],np.double)
    I60 = np.empty([pw.getheight(),pw.getwidth()],np.double)
    I90 = np.empty([pw.getheight(),pw.getwidth()],np.double)
    I120 = np.empty([pw.getheight(),pw.getwidth()],np.double)
    I150 = np.empty([pw.getheight(),pw.getwidth()],np.double)
    h,w = s0.shape
    ret = pw.readpi(s0,w,h,0);
    print('readpi() return:',ret)
    ret = pw.readpi(s1,w,h,1);
    print('readpi() return:',ret)
    ret = pw.readpi(s2,w,h,2);
    print('readpi() return:',ret)
    ret = pw.closereader()
    print('closereader() return:',ret)
    ret = p.formugrid(s0,s1,s2,ugrid,w,h,2)
    print('ugrid min',ugrid.min(),'ugrid max',ugrid.max())
    print('formugrid() return:',ret)
    p.computeintensity(s0,s1,s2,I0,w,h,0.0)
    p.computeintensity(s0,s1,s2,I30,w,h,30.0)
    p.computeintensity(s0,s1,s2,I60,w,h,60.0)
    p.computeintensity(s0,s1,s2,I90,w,h,90.0)
    p.computeintensity(s0,s1,s2,I120,w,h,120.0)
    p.computeintensity(s0,s1,s2,I150,w,h,150.0)

    filename = './intesity.pi'
    filename = filename.encode('utf-8')
    ret = pw.openpiwriter(ctypes.c_char_p(filename),w,h,3,True)
    print('openpiwriter() return:',ret)
    ret = pw.writepi(np.double(I0),1)
    ret = pw.writepi(np.double(I30),1)
    ret = pw.writepi(np.double(I60),1)
    ret = pw.writepi(np.double(I90),1)
    ret = pw.writepi(np.double(I120),1)
    ret = pw.writepi(np.double(I150),1)
    print('writepi() return:',ret)
    ret = pw.closewriter()
    print('closewriter() return:',ret)

    filename = './ugrid.pi'
    filename = filename.encode('utf-8')
    ret = pw.openpiwriter(ctypes.c_char_p(filename),w,h,100,True)
    print('openpiwriter() return:',ret)
    ret = pw.writepi(np.double(ugrid),1)
    print('writepi() return:',ret)
    ret = pw.closewriter()
    print('closewriter() return:',ret)

    s0 = (1/3)*(I0 + I30 + I60 + I90+ I120 + I150)
    s1 = (1/3)*((I0 - I90) + 2*(I30 - I60) + 2*(I150 - I120))
    s2 = (np.sqrt(3.0)/3.0)*(I30 + I60 - I120 - I150)

    filename = './stokes.pi'
    filename = filename.encode('utf-8')
    ret = pw.openpiwriter(ctypes.c_char_p(filename),w,h,4,True)
    print('openpiwriter() return:',ret)
    ret = pw.writepi(np.double(s0),1)
    ret = pw.writepi(np.double(s1),1)
    ret = pw.writepi(np.double(s2),1)
    print('writepi() return:',ret)
    ret = pw.closewriter()
    print('closewriter() return:',ret)
    
    # print('max s0',s0.max(),'min s0',s0.min())
    # print('max ugrid',ugrid.max(),'min ugrid',ugrid.min())
    # print('max I45',I45.max(),'min I45',I45.min())
    # sp.imgscale(np.double(s0),h*w,0,0,0)
    # sp.imgscale(np.double(ugrid),h*w,0,0,0)
    # sp.imgscale(np.double(I45),h*w,0,0,0)
    # print('max s0 scaled',s0.max(),'min s0 scaled',s0.min())
    # print('max ugrid scaled',ugrid.max(),'min ugrid scaled',ugrid.min())
    # print('max I45 scaled',I45.max(),'min I45 scaled',I45.min())
    # s0_fig = plt.figure()
    # plt.imshow(s0,cmap='gray')
    # plt.title("s0")
    # s0_fig.canvas.flush_events()
    # ugrid_fig = plt.figure()
    # plt.imshow(ugrid,cmap='gray')
    # plt.title("ugrid")
    # ugrid_fig.canvas.flush_events()
    # I45_fig = plt.figure()
    # plt.imshow(I45,cmap='gray')
    # plt.title("I45")
    # I45_fig.canvas.flush_events()

    # a = np.zeros([2,2])
    # b = np.ones([2,2])
    # e = np.ones([2,2])*2
    # c = np.stack((a,b,e),-1)
    # print(c[:,:,0])
    # print(c[:,:,1])
    # print(c[:,:,2])
    # print(c.shape)


    input("press any key to end script")










