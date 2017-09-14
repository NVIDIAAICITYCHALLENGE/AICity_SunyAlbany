/*
 * GraphMultiCarTracker
 * Do not distribute
 *  Created on: Nov 08, 2015
 *      Author: Longyin Wen
 */
#include "export.h"
#include <cstring>



CExport::CExport()
{
	m_pDataName = 0;
	m_pOutputImagePath = 0;
	m_pOutputTrackPath = 0;
	m_pOutputSpeedPath = 0;
	//Sequential Image Size.
	m_nWidth = 0;
	m_nHeight = 0;

	m_DebugMark = 0;
	//Debug Parameters.
	m_pOutTracksImagePath_Debug = 0;
	//Debug Information.
	m_pOutTargetsImagePath_Debug = 0;
}

CExport::~CExport()
{
	m_pDataName = 0;
	m_pOutputImagePath = 0;
	m_pOutputTrackPath = 0;
	m_pOutputSpeedPath = 0;

	//Sequential Image Size.
	m_nWidth = 0;
	m_nHeight = 0;

	m_DebugMark = 0;
	//Debug Parameters.
	m_pOutTracksImagePath_Debug = 0;
	//Debug Information.
	m_pOutTargetsImagePath_Debug = 0;
}

bool CExport::Init(ExportPara* pParameters)
{
	m_pDataName = pParameters->m_gDataName;
	m_pOutputImagePath = pParameters->m_gExportVideoFilePath;
	m_pOutputTrackPath = pParameters->m_gExportTrajFilePath;
	m_pOutputSpeedPath = pParameters->m_gExportSpeedFilePath;
	

	m_nWidth = pParameters->m_nWidth;
	m_nHeight = pParameters->m_nHeight;

	//Debug Parameters.
	m_pOutTracksImagePath_Debug = pParameters->m_gTracksImagePath_Debug;
	m_pOutTargetsImagePath_Debug = pParameters->m_gTargetsImagePath_Debug;
	m_pOccupyMapImagePath_Debug = pParameters->m_gOccupyImagePath_Debug;

	m_DebugMark = pParameters->m_gDebugMark;
	return true;
}


