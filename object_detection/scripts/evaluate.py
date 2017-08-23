#!/usr/bin/python2
"""
Evaluate Track 1 submissions for NVIDIA AI City 2017.
"""
import os
import sys
import json
import zipfile
import traceback
import numpy as np
import matplotlib.pyplot as plt
from operator import itemgetter
from itertools import groupby
from collections import defaultdict
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
}


TOCS = 20 # number of tocs
EMPTY = np.empty( shape=(0, 0) )
linestyles = ['-', '--', '-.', ':', '-']
chart_colors = ['#3366CC', # 0.  light blue
                '#DC3912', # 1.  orange
                '#FF9900', # 2.  yellow
                '#109618', # 3.  bright green
                '#990099', # 4.  bright purple
                '#3B3EAC', # 5.  mid blue
                '#0099C6', # 6.  turquoise
                '#DD4477', # 7.  pink
                '#66AA00', # 8.  pastel green
                '#B82E2E', # 9.  maroon
                '#316395', # 10. dark turquoise
                '#994499', # 11. pastel purple
                '#22AA99', # 12. aqua
                '#AAAA11', # 13. greenish yellow
                '#6633CC', # 14. dark purple
                '#E67300', # 15. dark yellow
                '#8B0707', # 16. dark maroon / brown
                '#329262', # 17. greenish blue
                'violet',  # 18.
                '#5574A6', # 19. muted blue
                'orange',  # 20.
                'gray',    # 21.
                'green',   # 22.
                '#3B3EAC', # 23. muted purple
                'red',     # 24. 
                '#5050A0', # 25. another muted dark blue
                'yellow',  # 26.
                '#9aeabb', # 27. green
                '#eec27f', # 28. orange
                '#90afe5', # 29. blue
                '#f6afaf', # 30. pink
                '#fbf8b6', # 31. yellow
                ]


def get_args():
    parser = ArgumentParser(add_help=False, usage=usageMsg())
    parser.add_argument("data", nargs=2, help="Path to <test_labels> <predicted_labels>.")
    parser.add_argument('--help', action='help', help='Show this help message and exit')
    parser.add_argument('-n', '--nsteps', type=int, default=100, help="Number of steps in recision recall graph. Set to 0 to display all points.")
    parser.add_argument('-i', '--interp', action='store_true', default=False, help="Interpolate precision-recall graph.")
    parser.add_argument('-g', '--graph', type=str, default=None, help="Prefix for precision-recall graphs.")
    parser.add_argument('-f', '--gfmt', type=str, default=None, help="Graph output format, e.g., pdf, png, jpeg.")
    parser.add_argument('-c', '--gcls', action='store_true', default=False, help="Create individual class graphs in addition to the composite graph containing all results.")
    parser.add_argument('-o', '--fout', type=str, default=None, help="Output file path for results in JSON format.")
    parser.add_argument('-w', '--minw', type=int, default=30, help="Minimum bbox width to consider.")
    parser.add_argument('-h', '--minh', type=int, default=30, help="Minimum bbox height to consider.")
    parser.add_argument('-t', '--iou', type=float, default=0.5, help="Minimum IOU threshold.")

    return parser.parse_args()


def usageMsg():
    return """  python2 evaluate.py <test_labels> <predicted_labels> [ -t <IOU_threshold> -i -n <nsteps> -g <graph_path+prefix> -f <graph_format> -c -o <results_file_path> -h <min_bbox_height> -w <min_bbox_width> ]

Labels are accepted in two formats:
\t- A directory or zip archive containing one file for each test image, with the same name as the image but 
\t  '.txt' extension. The text file will have one line for each predicted bounding box, in the following format:
\t\tclass xmin ymin xmax ymax confidence

\t- A JSON file containing a dictionary with elements for each image in the test set, in the following format:
{
    "great_neck_first_colonial_20140604_00016": [
        {
            "class": "Van", 
            "confidence": 0.93, 
            "xmax": 506.0, 
            "xmin": 424.0, 
            "ymax": 297.0, 
            "ymin": 252.0
        }, 
        {
            "class": "SUV", 
            "confidence": 0.24, 
            "xmax": 281.0, 
            "xmin": 179.0, 
            "ymax": 348.0, 
            "ymin": 293.0
        }
    ], 
}
\t  Note that image names do not have the file extension. Confidence scores are float vaules in the range[0,1]. 
\t  Bounding box coordinates are number of pixels from the lower-left corner of the image.

See `python2 evaluate.py --help` for more info.

"""

def usage():
    print "\nUsage:", usageMsg()
    exit()

