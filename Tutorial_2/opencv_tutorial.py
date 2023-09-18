# ENEE408I - Tutorial 2 - Section 3.5 code
# 
# Author: Kent Chesley
# Date: 09-17-23
#
# Things to be done to get this code to run:
# 1. Clone https://github.com/opencv/opencv to obtain the first image used. More details in Part (1) below.
# 2. For Part (3), download the image at https://media.gettyimages.com/id/1347947373/photo/two-business-people-working-together-in-office.jpg?s=612x612&w=0&k=20&c=sZf_nBKhfNpBwA4lUFtOBW6DVsTufgR7BOgVtg4_L08=.
# 3. Update the three file paths in the code to reflect the paths on your machine.
#
# After each image displays, press any key to continue.

# (1)

# Adapted these instructions: https://docs.opencv.org/4.x/db/deb/tutorial_display_image.html.

import cv2 as cv
import sys

# It was necessary to obtain the image. This was accomplished by running "git clone https://github.com/opencv/opencv" within 
# some_directory. It was then necessary to add the line of code 
# "cv.samples.addSamplesDataSearchPath("C:\\...\\<some_directory>\\opencv\\samples\\data")" as seen below. 
# Reference: https://stackoverflow.com/questions/59341359/why-does-reading-an-image-from-opencv-python-samples-giving-error-where-as-it-do.
cv.samples.addSamplesDataSearchPath("C:\\Users\\kente\\OneDrive\\Documents\\College\\Fall_2023\\ENEE408I_New\\Tutorial_2\\opencv\\samples\\data")

img = cv.imread(cv.samples.findFile("starry_night.jpg"))

cv.imshow("Original image", img)
k = cv.waitKey(0)   # Press any key to continue.

gs_img = cv.cvtColor(img, cv.COLOR_BGR2GRAY)    # Convert to gray scale. 
# Reference: https://techtutorialsx.com/2018/06/02/python-opencv-converting-an-image-to-gray-scale/.

cv.imshow("Gray scale", gs_img)
k = cv.waitKey(0)

# (2)

# Reference: https://www.geeksforgeeks.org/real-time-edge-detection-using-opencv-python/.

def canny_edge_detection(gray_frame):
    blurred = cv.GaussianBlur(src=gray_frame, ksize=(3,5), sigmaX=5)
    # Note that the elements of ksize must be odd per https://stackoverflow.com/questions/27527440/opencv-error-assertion-failed-ksize-width-for-gaussianblur.
    edges = cv.Canny(blurred, 70, 200)
    return blurred, edges

blurred, edges = canny_edge_detection(gs_img)

cv.imshow("Edges", edges)
k = cv.waitKey(0)

# (3)

# Adaped the code at https://docs.opencv.org/3.4/db/d28/tutorial_cascade_classifier.html.

import argparse

faces_img = cv.imread("C:\\Users\\kente\\OneDrive\\Documents\\College\\Fall_2023\\ENEE408I_New\\Tutorial_2\\faces.jpg") 
# Path to image file. Image obtained at https://media.gettyimages.com/id/1347947373/photo/two-business-people-working-together-in-office.jpg?s=612x612&w=0&k=20&c=sZf_nBKhfNpBwA4lUFtOBW6DVsTufgR7BOgVtg4_L08=.
cv.imshow("Original faces image", faces_img)
k = cv.waitKey(0)

def detectAndDisplay(frame):
    frame_gray = cv.cvtColor(frame, cv.COLOR_RGB2GRAY)  # Note that this is necessary to avoid errors despite the fact that the image is 
    # already grayscale. Reference: https://stackoverflow.com/questions/53829896/opencv-error-215assertion-failed-src-type-cv-8uc1-in-function-cve.
    frame_gray = cv.equalizeHist(frame_gray)

    faces = face_cascade.detectMultiScale(frame_gray)
    for (x,y,w,h) in faces:
        center = (x + w//2, y + y//2)
        frame = cv.ellipse(frame, center, (w//2, h//2), 0, 0, 360, (255, 0, 255), 4)

        cv.imshow('Faces detected', frame)

face_cascade = cv.CascadeClassifier('C:\\Users\\kente\\OneDrive\\Documents\\College\\Fall_2023\\ENEE408I_New\\Tutorial_2\\opencv\\data\\haarcascades\\haarcascade_frontalface_alt2.xml')
# I tested several .xml files and found that haarcascade_frontalface_alt2.xml performed best on this file.
# Reference: Answer by Keerthana Gopalakrishnan at https://stackoverflow.com/questions/30508922/error-215-empty-in-function-detectmultiscale.

detectAndDisplay(faces_img)
k = cv.waitKey(0)