//save the trajectory in the TXT format.
bool CExport::ExportTrajectory(pImageSet* pFrames, pTrackSet* pTracks)
{
	if (pFrames->empty())
		return false;
	
	//first and the end ID of the windows.
	int nf = (int)(pFrames->size());
	int fid, eid;
	fid = (*pFrames)[0]->m_nFrameIndex;
	eid = (*pFrames)[nf - 1]->m_nFrameIndex;

	int nframe = eid - fid + 1;
	int ntracks = (int)(pTracks->size());

	char lxPath[2048] = "";
	char lyPath[2048] = "";
	char wPath[2048] = "";
	char hPath[2048] = "";

	char* lxName = "_LX";
	char* lyName = "_LY";
	char* wName = "_W";
	char* hName = "_H";
	char* cSufix = ".txt";
	/*
	// YI: change strcpy_s, strcat_s to linux compatiable
	//for x coordinate.
	strcpy_s(lxPath, 1024, m_pOutputTrackPath);
	strcat_s(lxPath, 300, m_pDataName);
	strcat_s(lxPath, 300, lxName);
	strcat_s(lxPath, 300, cSufix);

	//for y coordinate.
	strcpy_s(lyPath, 1024, m_pOutputTrackPath);
	strcat_s(lyPath, 300, m_pDataName);
	strcat_s(lyPath, 300, lyName);
	strcat_s(lyPath, 300, cSufix);

	//for target width coordinate.
	strcpy_s(wPath, 1024, m_pOutputTrackPath);
	strcat_s(wPath, 300, m_pDataName);
	strcat_s(wPath, 300, wName);
	strcat_s(wPath, 300, cSufix);

	//for target height coordinate.
	strcpy_s(hPath, 1024, m_pOutputTrackPath);
	strcat_s(hPath, 300, m_pDataName);
	strcat_s(hPath, 300, hName);
	strcat_s(hPath, 300, cSufix);
	*/
	//for x coordinate.
	strcpy(lxPath, m_pOutputTrackPath);
	strcat(lxPath, m_pDataName);
	strcat(lxPath, lxName);
	strcat(lxPath, cSufix);

	//for y coordinate.
	strcpy(lyPath, m_pOutputTrackPath);
	strcat(lyPath, m_pDataName);
	strcat(lyPath, lyName);
	strcat(lyPath, cSufix);

	//for target width coordinate.
	strcpy(wPath, m_pOutputTrackPath);
	strcat(wPath, m_pDataName);
	strcat(wPath, wName);
	strcat(wPath, cSufix);

	//for target height coordinate.
	strcpy(hPath, m_pOutputTrackPath);
	strcat(hPath, m_pDataName);
	strcat(hPath, hName);
	strcat(hPath, cSufix);	

	//-------------------------------------------------------- Construct the matrix ------------------------------------------------------------------//
	CMatrix* lxPoints = matrix_create(nframe, ntracks);
	CMatrix* lyPoints = matrix_create(nframe, ntracks);
	CMatrix* wPoints = matrix_create(nframe, ntracks);
	CMatrix* hPoints = matrix_create(nframe, ntracks);

	//-------------------------------------------------------- Set values of the matrix ---------------------------------------------------------------------//
	int row, col;
	CDetBBox* detbox;
	for (int i = 0; i < (int)(pTracks->size()); i++) {
		for (int j = 0; j < (int)((*pTracks)[i]->m_pDetPoints->size()); j++) {
			detbox = (*((*pTracks)[i]->m_pDetPoints))[j];
			row = detbox->m_nFrameIndex - fid;
			col = i;
			lxPoints->m_pMatrix[row][col] = max(0, (int)(detbox->m_n2Dcx - detbox->m_nWidth / 2.0 + 1));
			lyPoints->m_pMatrix[row][col] = max(0, (int)(detbox->m_n2Dcy - detbox->m_nHeight / 2.0 + 1));
			wPoints->m_pMatrix[row][col] = detbox->m_nWidth;
			hPoints->m_pMatrix[row][col] = detbox->m_nHeight;
		}
	}

	//-------------------------------------------------------- Save the tracking information -----------------------------------------------------------------//
	//first save the x points
	fstream lxStream;
	lxStream.open(lxPath, ios::out);
	for (int i = 0; i < nframe; i++) {
		for (int j = 0; j < ntracks; j++) {
			lxStream<<(int)(lxPoints->m_pMatrix[i][j]+0.5)<<" ";
		}
		lxStream<<endl;
	}
	lxStream.close();

	//second save the y points
	fstream lyStream;
	lyStream.open(lyPath, ios::out);
	for (int i = 0; i < nframe; i++) {
		for (int j = 0; j < ntracks; j++) {
			lyStream<<(int)(lyPoints->m_pMatrix[i][j]+0.5)<<" ";
		}
		lyStream<<endl;
	}
	lyStream.close();

	//third save the width of the points.
	fstream wStream;
	wStream.open(wPath, ios::out);
	for (int i = 0; i < nframe; i++) {
		for (int j = 0; j < ntracks; j++) {
			wStream<<(int)(wPoints->m_pMatrix[i][j]+0.5)<<" ";
		}
		wStream<<endl;
	}
	wStream.close();

	//forth save the height of the points.
	fstream hStream;
	hStream.open(hPath, ios::out);
	for (int i = 0; i < nframe; i++) {
		for (int j = 0; j < ntracks; j++) {
			hStream<<(int)(hPoints->m_pMatrix[i][j]+0.5)<<" ";
		}
		hStream<<endl;
	}
	hStream.close();

	//release the applied memory.
	if (lxPoints) {
		RELEASE(lxPoints);
		lxPoints = 0;
	}
	if (lyPoints) {
		RELEASE(lyPoints);
		lyPoints = 0;
	}
	if (wPoints) {
		RELEASE(wPoints);
		wPoints = 0;
	}
	if (hPoints) {
		RELEASE(hPoints);
		hPoints = 0;
	}

	return true;
}

