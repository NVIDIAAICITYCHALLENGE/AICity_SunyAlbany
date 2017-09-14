/*
 * Parameter.h
 * Do not distribute
 *  Created on: Nov 08, 2015
 *      Author: Longyin Wen
 */
//Define some default parameters.
#ifndef _PARAMETER_H
#define _PARAMETER_H


#include <util/defs.h>

//********************************************************************************************//
//-------------------------------------------------------------------------------------------//
//--- Main Parameters for Optimization
//-------------------------------------------------------------------------------------------//

//Car standard size
extern int G_CarStdWidth;
extern int G_CarStdHeight;

//Minimal number of appending detections.
extern int G_AppNumber;

//The sampling breaks interval of the trajectory smooth.
extern int G_SmoothGaps[3];

//The trajectory length to be drawn
extern int G_DrawLength;


//Target maximum velocity.
//This parameter can be calculated according to the size of the image frame.
extern double G_TARGET_MAXVELOCITY;
//Occlusion ratio threshold to decide the conflict.
extern double G_OCCRATIO_CONFLICT_THRESHOLD;
//Safe gap threshold to decide the neighborhoods.
extern int G_SAFE_GAP_NEIGHBOR_THRESHOLD[2];

//Occlusion ratio threshold to decide whether two tracklets belong to the neighborhoods.
extern double G_OCCRATIO_NEIGHBOR_THRESHOLD[2];


//The post-process type of each layer
extern int G_POSTPROCESS_TYPE[2];

//General maximal distance between the two tracklets.
//Can be calculated according the size of the images
extern double G_GENERAL_TRACK_MAXIMAL_DISTANCE_NEIGHBOR[2];
//General maximal segment between the two tracklets
extern int G_GENERAL_TRACK_MAXIMAL_SEGMENT_NEIGHBOR[2];

//Minimal length of the the tracklets in each layer.
extern int G_TRACK_MINIMAL_LENGTH[2];


//Minimal number of nodes in the cluster.
extern int G_TRACK_MINIMAL_VERTEX_CLUSTER[2];
//The sampling step of the smooth term.
extern int G_TRACK_SMOOTH_SAMPLE_STEP[2];
//Move prediction for the tracklets.
extern int G_TRACK_MOVE_SAMPLE_STEP[2];


//The length of the detections to get the average appearance features for tracklet appearance.
extern int G_TRACK_NUMBER_DETECTION_AVERAGE_FEATURE;

//-------------------------------------------------
//Layer Tracklet Compensation Factor
extern double G_TRACK_APP_COMPENSATE_FACTOR[3];

//Appearance static sigma.
extern double G_APP_COLOR_WEIGHT;
extern double G_APP_SHAPE_WEIGHT;
extern double G_APP_TEXTURE_WEIGHT;


//---------------------------------------------------------------------------------------------------//
//-------- Parameters for pairwise graph dense neighborhoods revealing
//---------------------------------------------------------------------------------------------------//
//--- Alpha Information ---//
//Exhaust optimization, hypergraph appearance alpha
extern double G_EXHAUST_GRAPH_APPALPHA[2];
//Exhaust optimization, hypergraph move alpha
extern double G_EXHAUST_GRAPH_MOVEALPHA[2];
//Exhaust optimization, hypergraph smooth alpha
extern double G_EXHAUST_GRAPH_SMTHALPHA[2];

//--- Sigma Information ---//
//Exhaust optimization, hypergraph appearance sigma
extern double G_EXHAUST_GRAPH_APPSIGMA[2];
//Exhaust optimization, hypergraph move sigma.
extern double G_EXHAUST_GRAPH_MOVESIGMA[2];
//Exhaust optimization, hypergraph smooth sigma
extern double G_EXHAUST_GRAPH_SMTHSIGMA[2];

//--- Bound Information ---//
//Exhaust optimization, hypergraph appearance bound.
extern double G_EXHAUST_GRAPH_APPUPPERBOUND[2];
//Exhaust optimization, hypergraph smooth bound.
extern double G_EXHAUST_GRAPH_SMTHUPPERBOUND[2];
//Exhaust optimization, hypergraph move bound.
extern double G_EXHAUST_GRAPH_MOVEUPPERBOUND[2];
//Exhaust optimization, hypergraph mixture bound.
extern double G_EXHAUST_GRAPH_MIXUPPERBOUND[2];

//--- Threshold Information ---//
//Pairwise appearance threshold
extern double G_EXHAUST_GRAPH_APPTHRESHOLD[2];
//Pairwise smooth threshold
extern double G_EXHAUST_GRAPH_SMTHTHRESHOLD[2];
//Pairwise move threshold
extern double G_EXHAUST_GRAPH_MOVETHRESHOLD[2];

//--- Weight Information ---//
//Pairwise appearance weight
extern double G_EXHAUST_GRAPH_APPWEIGHT[2];
//Pairwise smooth weight
extern double G_EXHAUST_GRAPH_SMTHWEIGHT[2];
//Pairwise move weight
extern double G_EXHAUST_GRAPH_MOVEWEIGHT[2];


#endif