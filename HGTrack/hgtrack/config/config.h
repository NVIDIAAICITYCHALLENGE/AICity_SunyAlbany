//config.h   The configuration of the multiple pedestrian tracker
/*
 * GraphMultiCarTracker
 * Do not distribute
 *  Created on: Nov 08, 2015
 *      Author: Longyin Wen
 */

#ifndef _CONFIG_H
#define _CONFIG_H

#include <util/defs.h>

//Loading image name.
extern string G_INPUT_DATAFILE_NAME;
//Loading file path.
extern string G_INPUT_VIDEOFILE_PATH;
//Loading detection results path
extern string G_INPUT_DETECTFILE_PATH;

//detection types: by detector or read from files
extern string G_OUTPUT_VIDEOFILE_PATH;
extern string G_OUTPUT_SPEEDFILE_PATH;
extern string G_OUTPUT_TRAJFILE_PATH;


//图像大小参数
extern int G_IMAGE_WIDTH;
extern int G_IMAGE_HEIGHT;


//Total frame number
extern int G_TOTAL_FRAME_NUMBER;

//-------------------------------------------------------------------------------------------------------------//
//Car minimal size
extern int G_CAR_MIN_WIDTH;
extern int G_CAR_MIN_HEIGHT;

//It is a sequence integral values.
extern int G_SEGMENT_SIZE[2];

//The number of objects in the hyperedge.
extern int G_TRACK_KARY;


//--------------------------------------- Some Parameters Used for Debug -----------------------------------//
extern int G_DEBUG_MARK; //1: save the debug information; 0: do not save the debug information.
extern string G_TRACKSIMAGEPATH_DEBUG;

//determine whether draw the parts.
extern string G_TARGETIMAGE_PATH_DEBUG;
extern string G_OCCUPYIMAGE_PATH_DEBUG;


//We only remains some important parameters for turning.
bool LoadConfig(const char* configfile);

#endif