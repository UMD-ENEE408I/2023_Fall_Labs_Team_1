# Allows the use of the SKYDROID 5.8G OTG RECEIVER to stream video from the associated camera.
# 
# Author: Kent Chesley
# Date: 12/2/23
# 
# Adapted from 
# https://github.com/kevinam99/capturing-images-from-webcam-using-opencv-python/blob/master/webcam-capture-v1.01.py.
#
# Tested on a Windows machine.
#
# The SKYDROID must be connected dirctly to the laptop (in my case using a USB C port). Using the adapter to 
# connect to one of the laptop's USB A ports resulted in the receiver not being visible to the laptop. 
#
# The argument to cv2.VideoCapture specified the camera device index for the camera one wishes to use. Assuming one 
# is using a laptop with
# a built in webcam, setting the argument to 0 will use the webacam as the programs input while setting it to 1 will
# use the SKYDROID receiver as the input (assuming no other cameras are connected). Reference: 
# https://docs.opencv.org/4.x/dd/d43/tutorial_py_video_display.html.
#
# On windows, one can also determine which camera corresponds to which device index as follows: Open the Start menu
# and search for and open the "Device Maneger." Open the "Cameras" drop down menu. The cameras are listed in 
# ascending order of device index starting with index 0. Reference: 
# https://www.shellhacks.com/windows-lsusb-equivalent-powershell/. 
#
# Once the Python code begins to run, the received video stream is displayed in a pop-up window on the screen. Press
# the "s" key to capture a snapshot from the video (not tested). Press the "q" key to stop the program.
#
# If the displayed video is just static noise, press the small button above the camera's seven-segment display 
# multiple times unitl the camera video is received. Each time you press the button, the number on the seven
# -segment display changes. For me, 6 was the seven-segment display number which corresponded to successful video
# streaming. It took a few seconds for video to begin streaming once the correct seven-segment display number was 
# reached.

import cv2 

key = cv2. waitKey(1)
webcam = cv2.VideoCapture(1)
while True:
    try:
        check, frame = webcam.read()
        print(check) #prints true as long as the webcam is running
        print(frame) #prints matrix values of each framecd 
        cv2.imshow("Capturing", frame)
        key = cv2.waitKey(1)
        if key == ord('s'): 
            cv2.imwrite(filename='saved_img.jpg', img=frame)
            webcam.release()
            img_new = cv2.imread('saved_img.jpg', cv2.IMREAD_GRAYSCALE)
            img_new = cv2.imshow("Captured Image", img_new)
            cv2.waitKey(1650)
            cv2.destroyAllWindows()
            print("Processing image...")
            img_ = cv2.imread('saved_img.jpg', cv2.IMREAD_ANYCOLOR)
            print("Converting RGB image to grayscale...")
            gray = cv2.cvtColor(img_, cv2.COLOR_BGR2GRAY)
            print("Converted RGB image to grayscale...")
            print("Resizing image to 28x28 scale...")
            img_ = cv2.resize(gray,(28,28))
            print("Resized...")
            img_resized = cv2.imwrite(filename='saved_img-final.jpg', img=img_)
            print("Image saved!")
        
            break
        elif key == ord('q'):
            print("Turning off camera.")
            webcam.release()
            print("Camera off.")
            print("Program ended.")
            cv2.destroyAllWindows()
            break
        
    except(KeyboardInterrupt):
        print("Turning off camera.")
        webcam.release()
        print("Camera off.")
        print("Program ended.")
        cv2.destroyAllWindows()
        break
    