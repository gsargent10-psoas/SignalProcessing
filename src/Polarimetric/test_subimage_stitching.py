from polarimetric import Polarimetric
import numpy as np

def formugrid22a(image):
    image[::2,::2]=90.0
    image[::2,1::2]=135.0
    image[1::2,::2]=45.0
    image[1::2,1::2]=0.0
    #print("First 10x10 elements of ugrid22a")
    #for r in range(0,10):
        #for c in range(0,10):
            #print(image[r,c],end=",")
        #print()
    #print()

def formugrid22b(image):
    image[::2,::2]=22.5
    image[::2,1::2]=67.5
    image[1::2,::2]=112.5
    image[1::2,1::2]=157.5
    #print("First 10x10 elements of ugrid22b")
    #for r in range(0,10):
        #for c in range(0,10):
            #print(image[r,c],end=",")
        #print()
    #print()

def formugrid32(image):
    image[::2,::3]=0
    image[::2,1::3]=120
    image[::2,2::3]=112
    image[1::2,::3]=60
    image[1::2,1::3]=30
    image[1::2,2::3]=150
    #print("First 10x10 elements of ugrid32")
    #for r in range(0,10):
        #for c in range(0,10):
            #print(image[r,c],end=",")
        #print()
    #print()

def formugrid42(image):
    image[::2,::4]=0
    image[::2,1::4]=45
    image[::2,2::4]=90
    image[::2,3::4]=135
    image[1::2,::4]=90
    image[1::2,1::4]=135
    image[1::2,2::4]=0
    image[1::2,3::4]=45
    #print("First 10x10 elements of ugrid42")
    #for r in range(0,10):
        #for c in range(0,10):
            #print(image[r,c],end=",")
        #print()
    #print()

if __name__ == '__main__':
    # Configuration parameters
    image_y=2048 # 2048
    image_x=2448 # 2448
    dtype=np.float32
    sub_y=48
    sub_x=48
    overlap_factor=0
    plib = Polarimetric()

    # Initialize variables
    image22a = np.zeros([image_y,image_x],dtype=dtype)
    image22b = np.zeros([image_y,image_x],dtype=dtype)
    image32 = np.zeros([image_y,image_x],dtype=dtype)
    image42 = np.zeros([image_y,image_x],dtype=dtype)
    formugrid22a(image22a)
    formugrid22b(image22b)
    formugrid32(image32)
    formugrid42(image42)
    '''
    print("First 10x10 elements of ugrid22 after function call")
    for r in range(0,10):
        for c in range(0,10):
            print(image22[r,c],end=",")
        print()
    print()
    '''
    stitched_image = np.zeros([image_y,image_x],dtype=dtype)
    num_sub = plib.getNumberSubImages(image_y,image_x,sub_y,sub_x,overlap_factor)
    print("Number of subimages:",num_sub)
    sub_images = np.zeros([sub_y,sub_x,num_sub],dtype=dtype)
    error=-99 # no function returns error of -99

    # Test ugrid22
    error = plib.formSubImage(image22a, sub_images,overlap_factor)
    print("formSubImage (ugrid22a) error:",error)

    sub_check = 0
    for i in range(0,num_sub):
        sub_check = sub_check + (0 if image22a[0,0]==sub_images[0,0,i] else 1) # value_if_true if condition else value_if_false
    print("formSubImage (ugrid22a) check:",sub_check)
    print("First 10x10 elements of subimage")
    for r in range(0,10):
        for c in range(0,10):
            print(sub_images[r,c,5],end=",")
        print()

    error=-99
    error = plib.stitchsubimages(stitched_image,sub_images,overlap_factor)
    print("stitchsubimages (ugrid22a) error:",error)

    stitched_check = np.sum(np.abs(image22a-stitched_image))
    print("Stitching check (ugrid22a):",stitched_check)
    print()

    error=-99
    error = plib.formSubImage(image22b, sub_images,overlap_factor)
    print("formSubImage (ugrid22b) error:",error)

    sub_check = 0
    for i in range(0,num_sub):
        sub_check = sub_check + (0 if image22b[0,0]==sub_images[0,0,i] else 1) # value_if_true if condition else value_if_false
    print("formSubImage (ugrid22b) check:",sub_check)
    print("First 10x10 elements of subimage")
    for r in range(0,10):
        for c in range(0,10):
            print(sub_images[r,c,5],end=",")
        print()

    error=-99
    error = plib.stitchsubimages(stitched_image,sub_images,overlap_factor)
    print("stitchsubimages (ugrid22b) error:",error)

    stitched_check = np.sum(np.abs(image22b-stitched_image))
    print("Stitching check (ugrid22b):",stitched_check)
    print()

    # Test ugrid32
    error=-99
    error = plib.formSubImage(image32, sub_images,overlap_factor)
    print("formSubImage (ugrid32) error:",error)

    sub_check = 0
    for i in range(0,num_sub):
        sub_check = sub_check + (0 if image32[0,0]==sub_images[0,0,i] else 1) # value_if_true if condition else value_if_false
    print("formSubImage (ugrid32) check:",sub_check)
    for r in range(0,10):
        for c in range(0,10):
            print(sub_images[r,c,5],end=",")
        print()

    error=-99
    error = plib.stitchsubimages(stitched_image,sub_images,overlap_factor)
    print("stitchsubimages (ugrid32) error:",error)

    stitched_check = np.sum(np.abs(image32-stitched_image))
    print("Stitching check (ugrid32):",stitched_check)
    print()

    # Test ugrid42
    error=-99
    error = plib.formSubImage(image42, sub_images,overlap_factor)
    print("formSubImage (ugrid42) error:",error)

    sub_check = 0
    for i in range(0,num_sub):
        sub_check = sub_check + (0 if image42[0,0]==sub_images[0,0,i] else 1) # value_if_true if condition else value_if_false
    print("formSubImage (ugrid42) check:",sub_check)
    for r in range(0,10):
        for c in range(0,10):
            print(sub_images[r,c,5],end=",")
        print()

    error=-99
    error = plib.stitchsubimages(stitched_image,sub_images,overlap_factor)
    print("stitchsubimages (ugrid42) error:",error)

    stitched_check = np.sum(np.abs(image42-stitched_image))
    print("Stitching check (ugrid42):",stitched_check)