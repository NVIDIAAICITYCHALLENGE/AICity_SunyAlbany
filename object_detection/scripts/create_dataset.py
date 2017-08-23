#!/usr/bin/python
"""
Transform the datasets created for the NVIDIA AI City Challenge into popular formats, including KITTI, Pascal VOC, and Darknet.
The script creates symbolic lists when possible. Use -c to copy images instead.
The script assumes all images in a dataset have consistent height and width. Users can specifying a different height or width to rescale images and annotations.
"""

import os
import sys
from PIL import Image
from argparse import ArgumentParser


labels = {
    'DontCare' : 0,
    'Car' : 1,
    'SUV' : 2,
    'SmallTruck' : 3,
    'MediumTruck' : 4,
    'LargeTruck' : 5,
    'Pedestrian' : 6,
    'Bus' : 7,
    'Van' : 8,
    'GroupOfPeople' : 9,
    'Bicycle' : 10,
    'Motorcycle' : 11,
    'TrafficSignal-Green' : 12,
    'TrafficSignal-Yellow' : 13,
    'TrafficSignal-Red' : 14,
    # 'Crossing' : 15,  # ignore Crossing annotations
}


def get_args():
    parser = ArgumentParser(add_help=False)
    parser.add_argument("data", nargs=1, help="Output directory for dataset")
    parser.add_argument('--help', action='help', help='Show this help message and exit')
    parser.add_argument('-d', '--dataset', type=str, default=None, help="Dataset directory path")
    parser.add_argument('-f', '--format', type=str, default="kitti", help="Desired format: kitti (default), voc (i.e., Pascal VOC), darknet")
    parser.add_argument('-w', '--width', type=int, default=None, help="Width of image, e.g., 1920")
    parser.add_argument('-h', '--height', type=int, default=None, help="Height of image, e.g., 1080")
    
    return parser.parse_args()


def fn(f):
    """
    Get file name from path or URL.
    """
    return f[f.rfind('/')+1:]
    
    
def ensureDir(d):
    if not d[-1] == '/':
        d += '/'
    if not os.path.exists(d):
        os.makedirs(d)


def findWidthHeight():
    """Derive width and height of dataset images"""

    for f in os.listdir("%s/train/images/" % args.dataset):
        if f.endswith(".jpeg"):
            imf = "%s/train/images/%s" % (args.dataset, f)
            try:
                im = Image.open(imf)
            except:
                print "Could not open training image %s to read its size." %imf
                usage()
            break
            
    width = int(im.size[0])
    height = int(im.size[1])
    
    nwidth = width
    nheight = height
    if args.width:
        nwidth = args.width
    if args.height:
        nheight = args.height

    return width, height, nwidth, nheight, not(width == nwidth and height == nheight)

        

def readAnnotations(f):
    """ Read annotations for a given image """
    lbf = "../labels/" + f[: f.rfind('.')] + ".txt"
    b = []
    with open(lbf, "r") as fh:
        for l in fh:
            p = l.strip().split()
            b.append( (p[0], int(p[1]), int(p[2]), int(p[3]), int(p[4])) )

    return b


def resizeImage(f, odir, n=0):
    img = Image.open(f)
    img = img.resize((nwidth, nheight))
    nf = os.path.join(odir, "%06d.jpeg" % n)
    if args.format.lower() == 'voc':
        nf = os.path.join(odir, f)
    img.save(nf)
    
    
def linkImage(f, odir, n=0):
    nf = os.path.join(odir, "%06d.jpeg" % n)
    if args.format.lower() == 'voc':
        nf = os.path.join(odir, f)
    f = os.path.join(cwd, f)
    try:
        os.symlink( f, nf )
    except Exception as e:
        print "Could not create symbolic link from %s to %s. Make sure that the images directory in new dataset is empty." % (f, nf)
        print e
        exit()


def makeOutDirs(od):
    """ 
    Make the necessary dataset directories for the given format. 
    od is a train/test/val subdirectory within the output dataset directory.
    """
    if args.format.lower() == 'kitti':
        id = "%s/images" % od
        ld = "%s/labels" % od
    elif args.format.lower() == 'darknet':
        id = "%s/images" % od
        ld = "%s/annotations" % od
    else:
        print "Invalid output format %s!" % args.format
        usage()
    ensureDir(id)
    ensureDir(ld)
    return id, ld
    
    
def writeKitty(b, of):
    """ 
    Transform annoatations to KITTI format. 
    KITTY format info: https://github.com/NVIDIA/DIGITS/blob/v4.0.0-rc.3/digits/extensions/data/objectDetection/README.md
    Bounding box encoded as minx miny maxx maxy
    """
    with open(of, "w") as fh:
        for r in b:
            fh.write("%s 0 0 0 %d %d %d %d 0 0 0 0 0 0 0\n" % ( r[0], int(r[1]*dw), int(r[2]*dh), int(r[3]*dw), int(r[4]*dh) ) )
    
    
