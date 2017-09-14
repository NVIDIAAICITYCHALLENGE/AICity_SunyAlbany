/*
 * GraphMultiCarTracker
 * Do not distribute
 *  Created on: Nov 08, 2015
 *      Author: Longyin Wen
 */
#include "tframe.h"

//------------------------------ Frame Information Variables ----------------------------------------//
CTImage::CTImage()
{
	m_nWidth = 320;
	m_nHeight = 240;
	m_nFrameIndex = 0;
	m_pBGRFrame = cvCreateImage(cvSize(m_nWidth, m_nHeight), 8, 3);
	m_pBoxes = new pDetBoxSet;

	//Get the occupy map of current frame.
	m_pOccupyMap = new int*[m_nHeight];
	for (int i = 0; i < m_nHeight; i++)
		m_pOccupyMap[i] = new int[m_nWidth];

	//Initialize the occupy map to be null.
	for (int i = 0; i < m_nHeight; i++) {
		for (int j = 0; j < m_nWidth; j++) {
			m_pOccupyMap[i][j] = -1;
		}
	}

}

CTImage::CTImage(int width, int height, int id)
{
	m_nWidth = width;
	m_nHeight = height;
	m_nFrameIndex = id;
	m_pBGRFrame = cvCreateImage(cvSize(m_nWidth, m_nHeight), 8, 3);
 
	m_pBoxes = new pDetBoxSet;

	//Get the occupy map of current frame.
	m_pOccupyMap = new int*[m_nHeight];
	for (int i = 0; i < m_nHeight; i++)
		m_pOccupyMap[i] = new int[m_nWidth];

	//Initialize the occupy map to be null.
	for (int i = 0; i < m_nHeight; i++) {
		for (int j = 0; j < m_nWidth; j++) {
			m_pOccupyMap[i][j] = -1;
		}
	}
}

CTImage::~CTImage()
{
	m_nWidth = 0;
	m_nHeight = 0;
	m_nFrameIndex = 0;
	if (m_pBGRFrame) {
		cvReleaseImage(&m_pBGRFrame);
		m_pBGRFrame = 0;
	}
 
	if (m_pBoxes) {
		for (int i = 0; i < (int)(m_pBoxes->size()); i++) {
			RELEASE((*m_pBoxes)[i]);
			(*m_pBoxes)[i] = 0;
		}
		RELEASE(m_pBoxes);
		m_pBoxes = 0;
	}

	//Release the occupy map of current frame.
	if (m_pOccupyMap) {
		for (int i = 0; i < m_nHeight; i++) {
			RELEASEARRAY(m_pOccupyMap[i]);
			m_pOccupyMap[i] = 0;
		}
		RELEASEARRAY(m_pOccupyMap);
		m_pOccupyMap = 0;
	}
}


//Get the occupy map of current frame.
bool CTImage::GetOccupy()
{
	CDetBBox* pbox1;
	CDetBBox* pbox2;
	int nleft, nright, ntop, nbottom;
  
	//Reset the occupy map.
	for (int i = 0; i < m_nHeight; i++) {
		for (int j = 0; j < m_nWidth; j++) {
			m_pOccupyMap[i][j] = -1;
		}
	}
	
	for (int i = 0; i < (int)(m_pBoxes->size()); i++) {
		pbox1 = (*m_pBoxes)[i];
		
		//Get the occupy map according to their parts.
		nleft = (int)(pbox1->m_n2Dcx - (double)(pbox1->m_nWidth) / 2);
		nright = (int)(pbox1->m_n2Dcx + (double)(pbox1->m_nWidth) / 2);
		ntop = (int)(pbox1->m_n2Dcy - (double)(pbox1->m_nHeight) / 2);
		nbottom = (int)(pbox1->m_n2Dcy + (double)(pbox1->m_nHeight) / 2);

		for (int m = ntop; m <= nbottom; m++) {
			for (int j = nleft; j <= nright; j++) {
				if (m_pOccupyMap[m][j] != -1) {
					//Check the overlap relation between different targets.
					pbox2 = (*m_pBoxes)[m_pOccupyMap[m][j]];
					if (pbox1->m_n2Dcy > pbox2->m_n2Dcy) {
						m_pOccupyMap[m][j] = pbox1->m_nVirtIndex;
					}
				}
				else {
					m_pOccupyMap[m][j] = pbox1->m_nVirtIndex;
				}
			}
		}
	}
	return true;
}

