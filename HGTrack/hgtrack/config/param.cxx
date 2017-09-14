/*
 * GraphMultiCarTracker
 * Do not distribute
 *  Created on: Nov 08, 2015
 *      Author: Longyin Wen
 */
#include "param.h"

//********************************************************************************************//
//-------------------------------------------------------------------------------------------//
//--- Main Parameters for Optimization
//-------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------------------//


//Car standard size
int G_CarStdWidth = 40;
int G_CarStdHeight = 40;

//Minimal number of appending detections.
int G_AppNumber = 100;

//The sampling breaks interval of the trajectory smooth.
int G_SmoothGaps[3] = {1,2,3};

//The trajectory length to be drawn
int G_DrawLength = 40;

//Target maximum velocity.
//This parameter can be calculated according to the size of the image frame.
double G_TARGET_MAXVELOCITY = 45;
//Occlusion ratio threshold to decide the conflict.
double G_OCCRATIO_CONFLICT_THRESHOLD = 0.3;
//Safe gap threshold to decide the neighborhoods.
int G_SAFE_GAP_NEIGHBOR_THRESHOLD[2] = {15,8};

//Occlusion ratio threshold to decide whether two tracklets belong to the neighborhoods.
double G_OCCRATIO_NEIGHBOR_THRESHOLD[2] = {0.3,0.2};


//The post-process type of each layer
int G_POSTPROCESS_TYPE[2] = {1,2};

//General maximal distance between the two tracklets.
//Can be calculated according the size of the images
double G_GENERAL_TRACK_MAXIMAL_DISTANCE_NEIGHBOR[2] = {800,1500};
//General maximal segment between the two tracklets
int G_GENERAL_TRACK_MAXIMAL_SEGMENT_NEIGHBOR[2] = {20,6};


//Minimal length of the the tracklets in each layer.
int G_TRACK_MINIMAL_LENGTH[2] = {2,2};


//Minimal number of nodes in the cluster.
int G_TRACK_MINIMAL_VERTEX_CLUSTER[2] = {3,2};
//The sampling step of the smooth term.
int G_TRACK_SMOOTH_SAMPLE_STEP[2] = {2,3};
//Move prediction for the tracklets.
int G_TRACK_MOVE_SAMPLE_STEP[2] = {1,3};


//The length of the detections to get the average appearance features for tracklet appearance.
int G_TRACK_NUMBER_DETECTION_AVERAGE_FEATURE = 1;

//-------------------------------------------------
//Layer Tracklet Compensation Factor
double G_TRACK_APP_COMPENSATE_FACTOR[3] = {0,0.25,0.3};

//Appearance static sigma.
double G_APP_COLOR_WEIGHT = 0.7;
double G_APP_SHAPE_WEIGHT = 0.1;
double G_APP_TEXTURE_WEIGHT = 0.2;


//---------------------------------------------------------------------------------------------------//
//-------- Parameters for pairwise graph dense neighborhoods revealing
//---------------------------------------------------------------------------------------------------//
//--- Alpha Information ---//
//Exhaust optimization, hypergraph appearance alpha
double G_EXHAUST_GRAPH_APPALPHA[2] = {0.4, 0.3};
//Exhaust optimization, hypergraph move alpha
double G_EXHAUST_GRAPH_MOVEALPHA[2] = {2.0, 2.5};
//Exhaust optimization, hypergraph smooth alpha
double G_EXHAUST_GRAPH_SMTHALPHA[2] = {1.0,1.5};

//--- Sigma Information ---//
//Exhaust optimization, hypergraph appearance sigma
double G_EXHAUST_GRAPH_APPSIGMA[2] = {0.5,0.5};
//Exhaust optimization, hypergraph move sigma.
double G_EXHAUST_GRAPH_MOVESIGMA[2] = {0.05,0.03};
//Exhaust optimization, hypergraph smooth sigma
double G_EXHAUST_GRAPH_SMTHSIGMA[2] = {0.05,0.05};

//--- Bound Information ---//
//Exhaust optimization, hypergraph appearance bound.
double G_EXHAUST_GRAPH_APPUPPERBOUND[2] = {0.7,0.3};
//Exhaust optimization, hypergraph move bound.
double G_EXHAUST_GRAPH_MOVEUPPERBOUND[2] = {0.6,0.2};
//Exhaust optimization, hypergraph smooth bound.
double G_EXHAUST_GRAPH_SMTHUPPERBOUND[2] = {0.6,0.2};
//Exhaust optimization, hypergraph mixture bound.
double G_EXHAUST_GRAPH_MIXUPPERBOUND[2] = {0.5,0.08};

//--- Threshold Information ---//
//Pairwise appearance threshold
double G_EXHAUST_GRAPH_APPTHRESHOLD[2] = {0.2,0.1};
//Pairwise move threshold
double G_EXHAUST_GRAPH_MOVETHRESHOLD[2] = {0.4,0.2};
//Pairwise smooth threshold
double G_EXHAUST_GRAPH_SMTHTHRESHOLD[2] = {0.3,0.1};

//--- Weight Information ---//
//Pairwise appearance weight
double G_EXHAUST_GRAPH_APPWEIGHT[2] = {0.8,0.7};
//Pairwise smooth weight
double G_EXHAUST_GRAPH_SMTHWEIGHT[2] = {0.1,0.2};
//Pairwise move weight
double G_EXHAUST_GRAPH_MOVEWEIGHT[2] = {0.1,0.1};