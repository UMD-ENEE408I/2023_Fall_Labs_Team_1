import cv2 as cv
import sys

# Read the image
img = cv.imread(cv.samples.findFile("cap-1.jpg"))

if img is None:
    sys.exit("Could not read image")

haar_cascade = cv.CascadeClassifier(cv.data.haarcascades + 'haarcascade_frontalface_default.xml')

# Detect faces
faces_rect = haar_cascade.detectMultiScale(img, 1.2, 4,flags = (cv.CASCADE_DO_CANNY_PRUNING),minSize =(40,40))

# Place rectangles around faces in image
for (x,y,w,h) in faces_rect:
    cv.rectangle(img,(x,y),(x+w,y+h),(0,255,0),2)

cv.imshow('Detected faces',img)

k = cv.waitKey(0)