def writeVOC(b, ld, f):
    """ 
    Transform annotations to VOC format. 
    VOC uses XML format. An example can be seen in the VOCDevkit. We only include elements related to the bounding boxes, e.g.:
    <annotation>
        <folder>VOC2012</folder>
        <filename>2007_000042.jpg</filename>
        <source>
            <database>The VOC2007 Database</database>
            <annotation>PASCAL VOC2007</annotation>
            <image>flickr</image>
        </source>
        <size>
            <width>500</width>
            <height>335</height>
            <depth>3</depth>
        </size>
        <segmented>1</segmented>
        <object>
            <name>train</name>
            <bndbox>
                <xmin>263</xmin>
                <ymin>32</ymin>
                <xmax>500</xmax>
                <ymax>295</ymax>
            </bndbox>
        </object>
        <object>
            <name>train</name>
            <bndbox>
                <xmin>1</xmin>
                <ymin>36</ymin>
                <xmax>235</xmax>
                <ymax>299</ymax>
            </bndbox>
        </object>
    </annotation>
    """
    of = os.path.join(ld, f[: f.rfind('.')] + ".txt")
    with open(of, "w") as fh:
        dname = os.path.dirname(os.path.dirname(of))
        fh.write("""<annotation>
    <folder>%s</folder>
    <filename>%s</filename>
    <source>
        <database>The NVIDIA AI City 2017 dataset</database>
        <annotation>PASCAL VOC2007</annotation>
        <image>keyframes</image>
    </source>
    <size>
        <width>%d</width>
        <height>%d</height>
        <depth>3</depth>
    </size>
    <segmented>0</segmented>
""" % (dname, f, nwidth, nheight))
        for r in b:
            fh.write("""    <object>
        <name>%s</name>
        <bndbox>
            <xmin>%d</xmin>
            <ymin>%d</ymin>
            <xmax>%d</xmax>
            <ymax>%d</ymax>
        </bndbox>
    </object>
""" % ( r[0], int(r[1]*dw), int(r[2]*dh), int(r[3]*dw), int(r[4]*dh) ))
        fh.write("</annotation>")
    
    
def writeDarknet(b, of):
    """ 
    Transform annotations to Darknet format. 
    Darknet stores numeric class IDs followed by the center of the bounding box and width and height, all relative to the image dimensions.
    Class IDs start at 0.
    """
    with open(of, "w") as fh:
        for r in b:
            cx = (r[1]+r[3])*0.5*iw
            cy = (r[2]+r[4])*0.5*ih
            w = (r[3] - r[1])*iw
            h = (r[4] - r[2])*ih
            fh.write("%d %f %f %f %f\n" % ( labels[r[0]], cx*dw, cy*dh, w*dw, h*dh) )

    
def writeAnnotations(b, f, ld, n=0):
    """ Write annoation for a given image f in a file in directory ld """
    
    if args.format.lower() == 'kitti':
        writeKitty(b, os.path.join(ld, "%06d.txt" % n))
    elif args.format.lower() == 'voc':
        writeVOC(b, ld, f)
    elif args.format.lower() == 'darknet':
        writeDarknet(b, os.path.join(ld, "%06d.txt" % n))


def getClassCounts(b):
    """ Get counts for each class from existing list of bounding boxes for an image """
    c = {k:0 for k in labels.keys()}
    for r in b:
        c[r[0]] += 1
    return c
    

def usage():
    print "Usage: python2 create_dataset.py -d <dataset_path> [-f <format> -h <image_height> -w <image_width>] <output_dir>"
    exit()



if __name__ == '__main__':
    args = get_args();
    if not args.data:
        print "Please specify the output directory!"
        usage()
    outdir = args.data[0]
    if not args.dataset:
        print "Please specify the path to the dataset directory!"
        usage()

    # Find the dataset image size
    width, height, nwidth, nheight, resize = findWidthHeight()
    dw = dh = 1.
    if resize:
        dw = float(nwidth)/width
        dh = float(nheight)/height
    iw = 1./width
    ih = 1./height

    if outdir[0] != '/':
        outdir = os.path.join(os.getcwd(), outdir)
    ensureDir(outdir)
    
    os.chdir(args.dataset)

    if args.format.lower() != 'voc':
        for d in ["test", "val", "train"]:
            if os.path.isdir(d):
                print "Processing %s records..." % d
                os.chdir("%s/images" % d)
                cwd = os.getcwd()
                od = "%s/%s" % (outdir, d)
                id, ld = makeOutDirs(od)  # make image and labels dirs
                n = 1
                with open("%s/mapping.txt" % od, "w") as fh:
                    for f in sorted(os.listdir("./" )):
                        if f.endswith('.jpeg'):
                            # record file being processed in mapping
                            fh.write("%s\n" % f)
                            # read annoations
                            b = readAnnotations(f)
                            f = fn(f)
                            if resize:
                                resizeImage(f, id, n)
                            else:
                                linkImage(f, id, n)
                            writeAnnotations(b, f, ld, n)
                            n += 1
                os.chdir("../../")
    else:
        id = "%s/JPEGImages" % outdir
        ensureDir(id)
        ld = "%s/Annotations" % outdir
        ensureDir(ld)
        sd = "%s/ImageSets/Main" % outdir
        ensureDir(sd)
        cnts = {}
        for d in ["test", "val", "train"]:
            if os.path.isdir(d):
                print "Processing %s records..." % d
                os.chdir("%s/images" % d)
                cwd = os.getcwd()
                cnt = cnts[d] = {}
                for f in sorted(os.listdir("./" )):
                    if f.endswith('.jpeg'):
                        # read annoations
                        b = readAnnotations(f)
                        f = fn(f)
                        cnt[ f[: f.rfind('.') ] ] = getClassCounts(b)  # keep track of object counts
                        if resize:
                            resizeImage(f, id)
                        else:
                            linkImage(f, id)
                        writeAnnotations(b, f, ld)
                os.chdir("../../")
        # write out ImageSets
        for k,v in cnts.iteritems():
            # train.txt
            with open("%s/%s.txt" % (sd, k), "w") as fh:
                for i in sorted(v.keys()):
                    fh.write("%s\n" % i)
            cnt = cnts[k]
            for l in labels.keys():
                with open("%s/%s_%s.txt" % (sd, l.lower(), k), "w") as fh:
                    for i in sorted(v.keys()):
                        fh.write("%s %d\n" % (i, cnt[i][l]))

