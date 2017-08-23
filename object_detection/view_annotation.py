#!/usr/bin/python
"""
Show a given label and its associated bounding box from an AIC, KITTI, DarkNet, or
VOC dataset from the NVIDIA AI City Challenge.

Usage:
    directory organization: The dataset directory should be the organized as the AIC datasets.
    usage: we provide the argparse method for settings.
           provide --dspath for dataset root path
           provide --output to give the output directory if you want to save the output frames.
           provide --split for subset choice, <train|val>

"""

import os
from argparse import ArgumentParser
import matplotlib.pyplot as plt
import matplotlib.patches as patches
from PIL import Image
import numpy as np
import time
import  pdb
import csv
labels = [
    'DontCare',
    'Car',
    'SUV',
    'SmallTruck',
    'MediumTruck',
    'LargeTruck',
    'Pedestrian',
    'Bus',
    'Van',
    'GroupOfPeople',
    'Bicycle',
    'Motorcycle',
    'TrafficSignal-Green',
    'TrafficSignal-Yellow',
    'TrafficSignal-Red',
]


def get_args():
    parser = ArgumentParser(add_help=False)
    # parser.add_argument("data", nargs='+', help="Annotations or image file name or path.")
    parser.add_argument("--img_fn", type=str, help="image file name or path.")
    parser.add_argument("--ann_fn", type=str, help="Annotations file name or path.")
    parser.add_argument('--help', action='help', help='Show this help message and exit.')
    parser.add_argument('-o', '--output', type=str, default=None,
                        help="Path to store image with bounding boxes, instead of displaying to screen.")
    parser.add_argument('-d', '--dspath', type=str, default=None,
                        help="Path to dataset root. Default: current working directory.")
    parser.add_argument('-s', '--split', type=str, default='val', help="Dataset split, e.g., train.")
    return parser.parse_args()


def fn(f):
    """
    Get file name from path or URL.
    """
    return f[f.rfind('/') + 1:]


def parseAnnotations(f, width, height):
    """
    Read annoation bboxes and classes from annotations file.
    aic: fh.write("%s %d %d %d %d\n" % ( l, b[0], b[1], b[2], b[3]) )
    """
    lbs = []

    with open(f, "r") as fh:
        reader = csv.reader(fh, delimiter=' ')
        for p in reader:
            #p = l.split(' ')
            #p = p[0]
            print 'p:', p
            if not p:
                print 'not a file!'
                return None

            minx = float(p[1])
            miny = float(p[2])
            w = float(p[3]) - minx
            h = float(p[4]) - miny
            score = float(p[5])
            if score >= 0.3:
                lbs.append((p[0], minx, miny, w, h, score))

    return lbs


def getDsDirs():
    if dsFmt == "aic" or dsFmt == "kitti":
        return "images", "labels"
    elif dsFmt == "darknet":
        return "images", "annotations"
    elif dsFmt == "voc":
        return "JPEGImages", "Annotations"

def showImage(imgFile, annFile):
    """
    Show the image and associated bounding boxes
    """
    #imgFile, annFile = findData(imgFile, annFile)

    # read the image file
    try:
        im = Image.open(imgFile)
    except:
        print "Could not read the image file. Check your paths and try again."
        print "Image file path %s." % imgFile

    # read the associated labels
    lbs = parseAnnotations(annFile, im.size[0], im.size[1])
    #print lbs
    # Create figure and axes
    fig, ax = plt.subplots(1)
    DPI = float(fig.get_dpi())
    fig.set_size_inches(im.size[0] / DPI, im.size[1] / DPI)

    # Display the image
    ax.imshow(np.array(im, dtype=np.uint8))

    for l in lbs:
        rect = patches.Rectangle((l[1], l[2]), l[3], l[4], linewidth=1.5, edgecolor='r', facecolor='none')
        ax.add_patch(rect)
        ax.text(l[1], l[2] - 6, l[0]+'\n%.3f' %(l[5]), fontsize=7, color='b', weight='bold',
                bbox=dict(facecolor='white', alpha=0.7, edgecolor='r', boxstyle='square'))
        #ax.text(l[1]+l[3]-6, l[2] - 6, str(l[5]), fontsize=7, color='b', weight='bold',
         #       bbox=dict(facecolor='white', alpha=0.7, edgecolor='r', boxstyle='square'))

    if args.output:
        plt.savefig(args.output)
        plt.close()
    else:
        plt.show()
        time.sleep(0.03)


if __name__ == '__main__':
    args = get_args()
    #data_dir = '{}\\aic1080\\{}\\'.format(args.dspath, args.split)
    data_dir = '/media/cvml/79DA65B928695666/datasets/nVidia/aic540/test/'
    ana_dir = '/home/cvml/Projects/tensorflow_models/object_detection/nVidia_test540v2/'
    imgFiles = os.listdir(data_dir + 'images')
    output_dir = args.output
    #pdb.set_trace()
    NF = len(imgFiles)
    print 'totally %d images' % (NF)
    i = 0
    #for img in imgFiles[:500]:
    for img in imgFiles:
      name, extension = img.split('.')
      imgFile = data_dir + 'images/' + img
      annFile = ana_dir + name + '.txt'
      #print "annotation file: %s" %(annFile)
      #dsPath = args.dspath if args.dspath else os.getcwd()
      #dsFmt = args.format.lower()
      dsFmt = "None"
      if args.output:
        args.output = output_dir + img
      print 'output:', args.output   
      perc = 100.0 * i / NF
      print '%d (total %d, %.2f%%) reading %s' % (i, NF, perc, imgFile)
      showImage(imgFile, annFile)
      i = i + 1
