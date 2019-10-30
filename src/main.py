# import the necessary packages
import numpy as np
import pathlib
import analyze
import cv2
import argparse
import recognise
import util
import projection
import camera
import lsfparser
import statemachine
import constants


def main(filePath, savePath):
    loadedSequence = lsfparser.openFile(filePath)
    camera.init()
    statemachine.processManager(loadedSequence, savePath, True)
    while cv2.waitKey(100) != ord("q"):
        pass


def test_main(imagePath, filePath, savePath):
    # Load files
    loadedSequence = lsfparser.openFile(filePath)
    statemachine.processManager(loadedSequence, savePath, imagePath)
    while cv2.waitKey(100) != ord("q"):
        pass

def calibrate(imagePath):
    cropped_image = analyze.get_image(imagePath)
    util.draw_dot_matrix(cropped_image, constants.BASEPLATE_SIZE, constants.BASEPLATE_CUTOUT_POS, constants.BASEPLATE_CUTOUT_SIZE)
    cv2.imshow("calibration", util.fit_display(cropped_image))
    while cv2.waitKey(100) != ord("q"):
        pass


# Parse user arguments
parser = argparse.ArgumentParser(
    description='Guidance assistant for building LEGO buildings')
parser.add_argument('--testImage', required=False,
                    help='Run the detection algorithm on a still image')
parser.add_argument('--readFile', required=True,
                    help='Lego model description file')
parser.add_argument('--saveFile', required=False, default="Saved.gg",
                    help='State save file path')
parser.add_argument('--calibrate',
                    action='store_true',
                    help='Whether to display a calibration image')

args = parser.parse_args()

if args.calibrate:
    calibrate(args.testImage or True)
elif args.testImage:
    test_main(args.testImage, args.readFile, args.saveFile)
else:
    main(args.readFile, args.saveFile)
