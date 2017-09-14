/*
 * GraphMultiCarTracker
 * Do not distribute
 *  Created on: Nov 08, 2015
 *      Author: Longyin Wen
 */
//target.cpp  target files.
#include "tracklet.h"

//----------------------------------- Bounding box class------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////////
CDetBBox::CDetBBox() 
{
	m_nDetBoxID = -1;
	m_nFrameIndex = -1;
	m_nVirtIndex = -1;

	m_nConf = 0;
	//The target position in 2D image plane.
	m_n2Dcx = 0;
	m_n2Dcy = 0;
	
	m_nHeight = 0;
	m_nWidth = 0;

	m_bVirtMark = false;

	m_nTrackID = -1;

	m_pColor = new Palette;
	m_pColor->m_nR = rand()%256;
	m_pColor->m_nG = rand()%256;
	m_pColor->m_nB = rand()%128;

	m_pDetFeat = new CFeature;

	//Standard feature image variable.
	m_pFeatPict = cvCreateImage(cvSize(G_CarStdWidth, G_CarStdHeight), 8, 3);
}

//deep copy function
CDetBBox::CDetBBox(const CDetBBox &box)
{
	m_nDetBoxID = box.m_nDetBoxID;
	m_nFrameIndex = box.m_nFrameIndex;
	m_nVirtIndex = box.m_nVirtIndex;
	
	m_nConf = box.m_nConf;
	//Target in the 2D image plane
	m_n2Dcx = box.m_n2Dcx;
	m_n2Dcy = box.m_n2Dcy;

	m_nWidth = box.m_nWidth;
	m_nHeight = box.m_nHeight;

	m_bVirtMark = box.m_bVirtMark;
	m_nTrackID = box.m_nTrackID;

	m_pColor = new Palette;
	m_pColor->m_nR = (box.m_pColor)->m_nR;
	m_pColor->m_nG = (box.m_pColor)->m_nG;
	m_pColor->m_nB = (box.m_pColor)->m_nB;

	m_pDetFeat = new CFeature(box.m_pDetFeat);

	//Feature image extraction.
	m_pFeatPict = cvCreateImage(cvGetSize(box.m_pFeatPict), 8, 3);
	//Copy the normal image to this variable.
	cvCopy(box.m_pFeatPict, m_pFeatPict);
}


CDetBBox::CDetBBox(const CDetBBox* box)
{
	m_nDetBoxID = box->m_nDetBoxID;
	m_nFrameIndex = box->m_nFrameIndex;
	m_nVirtIndex = box->m_nVirtIndex;

	m_nConf = box->m_nConf;
	//The target position in the 2D image plane.
	m_n2Dcx = box->m_n2Dcx;
	m_n2Dcy = box->m_n2Dcy;

	m_nWidth = box->m_nWidth;
	m_nHeight = box->m_nHeight;
    
	m_bVirtMark = box->m_bVirtMark;
	m_nTrackID = box->m_nTrackID;

	m_pColor = new Palette;
	m_pColor->m_nR = box->m_pColor->m_nR;
	m_pColor->m_nG = box->m_pColor->m_nG;
	m_pColor->m_nB = box->m_pColor->m_nB;

	m_pDetFeat = new CFeature(box->m_pDetFeat);

	//Feature image initialization.
	m_pFeatPict = cvCreateImage(cvGetSize(box->m_pFeatPict), 8, 3);
	//Copy the normal image to this variable.
	cvCopy(box->m_pFeatPict, m_pFeatPict);
}

CDetBBox::~CDetBBox() 
{
	m_nDetBoxID = -1;
    m_nFrameIndex = -1;
	m_nVirtIndex = -1;

	m_nConf = 0;
	//2D image plane
	m_n2Dcx = 0;
	m_n2Dcy = 0;
	
	m_nHeight = 0;
	m_nWidth = 0;
  
	m_bVirtMark = true;
	m_nTrackID = -1;

	if (m_pColor) {
		RELEASE(m_pColor);
		m_pColor = 0;
	}
	
	if (m_pDetFeat) {
		RELEASE(m_pDetFeat);
		m_pDetFeat = 0;
	}

	//Release the normal image.
	if (m_pFeatPict) {
		cvReleaseImage(&m_pFeatPict);
		m_pFeatPict = 0;
	}
}

bool CDetBBox::GetFeatPict(IplImage* pimage)
{
	//Release the previous feature picture information.
	if (!m_pFeatPict)
		m_pFeatPict = cvCreateImage(cvSize(G_CarStdWidth, G_CarStdHeight), 8, 3);

    IplImage* pPedPict = cvCreateImage(cvSize(m_nWidth, m_nHeight), 8, 3);

	//Get the standard image.
	cvSetImageROI(pimage, cvRect((int)(m_n2Dcx - m_nWidth / 2), (int)(m_n2Dcy - m_nHeight / 2), m_nWidth, m_nHeight));
	cvCopy(pimage, pPedPict);
	cvResetImageROI(pimage);
    
	//Resize the image.
	cvResize(pPedPict, m_pFeatPict, CV_INTER_LINEAR);

	if (pPedPict) {
		cvReleaseImage(&pPedPict);
		pPedPict = 0;
	}

	return true;
}

//Copy the feature extraction region image
bool CDetBBox::CopyFeats(IplImage* pPict, CFeature* pFeat)
{
	//Release the previous memory.
	if (!m_pFeatPict)
		m_pFeatPict = cvCreateImage(cvSize(G_CarStdWidth, G_CarStdHeight), 8, 3);
	//Copy the feature extraction image region.
	cvCopy(pPict, m_pFeatPict);

	//Copy the features.
	m_pDetFeat->Copy(pFeat);
	return true;
}

//----------------------------------------------------- Tracklet Class ----------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------//
CTrack::CTrack()
{
	m_bActMark = false;
	//Add the reserve detections.
	m_pDetPoints = new pDetBoxSet;
   
	m_pColor = new Palette;
	m_pColor->m_nR = rand()%256;
	m_pColor->m_nG = rand()%256;
	m_pColor->m_nB = rand()%128;

	m_nEndFrmIdx = 0;
	m_nBegFrmIdx = 0;

	m_nTrackID = -1;
	m_nVirtIndex = -1;

	//The feature of the tracklets head.
	m_pHeadDetFeat = new CFeature;
	//The feature of the tracklets tail.
	m_pTailDetFeat = new CFeature;

	//The average feature of the tracklets head.
	m_pHeadAvgFeat = new CFeature;
	//The average feature of the tracklets tail.
	m_pTailAvgFeat = new CFeature;

	//Trajectory adjust spline.
	m_pAdjSpline = new spline1dinterpolant;

	//Types of the fitting.
	m_nBoundType = 2;
}

