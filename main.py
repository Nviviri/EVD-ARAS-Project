# import the necessary packages
from picamera.array import PiRGBArray
from picamera import PiCamera
import time
import cv2
import numpy as np

def inrangeLego(mat):
    hsvMat = cv2.cvtColor(mat, cv2.COLOR_BGR2HSV)
    return cv2.inRange(hsvMat, np.array([0, 80, 0]), np.array([255, 130, 255]))
 
# initialize the camera and grab a reference to the raw camera capture
camera = PiCamera()
camera.resolution = (1920, 1080)
camera.framerate = 30
rawCapture = PiRGBArray(camera, size=(1920, 1080))
 
# allow the camera to warmup
time.sleep(0.1)
 
# capture frames from the camera
for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
    # grab the raw NumPy array representing the image, then initialize the timestamp
    # and occupied/unoccupied text
    image = frame.array
    inrange_image = inrangeLego(image)
 
    # show the frame
    image = cv2.resize(image, dsize=(853, 480))
    cv2.imshow("Frame", image)
    #cv2.imshow("Frame2", inrange_image)
    key = cv2.waitKey(1) & 0xFF
 
    # clear the stream in preparation for the next frame
    rawCapture.truncate(0)
 
    # if the `q` key was pressed, break from the loop
    if key == ord("q"):
        break