def readLabels(f, fh, data):
    """ Read all bbox data from a .txt file """
    imgname = f[: f.rfind(".")]
    d = []
    for l in fh:
        p = l.strip().split()
        d.append({'class': p[0],
                  'xmin': float(p[1]),
                  'ymin': float(p[2]),
                  'xmax': float(p[3]),
                  'ymax': float(p[4]),
                  'confidence': float(p[5]) if len(p)>5 else 0.0})
    data[imgname] = d



def readAnns(path):
    """ Read annotations from an AIC formatted directory, archive, or JSON file """
    data = {}

    if path.endswith(".json"):
        if not os.path.isfile(path):
            raise ValueError("Invalid labels file path %s." % path)
        with open(path, "r") as fp:
            data = json.load(fp)
        if not isinstance(data, dict):
            raise ValueError("Invalid labels JSON file provided. Check the labels schema and try again.")
        it = None
        for k,v in data.iteritems():
            if not isinstance(v, list):
                raise ValueError("Invalid labels JSON file provided. Check the labels schema and try again.")
            if len(v) > 0:
                it = v[0]
                break
        if not it or any(not x in it for x in ['class', 'xmin', 'xmax', 'ymin', 'ymax', 'confidence']):
            raise ValueError("Invalid labels JSON file provided. Bounding box does not have correct keys.")

    elif path.endswith(".zip"):
        if not os.path.isfile(path):
            raise ValueError("Invalid labels archive path %s." % path)
        with zipfile.ZipFile(path) as z:
            for f in z.namelist():
                if f.endswith(".txt"):
                    with z.open(f) as fh:
                        readLabels(f, fh, data)
        
    elif os.path.isdir(path):
        for f in os.listdir(path):
            if f.endswith(".txt"):
                with open(os.path.join(path,f), "r") as fh:
                    readLabels(f, fh, data)

    else:
        raise ValueError('Invalid annotations file or path %s.' % path)

    # aggregate bounding box elements
    for k,v in data.iteritems():
        for i in xrange(len(v)):
            if v[i]['class'] not in labels:
                raise ValueError('Invalid class values %s. Classes shpould be Car, SUV, SmallTruck, etc.' % v[i]['class'])
            if v[i]['xmax'] < v[i]['xmin'] or v[i]['ymax'] < v[i]['ymin']:
                raise ValueError("Incorrect bounding box format. Encountered max value smaller than min value.")
            data[k][i]['bbox'] = np.array([ v[i]['xmin'], v[i]['ymin'], v[i]['xmax'], v[i]['ymax'] ])

    return data


def valid(pred, test):
    """ Check that we have valid labels """
    if sorted(pred.keys()) != sorted(test.keys()):
        raise ValueError('Invalid list of images. Check that label keys or file names match those of the test images.')


def countBoxes(dat):
    """ Count the number of bboxes for each class in the set dat. """
    cnt = defaultdict(int)
    for k,v in dat.iteritems():
        for i in v:
            cnt[i['class']] += 1;
    return cnt


def evaldet(test, pred, tcnt, pcnt, cls=None):
    """ Evaluate Localization + Classification for a given class.
        Function assumes bboxes pred list are sorted in decreasing confidence level
        and bboxes in each test value are sorted in bbox shape order.
        We follow the same strategy as in the Pascal VOC 2012 (see VOCevaldet.m).
        If cls is None, compute values ignoring class participation (i.e., Localization only).
    """
    print("Evaluating %s" % ("class "+cls if cls else "Localization")), 
    sys.stdout.flush()

    nd = pcnt[cls] if cls else np.sum(pcnt.values())
    if nd == 0:
        print ""
        return 0, 0, EMPTY, EMPTY, EMPTY, EMPTY
    tp = np.zeros(nd, dtype=np.float)
    fp = np.zeros(nd, dtype=np.float)
    tc = int(np.ceil(nd/float(TOCS)))  # tic
    d = 0
    for bbo in pred:
        if cls and bbo['class'] != cls:
            continue

        # display progress
        if d % tc == 0:
            print("."),
            sys.stdout.flush()

        # find ground truth image and restrict bboxes by class
        im = bbo['image']
        gtbbs = test[im]
        
        bb = bbo['bbox']
        ovmax = -np.inf
        jmax = -1
        # assign detection to ground truth if any
        for j in xrange(len(gtbbs)):
            if cls and gtbbs[j]['class'] != cls:
                continue
            bbgt = gtbbs[j]['bbox']
            if bbgt[0] > bb[2]:  # ground truth xmin > bbox xmax
                break
            if bbgt[1] > bb[3]:  # ground truth ymin > bbox ymax
                continue
            bi = [ max(bb[0],bbgt[0]), max(bb[1],bbgt[1]), min(bb[2],bbgt[2]), min(bb[3],bbgt[3]) ]
            iw = bi[2]-bi[0]+1
            ih = bi[3]-bi[1]+1
            ov = 0.0
            if iw > 0 and ih > 0:
                # compute overlap as area of intersection / area of union
                ua = (bb[2]-bb[0]+1)*(bb[3]-bb[1]+1) + \
                     (bbgt[2]-bbgt[0]+1)*(bbgt[3]-bbgt[1]+1) - \
                     iw * ih
                ov = iw * ih / float(ua)
            
            if ov > ovmax:
                ovmax = ov
                jmax = j


        # assign detection as test positive, don't care, or false positive
        if ovmax >= args.iou:
            if 'det' not in gtbbs[jmax] or not gtbbs[jmax]['det']:
                tp[d] = 1   # test positive
                gtbbs[jmax]['det'] = True
            else:
                fp[d] = 1   # false positive (multiple detection)
        else:
            fp[d] = 1       # false positive

        d += 1

    print ""
    sys.stdout.flush()

    assert(d == nd)

    # compute F1-score
    npos = float(tcnt[cls]) if cls else np.sum(tcnt.values())
    p    = np.sum(tp)/float(np.sum(tp)+np.sum(fp))
    r    = np.sum(tp)/npos
    fs   = 2.0*p*r/(p+r) if (p+r > 0) else 0.0

    # compute pprecision-recall curve and AP
    fp   = np.cumsum(fp)
    tp   = np.cumsum(tp)
    rec  = tp/npos
    prec = tp/(fp+tp)
    ap, mpre, mrec = computeAP(prec, rec)

    return ap, fs, prec, rec, mpre, mrec



