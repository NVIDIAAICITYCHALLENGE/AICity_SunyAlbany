# Python PySide OpenCV Detection Tracking Video Command-Line App
# Author:  Ming-Ching Chang
# Version / Development Log:
#   2017/07/25

import numpy as np

import cv2
import cv
import sys

from timeit import default_timer as timer

# ========== General Parameters ==========

# verbose print level:
#   0: no print out
#   1: succinct
#   2: verbose
#   3: detailed info
#   4: detailed debug info
VERBOSE = 1

INPUT_VIDEO = ''
INPUT_TRACKING_FILE = ''

START_FRAME_IDX = 1
SHOW_DETECTION_ID = 0
SHOW_DETECTION_CONF = 1

VIZ_TEXT_SCALE = 1.0

SAVE_VIZ_VIDEO = 1
VIZ_VIDEO_NAME = '_VizOut.avi'
VIZ_VIDEO_FOURCC = cv2.cv.CV_FOURCC(*'XVID')  # -1 for default

LOOP_VIDEO = 0
START_VIDEO_PLAYING = 1

# ========== Run Cases for Each Dataset ==========

if 0: # Virginia  - Day View
  INPUT_VIDEO = 'princess_anne_lynnhaven_20140604_1.avi'
  INPUT_TRACKING_FILE =  'princess_anne_lynnhaven_20140604_Track.txt'
  VIZ_TEXT_SCALE = 0.5

elif 0: # Day View
  INPUT_VIDEO = 'stevens_cr_winchester_20170605_1.mp4'
  INPUT_TRACKING_FILE =  'stevens_cr_winchester_20170605_1_Track.txt'

elif 0: # Night View
  INPUT_VIDEO = 'walsh_santomas_20170602_027.mp4'
  INPUT_TRACKING_FILE =  'walsh_santomas_20170602_027_Track.txt'

elif 0: # Day View
  INPUT_VIDEO = 'walsh_santomas_20170602_047.mp4'
  INPUT_TRACKING_FILE =  'walsh_santomas_20170602_047_Track.txt'

elif 1: # Night View
  INPUT_VIDEO = 'walsh_santomas_20170603_016.mp4'
  INPUT_TRACKING_FILE =  'walsh_santomas_20170603_016_Track.txt'

elif 0: # Day View
  INPUT_VIDEO = 'walsh_santomas_20170603_019.mp4'
  INPUT_TRACKING_FILE =  'walsh_santomas_20170603_019_Track.txt'

else:
  pass

# ========== Post-config calculations ===========
  
TW = int (72 * VIZ_TEXT_SCALE) # text str width
TH = int (24 * VIZ_TEXT_SCALE) # text str height
THS = int (6 * VIZ_TEXT_SCALE) # text str shift


# ========== Video Modules ==========

# Read a specified frame from a video
# result is numpy img[y,x,c] in RGB
def vid_get_frame_i (video_in, frame_num):
  vidfp = cv2.VideoCapture (video_in)
  vidfp.set (cv.CV_CAP_PROP_POS_FRAMES, frame_num)
  _,bgr_img = vidfp.read()
  if bgr_img is None:
    return bgr_img
  else:
    b,g,r = cv2.split (bgr_img)       # get b,g,r
    rgb_img = cv2.merge ([r,g,b])     # switch it to rgb
  return rgb_img

def vid_goto_frame (vidfp, frame_num):
  vidfp.set (cv.CV_CAP_PROP_POS_FRAMES, frame_num)

def vid_read_frame (vidfp):
  _,bgr_img = vidfp.read()
  if bgr_img is None:
    return bgr_img
  else:
    b,g,r = cv2.split (bgr_img)       # get b,g,r
    rgb_img = cv2.merge ([r,g,b])     # switch it to rgb
  return rgb_img

# Read an image sequence from the video
# result is numpy vid[t,y,x,c] in RGB
def vid_read_imgseq (video_in):
  print 'vid_read_imgseq (%s)' % (video_in)
  vidfp = cv2.VideoCapture (video_in)
  imgseq = []
  count = 0
  while True:
    _,bgr_img = vidfp.read()
    if bgr_img is None:
      break
    b,g,r = cv2.split (bgr_img)       # get b,g,r
    rgb_img = cv2.merge ([r,g,b])     # switch it to rgb
    imgseq.append (rgb_img)
    print '%d' % (count),
    count += 1
  print ''
  return np.array (imgseq)

