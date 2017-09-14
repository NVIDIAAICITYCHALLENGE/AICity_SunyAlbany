/*
 * GraphMultiCarTracker
 * Do not distribute
 *  Created on: Nov 08, 2015
 *      Author: Longyin Wen
 */
//this file mainly exports the tracking results.
#ifndef _EXPORT_H
#define _EXPORT_H
/*
#include "../Defs.h"
#include "../Types/Tracklet.h"
#include "../Types/TFrame.h"
#include "../Math/Algebra.h"
*/
#include <util/defs.h>
#include <util/tracklet.h>
#include <util/tframe.h>
#include <math/algebra.h>

//���������xmlĿ�������Ϣ����ֱ��ͨ����silding windows����ļ��������洢���
//���������Ŀ��ͼ����Ϣ����ֱ�Ӹ���Ŀ���б�����ɡ�

class CExport
{
public:
	CExport();
	virtual ~CExport();
public:
	bool Init(ExportPara* pParameters);
	//export the trajectory of the tracked targets.
	bool ExportTrajectory(pImageSet* pFrames, pTrackSet* pTracks);
	
	//--------------------------------------- Export image without the area constraints -----------------------------------//
	//export the trajectory of the tracked targets.
	bool ExportImages(pImageSet* pFrames, pTrackSet* pTracks);

	//--------------------------------------- Debug Function -----------------------------------------------//
	//export the low level tracklets of each windows.
	//bool ExportSegTrackImages_Debug(pTrackSet* pTracks, int nLayer);
     
	//export the final trajectories of the sequence.
	//bool ExportTrajectoryImage_Debug(pTrackSet* pTracks);
    
	//export the targets standard image.
	//bool ExportTargetsImage_Debug(CDetBBox* pBox);
  
	//export the occupy map of the targets.
	//bool ExportOccupyMap_Debug(CTImage* pFrame);
	
public:
	//Name of the Database.
	const char* m_pDataName;
	const char* m_pOutputImagePath;
	const char* m_pOutputTrackPath;
	const char* m_pOutputSpeedPath;

	//Image Sequence Size.
	int m_nWidth;
	int m_nHeight;

	//Debug Mark. 1:save debug; 0: do not save debug
	int m_DebugMark;
	//Used for Debug
	//Trajectory Image Path.
	const char* m_pOutTracksImagePath_Debug;
	//Detection Target Path.
	const char* m_pOutTargetsImagePath_Debug;
	//Occupy Map Path.
	const char* m_pOccupyMapImagePath_Debug;
};

int vBoxFind(int fid, pDetBoxSet* detset);


#endif