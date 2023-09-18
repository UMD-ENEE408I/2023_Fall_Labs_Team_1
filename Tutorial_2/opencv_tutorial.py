import cv2 as cv
import sys
from matplotlib import pyplot as plt

# Read the image
img = cv.imread(cv.samples.findFile("express.jpg"))

if img is None:
    sys.exit("Could not read image")

# Display the image
cv.imshow("Display window",img)

# Press k key to close the window
k = cv.waitKey(0)

# Convert the image to grayscale
img = cv.cvtColor(img, cv.COLOR_BGR2GRAY)

# Display the image
cv.imshow("Display window",img)

# Press k key to close the window
k = cv.waitKey(0)

# Perform Canny edge detection
edges = cv.Canny(img,100,200)

plt.subplot(121),plt.imshow(img,cmap = 'gray')
plt.title('Original image'), plt.xticks([]), plt.yticks([])
plt.subplot(122), plt.imshow(edges, cmap = 'gray')
plt.title('Edge Image'), plt.xticks([]), plt.yticks([])

plt.show()

haar_cascade = cv.CascadeClassifier(cv.data.haarcascades + 'haarcascade_frontalface_default.xml')

# Detect faces
faces_rect = haar_cascade.detectMultiScale(img, 1.2, 4,flags = (cv.CASCADE_DO_CANNY_PRUNING),minSize =(40,40))

# Place rectangles around faces in image
for (x,y,w,h) in faces_rect:
    cv.rectangle(img,(x,y),(x+w,y+h),(0,255,0),2)

cv.imshow('Detected faces',img)

k = cv.waitKey(0)