//-----------------------------------------------------------------------------------------------------------------//
//--------------------------------------- Export image without the area constraints -------------------------------//
//-----------------------------------------------------------------------------------------------------------------//

bool CExport::ExportImages(pImageSet* pFrames, pTrackSet* pTracks)
{
	//Word Font for Frame.
	CvFont FrmFont;
	cvInitFont(&FrmFont, CV_FONT_VECTOR0, 0.8, 0.8, 0, 2, 8);
	//Word Font for Targets.
	CvFont TagFont;
	cvInitFont(&TagFont, CV_FONT_VECTOR0, 0.8, 0.8, 0, 2, 8);

	//frame id number
	int fid = 0;
	int fpos = 0;
	char* cSufix = ".jpg";

	if (pFrames->empty())
		return false;

	int tleft, tright, ttop, tbottom;
	int thisxc, thisyc, nextxc, nextyc;
   
	//image for saving.
	IplImage* pSImage = cvCreateImage(cvSize(m_nWidth, m_nHeight), 8, 3);

	//iterate the frames.
	for (int k = 0; k < (int)(pFrames->size()); k++) {
		char str[1024] = "";
		//frame id show
		char fname[100] = "";
		//target id show
		char tname[100] = "";

		(*pFrames)[k]->Load();
		//get the frame image.
		memcpy((void*)pSImage->imageData, (void*)((*pFrames)[k]->m_pBGRFrame->imageData), 
			sizeof(unsigned char) * m_nWidth * m_nHeight * 3);

		//label the frame ID
		fid = (*pFrames)[k]->m_nFrameIndex;
		//YI: changing to linux compatiable
		//sprintf(str, "%d", value) is alternative for _itoa_		
		//_itoa_(fid, fname, 10, 10);
		sprintf(fname,"%d",fid); 
		strcat(str, "#");
		strcat(str, fname);
		cvPutText(pSImage, str, cvPoint(30, 30), &FrmFont, CV_RGB(255,0,0));

		//draw the rectangles of the targets.
		for (int j = 0; j < (int)(pTracks->size()); j++) {
			fpos = vBoxFind(fid, (*pTracks)[j]->m_pDetPoints);
			if (fpos < 0)
				continue;

			tleft = (int)((*((*pTracks)[j]->m_pDetPoints))[fpos]->m_n2Dcx - (*((*pTracks)[j]->m_pDetPoints))[fpos]->m_nWidth / 2);
			tright = (int)((*((*pTracks)[j]->m_pDetPoints))[fpos]->m_n2Dcx + (*((*pTracks)[j]->m_pDetPoints))[fpos]->m_nWidth / 2);
			ttop = (int)((*((*pTracks)[j]->m_pDetPoints))[fpos]->m_n2Dcy - (*((*pTracks)[j]->m_pDetPoints))[fpos]->m_nHeight / 2);
			tbottom = (int)((*((*pTracks)[j]->m_pDetPoints))[fpos]->m_n2Dcy + (*((*pTracks)[j]->m_pDetPoints))[fpos]->m_nHeight / 2);

// 			if ((*((*pTracks)[j]->m_pDetPoints))[fpos]->m_bVirtMark)
// 				cvRectangle(pSImage, cvPoint(tleft, ttop), cvPoint(tright, tbottom), 
// 				CV_RGB(255, 0, 0), 2, 8, 0);
// 			else
			cvRectangle(pSImage, cvPoint(tleft, ttop), cvPoint(tright, tbottom), 
				CV_RGB((*pTracks)[j]->m_pColor->m_nR, (*pTracks)[j]->m_pColor->m_nG, (*pTracks)[j]->m_pColor->m_nB), 2, 8, 0);

			//label target id.
			// YI: change _itoa_ to itoa
			//itoa((*pTracks)[j]->m_nTrackID, tname, 10);
			sprintf(tname,"%d",(*pTracks)[j]->m_nTrackID);
			cvPutText(pSImage, tname, cvPoint(tleft, (ttop+tbottom)/2), &TagFont, CV_RGB(255,0,0));

			//draw trajectory.
			for (int k = max(0, fpos - G_DrawLength); k < fpos - 1; k++) {
				thisxc = (int)((*((*pTracks)[j]->m_pDetPoints))[k]->m_n2Dcx);
				thisyc = (int)((*((*pTracks)[j]->m_pDetPoints))[k]->m_n2Dcy + (*((*pTracks)[j]->m_pDetPoints))[k]->m_nHeight / 2);
				nextxc = (int)((*((*pTracks)[j]->m_pDetPoints))[k+1]->m_n2Dcx);
				nextyc = (int)((*((*pTracks)[j]->m_pDetPoints))[k+1]->m_n2Dcy + (*((*pTracks)[j]->m_pDetPoints))[k+1]->m_nHeight / 2);
				cvLine(pSImage, cvPoint(thisxc, thisyc), cvPoint(nextxc, nextyc), 
					CV_RGB((*pTracks)[j]->m_pColor->m_nR, (*pTracks)[j]->m_pColor->m_nG, (*pTracks)[j]->m_pColor->m_nB), 2, 8, 0);
			}
		}

		//save path
		char cSavePath[1024] = "";
		//get the name of the image.
		//YI: change strcpy_s to strcpy
		if (fid < 10) {
			strcpy(cSavePath, m_pOutputImagePath);
			strcat(cSavePath, "0");
			strcat(cSavePath, "0");
			strcat(cSavePath, "0");
			strcat(cSavePath, fname);
			strcat(cSavePath, cSufix);

		}
		else if ((fid < 100)&&(fid >= 10)) {
			strcpy(cSavePath, m_pOutputImagePath);
			strcat(cSavePath, "0");
			strcat(cSavePath, "0");
			strcat(cSavePath, fname);
			strcat(cSavePath, cSufix);
		}
		else if ((fid < 1000)&&(fid >= 100)) {
			strcpy(cSavePath, m_pOutputImagePath);
			strcat(cSavePath, "0");
			strcat(cSavePath, fname);
			strcat(cSavePath, cSufix);
		}
		else if ((fid < 10000)&&(fid >= 1000)) {
			strcpy(cSavePath, m_pOutputImagePath);
			strcat(cSavePath, fname);
			strcat(cSavePath, cSufix);
		}

		cvSaveImage(cSavePath, pSImage);
		(*pFrames)[k]->Free();
	}

	//release the temporal image.
	if (pSImage) {
		cvReleaseImage(&pSImage);
		pSImage = 0;
	}
	return true;
}



