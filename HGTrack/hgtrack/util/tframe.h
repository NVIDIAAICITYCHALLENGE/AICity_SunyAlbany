/*
 * GraphMultiCarTracker
 * Do not distribute
 *  Created on: Nov 08, 2015
 *      Author: Longyin Wen
 */
//tracking windows.
#ifndef _TWINDOWS_H
#define _TWINDOWS_H
#include <util/defs.h>
#include <config/config.h>
#include "tracklet.h"


//����������ɺ���Ҫ����ID���и���
class CTImage
{
public:
	CTImage();
	CTImage(int width, int height, int id);
	virtual ~CTImage();
public:
	//Frame size.
	int m_nWidth;
	int m_nHeight;
	//Frame ID
	int m_nFrameIndex;

	//Frame color image.
	IplImage* m_pBGRFrame;
   
	//Occupancy map of this frame.
	int** m_pOccupyMap;
   
	//The detection boxes in the current frame.
	pDetBoxSet* m_pBoxes;

	//The image path of this frames.
	char m_cFramePath[1024];

public:
	//Get the occupancy map of the targets.
	bool GetOccupy();

	//**********************************************************************//
	//Free the memory of the frame image.
    bool Free();
	//Load the frame image.
	bool Load();
};

typedef vector<CTImage*> pImageSet;
typedef map<int, CTImage*> pImageMap;


//The optimization of the trajectory consists of three stage: 
//detection ---> low tracklets ---> middle tracklets --->final tracklets
//segmentation structure used for low tracklets clustering.
class CSegTracks
{
public:
	CSegTracks();
	CSegTracks(const CSegTracks* segtrack);
	CSegTracks(const CTrack* track);
	CSegTracks(const CDetBBox* box);

	virtual ~CSegTracks();

	//Update the tracks in the segment.
	bool Update();
	//Set the segment tracklet variable to be empty.
	bool Clear();
	//Copy the segment tracklet variable.
	bool Copy(const CSegTracks* segtracks);

public:
	//Marks, if it is true, means there exists the tracklets in the segment.
	bool m_bActMark;
	//Segment index.
	int m_nSegIndex;
	//Begin frame index of the segment tracks.
	int m_nBegFrmIdx;
	//End frame index of the segment tracks.
	int m_nEndFrmIdx;

	//This variable is just a pointer, do not need to apply the memory for it.
	pTrackSet* m_pTracks;
};

typedef vector<CSegTracks*> pSegTrackSet;


//Check whether the two tracklets have the time overlap.
bool CheckTrackOverlap(CTrack* pTrack1, CTrack* pTrack2);
//Check whether the two tracklets conflict with each other.
bool CheckConflict(CTrack* pTrack1, CTrack* pTrack2, double nThresh);

//Calculate the occlusion ratio between the interpolate bounding box and the mask image.
double CalcOccRatio(CDetBBox* pBox, CTImage* pFrame);
//Calculate the occlusion ratio between the box1 and box2
double CalcOccRatio(CDetBBox* pBox1, CDetBBox* pBox2);

//Calculate the 2D distance between detections.
double Calc2DDist(CDetBBox* pBox1, CDetBBox* pBox2);
//Calculate the 2D distance between tracklets.
double CalcTrack2DDist(CTrack* pTrack1, CTrack* pTrack2);


//Calculate the position deviation (the time index of pEnd is larger than pStart)
double CalcPosDeviate(CDetBBox* pStart, double vx, double vy, CDetBBox* pEnd);
//Calculate the size deviation (the time index of pEnd is larger than pStart)
double CalcSizeDeviate(CDetBBox* pStart, double vw, double vh, CDetBBox* pEnd);

//--------------------------------------------- Sort the data -------------------------------------------------------//
//Sort the data according to pData, and order pIndex at the same time. 
bool SortDescend(int* pData, int* pIndex, int nLen);
bool SortDescend(DoubleVector* pData, IntVector* pIndex);
bool ProduceDescend(DoubleVector& pData, IntVector& pIndex, int nLen);

//Calculate the combination results.
double CalcChoose(int n, int m);


#endif