CTrack::CTrack(const CDetBBox* box)
{
	m_bActMark = true;
	//reserved detections.
	m_pDetPoints = new pDetBoxSet;
	m_pDetPoints->clear();
  
	CDetBBox* newdet = new CDetBBox(box);
    m_pDetPoints->push_back(newdet);

	m_pColor = new Palette;
	
	m_pColor->m_nR = rand()%256;
	m_pColor->m_nG = rand()%256;
	m_pColor->m_nB = rand()%128;

	//------------------------------------- Motion Information -------------------------------------------------//
	m_nEndFrmIdx = newdet->m_nFrameIndex;
	m_nBegFrmIdx = newdet->m_nFrameIndex;

	m_nTrackID = -1;
	m_nVirtIndex = -1;

	//The feature of the tracklet head.
	m_pHeadDetFeat = new CFeature(box->m_pDetFeat);
	//The feature of the tracklet tail.
	m_pTailDetFeat = new CFeature(box->m_pDetFeat);

	//The average feature of the tracklet.
	m_pHeadAvgFeat = new CFeature(box->m_pDetFeat);
	//The average feature of the tracklet.
	m_pTailAvgFeat = new CFeature(box->m_pDetFeat);

	//Trajectory adjust spline.
	m_pAdjSpline = new spline1dinterpolant;

	//Types of the fitting.
	m_nBoundType = 2;
}

CTrack::CTrack(const CDetBBox& box)
{
	m_bActMark = true;
	//reserved detections.
	m_pDetPoints = new pDetBoxSet;
	m_pDetPoints->clear();

	CDetBBox* newdet = new CDetBBox(box);
	m_pDetPoints->push_back(newdet);

	m_pColor = new Palette;

	m_pColor->m_nR = rand()%256;
	m_pColor->m_nG = rand()%256;
	m_pColor->m_nB = rand()%128;

	//------------------------------------- Motion Information -------------------------------------------------//
	m_nEndFrmIdx = newdet->m_nFrameIndex;
	m_nBegFrmIdx = newdet->m_nFrameIndex;

	m_nTrackID = -1;
	m_nVirtIndex = -1;

	//The feature of the tracklet head.
	m_pHeadDetFeat = new CFeature(box.m_pDetFeat);
	//The feature of the tracklet tail.
	m_pTailDetFeat = new CFeature(box.m_pDetFeat);

	//The average feature of the tracklet.
	m_pHeadAvgFeat = new CFeature(box.m_pDetFeat);
	//The average feature of the tracklet.
	m_pTailAvgFeat = new CFeature(box.m_pDetFeat);

	//Trajectory adjust spline.
	m_pAdjSpline = new spline1dinterpolant;

	//Types of the fitting.
	m_nBoundType = 2;

}

CTrack::CTrack(const CTrack *track)
{
	m_pDetPoints = new pDetBoxSet;
	m_pDetPoints->clear();

	//reserve detection copy.
	pDetBoxSet::iterator box_iter;
	for (box_iter = track->m_pDetPoints->begin(); box_iter != track->m_pDetPoints->end(); box_iter++) {
		CDetBBox* box = new CDetBBox(*box_iter);
		m_pDetPoints->push_back(box);
	}

	m_pColor = new Palette;
  
	m_pColor->m_nR = track->m_pColor->m_nR;
	m_pColor->m_nG = track->m_pColor->m_nG;
	m_pColor->m_nB = track->m_pColor->m_nB;

	//--------------------------------- Motion Information of the Tracklet ---------------------------------------//
	m_nEndFrmIdx = track->m_nEndFrmIdx;
	m_nBegFrmIdx = track->m_nBegFrmIdx;

	m_nTrackID = track->m_nTrackID;
	m_nVirtIndex = track->m_nVirtIndex;

	m_bActMark = track->m_bActMark;

	//The segment index copy.
	m_nSegIndex = track->m_nSegIndex;

	//The feature of the tracklet head.
	m_pHeadDetFeat = new CFeature(track->m_pHeadDetFeat);
	//The feature of the tracklet tail.
	m_pTailDetFeat = new CFeature(track->m_pTailDetFeat);

	//The average feature of the tracklet head.
	m_pHeadAvgFeat = new CFeature(track->m_pHeadAvgFeat);
	//The average feature of the tracklet tail.
	m_pTailAvgFeat = new CFeature(track->m_pTailAvgFeat);

	//Types of the fitting.
	m_nBoundType = 2;

	//Trajectory adjust spline.
	m_pAdjSpline = new spline1dinterpolant;
}

CTrack::~CTrack()
{
	//Release the applied memory
	//display target id.
	//reserve detections.
	if (m_pDetPoints) {
		for (int i = 0; i < (int)(m_pDetPoints->size()); i++) {
			if ((*m_pDetPoints)[i]) {
				RELEASE((*m_pDetPoints)[i]);
				(*m_pDetPoints)[i] = 0;
			}
		}
		m_pDetPoints->clear();
		RELEASE(m_pDetPoints);
		m_pDetPoints = 0;
	}

	if (m_pColor) {
		RELEASE(m_pColor);
		m_pColor = 0;
	}

	//--------------------------------- Motion Information of the Tracklet ---------------------------------------//
	m_nEndFrmIdx = 0;
	m_nBegFrmIdx = 0;

	m_nTrackID = -1;
	m_nVirtIndex = -1;

	//Release the head and tail features.
	if (m_pHeadDetFeat) {
		RELEASE(m_pHeadDetFeat);
		m_pHeadDetFeat = 0;
	}
	if (m_pTailDetFeat) {
		RELEASE(m_pTailDetFeat);
		m_pTailDetFeat = 0;
	}
	//Release the average features of head and tail.
	if (m_pHeadAvgFeat) {
		RELEASE(m_pHeadAvgFeat);
		m_pHeadAvgFeat = 0;
	}
	if (m_pTailAvgFeat) {
		RELEASE(m_pTailAvgFeat);
		m_pTailAvgFeat = 0;
	}

	//Release the trajectory adjust variable.
	if (m_pAdjSpline) {
		RELEASE(m_pAdjSpline);
		m_pAdjSpline = 0;
	}

	m_nBoundType = 0;
}

bool CTrack::Add(const CDetBBox* box)
{
	CDetBBox* newbox = new CDetBBox(box);
	m_pDetPoints->push_back(newbox);
	SplineAdjust();
	
	return true;
}

bool CTrack::Add(const CTrack* track)
{
	CDetBBox* pbox = 0;
	for (int i = 0; i < (int)(track->m_pDetPoints->size()); i++) {
		pbox = (*(track->m_pDetPoints))[i];
		CDetBBox* newbox = new CDetBBox(pbox);
		m_pDetPoints->push_back(newbox);
	}
	SplineAdjust();
	
	return true;
}

bool CTrack::Add(const pDetBoxSet* boxes)
{
	CDetBBox* pbox = 0;
	for (int i = 0; i < (int)(boxes->size()); i++) {
		pbox = (*boxes)[i];
		CDetBBox* newbox = new CDetBBox(pbox);
		m_pDetPoints->push_back(newbox);
	}
	SplineAdjust();

	return true;
}

