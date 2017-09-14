/*
 * GraphMultiCarTracker
 * Do not distribute
 *  Created on: Nov 08, 2015
 *      Author: Longyin Wen
 */
//////////////////////////////////////////////////////////////////////////
//target.h
//target information

#ifndef _TRACKLET_H
#define _TRACKLET_H

#include <util/defs.h>
#include <feat/feat_class.h>
#include <math/algebra.h>
#include <spline_fit/interpolation.h>
#include <config/param.h>

/* From OpenCV library */
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>



using namespace alglib;


//detection results in each frame.
class CDetBBox
{
public:
	//Bounding box ID.
	int m_nDetBoxID;
	//Frame ID of the bounding box. �˱���Ϊһ���̶���ֵ����һ��������Ƶȷ��֮�󣬴�ֵ�͹̶�
	int m_nFrameIndex;
	//Target index in the frame target list, just a virtual.
	int m_nVirtIndex;

	//confidence of the bounding box.
	double m_nConf;
	//position of the target in the 2D image plane.
	double m_n2Dcx;
	double m_n2Dcy;
	
	//size of the target. 
	int m_nWidth;
	int m_nHeight;
	
	//color template used for the detection.
	Palette* m_pColor;

	//just for displaying and identify true targets �����ĸ�Ŀ���б��IDֵ
	int m_nTrackID;

	//����virtual detection�ı�־λ
	bool m_bVirtMark;

	//features in the bounding box.
	CFeature* m_pDetFeat;

	//Standard feature extraction area color image.
	IplImage* m_pFeatPict;

public:
	CDetBBox();
	CDetBBox(const CDetBBox* box);
	CDetBBox(const CDetBBox& box);
	virtual ~CDetBBox();

	//Get the picture of the feature extraction region.
	bool GetFeatPict(IplImage* pImage);

	bool CopyFeats(IplImage* pPict, CFeature* pFeat);

};

typedef vector<CDetBBox*> pDetBoxSet;


//Ϊ�˴��������Կ��ǣ���ֻ��һ��bounding box��trajectoryҲ��Ϊһ��tracklet.
//�����е��Ż������������tracklet��������
class CTrack
{
public:
	//initial a null tracklet.
	CTrack();
	//initial a existing tracklet.
	CTrack(const CDetBBox* box);
	//initial a existing tracklet.
	CTrack(const CDetBBox& box);
    //copy a existing tracklet.
	CTrack(const CTrack* track);

	virtual ~CTrack();

	//Active mark, if it is true, reflects the tracklet is active.
	bool m_bActMark;
	//Segment index, decide which segment the tracklet belongs.
	int m_nSegIndex;

	//Detections to be added.
	pDetBoxSet* m_pDetPoints;
	//color template of the targets
	Palette* m_pColor;

	//--------------------------------- Tracklet Motion Information ----------------------------------------//
	//number of frames in the trajectory variable in the current sliding windows.
	int m_nEndFrmIdx;
	//number of frames of the trajectory in the previous sliding windows
	int m_nBegFrmIdx;

	//display id, it is just used for final target displaying.
	int m_nTrackID;

	//Target index in the segment tracklet list, just a virtual.
	int m_nVirtIndex;

	//The features of the tracklet head detection.
	CFeature* m_pHeadDetFeat;
	//The features of the tracklet tail detection.
	CFeature* m_pTailDetFeat;

	//The average features of the tracklet head.
	CFeature* m_pHeadAvgFeat;
	//The average features of the tracklet tail.
	CFeature* m_pTailAvgFeat;

	//Spline variables.
	ae_int_t m_nBoundType;

    //Spline used to adjust the trajectory.
	spline1dinterpolant* m_pAdjSpline;

public:
	//--------------------------------- Used for Optimization ----------------------------------------------//
	//add the bounding box to get the trajectory of the target.
	bool Add(const CDetBBox* box);
	//add the tracklet to get the trajectory of the target.
	bool Add(const CTrack* track);
	//Add the bounding box set to get the trajectory of the target.
	bool Add(const pDetBoxSet* boxes);

	//Forced copy. (Copy the tracklet value to this variable)
	bool Copy(const CTrack* track);

	//Order and revise the spline and delete the conflict points.
	bool SplineAdjust();

	//spline interpolate (Repair)
	//��Ŀ������˶�ģ�͸���ǰ��ʱ�򣬾Ͱ�trajectory�ڲ幤�����
	//---------------------------------- Inner Plot the Target Positions ----------------------------------//
	bool SplineFitting();
	//---------------------------------- Get the Appearance of the Tracklet ------------------------//
	bool GetAppearance();
};


typedef vector<CTrack*> pTrackSet;

//Swap function of the data
bool Swap(IntVector& vData,int nIdx1,int nIdx2);

#endif
