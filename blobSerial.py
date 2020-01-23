#!/usr/bin/python

# Imports
import cv2
import numpy as np
import serial
import socket
import os
import glob

ser1 = serial.Serial('/dev/ttyUSB1', 9600)
xyList = []

#Get the name of the latest file
list_of_files = glob.glob('share/*.JPG') # * means all if need specific format then *.csv
latest_file = max(list_of_files, key=os.path.getctime)
print(latest_file)

#Connect with the cognex camera
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_address = ("192.168.0.101", 23)
print("Start connection")
s.connect(server_address) 
cmd = "admin\r\n\r\n".encode()
print("Sending command: 'SE8'")
s.sendall(cmd)
s.sendall("SE8\n\r".encode())
print("Command sent!")
print("End connection!")

# Read image
img = cv2.imread(latest_file, cv2.IMREAD_GRAYSCALE)
rotated = cv2.rotate(img, cv2.ROTATE_90_CLOCKWISE)
im = cv2.resize(rotated,(480, 640))

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
#im_with_keypoints_resized = cv2.resize(im_with_keypoints, (w / 2, h / 2))
img_mod = cv2.line(im_with_keypoints,(im_with_keypoints.shape[1],im_with_keypoints.shape[1]),(0,im_with_keypoints.shape[1]),(0,0,255),1)
cv2.imshow("Keypoints", img_mod)

# Print the keypoint id and the X and Y coordinates of the blobs found
for key in keypoints:
    print(key)
    # x coordinate = key.pt[0] & size x-axis = im.shape[1]
    # This way, the coordinates will fit the y axis,
    # but only use part of the x axis
    variableY = 100 - round(100 - key.pt[0] / im.shape[1] * 100) 
    variableX = 100 - round(100 - key.pt[1] / im.shape[1] * 100)
    if round(key.pt[1] / im.shape[1] * 100) <= 100:
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