//Copy the tracklet variable.
bool CTrack::Copy(const CTrack* track)
{
	//Segment index of the tracklet.
	m_nSegIndex = track->m_nSegIndex;

	//Color of the tracklet
	m_pColor->m_nB = track->m_pColor->m_nB;
	m_pColor->m_nG = track->m_pColor->m_nG;
	m_pColor->m_nR = track->m_pColor->m_nR;

	//Start and end frame index of the tracklet.
	m_nEndFrmIdx = track->m_nEndFrmIdx;	
	m_nBegFrmIdx = track->m_nBegFrmIdx;

	//Tracklet index.
	m_nTrackID = track->m_nTrackID;
	m_nVirtIndex = track->m_nVirtIndex;

	//Feature copy.
	//Copy the head detection feature of the tracklet.
	m_pHeadDetFeat->Copy(track->m_pHeadDetFeat);
	//Copy the tail detection feature of the tracklet.
	m_pTailDetFeat->Copy(track->m_pTailDetFeat);

	//Copy the average head detection feature of the tracklet.
	m_pHeadAvgFeat->Copy(track->m_pHeadAvgFeat);
	//Copy the average tail detection feature of the tracklet.
	m_pTailAvgFeat->Copy(track->m_pTailAvgFeat);

	//Set the detections in the tracklet.
	//First release the detection variable of the tracklet.
	for (int i = 0; i < (int)(m_pDetPoints->size()); i++) {
		if ((*m_pDetPoints)[i]) {
			RELEASE((*m_pDetPoints)[i]);
			(*m_pDetPoints)[i] = 0;
		}
	}
	m_pDetPoints->clear();

	CDetBBox* pBox = 0;
	for (int i = 0; i < (int)(track->m_pDetPoints->size()); i++) {
		pBox = (*(track->m_pDetPoints))[i];
		CDetBBox* pNewDet = new CDetBBox(pBox);
		m_pDetPoints->push_back(pNewDet);
	}
	return true;
}


//Adjust the order of the spline.
bool CTrack::SplineAdjust()
{
	int nPoints = (int)(m_pDetPoints->size());
	if (nPoints > 0) {
		//first delete the same bounding box in the same sequences.
		vector<int> DeleteSet;
		DeleteSet.clear();

		for (int i = 0; i < nPoints - 1; i++) {
			for (int j = i + 1; j < nPoints; j++) {
				if ((*m_pDetPoints)[i]->m_nFrameIndex == (*m_pDetPoints)[j]->m_nFrameIndex) {
					if ((*m_pDetPoints)[i]->m_nDetBoxID == (*m_pDetPoints)[j]->m_nDetBoxID) {
						DeleteSet.push_back(j);
					}
					else {
						DeleteSet.push_back(i);
						DeleteSet.push_back(j);
					}
				}
			}
		}

		//delete the same elements.
		sort(DeleteSet.begin(), DeleteSet.end());
		DeleteSet.erase(unique(DeleteSet.begin(), DeleteSet.end()), DeleteSet.end());

		pDetBoxSet::iterator box_iter;
		for (int i = (int)(DeleteSet.size()) - 1; i >= 0; i--) {
			box_iter = m_pDetPoints->begin();
			for (int j = 0; j < DeleteSet[i]; j++)
				box_iter++;
			RELEASE(*box_iter);
			*box_iter = 0;
			box_iter = m_pDetPoints->erase(box_iter);
		}


		//order the list.
		CDetBBox* ptemp = 0;
		for (int i = 0; i < (int)(m_pDetPoints->size()) - 1; i++) {
			for (int j = i + 1; j < (int)(m_pDetPoints->size()); j++) {
				if ((*m_pDetPoints)[i]->m_nFrameIndex > (*m_pDetPoints)[j]->m_nFrameIndex) {
					ptemp = (*m_pDetPoints)[i];
					(*m_pDetPoints)[i] = (*m_pDetPoints)[j];
					(*m_pDetPoints)[j] = ptemp;
				}
			}
		}
	}

	nPoints = (int)(m_pDetPoints->size());
	//Update the start frame and end frame index.
	if (nPoints > 0) {
		m_nBegFrmIdx = (*m_pDetPoints)[0]->m_nFrameIndex;
		m_nEndFrmIdx = (*m_pDetPoints)[(int)(m_pDetPoints->size()) - 1]->m_nFrameIndex;
	}
	else {
		m_nBegFrmIdx = 0;
		m_nEndFrmIdx = 0;
	}

	return true;
}