# Read an image sequence from the video
# result is numpy vid[t,y,x]
def vid_read_imgseq_gray (video_in):
  print 'vid_read_imgseq_gray (%s)' % (video_in)
  vidfp = cv2.VideoCapture (video_in)
  imgseq = []
  count = 0
  while True:
    _,bgr_img = vidfp.read()
    if bgr_img is None:
      break
    gray = cv2.cvtColor (bgr_img, cv.CV_RGB2GRAY)
    imgseq.append (gray)
    print '%d' % (count),
    count += 1
  print ''
  return np.array (imgseq)

# input imgseq is [t,y,x,c] in BGR, output is [t,y,x]
def imgseq_color_to_gray (imcseq):
  NF = imcseq.shape[0]
  NY = imcseq.shape[1]
  NX = imcseq.shape[2]
  imgseq = np.zeros ((NF, NY, NX), 'uint8')
  for f in ranges (NF):
    bgr_img = imcseq[f]
    gray = cv2.cvtColor (bgr_img, cv.CV_RGB2GRAY)
    imgseq[f] = gray
  return imgseq

# count total number of frames in the input video
'''def vid_count_num_frames (video_in):
  vidfp = cv2.VideoCapture (video_in)
  if not vidfp.isOpened():
    print "could not open :",video_in
    return

  length = int(vidfp.get(cv2.cv.CV_CAP_PROP_FRAME_COUNT))
  width  = int(vidfp.get(cv2.cv.CV_CAP_PROP_FRAME_WIDTH))
  height = int(vidfp.get(cv2.cv.CV_CAP_PROP_FRAME_HEIGHT))
  fps    = vidfp.get(cv2.cv.CV_CAP_PROP_FPS)
  return length
  '''

# count total number of frames in the input video
def vid_count_num_frames (vidfp):
  if not vidfp.isOpened():
    print "could not open :",vidfp
    return

  length = int(vidfp.get(cv2.cv.CV_CAP_PROP_FRAME_COUNT))
  width  = int(vidfp.get(cv2.cv.CV_CAP_PROP_FRAME_WIDTH))
  height = int(vidfp.get(cv2.cv.CV_CAP_PROP_FRAME_HEIGHT))
  format = int(vidfp.get(cv2.cv.CV_CAP_PROP_FORMAT))
  if format == cv2.cv.CV_8UC1:
    channel = 1
  elif format == cv2.cv.CV8UC3:
    channel = 3
  else:
    channel = 0 # !! Wrong
  fps    = vidfp.get(cv2.cv.CV_CAP_PROP_FPS)
  return length, width, height, channel

# count total number of frames in the input video
# This is the dumb way (slow). Better solution?
def vid_count_num_frames_bad (video_in):
  vidfp = cv2.VideoCapture (video_in)
  count = 0
  while True:
    _,img = vidfp.read()
    if img is None:
      break
    count += 1
  return count

# write [t,y,x,c] numpy array to video.avi
# See http://docs.opencv.org/trunk/doc/py_tutorials/py_gui/py_video_display/py_video_display.html
def write_imgseq_to_video (imgseq, name):
  #NT = imgseq.shape[0]
  NY = imgseq.shape[1]
  NX = imgseq.shape[2]
  #fourcc = cv2.VideoWriter_fourcc(*'XVID')
  #fourcc = cv2.cv.CV_FOURCC('M','J','P','G')
  fourcc = cv2.cv.CV_FOURCC('X','V','I','D')
  fps = 25
  isColor = False
  if imgseq.ndim > 3:
    isColor = True
  vw = cv2.VideoWriter (name, fourcc, fps, (NX,NY), isColor)
  for rgb_img in imgseq:
    #convert rgb_img to bgr_img
    r,g,b = cv2.split (rgb_img)
    bgr_img = cv2.merge ([b,g,r])
    vw.write (bgr_img)
  vw.release()


# ========== App - Start ==========

