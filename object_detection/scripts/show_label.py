#!/usr/bin/python
"""
Show a given label and its associated bounding box from an AIC, KITTI, DarkNet, or 
VOC dataset from the NVIDIA AI City Challenge.
"""

import os
from argparse import ArgumentParser
import matplotlib.pyplot as plt
import matplotlib.patches as patches
from PIL import Image
import numpy as np

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
    parser.add_argument("data", nargs='+', help="Annotations or image file name or path.")
    parser.add_argument('--help', action='help', help='Show this help message and exit.')
    parser.add_argument('-o', '--output', type=str, default=None, help="Path to store image with bounding boxes, instead of displaying to screen.")
    parser.add_argument('-f', '--format', type=str, default="aic", help="Dataset format: aic (default), kitti, darknet, voc.")
    parser.add_argument('-d', '--dspath', type=str, default=None, help="Path to dataset root. Default: current working directory.")
    parser.add_argument('-s', '--split', type=str, default=None, help="Dataset split, e.g., train.")
    return parser.parse_args()


def fn(f):
    """
    Get file name from path or URL.
    """
    return f[f.rfind('/')+1:]


def parseAnnotations(f, width, height):
    """ 
    Read annoation bboxes and classes from annotations file.
    aic: fh.write("%s %d %d %d %d\n" % ( l, b[0], b[1], b[2], b[3]) )
    """
    lbs = []
    if dsFmt == "voc":
        print "Reading VOC format annoations has not yet been implemented... Check back soon."
        exit()

    with open(f, "r") as fh:
        for l in fh:
            p = l.split()
            if not p:
                continue
            if dsFmt == "aic":
                if len(p) < 5 or float(p[3]) - float(p[1]) < 3.:
                    print "Label format does not seem to match 'aic'. Check the -f parameter."
                    usage()
                lbs.append( (p[0], float(p[1]), float(p[2]), float(p[3]), float(p[4])) )
            elif dsFmt == "kitti":
                if len(p) < 8 or float(p[6]) - float(p[4]) < 3.:
                    print "Label format does not seem to match 'kitti'. Check the -f parameter."
                    usage()
                lbs.append( (p[0], float(p[4]), float(p[5]), float(p[6]), float(p[7])) )
            elif dsFmt == "darknet":
                if len(p) < 5 or float(p[3]) - float(p[1]) > 1.:
                    print "Label format does not seem to match 'darknet'. Check the -f parameter."
                    usage()
                cx = float(p[1]) * width
                cy = float(p[2]) * height
                w = float(p[3]) * width
                h = float(p[4]) * height
                minx = cx - w * 0.5
                miny = cy - h * 0.5
                maxx = minx + w
                maxy = miny + h
                if int(p[0]) >= len(labels):
                    print "Label ID does not match stored set of labels. Is this an AIC 2017 dataset?"
                    usage()
                lbs.append( (labels[int(p[0])], minx, miny, maxx, maxy) )

    return lbs


def getDsDirs():
    if dsFmt == "aic" or dsFmt == "kitti":
        return "images", "labels"
    elif dsFmt == "darknet":
        return "images", "annotations"
    elif dsFmt == "voc":
        return "JPEGImages", "Annotations"


def findData(imgFile, annFile):
    """
    Find the image and annotation file paths.
    """
    if imgFile and annFile:
        if imgFile.endswith(".txt") and annFile.endswith(".jpeg"):
            tmp = imgFile
            imgFile = annFile
            annFile = tmp
        if os.path.isfile(imgFile) and os.path.isfile(annFile):
            return imgFile, annFile

    imgdir, anndir = getDsDirs()
    split = args.split
    if not os.path.isfile(imgFile): # find the image
        if dsFmt != "voc":
            if not split:
                for t in ['train', 'val', 'test']:
                     if os.path.isfile(os.path.join(dsPath, t, imgdir, fn(imgFile))):
                         split = t
                         break
            if not split:
                print "Could not find image file. Plese provide full path or correct dataset root directory."
                usage()
            imgFile = os.path.join(dsPath, split, imgdir, fn(imgFile))
        else:
            imgFile = os.path.join(dsPath, imgdir, fn(imgFile))
        if not os.path.isfile(imgFile):
            print "Could not find image file. Our best guess for the path was: %s." % imgFile
            print "Try profiding the full path for the image file."
            usage()
    if imgFile.endswith(".txt"):  # annotations file provided
        imgFile = imgFile.strip()
        imgFile = imgFile[: imgFile.rfind('.')] + '.jpeg'
        imgFile = imgFile.replace(anndir, imgdir)
        if not os.path.isfile(imgFile):
            print "Could not find image file. Our best guess for the path was: %s." % imgFile
            print "Try profiding the full path for the image file."
            usage()

    if not annFile or not os.path.isFile(annFile):
        annFile = imgFile[:]
        annFile = annFile[: annFile.rfind('.')] + '.txt'
        annFile = annFile.replace(imgdir, anndir)
        if not os.path.isfile(annFile):
            print "Could not find annotations file. Our best guess for the path was: %s." % annFile
            print "Try profiding the full path for the image and annotation files."
            usage()

    return imgFile, annFile
        

def showImage(imgFile, annFile):
    """
    Show the image and associated bounding boxes
    """
    
    if not ('DISPLAY' in os.environ and os.environ['DISPLAY']) and not args.output:
        print "The image cannot be displayed as you do not currently have X11 Forwarding enabled or"
        print "are not running X. To start an ssh session with X11 Forwarding enabled, add the '-X'"
        print "parameter to the ssh call, e.g.,"
        print "    ssh -X user@10.10.10.10"
        print "Alternatively, you can output the image to a file by envoking this script with -o:"
        usage()
 
    imgFile, annFile = findData(imgFile, annFile)

    # read the image file
    try:
        im = Image.open(imgFile)
    except:
        print "Could not read the image file. Check your paths and try again."
        print "Image file path %s." % imgFile
        usage()

    # read the associated labels
    lbs = parseAnnotations(annFile, im.size[0], im.size[1])

    # Create figure and axes
    fig,ax = plt.subplots(1)
    DPI = float( fig.get_dpi())
    fig.set_size_inches( im.size[0]/DPI, im.size[1]/DPI )

    # Display the image
    ax.imshow(np.array(im, dtype=np.uint8))

    for l in lbs:
        rect = patches.Rectangle((l[1],l[2]),l[3]-l[1],l[4]-l[2], linewidth=1.5, edgecolor='r', facecolor='none')
        ax.add_patch(rect)
        ax.text(l[1], l[2]-6, l[0], fontsize=7, color='b', weight='bold',
                bbox=dict(facecolor='white', alpha=0.7, edgecolor='r', boxstyle='square'))
    
    if args.output:
        plt.savefig(args.output, bbox_inches='tight')
    else:
        plt.show()


def usage():
    print "Usage: ../show_labels.py [-d <dataset_root_path> -s <train|val> -o <output_file>] <image_path> <annotations_path> "
    exit()



if __name__ == '__main__':
    args = get_args();
    if not args.data:
        print "Please specify an image or annotations file!"
        usage()
    imgFile = args.data[0]
    annFile = args.data[1] if len(args.data) > 1 else None
    dsPath  = args.dspath if args.dspath else os.getcwd()
    dsFmt   = args.format.lower()

    showImage(imgFile, annFile)