def computeAP(prec, rec):
    """ Compute average precision, given cumulative sum vectors for precision and recall """
    # add limit points
    mrec = np.r_[0, rec, 1]
    mpre = np.r_[0, prec, 0]
    # compute interpolated precision
    n = len(mpre)
    for i in xrange(n-2,-1,-1):
        mpre[i] = max(mpre[i], mpre[i+1])
    # find recall change points and compute ap
    ap = 0.0
    j  = 0
    for i in xrange(1,n):
        if mrec[i] != mrec[j]:
            ap += (mrec[i]-mrec[j]) * mpre[i]
            j = i

    return ap, mpre[1:-1], mrec[1:-1]
    

def reduceSteps(prec, rec, nsteps):
    """ Reduce the number of points being displayed in the precision-recall graph to max nsteps. """
    if not nsteps or len(rec) <= nsteps or not (args.graph or args.fout):
        return prec, rec
    p = int(np.ceil(len(rec)/float(nsteps)))
    s = [p] * int(len(rec)/float(p))
    s = np.r_[0, np.cumsum(s)].tolist()
    if s[-1] == len(rec):
        s = s[:-1]
    if s[-1] < len(rec)-1:
        s.append(len(rec)-1)

    return prec[s], rec[s]


def createGraph(results, cls=None, meanAP=None):
    """ Create a precision-recall graph for a given class or all classes. """

    fig, ax = plt.subplots(1)
    plt.rc('legend',**{'fontsize':11})
    plt.tick_params(axis='both', which='major', labelsize=12)
    
    ax.set_xlim(0, 1)
    ax.set_ylim(0, 1)

    def pltcls(cls, i=0):
        if cls not in results:
            return
        r = results[cls]
        rec = r['recall']
        prec = r['precision']
        if len(r['recall']) == 0:
            rec = prec = [None]
        plt.plot(rec, prec, 
            label="%s [AP %.2f, F1 %.2f]" % (cls, r['AP'], r['F1-score']),
            linestyle=linestyles[i % len(linestyles)],
            marker=None,
            color=chart_colors[i+3],
            linewidth=2
        )
    
    ly = 1.015
    if cls:
        r = results[cls]
        plt.plot(r['recall'], r['precision'], label=None)
        plt.title("%s, AP %.2f, F1 %.2f" % (cls, r['AP'], r['F1-score']))
        fname = args.graph+"_"+cls+"."+args.gfmt
    else:
        fname = args.graph+"."+args.gfmt
        pltcls('Localization')
        for i,cls in enumerate(sorted(results.keys())):
            if cls == 'Localization':   # we want it last
                continue
            pltcls(cls, i+1)
        if meanAP:
            plt.text(1.01, 0.97, 'mAP=%.2f' % meanAP, transform=ax.transAxes)
            ly = 0.96

    plt.xlabel("Recall", fontweight='bold', fontsize=12)
    plt.ylabel("Precision", fontweight='bold', fontsize=12)
    plt.tight_layout(pad=0, w_pad=0, h_pad=0)
    plt.legend(loc='upper left', bbox_to_anchor=(1, ly))

    fig.savefig(fname, format=args.gfmt, bbox_inches='tight')
    print "Created graph %s" % fname


