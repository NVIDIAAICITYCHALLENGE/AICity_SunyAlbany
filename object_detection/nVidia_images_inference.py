import numpy as np
import os
import six.moves.urllib as urllib
import sys
import tarfile
import tensorflow as tf
import zipfile
import csv

from collections import defaultdict
from io import StringIO
from matplotlib import pyplot as plt
from PIL import Image

sys.path.append("..")

from utils import label_map_util

from utils import visualization_utils as vis_util


# Path to frozen detection graph. This is the actual model that is used for the object detection.
#PATH_TO_CKPT = MODEL_NAME + 'frozen_inference_graph.pb'

PATH_TO_CKPT = 'models/model' + '/aic480_inference_graph.pb'

# List of the strings that is used to add correct label for each box.
PATH_TO_LABELS = os.path.join('data', 'aic_label_map.pbtxt')

NUM_CLASSES = 15  # aic


detection_graph = tf.Graph()
with detection_graph.as_default():
    od_graph_def = tf.GraphDef()
    with tf.gfile.GFile(PATH_TO_CKPT, 'rb') as fid:
        serialized_graph = fid.read()
        od_graph_def.ParseFromString(serialized_graph)
        tf.import_graph_def(od_graph_def, name='') 

#print 'import graph'

label_map = label_map_util.load_labelmap(PATH_TO_LABELS)
categories = label_map_util.convert_label_map_to_categories(label_map, max_num_classes=NUM_CLASSES, use_display_name=True)
category_index = label_map_util.create_category_index(categories)
#print 'category_index:', category_index

def load_image_into_numpy_array(image):
  (im_width, im_height) = image.size
  return np.array(image.getdata()).reshape(
      (im_height, im_width, 3)).astype(np.uint8) 

# For the sake of simplicity we will use only 2 images:
# image1.jpg
# image2.jpg
# If you want to test the code with your images, just add path to the images to the TEST_IMAGE_PATHS.
PATH_TO_TEST_IMAGES_DIR = '/media/TEST_IMAGE_PATHS'

# Size, in inches, of the output images.
IMAGE_SIZE = (12, 8)

writer_path = './anatation_output/'
#writer_file = writer_path + 'aic_20170602_006' + '_Det_Jon.txt'
#writer = csv.writer(open(writer_file, 'w'))
n = 0
with detection_graph.as_default():
  with tf.Session(graph=detection_graph) as sess:
    with open('480list.txt', 'r') as csvfile:
        reader = csv.reader(csvfile)
        for row in reader:
            #curr = row[0][-14:-11]
            #PATH_TO_TEST_IMAGES_DIR = '/home/nenghui/Projects/nVidia/datasets/aic1080/val/images/' + row[0]  
	    
	    name, extension = row[0].split('.')
            writer_file = writer_path + name + '.txt'
	    print writer_file
            writer = csv.writer(open(writer_file, 'w'))  
            image_path = os.path.join(PATH_TO_TEST_IMAGES_DIR, row[0])
            print n, '-----image_path:', image_path 
            image = Image.open(image_path)
            image_np = load_image_into_numpy_array(image)
            image_np_expanded = np.expand_dims(image_np, axis=0)
            image_tensor = detection_graph.get_tensor_by_name('image_tensor:0')
            boxes = detection_graph.get_tensor_by_name('detection_boxes:0')
            scores = detection_graph.get_tensor_by_name('detection_scores:0')
            classes = detection_graph.get_tensor_by_name('detection_classes:0')
            num_detections = detection_graph.get_tensor_by_name('num_detections:0')
            (boxes, scores, classes, num_detections) = sess.run(
                    [boxes, scores, classes, num_detections], feed_dict={image_tensor: image_np_expanded})
            boxes_out, scores_out, class_out = vis_util.visualize_boxes_and_labels_on_image_array(image_np,np.squeeze(boxes),np.squeeze(classes).astype(np.int32),\
                    np.squeeze(scores),category_index,use_normalized_coordinates=True,line_thickness=4,  min_score_thresh=0.05, max_boxes_to_draw=None)
            #vis_util.visualize_boxes_and_labels_on_image_array(image_np,np.squeeze(boxes),np.squeeze(classes).astype(np.int32),\
             #       np.squeeze(scores),category_index,use_normalized_coordinates=True,line_thickness=4)
            #plt.figure(figsize=IMAGE_SIZE)
            #plt.imshow(image_np)
            #plt.show()
            #print 'frame:', frame
            n = n + 1
            for i, score in enumerate(scores_out):
                if class_out[i] != 'None':
                    writer.writerow(( class_out[i], boxes_out[i][0], boxes_out[i][1], boxes_out[i][2], boxes_out[i][3], score))
                    #print class_out[i], boxes_out[i][0], boxes_out[i][1], boxes_out[i][2], boxes_out[i][3], score
            #for i, score in enumerate(scores_out):
            #    writer.writerow(( frame, i+1, boxes_out[i][0], boxes_out[i][1], boxes_out[i][2], boxes_out[i][3], score, class_out[i]))
            #n = n + 1                    
