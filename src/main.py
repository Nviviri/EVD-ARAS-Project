# import the necessary packages
try:
    from picamera.array import PiRGBArray
    from picamera import PiCamera
except ImportError:
    print("Failed to load PiCamera. Expect errors when trying to read from the camera.")
import numpy as np
import cv2
import argparse
import time
import recognise
import util

def main():
    # initialize the camera and grab a reference to the raw camera capture
    captureResolution = (1640, 1232)
    camera = PiCamera()
    camera.resolution = captureResolution
    camera.framerate = 15
    camera.iso = 100
    camera.meter_mode = "matrix"
    camera.awb_mode = "fluorescent"
    rawCapture = PiRGBArray(camera, size=captureResolution)

    # allow the camera to warmup
    time.sleep(0.1)

    # capture frames from the camera
    for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
        # grab the raw NumPy array representing the image, then initialize the timestamp
        # and occupied/unoccupied text
        image = frame.array

        dispImage = image.copy()
        playfieldCorners = recognise.get_playfield_corners_by_aruco(cv2.cvtColor(image, cv2.COLOR_BGR2GRAY))

        for playfieldCorner in playfieldCorners:
            cv2.circle(dispImage, playfieldCorner, 25, (0, 0, 255), cv2.FILLED)

        # show the frame
        cv2.imshow("Frame", util.fit_display(dispImage))
        #cv2.imshow("Frame2", inrange_image)
        key = cv2.waitKey(1) & 0xFF

        # clear the stream in preparation for the next frame
        rawCapture.truncate(0)

        # if the `q` key was pressed, break from the loop
        if key == ord("q"):
            break

def test_main(imagePath):
    image = util.white_balance(cv2.imread(imagePath))
    cv2.imshow("Output image", util.fit_display(recognise.filter_bricks(image)))
    displayImage = recognise.find_bricks_by_color(image)[1]
    playfieldCorners = recognise.get_playfield_corners_by_aruco(cv2.cvtColor(image, cv2.COLOR_BGR2GRAY))
    cv2.drawContours(displayImage, np.array([playfieldCorners], dtype=np.int32), -1, (0, 0, 255), 5)
    cv2.imshow("Output image 2", util.fit_display(displayImage))
    while cv2.waitKey(100) != ord("q"):
        pass

parser = argparse.ArgumentParser(description='Guidance assistant for building LEGO buildings')
parser.add_argument('--testImage', required=False, help='Run the detection algorithm on a still image')

args = parser.parse_args()
if args.testImage:
    test_main(args.testImage)
else:
    main()