//Release the memory of the images.
bool CTImage::Free()
{
	if (m_pBGRFrame) {
		cvReleaseImage(&m_pBGRFrame);
		m_pBGRFrame = 0;
	}
	return true;
}

//Reload the images.
bool CTImage::Load()
{
	IplImage* pOrgFrame = cvLoadImage(m_cFramePath, CV_LOAD_IMAGE_COLOR);
	if ((pOrgFrame->width != m_nWidth)||(pOrgFrame->height != m_nHeight)) {
		m_pBGRFrame = cvCreateImage(cvSize(m_nWidth, m_nHeight), 8, 3);
		cvResize(pOrgFrame, m_pBGRFrame, CV_INTER_LINEAR);
	}
	else {
		m_pBGRFrame = cvCreateImage(cvSize(m_nWidth, m_nHeight), 8, 3);
		cvCopy(pOrgFrame, m_pBGRFrame);
	}
	return true;
}


//------------------------------------------------- Segment Variables -----------------------------------------------------//
//Used to generate the middle level tracklets.
CSegTracks::CSegTracks()
{
	//Default the segment tracks to be active.
	m_bActMark = false;
	m_nSegIndex = -1;
	m_nBegFrmIdx = 0;
	m_nEndFrmIdx = 0;
	m_pTracks = new pTrackSet;
	m_pTracks->clear();
}

//Deep construction function.
CSegTracks::CSegTracks(const CSegTracks* segtrack)
{
	//Set the mark of the segment tracks.
	m_bActMark = segtrack->m_bActMark;
	m_nSegIndex = segtrack->m_nSegIndex;
	m_nBegFrmIdx = segtrack->m_nBegFrmIdx;
	m_nEndFrmIdx = segtrack->m_nEndFrmIdx;
	//Apply the memory.
	m_pTracks = new pTrackSet;
	m_pTracks->clear();

	for (int i = 0; i < (int)(segtrack->m_pTracks->size()); i++) {
		CTrack* pNewTrack = new CTrack((*(segtrack->m_pTracks))[i]);
		m_pTracks->push_back(pNewTrack);
	}
}

//Deep construction function.
CSegTracks::CSegTracks(const CTrack* track)
{
	//Set the mark of the segment tracks.
	m_bActMark = true;
	m_nSegIndex = track->m_nSegIndex;
	m_nBegFrmIdx = track->m_nBegFrmIdx;
	m_nEndFrmIdx = track->m_nEndFrmIdx;
	//Apply the memory.
	m_pTracks = new pTrackSet;
	m_pTracks->clear();
	CTrack* pNewTrack = new CTrack(track);
	m_pTracks->push_back(pNewTrack);
}

//Deep construction function.
CSegTracks::CSegTracks(const CDetBBox* box)
{
	//Set the mark of the segment tracks.
	m_bActMark = true;
	m_nBegFrmIdx = box->m_nFrameIndex;
	m_nEndFrmIdx = box->m_nFrameIndex;
	//Apply the memory.
	m_pTracks = new pTrackSet;
	m_pTracks->clear();
	CTrack* pNewTrack = new CTrack(box);
	m_pTracks->push_back(pNewTrack);

}


CSegTracks::~CSegTracks()
{
	m_bActMark = false;
	m_nSegIndex = -1;

	m_nBegFrmIdx = 0;
	m_nEndFrmIdx = 0;

	if (m_pTracks) {
		for (int i = 0; i < (int)(m_pTracks->size()); i++) {
			RELEASE((*m_pTracks)[i]);
			(*m_pTracks)[i] = 0;
		}
		m_pTracks->clear();
		RELEASE(m_pTracks);
		m_pTracks = 0;
	}
}

//Update the information of the tracklets in the segment.
bool CSegTracks::Update()
{
	CTrack* pTk = 0;
	//Whether update the information of the tracklets in the segment according to the mark.
	if (m_bActMark) {
		for (int i = 0; i < (int)(m_pTracks->size()); i++) {
			pTk = (*m_pTracks)[i];
			if (pTk->m_bActMark) {
				pTk->SplineAdjust();
				pTk->SplineFitting();
				pTk->GetAppearance();
			}
		}
		//Get the start and end frames of the segment tracks.
		m_nBegFrmIdx = (int)(1e4);
		m_nEndFrmIdx = -(int)(1e4);

		for (int i = 0; i < (int)(m_pTracks->size()); i++) {
			pTk = (*m_pTracks)[i];
			m_nEndFrmIdx = max(m_nEndFrmIdx, pTk->m_nEndFrmIdx);
			m_nBegFrmIdx = min(m_nBegFrmIdx, pTk->m_nBegFrmIdx);
		}
	}
	pTk = 0;
	return true;
}