//--------------------------------------------------------------------------------------------------------------------------------//
//------------------------------------------ Debug Functions without Tracking Area Constraints -----------------------------------//
//--------------------------------------------------------------------------------------------------------------------------------//
/*
bool CExport::ExportSegTrackImages_Debug(pTrackSet* pTracks, int nLayer)
{
	//Check the debug mark
	if (m_DebugMark == 0)
		return true;

	if (pTracks->empty())
		return false;

	//Check tracklets segment index.
	int nSegIndex = (*pTracks)[0]->m_nSegIndex;
	for (int i = 0; i < (int)(pTracks->size()); i++) {
		if (nSegIndex != (*pTracks)[i]->m_nSegIndex) {
			fprintf(stderr, "Errors in ExportSegTrackImages_Debug function, the segment index is not equally!\n");
			return false;
		}
	}

	//Font type of the label.
	CvFont font;
	cvInitFont(&font, CV_FONT_VECTOR0, 0.8, 0.8, 0, 2, 8);

	//Draw the tracks in the window on the image.
	IplImage* pTracksImage = cvCreateImage(cvSize(m_nWidth, m_nHeight), 8, 3);
	//Set the image to be white.
	for (int i = 0; i < m_nHeight; i++) {
		for (int j = 0; j < m_nWidth; j++) {
			for (int k = 0; k < 3; k++) {
				CV_IMAGE_ELEM(pTracksImage, uchar, i, 3*j+k) = (uchar)255;
			}
		}
	}

	//Just a pointer.
	CTrack* pTrack = 0;
	CDetBBox* pBox = 0;
	
	//window tracklet id show
	char tname[100] = "";

	//Number of tracks.
	int NumOfTracks = (int)(pTracks->size());
	int nLen = 0;
	//Draw center points of the target.
	for (int i = 0; i < NumOfTracks; i++) {
		pTrack = (*pTracks)[i];
		nLen = (int)(pTrack->m_pDetPoints->size());
		for (int j = 0; j < nLen; j++) {
			//If the detection is interpolated, draw null circle.
			if ((*(pTrack->m_pDetPoints))[j]->m_bVirtMark)
				continue;
			pBox = (*(pTrack->m_pDetPoints))[j];
			cvCircle(pTracksImage, cvPoint((int)(pBox->m_n2Dcx), (int)(pBox->m_n2Dcy)), 2, 
				CV_RGB(pTrack->m_pColor->m_nR, pTrack->m_pColor->m_nG, pTrack->m_pColor->m_nB), 2, 8, 0);
		}

		pBox = (*(pTrack->m_pDetPoints))[0];
		//Label the tracklets.
		//_itoa_s(pTrack->m_nVirtIndex, tname, 10, 10);
		//cvPutText(pTracksImage, tname, cvPoint(pBox->m_n2Dcx - 10, pBox->m_n2Dcy - 10), &font, CV_RGB(255,0,0));
	}

	//Draw tracklets.
	CDetBBox* pBox1 = 0;
	CDetBBox* pBox2 = 0;
	for (int i = 0; i < NumOfTracks; i++) {
		pTrack = (*pTracks)[i];
		nLen = (int)(pTrack->m_pDetPoints->size());
		for (int j = 0; j < nLen - 1; j++) {
			pBox1 = (*(pTrack->m_pDetPoints))[j];
			pBox2 = (*(pTrack->m_pDetPoints))[j+1];
			cvLine(pTracksImage, cvPoint((int)(pBox1->m_n2Dcx), (int)(pBox1->m_n2Dcy)), 
				cvPoint((int)(pBox2->m_n2Dcx), (int)(pBox2->m_n2Dcy)), 
				CV_RGB(pTrack->m_pColor->m_nR, pTrack->m_pColor->m_nG, pTrack->m_pColor->m_nB), 1, 8, 0);

		}
	}

	//Save the images.
	//save path
	char cSavePath[8192] = "";
	char* cSufix = ".jpg";
	//window id show
	char wname[100] = "";
	char layername[100] = "";
    int layerid = nLayer;
	_itoa_s(layerid, layername, 10, 10);

	int fid = nSegIndex;
	_itoa_s(fid, wname, 10, 10);
	//get the name of the image.
	if (fid < 10) {
		strcpy_s(cSavePath, 500, m_pOutTracksImagePath_Debug);
		strcat_s(cSavePath, 500, "Layer");
		strcat_s(cSavePath, 500, layername);
		strcat_s(cSavePath, 500, "Seg");
		strcat_s(cSavePath, 500, "0");
		strcat_s(cSavePath, 500, "0");
		strcat_s(cSavePath, 500, wname);
		strcat_s(cSavePath, 520, cSufix);
	}
	else if ((fid < 100)&&(fid >= 10)) {
		strcpy_s(cSavePath, 500, m_pOutTracksImagePath_Debug);
		strcat_s(cSavePath, 500, "Layer");
		strcat_s(cSavePath, 500, layername);
		strcat_s(cSavePath, 500, "Seg");
		strcat_s(cSavePath, 500, "0");
		strcat_s(cSavePath, 500, wname);
		strcat_s(cSavePath, 520, cSufix);
	}
	else if ((fid < 1000)&&(fid >= 100)) {
		strcpy_s(cSavePath, 500, m_pOutTracksImagePath_Debug);
		strcat_s(cSavePath, 500, "Layer");
		strcat_s(cSavePath, 500, layername);
		strcat_s(cSavePath, 500, "Seg");
		strcat_s(cSavePath, 500, wname);
		strcat_s(cSavePath, 520, cSufix);
	}

	cvSaveImage(cSavePath, pTracksImage);

	//Release the applied memory.
	if (pTracksImage) {
		cvReleaseImage(&pTracksImage);
		pTracksImage = 0;
	}

	return true;
}


bool CExport::ExportTrajectoryImage_Debug(pTrackSet* pTracks)
{
	//Check the debug mark
	if (m_DebugMark == 0)
		return true;

	//Draw the tracks in the window on the image.
	IplImage* pTracksImage = cvCreateImage(cvSize(m_nWidth, m_nHeight), 8, 3);
	//Set the image to be white.
	for (int i = 0; i < m_nHeight; i++) {
		for (int j = 0; j < m_nWidth; j++) {
			for (int k = 0; k < 3; k++) {
				CV_IMAGE_ELEM(pTracksImage, uchar, i, 3*j+k) = (uchar)255;
			}
		}
	}

	//Number of tracks.
	int NumOfTracks = (int)(pTracks->size());
	int nLen = 0;
	//Just a pointer.
	CTrack* pTrack = 0;
	CDetBBox* pBox = 0;
	//Draw center points of the target.
	for (int i = 0; i < NumOfTracks; i++) {
		pTrack = (*pTracks)[i];
		nLen = (int)(pTrack->m_pDetPoints->size());
		for (int j = 0; j < nLen; j++) {
			//If the detection is interpolated, draw null circle.
			if ((*(pTrack->m_pDetPoints))[j]->m_bVirtMark)
				continue;
			pBox = (*(pTrack->m_pDetPoints))[j];
			cvCircle(pTracksImage, cvPoint((int)(pBox->m_n2Dcx), (int)(pBox->m_n2Dcy)), 2, 
				CV_RGB(pTrack->m_pColor->m_nR, pTrack->m_pColor->m_nG, pTrack->m_pColor->m_nB), 2, 8, 0);
		}
	}

	//Draw tracklets.
	CDetBBox* pBox1 = 0;
	CDetBBox* pBox2 = 0;
	for (int i = 0; i < NumOfTracks; i++) {
		pTrack = (*pTracks)[i];
		nLen = (int)(pTrack->m_pDetPoints->size());
		for (int j = 0; j < nLen - 1; j++) {
			pBox1 = (*(pTrack->m_pDetPoints))[j];
			pBox2 = (*(pTrack->m_pDetPoints))[j+1];
			cvLine(pTracksImage, cvPoint((int)(pBox1->m_n2Dcx), (int)(pBox1->m_n2Dcy)), 
				cvPoint((int)(pBox2->m_n2Dcx), (int)(pBox2->m_n2Dcy)), 
				CV_RGB(pTrack->m_pColor->m_nR, pTrack->m_pColor->m_nG, pTrack->m_pColor->m_nB), 1, 8, 0);

		}
	}

	//Save the images.
	//save path
	char cSavePath[1024] = "";
	char* cSufix = ".jpg";
	//get the name of the image.
	strcpy_s(cSavePath, 500, m_pOutTracksImagePath_Debug);
	strcat_s(cSavePath, 500, "Tracks");
	strcat_s(cSavePath, 520, cSufix);

	cvSaveImage(cSavePath, pTracksImage);

	//Release the applied memory.
	if (pTracksImage) {
		cvReleaseImage(&pTracksImage);
		pTracksImage = 0;
	}

	return true;
}

bool CExport::ExportTargetsImage_Debug(CDetBBox* pBox)
{
	//Save the images.
	//save path
	char cSavePath[8192] = "";
	char* cSufix = ".jpg";
	//window id show
	char ctarget[100] = "";
	char cframe[100] = "";
	_itoa_s(pBox->m_nFrameIndex, cframe, 10, 10);
	_itoa_s(pBox->m_nVirtIndex, ctarget, 10, 10);

	//get the name of the image.
	strcpy_s(cSavePath, 500, m_pOutTargetsImagePath_Debug);
	strcat_s(cSavePath, 500, "Target_F");
	strcat_s(cSavePath, 500, cframe);
	strcat_s(cSavePath, 500, "_T");
	strcat_s(cSavePath, 500, ctarget);
	strcat_s(cSavePath, 520, cSufix);

	cvSaveImage(cSavePath, pBox->m_pFeatPict);

	return true;
}


bool CExport::ExportOccupyMap_Debug(CTImage* pFrame)
{
	pDetBoxSet* pDets = pFrame->m_pBoxes;
	IplImage* pOccMap = cvCreateImage(cvSize(pFrame->m_nWidth, pFrame->m_nHeight), 8, 3);

	//Set the occupy map to be all white.
	for (int i = 0; i < pFrame->m_nHeight; i++) {
		for (int j = 0; j < pFrame->m_nWidth; j++) {
			if (pFrame->m_pOccupyMap[i][j] >= 0) {
				CV_IMAGE_ELEM(pOccMap, uchar, i, j * 3) = (uchar)((*pDets)[pFrame->m_pOccupyMap[i][j]]->m_pColor->m_nB);
				CV_IMAGE_ELEM(pOccMap, uchar, i, j * 3 + 1) = (uchar)((*pDets)[pFrame->m_pOccupyMap[i][j]]->m_pColor->m_nG);
				CV_IMAGE_ELEM(pOccMap, uchar, i, j * 3 + 2) = (uchar)((*pDets)[pFrame->m_pOccupyMap[i][j]]->m_pColor->m_nR);
			}
			else {
				CV_IMAGE_ELEM(pOccMap, uchar, i, j * 3) = (uchar)(255);
				CV_IMAGE_ELEM(pOccMap, uchar, i, j * 3 + 1) = (uchar)(255);
				CV_IMAGE_ELEM(pOccMap, uchar, i, j * 3 + 2) = (uchar)(255);
			}
		}
	}

	//Get the occupy map path.
	char cSavePath[8192] = "";
	char* cSufix = ".jpg";
	//window id show
	char cframe[100] = "";
	_itoa_s(pFrame->m_nFrameIndex, cframe, 10, 10);

	//get the name of the image.
	strcpy_s(cSavePath, 500, m_pOccupyMapImagePath_Debug);
	strcat_s(cSavePath, 500, "Frame");
	strcat_s(cSavePath, 500, cframe);
	strcat_s(cSavePath, 520, cSufix);

	cvSaveImage(cSavePath, pOccMap);

	if (pOccMap) {
		cvReleaseImage(&pOccMap);
		pOccMap = 0;
	}

	return true;
}
*/


int vBoxFind(int fid, pDetBoxSet* detset)
{
	int pos = -1;
	int cnt = 0;
	pDetBoxSet::iterator det_iter;
	for (det_iter = detset->begin(); det_iter != detset->end(); det_iter++) {
		if ((*det_iter)->m_nFrameIndex == fid) {
			pos = cnt;
			break;
		}
		cnt++;
	}
	return pos;
}