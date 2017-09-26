# Steps of running code

# 0. Git clone the code

git clone https://github.com/NVIDIAAICITYCHALLENGE/AICity_SunyAlbany.git

cd AICity_SunyAlbany

export PYTHONPATH=$PYTHONPATH:`pwd`:`pwd`/slim

# 1. Preparing Inputs (datasets)
Tensorflow Object Detection API reads data using the TFRecord file format. One sample scripts (create_pascal_tf_record.py) are provided to convert from the PASCAL VOC dataset to TFRecords.

python object_detection/create_pascal_tf_record.py \
    --label_map_path=object_detection/data/aic_label_map.pbtxt \
    --data_dir=aic_dataset --year=VOC2007 --set=train \
    --output_path=aic_train.record

You should end up with one TFRecord files named aic_train.record in the tensorflow/models directory.
Please put the TFRecord file in /object_detection/data/

The label map for the AIC VOC data set can be found at object_detection/data/aic_label_map.pbtxt.    


# 2. Configuring the Training Pipeline

The Tensorflow Object Detection API uses protobuf files to configure the training and evaluation process. The schema for the training pipeline can be found in object_detection/protos/pipeline.proto. At a high level, the config file is split into 5 parts:

	1,The model configuration. This defines what type of model will be trained (ie. meta-architecture, feature extractor).
	2,The train_config, which decides what parameters should be used to train model parameters (ie. SGD parameters, input preprocessing and feature extractor initialization values).
	3,The eval_config, which determines what set of metrics will be reported for evaluation (currently we only support the PASCAL VOC metrics).
	4,The train_input_config, which defines what dataset the model should be trained on.
	5,The eval_input_config, which defines what dataset the model will be evaluated on. Typically this should be different than the training input dataset.

To help you get started, sample model configurations have been provided in the object_detection/samples/model_configs folder,
Our configuration file can be found in object_detection/models/model/faster_rcnn_resnet101_aic.config
Please make sure you change the train data and val data path in this config file.      


# 3. Training Locally

Before you training, you can download a pretrained model to speed up your training from here:
https://github.com/tensorflow/models/blob/master/object_detection/g3doc/detection_model_zoo.md

Recommended Directory Structure for Training and Evaluation
+data
     -label_map file
     -train TFRecord file
     -eval TFRecord file
+models
     + model
        -pipeline config file
        +train
        +eval

A local training job can be run with the following command:

From the tensorflow/models/ directory
python object_detection/train.py \
    --logtostderr \
    --pipeline_config_path=./object_detection/models/model/faster_rcnn_resnet101_aic.config \
    --train_dir=./object_detection/models/model/train/
where pipeline_config_path points to the pipeline config and train_dir points to the directory in which training checkpoints and events will be written to. By default, the training job will run indefinitely until the user kills it.


# 4. Exporting a trained model for inference

After our model has been trained, you should export it to a Tensorflow graph proto. A checkpoint will typically consist of three files:

model.ckpt-${CHECKPOINT_NUMBER}.data-00000-of-00001,
model.ckpt-${CHECKPOINT_NUMBER}.index
model.ckpt-${CHECKPOINT_NUMBER}.meta
After you've identified a candidate checkpoint to export, run the following command from tensorflow/models/object_detection:

From tensorflow/models

python object_detection/export_inference_graph \
    --input_type image_tensor \
    --pipeline_config_path ./object_detection/models/model/faster_rcnn_resnet101_aic.config \
    --checkpoint_path ./object_detection/models/model/train/model.ckpt-xxxxxx \
    --inference_graph_path aic_inference_graph.pb
    
Afterwards, you should see a graph named aic_inference_graph.pb.
You can download our trained model from https://www.dropbox.com/s/nnbt8r2c470ylq1/aic_inference_model.pb?dl=0

# 5. inference test set

From object_detection, change your test dataset and output direction

python nVidia_images_inference.py   