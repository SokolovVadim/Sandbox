#!/usr/bin/python

import sys
import cv2
import os
import numpy as np

import argparse
import matplotlib.pyplot as plt


parser = argparse.ArgumentParser( description = 'Load image to the light ball')
parser.add_argument('image_name' , type = str ,help = 'Input image name')
args = parser.parse_args()



img = cv2.imread( args.image_name)

if img is None:
    sys.exit( "Invalid image name: " + sys.argv[0])

# array file writing
x = img.shape[0]
y = img.shape[1]

array = np.zeros((2, y))

for i in range(0, y, 1):
    first = y
    last = 0.
    for j in range(x-1, -1, -1):
        if( (img[j][i][0] == 255) and (img[j][i][1] == 255) and (img[j][i][2] == 255)):
            if( first > j):
                first = (x-j)
            if( last < j):
                last = (x-j)
    array[0][i] = float(first - last)/float(last + first)
    array[1][i] = i * 300000. / y
    #print(array[0][i], array[1][i])

plot = plt.figure(1)
plt.plot(array[1], array[0])
plt.xlabel('Count')
plt.ylabel('V')
plt.title('V of ' + args.image_name )

#plot1 = plt.figure(1)
#plt.plot(img[0], img[1])

plt.show()