//Clear the segment tracklet.
bool CSegTracks::Clear()
{
	m_bActMark = false;
	//Segment index.
	m_nSegIndex = -1;
	//End frame of the segment tracks.
	m_nEndFrmIdx = 0;
	//Begin frame of the segment tracks.
	m_nBegFrmIdx = 0;

	//Clear the tracklet set.
	for (int i = 0; i < (int)(m_pTracks->size()); i++) {
		if ((*m_pTracks)[i]) {
			RELEASE((*m_pTracks)[i]);
			(*m_pTracks)[i] = 0;
		}
	}
	m_pTracks->clear();
	return true;
}

//Copy the segment tracklet.
bool CSegTracks::Copy(const CSegTracks* segtracks)
{
	m_bActMark = segtracks->m_bActMark;
	//Segment index.
	m_nSegIndex = segtracks->m_nSegIndex;
	//End frame index of the segment.
	m_nEndFrmIdx = segtracks->m_nEndFrmIdx;
	//Begin frame index of the segment.
	m_nBegFrmIdx = segtracks->m_nBegFrmIdx;

	//Release the previous memory.
	for (int i = 0; i < (int)(m_pTracks->size()); i++) {
		if ((*m_pTracks)[i]) {
			RELEASE((*m_pTracks)[i]);
			(*m_pTracks)[i] = 0;
		}
	}
	m_pTracks->clear();

	//Copy the memory.
	for (int i = 0; i < (int)(segtracks->m_pTracks->size()); i++) {
		CTrack* pNewTrack = new CTrack((*(segtracks->m_pTracks))[i]);
		m_pTracks->push_back(pNewTrack);
	}
	return true;
}


//-------------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------------//
//--- Some help function.
bool CheckTrackOverlap(CTrack* pTrack1, CTrack* pTrack2)
{
	bool r1, r2;
	int sfpt1 = (*(pTrack1->m_pDetPoints))[0]->m_nFrameIndex;
	int efpt1 = (*(pTrack1->m_pDetPoints))[(int)(pTrack1->m_pDetPoints->size() - 1)]->m_nFrameIndex;
	int sfpt2 = (*(pTrack2->m_pDetPoints))[0]->m_nFrameIndex;
	int efpt2 = (*(pTrack2->m_pDetPoints))[(int)(pTrack2->m_pDetPoints->size() - 1)]->m_nFrameIndex;

	r1 = efpt2 < sfpt1 ? true: false;
	r2 = efpt1 < sfpt2 ? true: false;

	if (r1||r2)
		return false;
	else 
		return true;
}

bool CheckConflict(CTrack* pTrack1, CTrack* pTrack2, double nThresh)
{
	//Check whether the two tracklets overlap with each other in the time domain.
	if (!CheckTrackOverlap(pTrack1, pTrack2))
		return false;

	//Check whether the two tracklets have the same detections.
	CDetBBox* pBox1 = 0;
	CDetBBox* pBox2 = 0;
	bool fBreak = false;

	for (int i = 0; i < (int)(pTrack1->m_pDetPoints->size()); i++) {
		pBox1 = (*(pTrack1->m_pDetPoints))[i];
		for (int j = 0; j < (int)(pTrack2->m_pDetPoints->size()); j++) {
			pBox2 = (*(pTrack2->m_pDetPoints))[j];
			if ((pBox1->m_nDetBoxID == pBox2->m_nDetBoxID)
				&&(pBox1->m_nDetBoxID >= 0)&&(pBox2->m_nDetBoxID >= 0)) {
					fBreak = true;
					break;
			}
		}
		if (fBreak)
			break;
	}
	if (fBreak)
		return true;

	double nRatio = 0;
	for (int i = 0; i < (int)(pTrack1->m_pDetPoints->size()); i++) {
		pBox1 = (*(pTrack1->m_pDetPoints))[i];
		for (int j = 0; j < (int)(pTrack2->m_pDetPoints->size()); j++) {
			pBox2 = (*(pTrack2->m_pDetPoints))[j];
			if (pBox1->m_nFrameIndex == pBox2->m_nFrameIndex) {
				nRatio += CalcOccRatio(pBox1, pBox2);
				break;
			}
		}
	}
	nRatio = nRatio / (double)(pTrack1->m_pDetPoints->size());

	if (nRatio > nThresh)
		return true;
	else
		return false;
}