def eval(testpath, predpath):
    """ Evaluate Localization + Classification in images (video keyframes) """

    print "Reading label files..."
    print "True: %s" % testpath
    test = readAnns(testpath)
    print "Pred: %s" % predpath
    pred = readAnns(predpath)

    # make sure we have predicted annotations for the right images
    print "Checking labels..."
    sys.stdout.flush()
    valid(pred, test)
   
    # eliminate bboxes smaller than threshold
    for k,v in test.iteritems():
        test[k] = [x for x in v if x['xmax'] - x['xmin'] >= args.minw and x['ymax'] - x['ymin'] >= args.minh]
    for k,v in pred.iteritems():
        pred[k] = [x for x in v if x['xmax'] - x['xmin'] >= args.minw and x['ymax'] - x['ymin'] >= args.minh]

    # sort test bboxes in increasing x, y order
    print "Sorting labels..."
    dups = False
    for k,v in test.iteritems():
        # eliminate duplicate bounding boxes if any
        vs = {"%s_%f_%f_%f_%f" % (x['class'], x['xmin'], x['ymin'], x['xmax'], x['ymax']): x for x in v}
        if(len(vs.keys()) < len(v)):
            dups = True
            v = sorted(vs.values(), key=lambda x: (x['xmin'], x['ymin'], x['xmax'], x['ymax']))
        else:
            v = sorted(v, key=lambda x: (x['xmin'], x['ymin'], x['xmax'], x['ymax']))
        test[k] = v
    if dups:
        print "Warning: test labels data contains duplicate bounding boxes."

    # get bbox counts for each class
    tcnt = countBoxes(test)
    pcnt = countBoxes(pred)

    # make list of predicted bboxes and sort in decreasing confidence level order
    sys.stdout.flush()
    for k,v in pred.iteritems():
        for i in v:
            i['image'] = k
    pred = sorted([i for v in pred.values() for i in v], key=lambda x: x['confidence'], reverse=True)
    
    # evaluate detection + localization + classification
    results = {}
    print ""
    for cls in sorted(tcnt.keys()):
        ap, fs, prec, rec, mpre, mrec = evaldet(test, pred, tcnt, pcnt, cls)
        prec, rec = reduceSteps(prec, rec, args.nsteps)
        mpre, mrec = reduceSteps(mpre, mrec, args.nsteps)
        results[cls] = {
            'AP': ap,
            'F1-score': fs,
            'precision': mpre if args.interp else prec,
            'recall': mrec if args.interp else rec
        }

    # clear detected true bboxes
    for k,v in test.iteritems():
        for i in v:
            i['det'] = False

    # evaluate overall localization
    ap, fs, prec, rec, mpre, mrec = evaldet(test, pred, tcnt, pcnt)
    prec, rec = reduceSteps(prec, rec, args.nsteps)
    mpre, mrec = reduceSteps(mpre, mrec, args.nsteps)
    results['Localization'] = {
        'AP': ap,
        'F1-score': fs,
        'precision': mpre if args.interp else prec,
        'recall': mrec if args.interp else rec
    }
    meanAP = np.mean([v['AP'] for v in results.values()])

    print ""
    print "class\tAP\tF1-score"
    for cls in sorted(tcnt.keys())+['Localization']:
        print "%s\t%.6f\t%.6f" % (cls, results[cls]['AP'], results[cls]['F1-score'])
        if args.graph and args.gcls:
            createGraph(results, cls)

    # create composite graph
    if args.graph:
        createGraph(results, meanAP=meanAP)

    print "\nOverall mAP: %.6f" % meanAP
    results['mAP'] = meanAP

    if args.fout:
        # write restuls to JSON output file
        for cls in tcnt.keys()+['Localization']:
            results[cls]['precision'] = results[cls]['precision'].tolist()
            results[cls]['recall'] = results[cls]['recall'].tolist()
        with open(args.fout, "w") as fh:
            json.dump(results, fh, sort_keys=True, indent=4)
        print "Wrote results to %s" % args.fout
    print ""
    

if __name__ == '__main__':
    args = get_args();
    if not args.data or len(args.data) < 2:
        usage()

    if args.graph:
        if not args.gcls and args.gfmt is None:
            # try to derive graph format form extension
            args.gfmt = args.graph[args.graph.rfind(".")+1 :]
            if args.gfmt not in plt.gcf().canvas.get_supported_filetypes().keys():
                print "Error: Invalid graph format %s." % args.gfmt
                usage()
        if os.path.isdir(args.graph):
            args.graph = os.path.join(args.path, "graph")
        if args.graph.endswith(args.gfmt):
            args.graph = args.graph[:-len(args.gfmt)-1]
    if args.fout and os.path.isdir(args.fout):
        args.fout = os.path.join(args.fout, "results.json")

    try:
        eval(args.data[0], args.data[1])
    except Exception as e:
        print ""
        print "Error: %s" % e
        usage()