//interpolate the tracklet
// bool CTrack::SplineFitting()
// {
// 	//Smooth the detection in the list.
// 	//Use the spline to get the smooth results of the target.
// 	//Adjust the size and position of the target.
// 	//----------------------------------------------------------------------------------//
// 	//------------------------- Position adjustment ------------------------------------//
// 	//----------------------------------------------------------------------------------//
//     int nOrgPts = (int)(m_pDetPoints->size());
//     int nFitPts = m_nEndFrmIdx - m_nBegFrmIdx + 1;
// 	//Check the parameters.
// 	if (nFitPts < nOrgPts) {
// 		fprintf(stderr, "Errors in Parameters of Tracklet Spline Fitting!\n");
// 		return false;
// 	}
// 
// 	//Check whether the points number satisfied to do the adjustment.
// 	if (nOrgPts >= 5) {
// 		//Construct the predict center position of the tracklet.
// 		//x, y position.
// 		CMatrix* pAdjMatx = matrix_create(3, nOrgPts);
// 		CMatrix* pAdjMaty = matrix_create(3, nOrgPts);
// 		//width, height size.
// 		CMatrix* pAdjMatw = matrix_create(3, nOrgPts);
// 		CMatrix* pAdjMath = matrix_create(3, nOrgPts);
// 
// 		//Some temporal variables.
// 		double* pAdjFitx = 0;
// 		double* pAdjFity = 0;
// 		double* pAdjFitw = 0;
// 		double* pAdjFith = 0;
// 		//Frame index variable
// 		double* pAdjFitIdx = 0;
// 
// 		double* pAdjGetx = 0;
// 		double* pAdjGety = 0;
// 		double* pAdjGetw = 0;
// 		double* pAdjGeth = 0;
// 		//Frame index variable
// 		double* pAdjGetIdx = 0;
// 
// 		//Mark variables.
// 		bool* pAdjMark = new bool[nOrgPts];
// 		for (int i = 0; i < nOrgPts; i++)
// 			pAdjMark[i] = false;
// 
// 		CDetBBox* pTempBox = 0;
// 
// 		int nAdjAry = 0;
// 		int nFitPts = 0;
// 		int nGetPts = 0;
// 
// 		for (int i = 0; i < 3; i++) {
// 			nAdjAry = G_SmoothGaps[i] + 1;
// 			if (nOrgPts % nAdjAry == 0)
// 				nFitPts = nOrgPts / nAdjAry;
// 			else
// 				nFitPts = nOrgPts / nAdjAry + 1;
// 
// 			nGetPts = nOrgPts - nFitPts;
// 
// 			//Reset the type variable.
// 			for (int j = 0; j < nOrgPts; j++)
// 				pAdjMark[j] = false;
// 
// 			//Construct the types array.
// 			pAdjMark[0] = true;
// 			pAdjMark[nOrgPts - 1] = true;
// 			for (int j = 0; j < nFitPts - 2; j++) 
// 				pAdjMark[(j + 1) * nAdjAry] = true;
// 
// 
// 			pAdjFitx = new double[nFitPts];
// 			pAdjFity = new double[nFitPts];
// 			pAdjFitw = new double[nFitPts];
// 			pAdjFith = new double[nFitPts];
// 			pAdjFitIdx = new double[nFitPts];
// 			
// 			pAdjGetx = new double[nGetPts];
// 			pAdjGety = new double[nGetPts];
// 			pAdjGetw = new double[nGetPts];
// 			pAdjGeth = new double[nGetPts];
// 			pAdjGetIdx = new double[nGetPts];
// 
// 			int nFitCount = 0;
// 			int nGetCount = 0;
// 			for (int j = 0; j < nOrgPts; j++) {
// 				pTempBox = (*m_pDetPoints)[j];
// 				if (pAdjMark[j]) {
// 					pAdjFitx[nFitCount] = pTempBox->m_n2Dcx;
// 					pAdjFity[nFitCount] = pTempBox->m_n2Dcy;
// 					pAdjFitw[nFitCount] = pTempBox->m_nWidth;
// 					pAdjFith[nFitCount] = pTempBox->m_nHeight;
// 					pAdjFitIdx[nFitCount] = pTempBox->m_nFrameIndex;
// 					nFitCount++;
// 				}
// 				else {
// 					pAdjGetx[nGetCount] = 0;
// 					pAdjGety[nGetCount] = 0;
// 					pAdjGetw[nGetCount] = 0;
// 					pAdjGeth[nGetCount] = 0;
// 					pAdjGetIdx[nGetCount] = pTempBox->m_nFrameIndex;
// 					nGetCount++;
// 				}
// 			}
// 
// 			//Set the spline values.
// 			real_1d_array spline_fit_cx;
// 			real_1d_array spline_fit_cy;
// 			real_1d_array spline_fit_cw;
// 			real_1d_array spline_fit_ch;
// 			real_1d_array spline_fit_ct;
// 
// 			spline_fit_cx.setcontent(nFitPts, pAdjFitx);
// 			spline_fit_cy.setcontent(nFitPts, pAdjFity);
// 			spline_fit_cw.setcontent(nFitPts, pAdjFitw);
// 			spline_fit_ch.setcontent(nFitPts, pAdjFith);
// 			spline_fit_ct.setcontent(nFitPts, pAdjFitIdx);
// 
// 			//Get the cubic spline of x coordinate.
// 			spline1dbuildcubic(spline_fit_ct, spline_fit_cx, nFitPts, m_nBoundType, 0.0, m_nBoundType, 0.0, *m_pAdjSpline);
// 			for (int j = 0; j < nGetPts; j++)
// 				pAdjGetx[j] = spline1dcalc(*m_pAdjSpline, pAdjGetIdx[j]);
// 
// 			//Get the cubic spline of y coordinate.
// 			spline1dbuildcubic(spline_fit_ct, spline_fit_cy, nFitPts, m_nBoundType, 0.0, m_nBoundType, 0.0, *m_pAdjSpline);
// 			for (int j = 0; j < nGetPts; j++)
// 				pAdjGety[j] = spline1dcalc(*m_pAdjSpline, pAdjGetIdx[j]);
// 
// 			//Get the cubic spline of w coordinate.
// 			spline1dbuildcubic(spline_fit_ct, spline_fit_cw, nFitPts, m_nBoundType, 0.0, m_nBoundType, 0.0, *m_pAdjSpline);
// 			for (int j = 0; j < nGetPts; j++)
// 				pAdjGetw[j] = spline1dcalc(*m_pAdjSpline, pAdjGetIdx[j]);
// 
// 			//Get the cubic spline of h coordinate.
// 			spline1dbuildcubic(spline_fit_ct, spline_fit_ch, nFitPts, m_nBoundType, 0.0, m_nBoundType, 0.0, *m_pAdjSpline);
// 			for (int j = 0; j < nGetPts; j++)
// 				pAdjGeth[j] = spline1dcalc(*m_pAdjSpline, pAdjGetIdx[j]);
// 
// 			//Copy the results.
// 			nFitCount = 0;
// 			nGetCount = 0;
// 			for (int j = 0; j < nOrgPts; j++) {
// 				if (pAdjMark[j]) {
// 					pAdjMatx->m_pMatrix[i][j] = pAdjFitx[nFitCount];
// 					pAdjMaty->m_pMatrix[i][j] = pAdjFity[nFitCount];
// 					pAdjMatw->m_pMatrix[i][j] = pAdjFitw[nFitCount];
// 					pAdjMath->m_pMatrix[i][j] = pAdjFith[nFitCount];
// 					nFitCount++;
// 				}
// 				else {
// 					pAdjMatx->m_pMatrix[i][j] = pAdjGetx[nGetCount];
// 					pAdjMaty->m_pMatrix[i][j] = pAdjGety[nGetCount];
// 					pAdjMatw->m_pMatrix[i][j] = pAdjGetw[nGetCount];
// 					pAdjMath->m_pMatrix[i][j] = pAdjGeth[nGetCount];
// 					nGetCount++;
// 				}
// 			}
// 
// 			//Release the memory.
// 			if (pAdjFitx) {
// 				RELEASEARRAY(pAdjFitx);
// 				pAdjFitx = 0;
// 			}
// 			if (pAdjFity) {
// 				RELEASEARRAY(pAdjFity);
// 				pAdjFity = 0;
// 			}
// 			if (pAdjFitw) {
// 				RELEASEARRAY(pAdjFitw);
// 				pAdjFitw = 0;
// 			}
//             if (pAdjFith) {
// 				RELEASEARRAY(pAdjFith);
// 				pAdjFith = 0;
//             }
// 			if (pAdjFitIdx) {
// 				RELEASEARRAY(pAdjFitIdx);
// 				pAdjFitIdx = 0;
// 			}
// 
// 			if (pAdjGetx) {
// 				RELEASEARRAY(pAdjGetx);
// 				pAdjGetx = 0;
// 			}
// 			if (pAdjGety) {
// 				RELEASEARRAY(pAdjGety);
// 				pAdjGety = 0;
// 			}
// 			if (pAdjGetw) {
// 				RELEASEARRAY(pAdjGetw);
// 				pAdjGetw = 0;
// 			}
// 			if (pAdjGeth) {
// 				RELEASEARRAY(pAdjGeth);
// 				pAdjGeth = 0;
// 			}
// 			if (pAdjGetIdx) {
// 				RELEASEARRAY(pAdjGetIdx);
// 				pAdjGetIdx = 0;
// 			}
// 		}
// 
// 		//----------------------------------------------------------------------------------------------------------//
// 		//--------- Update the position and size of the target detections.
// 		//---------------------------------------------------------------------------------------------------------//
// 		double nCx, nCy;
// 		double nCw, nCh;
// 		//Update the position of the targets.
// 		for (int i = 0; i < nOrgPts; i++) {
// 			nCx = 0;
// 			nCy = 0;
// 			nCw = 0;
// 			nCh = 0;
// 			for (int j = 0; j < 3; j++) {
// 				nCx += pAdjMatx->m_pMatrix[j][i];
// 				nCy += pAdjMaty->m_pMatrix[j][i];
// 				nCw += pAdjMatw->m_pMatrix[j][i];
// 				nCh += pAdjMath->m_pMatrix[j][i];
// 			}
// 			nCx = nCx / 3;
// 			nCy = nCy / 3;
// 			nCw = nCw / 3;
// 			nCh = nCh / 3;
// 
// 			pTempBox = (*m_pDetPoints)[i];
// 			pTempBox->m_n2Dcx = (int)(nCx);
// 			pTempBox->m_n2Dcy = (int)(nCy);
// 			pTempBox->m_nWidth = (int)(nCw);
// 			pTempBox->m_nHeight = (int)(nCh);
// 		}
// 
// 		//----------------------- Release the memory ----------------------------------//
// 		if (pAdjMatx) {
// 			RELEASE(pAdjMatx);
// 			pAdjMatx = 0;
// 		}
// 		if (pAdjMaty) {
// 			RELEASE(pAdjMaty);
// 			pAdjMaty = 0;
// 		}
// 		if (pAdjMatw) {
// 			RELEASE(pAdjMatw);
// 			pAdjMatw = 0;
// 		}
// 		if (pAdjMath) {
// 			RELEASE(pAdjMath);
// 			pAdjMath = 0;
// 		}
// 		if (pAdjMark) {
// 			RELEASEARRAY(pAdjMark);
// 			pAdjMark = 0;
// 		}
// 		pTempBox = 0;
// 	}
// 
// 	nFitPts = m_nEndFrmIdx - m_nBegFrmIdx + 1;
//     //----------------------------------------------------------------------------------//
// 	//----------------------- Trajectory Spline Fitting --------------------------------//
// 	//----------------------------------------------------------------------------------//
// 	//Decide according to the length of the trajectory.
// 	if (nOrgPts >= 2) {
// 		//Get the length of the detection set.
// 		nOrgPts = (int)(m_pDetPoints->size());
// 		//Construct the predict center position of the tracklet.
// 		//Fitting points.
// 		double* pF2DXs = new double[nOrgPts];
// 		double* pF2DYs = new double[nOrgPts];
// 
// 		double* pF2DWs = new double[nOrgPts];
// 		double* pF2DHs = new double[nOrgPts];
// 
// 		double* pFFrms = new double[nOrgPts];
// 		//Prediction points.
// 		double* pP2DXs = new double[nFitPts];
// 		double* pP2DYs = new double[nFitPts];
// 
// 		double* pP2DWs = new double[nFitPts];
// 		double* pP2DHs = new double[nFitPts];
// 
// 		double* pPFrms = new double[nFitPts];
// 
// 		//Mark vector to indicate whether it is a interpolate point or not.
// 		bool* pMarks = new bool[nFitPts];
// 		for (int i = 0; i < nFitPts; i++)
// 			pMarks[i] = false;
// 
// 		CDetBBox* pBox = 0;
// 		//Set existing point marks.
// 		for (int i = 0; i < nOrgPts; i++) {
// 			pBox = (*m_pDetPoints)[i];
// 			pMarks[pBox->m_nFrameIndex - m_nBegFrmIdx] = true;
// 		}
// 
// 		//***********************************************//
// 		//************** 2D Position and Size Adjustment
// 		//***********************************************//
// 		for (int i = 0; i < nOrgPts; i++) {
// 			pBox = (*m_pDetPoints)[i];
// 			pF2DXs[i] = pBox->m_n2Dcx;
// 			pF2DYs[i] = pBox->m_n2Dcy;
// 			pF2DWs[i] = pBox->m_nWidth;
// 			pF2DHs[i] = pBox->m_nHeight;
// 			pFFrms[i] = pBox->m_nFrameIndex;
// 		}
// 
// 		//Set the spline values.
// 		real_1d_array spline_cx;
// 		real_1d_array spline_cy;
// 		real_1d_array spline_cw;
// 		real_1d_array spline_ch;
// 		real_1d_array spline_ct;
// 
// 		spline_cx.setcontent(nOrgPts, pF2DXs);
// 		spline_cy.setcontent(nOrgPts, pF2DYs);
// 		spline_cw.setcontent(nOrgPts, pF2DWs);
// 		spline_ch.setcontent(nOrgPts, pF2DHs);
// 		spline_ct.setcontent(nOrgPts, pFFrms);
// 
// 		//Get the cubic spline of x coordinate.
// 		spline1dbuildcubic(spline_ct, spline_cx, nOrgPts, m_nBoundType, 0.0, m_nBoundType, 0.0, *m_pSplineX);
// 		//Get the cubic spline of y coordinate.
// 		spline1dbuildcubic(spline_ct, spline_cy, nOrgPts, m_nBoundType, 0.0, m_nBoundType, 0.0, *m_pSplineY);
// 		//Get the cubic spline of w coordinate.
// 		spline1dbuildcubic(spline_ct, spline_cw, nOrgPts, m_nBoundType, 0.0, m_nBoundType, 0.0, *m_pSplineW);
// 		//Get the cubic spline of h coordinate.
// 		spline1dbuildcubic(spline_ct, spline_ch, nOrgPts, m_nBoundType, 0.0, m_nBoundType, 0.0, *m_pSplineH);
// 
// 		//Get the fitting time coordinates.
// 		for (int i = 0; i < nFitPts; i++)
// 			pPFrms[i] = m_nBegFrmIdx + i;
// 
// 		//Get the fitting values of x coordinate.
// 		for (int i = 0; i < nFitPts; i++)
// 			pP2DXs[i] = spline1dcalc(*m_pSplineX, pPFrms[i]);
// 		for (int i = 0; i < nFitPts; i++)
// 			pP2DYs[i] = spline1dcalc(*m_pSplineY, pPFrms[i]);
// 		for (int i = 0; i < nFitPts; i++)
// 			pP2DWs[i] = spline1dcalc(*m_pSplineW, pPFrms[i]);
// 		for (int i = 0; i < nFitPts; i++)
// 			pP2DHs[i] = spline1dcalc(*m_pSplineH, pPFrms[i]);
// 
// 		//****************************************************************************************************//
// 		//Add the Missing Data and Update the 2D Position and Size of the Target Detections.
// 		//*****************************************************************************************************//
// 		//Original points count.
// 		int nCount = 0;
// 		//Add detection box set.
// 		pDetBoxSet* pAddBoxes = new pDetBoxSet;
// 		pAddBoxes->clear();
// 
// 		double n2Dx = 0;
// 		double n2Dy = 0;
// 		for (int i = 0; i < nFitPts; i++) {
// 			if (pMarks[i]) {
// 				pBox = (*m_pDetPoints)[nCount];
// 				//2D position.
// 				pBox->m_n2Dcx = (int)(pP2DXs[i]);
// 				pBox->m_n2Dcy = (int)(pP2DYs[i]);
// 				//2D size.
// 				pBox->m_nWidth = (int)(pP2DWs[i]);
// 				pBox->m_nHeight = (int)(pP2DHs[i]);
// 
// 				nCount++;
// 			}
// 			else {
// 				//Create a new detection points.
// 				CDetBBox* pNewDet = new CDetBBox;
// 				pNewDet->m_bVirtMark = true;
// 				pNewDet->m_nTrackID = (*m_pDetPoints)[0]->m_nTrackID;
// 
// 				pNewDet->m_nFrameIndex = i + m_nBegFrmIdx;
// 				pNewDet->m_nConf = 0;
// 				pNewDet->m_nDetBoxID = -1;
// 				//Some interpolate information in the space.
// 				pNewDet->m_n2Dcx = pP2DXs[i];
// 				pNewDet->m_n2Dcy = pP2DYs[i];
// 				//2D size.
// 				pNewDet->m_nWidth = (int)(pP2DWs[i]);
// 				pNewDet->m_nHeight = (int)(pP2DHs[i]);
// 
// 				//Get the feature information according to the first and second points.
// 				pNewDet->CopyFeats((*m_pDetPoints)[nCount - 1]->m_pFeatPict, (*m_pDetPoints)[nCount - 1]->m_pDetFeat);
// 
// 				pAddBoxes->push_back(pNewDet);
// 			}
// 		}
// 
// 		Add(pAddBoxes);
// 		//*******************************************************************//
// 		//Release the memory.
// 		if (pF2DXs) {
// 			RELEASEARRAY(pF2DXs);
// 			pF2DXs = 0;
// 		}
// 		if (pF2DYs) {
// 			RELEASEARRAY(pF2DYs);
// 			pF2DYs = 0;
// 		}
// 		if (pF2DWs) {
// 			RELEASEARRAY(pF2DWs);
// 			pF2DWs = 0;
// 		}
// 		if (pF2DHs) {
// 			RELEASEARRAY(pF2DHs);
// 			pF2DHs = 0;
// 		}
// 		if (pFFrms) {
// 			RELEASEARRAY(pFFrms);
// 			pFFrms = 0;
// 		}
// 		if (pP2DXs) {
// 			RELEASEARRAY(pP2DXs);
// 			pP2DXs = 0;
// 		}
// 		if (pP2DYs) {
// 			RELEASEARRAY(pP2DYs);
// 			pP2DYs = 0;
// 		}
// 		if (pP2DWs) {
// 			RELEASEARRAY(pP2DWs);
// 			pP2DWs = 0;
// 		}
// 		if (pP2DHs) {
// 			RELEASEARRAY(pP2DHs);
// 			pP2DHs = 0;
// 		}
// 		if (pPFrms) {
// 			RELEASEARRAY(pPFrms);
// 			pPFrms = 0;
// 		}
// 		if (pMarks) {
// 			RELEASEARRAY(pMarks);
// 			pMarks = 0;
// 		}
// 		if (pAddBoxes) {
// 			for (int i = 0; i < (int)(pAddBoxes->size()); i++) {
// 				if ((*pAddBoxes)[i]) {
// 					RELEASE((*pAddBoxes)[i]);
// 					(*pAddBoxes)[i] = 0;
// 				}
// 			}
// 			pAddBoxes->clear();
// 			RELEASE(pAddBoxes);
// 			pAddBoxes = 0;
// 		}
// 		pBox = 0;
// 
// 		//Set the spline mark to true.
// 		m_bSplineMark = true;
// 
// 		return true;
// 
// 	}
// 	else {
// 		return true;
// 	}
// }