class App:
  def __init__ (self, parent=None):

    # ===== Initialization =====
    global INPUT_VIDEO
    self.INPUT_VIDEO = INPUT_VIDEO
    self.FI = 0 # current frame index
    self.FPS = 0

    print 'Opening %s' % (self.INPUT_VIDEO)
    self.VIDFP = cv2.VideoCapture (self.INPUT_VIDEO)
    self.NF, self.NX, self.NY, self.NC = vid_count_num_frames (self.VIDFP)
    self.NF = self.NF + 1
    self.FI = 0
    self.CUR_FRAME = None
    print '  %s %d frames [y%d,x%d] color%d in RGB' % (self.INPUT_VIDEO, self.NF, self.NY, self.NX, self.NC)

    # Read in INPUT_TRACKING_FILE file (if available)
    global INPUT_TRACKING_FILE
    if INPUT_TRACKING_FILE:
      print 'Reading Tracking File %s' % (INPUT_TRACKING_FILE)
      self.TRACKING_DATA = np.loadtxt (INPUT_TRACKING_FILE, dtype=np.int32)
      fn = self.TRACKING_DATA[-1, 0]     # total frame numbers
      print("Total tracking data frame numbers: ", fn)

    global SAVE_VIZ_VIDEO
    global VIZ_VIDEO_NAME
    global VIZ_VIDEO_FOURCC
    if SAVE_VIZ_VIDEO:
      self.VizVid = cv2.VideoWriter (
        INPUT_VIDEO+VIZ_VIDEO_NAME,
        VIZ_VIDEO_FOURCC,
        30.0, # FPS
        (self.NX, self.NY))

  def step_frame (self):
    ts = timer()
    self.CUR_FRAME = vid_read_frame (self.VIDFP)
    if self.CUR_FRAME is None:
      if LOOP_VIDEO:
        self.FI = 0
        vid_goto_frame (self.VIDFP, self.FI)
        self.CUR_FRAME = vid_read_frame (self.VIDFP)
      else:
        print 'End of input video. Done!'
        return False
    else:
      self.FI = self.FI + 1

    if VERBOSE>1:
      print '  frame %d' % (self.FI)
    f_str = '%d' % (self.FI)
    te = timer()
    elapsed_t = te - ts
    self.FPS = 1.0/elapsed_t
    return True

  #Draw tracking results
  def draw_tracking (self):
    if self.TRACKING_DATA is None:
      return
    ith_frame = self.FI + START_FRAME_IDX
    TBoxes = self.TRACKING_DATA[np.argwhere(self.TRACKING_DATA[:,0] == ith_frame), :]
    TBoxes = TBoxes.reshape (TBoxes.shape[0], TBoxes.shape[2])

    Tcolordict = [(255,0,0), (0,255,0), (0,0,255), (255,255,0), (0,255,255), (255,0,255), (128,0,0), (128,0,128)]

    for b in TBoxes:
      # b[2] b[3] is box (X0,Y0)
      X0 = b[2]
      Y0 = b[3]
      # b[4],b[5] is box W, H
      W = b[4]
      H = b[5]
      # b[1] is the track id
      ID = str(b[1])
      # box extention
      BE = 2
      cv2.rectangle (self.CUR_FRAME,
                     (X0-BE, Y0-BE), # upper-left pos
                     (X0+W+BE, Y0+H+BE), # bottom-right pos
                     Tcolordict[b[1]%8], # color
                     2) # line type

      #cv2.rectangle (self.CUR_FRAME, (X0,Y0-TH-THS), (X0+TW,Y0-THS),
      #               (0,0,0), -1) # -1 to draw a filled rect
      cv2.putText (self.CUR_FRAME,
                   ID,
                   (X0, Y0-THS),
                   cv2.FONT_HERSHEY_SIMPLEX,
                   VIZ_TEXT_SCALE, # font scale
                   Tcolordict[b[1]%8], # color (244,164,96),
                   2) # line type

# ========== App - End ==========

def main():
  app_init_time = timer()
  global INPUT_VIDEO
  global INPUT_TRACKING_FILE
  if len(INPUT_VIDEO) == 0 :
    if len(sys.argv) == 1:
      print 'Run: <video.avi> [tracking.txt]'
      sys.exit()

  if len(sys.argv) > 1:
    # Use the command-line input file
    INPUT_VIDEO = str(sys.argv[1])
  else:
    # Use the pre-defined INPUT_VIDEO
    pass
  print 'INPUT_VIDEO = %s' % (INPUT_VIDEO)

  if len(sys.argv) > 2:
    # Use the command-line input file
    INPUT_TRACKING_FILE = str(sys.argv[2])
  else:
    # Use the pre-defined INPUT_TRACKING_FILE
    pass
  print 'INPUT_TRACKING_FILE = %s' % (INPUT_TRACKING_FILE)

  app = App ()

  # ======= while loop ============
  while app.FI < app.NF:
    ret = app.step_frame ()
    if ret:
      app.draw_tracking ()
      # Save viz fame to the output VizVid
      if SAVE_VIZ_VIDEO:
        app.VizVid.write (app.CUR_FRAME)
        print 'F %d (total NF %d), %.2f FPS' % (app.FI, app.NF, app.FPS)

    else:
      print 'Video end. Exit!'
      break

  app.VizVid.release()
  print 'Viz Video written done'
  te = timer()
  app_total_time = (te - app_init_time)/60
  print '--> App total elapsed time = %.2f min' % (app_total_time)

if __name__ == "__main__":
  main()
