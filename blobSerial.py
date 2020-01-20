#!/usr/bin/python

# Standard imports
import cv2
import numpy as np
import serial
from picamera import PiCamera
from time import sleep

ser1 = serial.Serial('/dev/ttyUSB0', 9600)
xyList = []

camera = PiCamera()
camera.start_preview()
sleep(5)
camera.capture('/home/pi/temp.jpg')
camera.stop_preview()

# Read image
im = cv2.imread("temp.jpg", cv2.IMREAD_GRAYSCALE)

# Setup SimpleBlobDetector parameters.
params = cv2.SimpleBlobDetector_Params()


# Change thresholds
params.minThreshold = 10
params.maxThreshold = 200

# Filter by Area.
params.filterByArea = True
params.minArea = 100

# Filter by Circularity
params.filterByCircularity = True
params.minCircularity = 0.1

# Filter by Convexity
params.filterByConvexity = True
params.minConvexity = 0.87
    
# Filter by Inertia
params.filterByInertia = False
params.minInertiaRatio = 0.01



# Create a detector with the parameters
ver = (cv2.__version__).split('.')
if int(ver[0]) < 3 :
    detector = cv2.SimpleBlobDetector(params)
else : 
    detector = cv2.SimpleBlobDetector_create(params)

# Detect blobs.
keypoints = detector.detect(im)

# Draw detected blobs as red circles.
# cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS ensures
# the size of the circle corresponds to the size of blob

im_with_keypoints = cv2.drawKeypoints(im, keypoints, np.array([]), (0,0,255), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)

# Show blobs in a resized window
im_with_keypoints_resized = cv2.resize(im_with_keypoints, (1000, 600))
img_mod = cv2.line(im_with_keypoints_resized,(im_with_keypoints_resized.shape[0],0),(im_with_keypoints_resized.shape[0],im_with_keypoints_resized.shape[0]),(0,0,255),1)
cv2.imshow("Keypoints", img_mod)

# Print the keypoint id and the X and Y coordinates of the blobs found
for key in keypoints:
    print(key)
    # x coordinate = key.pt[0] & size x-axis = im.shape[1]
    # This way, the coordinates will fit the y axis,
    # but only use part of the x axis
    variableX = round(key.pt[1] / im.shape[0] * 100)
    variableY = round(key.pt[0] / im.shape[0] * 100)
    if round(key.pt[0] / im.shape[0] * 100) <= 100:
        xyList.append(variableX)
        xyList.append(variableY)
    print("X:", 100 - variableX, "Y:", 100 - variableY)
    
# Print the total amount of keypoints (blobs) found
print("Total keypoints found:",len(keypoints))
print("Keypoints used:", round(len(xyList) / 2))

for xy in xyList:
    ser1.write(str(xy).encode())
    ser1.write(','.encode())

print(xyList)

cv2.waitKey(0)