bool CTrack::SplineFitting()
{
	//Smooth the detection size in the list.
	//Utilize the spline line to get the smooth results of the target.
	//Adjust the size and position of the target.
	//----------------------- Position adjustment ---------------------------------//
	//Sampling the points to be the input of the spline line
	int nPoints = (int)(m_pDetPoints->size());
	int nBreaks = 0;
	int nTests = 0;
	int nAry = 0;
	int nFitCt = 0;
	int nTestCt = 0;

	//it is just a pointer.
	pDetBoxSet* pAddDets = new pDetBoxSet;

	int nPreIdx = 0;
	int nNetIdx = 0;
	int nMidIdx = 0;
	int nAdds = 0;
	for (int i = 0; i < (int)(m_pDetPoints->size() - 1); i++) {
		nPreIdx = (*m_pDetPoints)[i]->m_nFrameIndex;
		nNetIdx = (*m_pDetPoints)[i+1]->m_nFrameIndex;
		nAdds = nNetIdx - nPreIdx - 1;
		if (nAdds > 0) {
			nMidIdx = (nPreIdx + nNetIdx) / 2;
			for (int j = 0; j < nAdds; j++) {
				CDetBBox* pNewBox = new CDetBBox;
				pNewBox->m_bVirtMark = true;
				pNewBox->m_nTrackID = (*m_pDetPoints)[0]->m_nTrackID;

				pNewBox->m_nFrameIndex = nPreIdx + j + 1;
				pNewBox->m_nConf = 0;
				pNewBox->m_nDetBoxID = -1;
				pNewBox->m_n2Dcx = (*m_pDetPoints)[i]->m_n2Dcx + ((*m_pDetPoints)[i+1]->m_n2Dcx - (*m_pDetPoints)[i]->m_n2Dcx) * (j + 1) / (nAdds + 1);
				pNewBox->m_n2Dcy = (*m_pDetPoints)[i]->m_n2Dcy + ((*m_pDetPoints)[i+1]->m_n2Dcy - (*m_pDetPoints)[i]->m_n2Dcy) * (j + 1) / (nAdds + 1);
				pNewBox->m_nWidth = (*m_pDetPoints)[i]->m_nWidth + ((*m_pDetPoints)[i+1]->m_nWidth - (*m_pDetPoints)[i]->m_nWidth) * (j + 1) / (nAdds + 1);
				pNewBox->m_nHeight = (*m_pDetPoints)[i]->m_nHeight + ((*m_pDetPoints)[i+1]->m_nHeight - (*m_pDetPoints)[i]->m_nHeight) * (j + 1) / (nAdds + 1);
				

				//Get the features according to the first and second points.
				if (pNewBox->m_nFrameIndex > nMidIdx)
					pNewBox->CopyFeats((*m_pDetPoints)[i+1]->m_pFeatPict, (*m_pDetPoints)[i+1]->m_pDetFeat);
				else 
					pNewBox->CopyFeats((*m_pDetPoints)[i]->m_pFeatPict, (*m_pDetPoints)[i]->m_pDetFeat);

				pAddDets->push_back(pNewBox);
			}
		}
	}

	for (int i = 0; i < (int)(pAddDets->size()); i++) {
		m_pDetPoints->push_back((*pAddDets)[i]);
	}

	pAddDets->clear();
	RELEASE(pAddDets);
	pAddDets = 0;

	//Adjust the bounding box in the spline.
	SplineAdjust();

	//Check the number of points on the spline
	nPoints = (int)(m_pDetPoints->size());
	if (nPoints <= 5)
		return true;

	//Construct the predict center position of the tracklet.
	//x, y position.
	CMatrix* pPredx = matrix_create(3, nPoints);
	CMatrix* pPredy = matrix_create(3, nPoints);
	//width, height size
	CMatrix* pPredsw = matrix_create(3, nPoints);
	CMatrix* pPredsh = matrix_create(3, nPoints);

	double* pFxs = 0;
	double* pFys = 0;
	double* pFws = 0;
	double* pFhs = 0;
	double* pFsign = 0;

	double* pPxs = 0;
	double* pPys = 0;
	double* pPws = 0;
	double* pPhs = 0;
	double* pTsign = 0;

	bool* pTypes = new bool[nPoints];
	for (int i = 0; i < nPoints; i++)
		pTypes[i] = false;

	CDetBBox* pBox = 0;

	for (int i = 0; i < 1; i++) {
		nAry = G_SmoothGaps[i] + 1;
		if (nPoints % nAry == 0)
			nBreaks = nPoints / nAry;
		else
			nBreaks = nPoints / nAry + 1;

		nTests = nPoints - nBreaks;

		//Reset the type variable.
		for (int j = 0; j < nPoints; j++)
			pTypes[j] = false;

		//Construct the types array.
		pTypes[0] = true;
		pTypes[nPoints - 1] = true;
		for (int j = 0; j < nBreaks - 2; j++) 
			pTypes[(j + 1) * nAry] = true;


		pFxs = new double[nBreaks];
		pFys = new double[nBreaks];
		pFsign = new double[nBreaks];
		pPxs = new double[nTests];
		pPys = new double[nTests];
		pTsign = new double[nTests];

		nFitCt = 0;
		nTestCt = 0;
		for (int j = 0; j < nPoints; j++) {
			pBox = (*m_pDetPoints)[j];
			if (pTypes[j]) {
				pFxs[nFitCt] = pBox->m_n2Dcx;
				pFys[nFitCt] = pBox->m_n2Dcy;
				pFsign[nFitCt] = pBox->m_nFrameIndex;
				nFitCt++;
			}
			else {
				pTsign[nTestCt] = pBox->m_nFrameIndex;
				nTestCt++;
			}
		}

		//Set the spline values.
		real_1d_array algcx;
		real_1d_array algcy;
		real_1d_array algct;

		algcx.setcontent(nBreaks, pFxs);
		algcy.setcontent(nBreaks, pFys);
		algct.setcontent(nBreaks, pFsign);

		//Get the cubic spline of x coordinate.
		spline1dbuildcubic(algct, algcx, nBreaks, m_nBoundType, 0.0, m_nBoundType, 0.0, *m_pAdjSpline);
		for (int j = 0; j < nTests; j++)
			pPxs[j] = spline1dcalc(*m_pAdjSpline, pTsign[j]);

		//Get the cubic spline of y coordinate.
		spline1dbuildcubic(algct, algcy, nBreaks, m_nBoundType, 0.0, m_nBoundType, 0.0, *m_pAdjSpline);
		for (int j = 0; j < nTests; j++)
			pPys[j] = spline1dcalc(*m_pAdjSpline, pTsign[j]);


		//Copy the results.
		nFitCt = 0;
		nTestCt = 0;
		for (int j = 0; j < nPoints; j++) {
			if (pTypes[j]) {
				pPredx->m_pMatrix[i][j] = pFxs[nFitCt];
				pPredy->m_pMatrix[i][j] = pFys[nFitCt];
				nFitCt++;
			}
			else {
				pPredx->m_pMatrix[i][j] = pPxs[nTestCt];
				pPredy->m_pMatrix[i][j] = pPys[nTestCt];
				nTestCt++;
			}
		}

		//Release the memory.
		if (pFxs) {
			RELEASEARRAY(pFxs);
			pFxs = 0;
		}
		if (pFys) {
			RELEASEARRAY(pFys);
			pFys = 0;
		}
		if (pFsign) {
			RELEASEARRAY(pFsign);
			pFsign = 0;
		}
		if (pPxs) {
			RELEASEARRAY(pPxs);
			pPxs = 0;
		}
		if (pPys) {
			RELEASEARRAY(pPys);
			pPys = 0;
		}
		if (pTsign) {
			RELEASEARRAY(pTsign);
			pTsign = 0;
		}
	}

	//----------------------- Size adjustment -------------------------------------//
	for (int i = 0; i < 1; i++) {
		nAry = G_SmoothGaps[i] + 1;
		if (nPoints % nAry == 0)
			nBreaks = nPoints / nAry;
		else
			nBreaks = nPoints / nAry + 1;

		nTests = nPoints - nBreaks;

		//Reset the type variable.
		for (int j = 0; j < nPoints; j++)
			pTypes[j] = false;

		//Construct the types array.
		pTypes[0] = true;
		pTypes[nPoints - 1] = true;
		for (int j = 0; j < nBreaks - 2; j++) 
			pTypes[(j + 1) * nAry] = true;


		pFws = new double[nBreaks];
		pFhs = new double[nBreaks];
		pFsign = new double[nBreaks];
		pPws = new double[nTests];
		pPhs = new double[nTests];
		pTsign = new double[nTests];

		nFitCt = 0;
		nTestCt = 0;
		for (int j = 0; j < nPoints; j++) {
			pBox = (*m_pDetPoints)[j];
			if (pTypes[j]) {
				pFws[nFitCt] = pBox->m_nWidth;
				pFhs[nFitCt] = pBox->m_nHeight;
				pFsign[nFitCt] = pBox->m_nFrameIndex;
				nFitCt++;
			}
			else {
				pTsign[nTestCt] = pBox->m_nFrameIndex;
				nTestCt++;
			}
		}

		//Set the spline values.
		real_1d_array algcw;
		real_1d_array algch;
		real_1d_array algct;

		algcw.setcontent(nBreaks, pFws);
		algch.setcontent(nBreaks, pFhs);
		algct.setcontent(nBreaks, pFsign);

		//Get the cubic spline of x coordinate.
		spline1dbuildcubic(algct, algcw, nBreaks, m_nBoundType, 0.0, m_nBoundType, 0.0, *m_pAdjSpline);
		for (int j = 0; j < nTests; j++)
			pPws[j] = spline1dcalc(*m_pAdjSpline, pTsign[j]);

		//Get the cubic spline of y coordinate.
		spline1dbuildcubic(algct, algch, nBreaks, m_nBoundType, 0.0, m_nBoundType, 0.0, *m_pAdjSpline);
		for (int j = 0; j < nTests; j++)
			pPhs[j] = spline1dcalc(*m_pAdjSpline, pTsign[j]);


		//Copy the results.
		nFitCt = 0;
		nTestCt = 0;
		for (int j = 0; j < nPoints; j++) {
			if (pTypes[j]) {
				pPredsw->m_pMatrix[i][j] = pFws[nFitCt];
				pPredsh->m_pMatrix[i][j] = pFhs[nFitCt];
				nFitCt++;
			}
			else {
				pPredsw->m_pMatrix[i][j] = pPws[nTestCt];
				pPredsh->m_pMatrix[i][j] = pPhs[nTestCt];
				nTestCt++;
			}
		}

		//Release the memory.
		if (pFws) {
			RELEASEARRAY(pFws);
			pFws = 0;
		}
		if (pFhs) {
			RELEASEARRAY(pFhs);
			pFhs = 0;
		}
		if (pFsign) {
			RELEASEARRAY(pFsign);
			pFsign = 0;
		}
		if (pPws) {
			RELEASEARRAY(pPws);
			pPws = 0;
		}
		if (pPhs) {
			RELEASEARRAY(pPhs);
			pPhs = 0;
		}
		if (pTsign) {
			RELEASEARRAY(pTsign);
			pTsign = 0;
		}
	}

	//----------------------------------------------------------------------------------------------------------//
	//--------- Update the position and size of the target detections.
	//---------------------------------------------------------------------------------------------------------//
	double nCx, nCy;
	double nSw, nSh;
	//Update the position of the targets.
	for (int i = 0; i < nPoints; i++) {
		nCx = 0;
		nCy = 0;
		nSw = 0;
		nSh = 0;
		for (int j = 0; j < 1; j++) {
			nCx += pPredx->m_pMatrix[j][i];
			nCy += pPredy->m_pMatrix[j][i];
			nSw += pPredsw->m_pMatrix[j][i];
			nSh += pPredsh->m_pMatrix[j][i];
		}
		nCx = nCx;
		nCy = nCy;
		nSw = nSw;
		nSh = nSh;

		pBox = (*m_pDetPoints)[i];
		pBox->m_n2Dcx = (int)(nCx);
		pBox->m_n2Dcy = (int)(nCy);
		pBox->m_nWidth = (int)(nSw);
		pBox->m_nHeight = (int)(nSh);
	}

	//----------------------- Release the memory ----------------------------------//
	if (pPredx) {
		RELEASE(pPredx);
		pPredx = 0;
	}
	if (pPredy) {
		RELEASE(pPredy);
		pPredy = 0;
	}
	if (pPredsw) {
		RELEASE(pPredsw);
		pPredsw = 0;
	}
	if (pPredsh) {
		RELEASE(pPredsh);
		pPredsh = 0;
	}
	if (pTypes) {
		RELEASEARRAY(pTypes);
		pTypes = 0;
	}
	pBox = 0;

	return true;
}



