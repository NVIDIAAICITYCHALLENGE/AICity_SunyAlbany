/*
 * GraphMultiCarTracker
 * Do not distribute
 *  Created on: Nov 08, 2015
 *      Author: Longyin Wen
 */
//structure graph tracker: StructGraphTrack.h
//��Ҫ��֤������б�ĵ�һ֡�������ͼ������б�ĵ�һ֡���Ҫ��Ӧ�ϣ���Ȼ�����

#ifndef _HYPERGRAPHTRACKER_H
#define _HYPERGRAPHTRACKER_H

//#include "../Types/TFrame.h"
//#include "../Detection/Detector.h"
//#include "../Configuration/Config.h"
//#include "../Optimization/HierarchHyperGraph.h"
//#include "../Configuration/Parameter.h"
#include <util/tframe.h>
#include <detector/detector.h>
#include <config/config.h>
#include <config/param.h>
#include <opt/hierarch_hypergraph.h>

//global random function
static CvRNG rng_state = cvRNG(1);
int randint(const int min, const int max);
double randdouble();


class CHyperGraphTracker
{
public:
	CHyperGraphTracker();
	CHyperGraphTracker(const char* pConfig);
	virtual ~CHyperGraphTracker();

//---------------------------------- Variables ----------------------------------------------//
//-------------------------------------------------------------------------------------------//
public:

	//Global parameters of the tracker
	CGraphTrackerPara* m_pTrackParas;
	//Frame image set.
	pImageSet* m_pSeqFrames;

	//----------------------------------- Tracking Sliding Windows ----------------------------------------//
	//Total number of frames.
    int m_NumOfFrames;
	//Total number of layers.
	int m_NumOfLayers;

	//Tracking class.
	CHierarchHyperGraph* m_pOptimizer;

	//image size.
	int m_nWidth;
	int m_nHeight;

	//-------------------------------------------------------------------------------------------------------//

	//--------------------------------------------- Detection Parameters -----------------------------------//
	//detector class
	CDetector* m_pDetector;

	//-------------------------------------------- Detection ID generator -------------------------------------------------//
	CAllotID* m_pDetAllot;

	//------------------------------------- Save the Tracking Results ------------------------------------//
	//Including Some Debug Informations.
	//Tracking Results Handling
	CExport* m_pExpoter;

	//------------------------------------- Config File Path ---------------------------------------------//
	const char* m_pConfigPath;

//----------------------------------- Functions ----------------------------------------------//
//--------------------------------------------------------------------------------------------//
    //-------------------------------------------- Tracking Main Function ------------------------------------//
	//main calling function
	bool Tracker_API();
	//tracking function for image
	bool HierarchTracker();
	//Set the global parameters of the tracker and initialize it.
	bool InitTracker();

	//Construct the frames in image format, including the detection results.
	bool ImportImages();
};

#endif