double CalcPosDeviate(CDetBBox* pStart, double vx, double vy, CDetBBox* pEnd)
{
	double px, py;
	double dx, dy;
	double dev;
	int nt = abs(pEnd->m_nFrameIndex - pStart->m_nFrameIndex);
	px = pStart->m_n2Dcx + nt * vx;
	py = pStart->m_n2Dcy + nt * vy;
	//Get the deviation.
	dx = px - pEnd->m_n2Dcx;
	dy = py - pEnd->m_n2Dcy;
	dev = sqrt(dx*dx + dy*dy);

	return dev;
}

double CalcSizeDeviate(CDetBBox* pStart, double vw, double vh, CDetBBox* pEnd)
{
	double pw, ph;
	double dw, dh;
	double dev;
	int nt = abs(pEnd->m_nFrameIndex - pStart->m_nFrameIndex);
	pw = pStart->m_nWidth + nt * vw;
	ph = pStart->m_nHeight + nt * vh;
	//Get the deviation.
	dw = pw - pEnd->m_nWidth;
	dh = ph - pEnd->m_nHeight;
	dev = sqrt(dw*dw + dh*dh);

	return dev;
}

//Calculate the occlusion ratio of the interpolated bounding box.
double CalcOccRatio(CDetBBox* pBox, CTImage* pFrame)
{
	double nRatio = 0;
	double nArea = 0;
	int nLeft, nRight, nTop, nBottom;
	nLeft = (int)(pBox->m_n2Dcx - (double)pBox->m_nWidth / 2);
	nRight = (int)(pBox->m_n2Dcx + (double)pBox->m_nWidth / 2);
	nTop = (int)(pBox->m_n2Dcy - (double)pBox->m_nHeight / 2);
	nBottom = (int)(pBox->m_n2Dcy + (double)pBox->m_nHeight / 2);

	for (int m = nTop; m <= nBottom; m++) {
		for (int j = nLeft; j <= nRight; j++) {
			if (pFrame->m_pOccupyMap[m][j] > -1) {
				nRatio++;
			}
		}
	}
	nArea = (nRight - nLeft + 1) * (nBottom - nTop + 1);

	nRatio = nRatio / nArea;
	return nRatio;
}

//box1 is occluded by box2
double CalcOccRatio(CDetBBox* pBox1, CDetBBox* pBox2)
{
	double nRatio = 0;
	double nArea = 0;
	//Calculate the area of box1
	nArea = (double)(pBox1->m_nWidth * pBox2->m_nHeight);

	double nLeft1 = pBox1->m_n2Dcx - pBox1->m_nWidth / 2.0;
	double nTop1 = pBox1->m_n2Dcy - pBox1->m_nHeight / 2.0;
	double nRight1 = pBox1->m_n2Dcx + pBox1->m_nWidth / 2.0;
	double nBottom1 = pBox1->m_n2Dcy + pBox1->m_nHeight / 2.0;

	double nLeft2 = pBox2->m_n2Dcx - pBox2->m_nWidth / 2.0;
	double nTop2 = pBox2->m_n2Dcy - pBox2->m_nHeight / 2.0;
	double nRight2 = pBox2->m_n2Dcx + pBox2->m_nWidth / 2.0;
	double nBottom2 = pBox2->m_n2Dcy + pBox2->m_nHeight / 2.0;

	double nDx = max((double)0, min(nRight1,nRight2) - max(nLeft1,nLeft2));
	double nDy = max((double)0, min(nBottom1,nBottom2) - max(nTop1,nTop2));

	nRatio = nDx * nDy / nArea;
	return nRatio;

}

double Calc2DDist(CDetBBox* pBox1, CDetBBox* pBox2)
{
	double dist = 0;
	dist = sqrt((pBox1->m_n2Dcx-pBox2->m_n2Dcx)*(pBox1->m_n2Dcx-pBox2->m_n2Dcx)
		+(pBox1->m_n2Dcy-pBox2->m_n2Dcy) * (pBox1->m_n2Dcy-pBox2->m_n2Dcy));
	return dist;
}