//Get the appearance feature of the tracklets.
bool CTrack::GetAppearance()
{
	//******************************************************************************//
	//*** Extract the head and tail detection features.
	//Clear the previous memory of the tracklet.
	m_pHeadDetFeat->Reset();
	m_pTailDetFeat->Reset();
	//---------------------Copy the head detection feature.
	m_pHeadDetFeat->Copy((*m_pDetPoints)[0]->m_pDetFeat);

	//---------------------Copy the tail detection feature.
	m_pTailDetFeat->Copy((*m_pDetPoints)[(int)(m_pDetPoints->size() - 1)]->m_pDetFeat);

	//********************************************************************************//
	//*** Extract the average head and tail detection features.
	//Clear the previous memory of the tracklet.
	m_pHeadAvgFeat->Reset();
	m_pTailAvgFeat->Reset();

	int nLength = (int)(m_pDetPoints->size()) / 2;
	int nAverage = min(nLength, G_TRACK_NUMBER_DETECTION_AVERAGE_FEATURE);
	CDetBBox* pDet = 0;
	CHistogram* pHist = 0;

	//-------------------------------------------------------------------------------------//
	//--------------------Extract the head average features.
	//Number of active detection box with features.
	int nAdd = 0;
	for (int i = 0; i < nAverage; i++) {
		pDet = (*m_pDetPoints)[i];
		if (pDet->m_bVirtMark)
			continue;
		//Get the color histogram.
		pHist = pDet->m_pDetFeat->m_pColorFeat;
		for (int j = 0; j < m_pHeadAvgFeat->m_pColorFeat->m_nDim; j++)
			m_pHeadAvgFeat->m_pColorFeat->m_pHist[j] += pHist->m_pHist[j];

		pHist = pDet->m_pDetFeat->m_pShapeFeat;
		for (int j = 0; j < m_pHeadAvgFeat->m_pShapeFeat->m_nDim; j++)
			m_pHeadAvgFeat->m_pShapeFeat->m_pHist[j] += pHist->m_pHist[j];

		pHist = pDet->m_pDetFeat->m_pTextFeat;
		for (int j = 0; j < m_pHeadAvgFeat->m_pTextFeat->m_nDim; j++)
			m_pHeadAvgFeat->m_pTextFeat->m_pHist[j] += pHist->m_pHist[j];

		nAdd++;
	}

	if (nAdd > 0) {
		//Divide the adding number
		for (int j = 0; j < m_pHeadAvgFeat->m_pColorFeat->m_nDim; j++)
			m_pHeadAvgFeat->m_pColorFeat->m_pHist[j] /= nAdd;

		for (int j = 0; j < m_pHeadAvgFeat->m_pShapeFeat->m_nDim; j++)
			m_pHeadAvgFeat->m_pShapeFeat->m_pHist[j] /= nAdd;

		for (int j = 0; j < m_pHeadAvgFeat->m_pTextFeat->m_nDim; j++)
			m_pHeadAvgFeat->m_pTextFeat->m_pHist[j] /= nAdd;
	}
	
	//-------------------------------------------------------------------------------------//
	//--------------------Extract the tail average features.
	nAdd = 0;
	for (int i = nLength - nAverage; i < nLength; i++) {
		pDet = (*m_pDetPoints)[i];
		if (pDet->m_bVirtMark)
			continue;
		//Get the color histogram.
		pHist = pDet->m_pDetFeat->m_pColorFeat;
		for (int j = 0; j < m_pTailAvgFeat->m_pColorFeat->m_nDim; j++)
			m_pTailAvgFeat->m_pColorFeat->m_pHist[j] += pHist->m_pHist[j];

		pHist = pDet->m_pDetFeat->m_pShapeFeat;
		for (int j = 0; j < m_pTailAvgFeat->m_pShapeFeat->m_nDim; j++)
			m_pTailAvgFeat->m_pShapeFeat->m_pHist[j] += pHist->m_pHist[j];

		pHist = pDet->m_pDetFeat->m_pTextFeat;
		for (int j = 0; j < m_pTailAvgFeat->m_pTextFeat->m_nDim; j++)
			m_pTailAvgFeat->m_pTextFeat->m_pHist[j] += pHist->m_pHist[j];

		nAdd++;
	}

	if (nAdd > 0) {
		//Divide the adding number
		for (int j = 0; j < m_pTailAvgFeat->m_pColorFeat->m_nDim; j++)
			m_pTailAvgFeat->m_pColorFeat->m_pHist[j] /= nAdd;

		for (int j = 0; j < m_pTailAvgFeat->m_pShapeFeat->m_nDim; j++)
			m_pTailAvgFeat->m_pShapeFeat->m_pHist[j] /= nAdd;

		for (int j = 0; j < m_pTailAvgFeat->m_pTextFeat->m_nDim; j++)
			m_pTailAvgFeat->m_pTextFeat->m_pHist[j] /= nAdd;
	}
	
	return true;
}

bool Swap(IntVector& vData,int nIdx1,int nIdx2) 
{     
	int nTemp;
	nTemp = vData[nIdx1];
	vData[nIdx1] = vData[nIdx2];
	vData[nIdx2] = nTemp;
	return true;
} 