//Calcuate the distance between two tracklets
double CalcTrack2DDist(CTrack* pTrack1, CTrack* pTrack2)
{
	//First check whether these two tracklets have time overlap
	double dist = 0;
	if (CheckTrackOverlap(pTrack1, pTrack2)) {
		dist = DBL_MAXVAL;
		return dist;
	}
	//Calculate the head and tail of these two tracklets.
	int nHead1 = pTrack1->m_nBegFrmIdx;
	int nHead2 = pTrack2->m_nBegFrmIdx;

	CDetBBox* pBox1 = 0;
	CDetBBox* pBox2 = 0;

	if (nHead1 < nHead2) {
		//Track1 tail and Track2 head
		pBox1 = (*(pTrack1->m_pDetPoints))[(int)(pTrack1->m_pDetPoints->size() - 1)];
		pBox2 = (*(pTrack2->m_pDetPoints))[0];
		dist = Calc2DDist(pBox1, pBox2);
	}
	else {
		//Track1 head and Track2 tail
		pBox1 = (*(pTrack1->m_pDetPoints))[0];
		pBox2 = (*(pTrack2->m_pDetPoints))[(int)(pTrack2->m_pDetPoints->size() - 1)];
		dist = Calc2DDist(pBox1, pBox2);
	}
	return dist;
}

//------------------------------------------------------------ Sort Function ----------------------------------------------------//
bool SortDescend(int* pData, int* pIndex, int nLen)
{
	int data;
	int index;
	for (int i = 0; i < nLen - 1; i++) {
		for (int j = i + 1; j < nLen; j++) {
			if (pData[i] < pData[j]) {
				//Data Exchange
				data = pData[i];
				pData[i] = pData[j];
				pData[j] = data;
				//Index Exchange
				index = pIndex[i];
				pIndex[i] = pIndex[j];
				pIndex[j] = index;
			}
		}
	}
	return true;
}


bool SortDescend(DoubleVector* pData, IntVector* pIndex)
{
	int nLen1 = (int)(pData->size());
	int nLen2 = (int)(pIndex->size());
	if (nLen1 != nLen2) {
		fprintf(stderr, "The data dimensions are not matched in SortDescend function!\n");
		return false;
	}

	double data;
	int index;
	for (int i = 0; i < nLen1 - 1; i++) {
		for (int j = i + 1; j < nLen1; j++) {
			if ((*pData)[i] < (*pData)[j]) {
				//Data Exchange
				data = (*pData)[i];
				(*pData)[i] = (*pData)[j];
				(*pData)[j] = data;
				//Index Exchange
				index = (*pIndex)[i];
				(*pIndex)[i] = (*pIndex)[j];
				(*pIndex)[j] = index;
			}
		}
	}
	return true;
}

bool ProduceDescend(DoubleVector& pData, IntVector& pIndex, int nLen)
{
	//Sort the data in descending order.
	for (int i = 0; i < nLen; i++) 
		pIndex[i] = i;

	double data;
	int index;
	for (int i = 0; i < nLen - 1; i++) {
		for (int j = i + 1; j < nLen; j++) {
			if (pData[i] < pData[j]) {
				//Data Exchange
				data = pData[i];
				pData[i] = pData[j];
				pData[j] = data;
				//Index Exchange
				index = pIndex[i];
				pIndex[i] = pIndex[j];
				pIndex[j] = index;
			}
		}
	}
	return true;
}

//Calculate the combination of the data.
double CalcChoose(int n, int m)
{
	double nComb = 0;
	if ((m > n)||(m < 0)||(n < 0)) {
		nComb = 0;
		return nComb;
	}
	if ((m == 0)||(n == m)) {
		nComb = 1;
		return nComb;
	}

	if (m < n / 2.0)
		m = n - m;

	double nSum1 = 0;
	for (int i = m + 1; i <= n; i++)
		nSum1 += log((double)i);

	double nSum2 = 0;
	int nUb = n - m;
	for (int i = 2; i <= nUb; i++)
		nSum2 += log((double)i);

	nComb = nSum1 - nSum2;
	nComb = exp(nComb);
	return nComb;
}