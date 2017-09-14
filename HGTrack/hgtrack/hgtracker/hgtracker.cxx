/*
 * GraphMultiCarTracker
 * Do not distribute
 *  Created on: Nov 08, 2015
 *      Author: Longyin Wen
 */
#include "hgtracker.h"
#include <export/export.h>
//----------------------------------------------------------- Global Function ---------------------------------------------------------//

int randint(const int min, const int max)
{
	return cvRandInt( &rng_state )%(max-min+1) + min;
}

double randdouble()
{
	return cvRandReal( &rng_state );
}

CHyperGraphTracker::CHyperGraphTracker()
{
	//image size.
	m_nWidth = 0;
	m_nHeight = 0;

	//------------------------------ Image or Video information part ---------------------------//
	m_pTrackParas = 0;
	m_pSeqFrames = new pImageSet;

	m_pDetector = 0;

	//----------------------------------- Tracking Sliding Windows ----------------------------------------//
	m_NumOfFrames = 0;
	m_NumOfLayers = 0;

	//------------------------------------ Optimizer Block -----------------------------------------------//
	m_pOptimizer = new CHierarchHyperGraph;

	//------------------------------- Tracking Results Handling --------------------------------//
	m_pExpoter = new CExport;

	//------------------------------- Detection ID Variables -----------------------------------//
	m_pDetAllot = new CAllotID;

	//------------------------------ Config File Path -------------------------------------------//
	m_pConfigPath = "GraphMultiTargetTracker_v03_Config.txt";

}

CHyperGraphTracker::CHyperGraphTracker(const char* pConfig)
{
	//image size.
	m_nWidth = 0;
	m_nHeight = 0;

	//------------------------------ Image or Video information part ---------------------------//
	m_pTrackParas = 0;
	m_pSeqFrames = new pImageSet;

	m_pDetector = 0;

	//----------------------------------- Tracking Sliding Windows ----------------------------------------//
	m_NumOfFrames = 0;
	m_NumOfLayers = 0;

	//------------------------------------ Optimizer Block -----------------------------------------------//
	m_pOptimizer = new CHierarchHyperGraph;

	//------------------------------- Tracking Results Handling --------------------------------//
	m_pExpoter = new CExport;
	
	//------------------------------- Detection ID Variables -----------------------------------//
	m_pDetAllot = new CAllotID;

	//-------------------------------- Config File Path --------------------------------------------//
	m_pConfigPath = pConfig;
}

CHyperGraphTracker::~CHyperGraphTracker()
{
	//image size.
	m_nWidth = 0;
	m_nHeight = 0;
	m_NumOfFrames = 0;
	m_NumOfLayers = 0;

	if (m_pTrackParas) {
		RELEASE(m_pTrackParas);
		m_pTrackParas = 0;
	}

	if (m_pSeqFrames) {
		for (int i = 0; i < (int)(m_pSeqFrames->size()); i++) {
			if ((*m_pSeqFrames)[i]) {
				RELEASE((*m_pSeqFrames)[i]);
				(*m_pSeqFrames)[i] = 0;
			}
		}
		RELEASE(m_pSeqFrames);
		m_pSeqFrames = 0;
	}

	//release it.
	if (m_pDetector) {
		RELEASE(m_pDetector);
		m_pDetector = 0;
	}
	

	//------------------------------------ Optimizer Block -----------------------------------------------//
	if (m_pOptimizer) {
		RELEASE(m_pOptimizer);
		m_pOptimizer = 0;
	}
	//------------------------------ Tracking Results Handling ----------------------------------------------//
	if (m_pExpoter) {
		RELEASE(m_pExpoter);
		m_pExpoter = 0;
	}

	//------------------------------- Detection ID Variables -----------------------------------//
	if (m_pDetAllot) {
		RELEASE(m_pDetAllot);
		m_pDetAllot = 0;
	}
}


//---------------------------------------- Main Tracking Part -------------------------------------------------//

bool CHyperGraphTracker::Tracker_API()
{
	//Load the config files and check whether it is successfully initialized.
	if(!LoadConfig(m_pConfigPath)) {
		fprintf(stderr,  "Errors In Loading Config File!\n");
		return false;
	}

	//---------------- Initialize the tracker -------------------------------------//
	if(InitTracker()) {
		//--------------------------------------------- Detection Part -------------------------------------------------//
		m_pDetector->LoadDetections(m_pTrackParas->m_pExportInfos->m_gImportDetFilePath);
		cout<<"Getting the detections correctly!"<<endl;

		//initial the export class.
		m_pExpoter->Init(m_pTrackParas->m_pExportInfos);
		//------------------------------------ Tracking Part -------------------------------------------------//
		if (ImportImages())
			HierarchTracker();
	
		return true;
	}
	else 
		return false;
}


bool CHyperGraphTracker::ImportImages()
{
	//loading the image file.
	ifstream pathfin;
	pathfin.open(m_pTrackParas->m_pExportInfos->m_gImportVideoFilePath);
	if (pathfin.fail()) {
		fprintf(stderr, "Errors In Opening The Image Path File!\n");
		return false;
	}
	//image path.
	string sImagePath;
	//image variables.
	IplImage* pOrgImage = 0;
	IplImage* pStdImage = cvCreateImage(cvSize(m_nWidth, m_nHeight), 8, 3);

	//loading number counter.
	int nFrmCount = 0;
	//active number counter.
	int nUseCount = 0;
	//Number of targets counter.
	int nTagCount = 0;
	//Getting the windows.
	pDetBoxSet::iterator box_iter;

	//Getting the frame detections.
	map<int, CFrameDets*>::iterator iter_frame;

	//Loading the tracking sequences.
	cout<<"Begin to Load the Video Sequences: "<<m_pTrackParas->m_pExportInfos->m_gDataName<<endl;
	while(1) {
		if (!getline(pathfin, sImagePath, '\n'))
			break;

		//If the image satisfied the required ones, break
		if (nFrmCount >= m_NumOfFrames)
			break;

		//check whether the path is correct
		if ((sImagePath.find(".jpg") == -1)&&(sImagePath.find(".bmp") == -1)&&(sImagePath.find(".jpeg") == -1)) {
			fprintf(stderr, "Errors of the Path Files!\n");
			nFrmCount++;
			continue;
		}

		//--------------------------------------------- Loading Images -------------------------------------------------------//
		char pLine[1024] = "";
		// YI: strcpy_s to strcpy
		//strcpy_s(pLine, 1024, sImagePath.data());
		strcpy(pLine, sImagePath.data());
		pOrgImage = cvLoadImage(pLine, CV_LOAD_IMAGE_COLOR);

		if (!pOrgImage) {
			printf("Errors of Loading the %d th Image!\n", nFrmCount);
			nFrmCount++;	
			continue;
		}

		cvResize(pOrgImage, pStdImage);

		CTImage* pFrame = new CTImage(pStdImage->width, pStdImage->height, nUseCount);
		//Get a copy of the frame image.
		cvCopy(pStdImage, pFrame->m_pBGRFrame);
		//Get the frame image path.
		//YI:
		//strcpy_s(pFrame->m_cFramePath, 1024, sImagePath.data());
		strcpy(pFrame->m_cFramePath, sImagePath.data());

		//------------------------------------------- Get the Detection Results --------------------------------------------------------//
		//Get the number of detections. �����һ��ͼ��϶����ڼ����
		iter_frame = m_pDetector->m_pDetections->find(nFrmCount);

		if (iter_frame != m_pDetector->m_pDetections->end()) {

			//Get the number of detections.
			iter_frame->second->m_nNum = (int)(iter_frame->second->m_pBoxes->size());

			nTagCount = 0;
			//copy the detection results.
			for (box_iter = (*(m_pDetector->m_pDetections))[nFrmCount]->m_pBoxes->begin(); box_iter != (*(m_pDetector->m_pDetections))[nFrmCount]->m_pBoxes->end(); box_iter++) {
				//Frame index just concentrate on the location in one window.
				(*box_iter)->m_nFrameIndex = nFrmCount;
				(*box_iter)->m_nVirtIndex = nTagCount;
				(*box_iter)->m_nDetBoxID = m_pDetAllot->DispatchID();
				(*box_iter)->m_bVirtMark = false;

				(*box_iter)->GetFeatPict(pFrame->m_pBGRFrame);
				ExtractColorFeature(*box_iter);
				ExtractShapeFeature(*box_iter);
				ExtractTextureFeature(*box_iter);
				//Save the targets information----------------------//
				//m_pExpoter->ExportTargetsImage_Debug((*box_iter));
				//--------------------------------------------------//
				CDetBBox* pNewBox = new CDetBBox(*box_iter);
				pFrame->m_pBoxes->push_back(pNewBox);
				nTagCount++;
			}
		}

		//Get the frame occupy map.
		pFrame->GetOccupy();
		//Just used for Debugging.
		//m_pExpoter->ExportOccupyMap_Debug(pFrame);

		//Release the frame image.
		pFrame->Free();
		//push back the frame images.
		m_pSeqFrames->push_back(pFrame);
		printf("\r");
		printf("Successful Loading the %d th Image!", nFrmCount);
		nFrmCount++;
		nUseCount++;

		if (pOrgImage) {
			cvReleaseImage(&pOrgImage);
			pOrgImage = 0;
		}

		if (cvWaitKey(1) == 27)
			break;
	}

	if (pStdImage) {
		cvReleaseImage(&pStdImage);
		pStdImage = 0;
	}

	cout<<endl;
	cout<<"Finish Loading The Tracking Data!"<<endl;
	return true;
}


bool CHyperGraphTracker::HierarchTracker()
{
	clock_t cstart, cfinish;
	double tsec = 0;
	double tfps = 0;
	//Set the start point.
	cstart = clock();
	//HyperGraph optimization.
	//Initial the optimizer.
	m_pOptimizer->InitOptimizer(m_pTrackParas->m_pOptLayerParas, m_NumOfLayers, m_pSeqFrames, m_pExpoter);
	
	//***************************************************************************************************//
	//Just count the computation times.
	//Main function used to make the optimization.
	m_pOptimizer->HierarchOptimize();

	//Set the end point.
	cfinish = clock();
	//***************************************************************************************************//
	//Save the tracking results.
	cout<<"Begin to Save the Tracking Results!"<<endl;
	//save the tracking results.
	//Get final tracks.
	pTrackSet* pTracks = m_pOptimizer->GetOptTracks();
	m_pExpoter->ExportImages(m_pSeqFrames, pTracks);
	m_pExpoter->ExportTrajectory(m_pSeqFrames, pTracks);
	cout<<"Finish Step 2!"<<endl;

	//Save the speed.
	//Time consume calculation.
	tfps = CLOCKS_PER_SEC / (double)(cfinish - cstart + 0.000001);
	//cout<<"Time Consume in Processing This Sequence: "<<tfps<<endl;

	//Export the average running speed.
	tfps *= (m_NumOfFrames + 1);

	char SpeedPath[2048] = "";
	//for x coordinate.
	//YI
	//strcpy_s(SpeedPath, 1024, m_pExpoter->m_pOutputSpeedPath);
	//strcat_s(SpeedPath, 300, m_pExpoter->m_pDataName);
	//strcat_s(SpeedPath, 300, "_Speed");
	//strcat_s(SpeedPath, 300, ".txt");
	strcpy(SpeedPath, m_pExpoter->m_pOutputSpeedPath);
	strcat(SpeedPath, m_pExpoter->m_pDataName);
	strcat(SpeedPath, "_Speed");
	strcat(SpeedPath, ".txt");

	fstream SpeedStream;
	SpeedStream.open(SpeedPath, ios::out);
	SpeedStream<<tfps<<endl;
	SpeedStream.close();

	return true;
};


bool CHyperGraphTracker::InitTracker()
{
	//*****************************************************************************************//
	
	//Tracker variables.
	m_nWidth = G_IMAGE_WIDTH;
	m_nHeight = G_IMAGE_HEIGHT;

	m_NumOfFrames = G_TOTAL_FRAME_NUMBER;


	//Load the segment size variable.
	//Calculate the number of layers.
	double nBase = m_NumOfFrames / (double)(G_SEGMENT_SIZE[0]);

	if (nBase < 1.0) {
		fprintf(stderr, "The number of frames is less the first segment size!\n");
		system("pause");
		return false;
	}

	m_NumOfLayers = 1;
	while (1) {
		//Check whether it can divide or not.
		nBase = nBase / G_SEGMENT_SIZE[1];
		if (nBase < 1.0) {
			m_NumOfLayers++;
			break;
		}
		m_NumOfLayers++;
	}

	//Set the parameters.
	m_pTrackParas = new CGraphTrackerPara(1, m_NumOfLayers - 1);

	//The path parameters.
	//Input path variables.
	m_pTrackParas->m_pExportInfos->m_gDataName = G_INPUT_DATAFILE_NAME.data();
	m_pTrackParas->m_pExportInfos->m_gImportVideoFilePath = G_INPUT_VIDEOFILE_PATH.data();
	m_pTrackParas->m_pExportInfos->m_gImportDetFilePath = G_INPUT_DETECTFILE_PATH.data();
	//Output path variables.
	m_pTrackParas->m_pExportInfos->m_gExportVideoFilePath = G_OUTPUT_VIDEOFILE_PATH.data();
	m_pTrackParas->m_pExportInfos->m_gExportTrajFilePath = G_OUTPUT_TRAJFILE_PATH.data();
	m_pTrackParas->m_pExportInfos->m_gExportSpeedFilePath = G_OUTPUT_SPEEDFILE_PATH.data();


	//Set the layer index.
	for (int i = 0; i < m_NumOfLayers; i++)
		(m_pTrackParas->m_pOptLayerParas)[i].m_nCurLayer = i;

	//Segment Size in the tracker
	(m_pTrackParas->m_pOptLayerParas)[0].m_nSegSize = G_SEGMENT_SIZE[0];
	for (int i = 1; i < m_NumOfLayers; i++)
		(m_pTrackParas->m_pOptLayerParas)[i].m_nSegSize = G_SEGMENT_SIZE[1];

	//Degree of the graph
	(m_pTrackParas->m_pOptLayerParas)[0].m_nTrackNumK = G_TRACK_KARY;
	for (int i = 1; i < m_NumOfLayers; i++)
		(m_pTrackParas->m_pOptLayerParas)[i].m_nTrackNumK = 2;

	//Minimal number of vertices
	(m_pTrackParas->m_pOptLayerParas)[0].m_nTrackMinVertices = max(G_TRACK_KARY, G_TRACK_MINIMAL_VERTEX_CLUSTER[0]);
	for (int i = 1; i < m_NumOfLayers; i++)
		(m_pTrackParas->m_pOptLayerParas)[i].m_nTrackMinVertices = max(2, G_TRACK_MINIMAL_VERTEX_CLUSTER[1]);

	//Occlusion conflict
	for (int i = 0; i < m_NumOfLayers; i++)
		(m_pTrackParas->m_pOptLayerParas)[i].m_nOccConflictThresh = G_OCCRATIO_CONFLICT_THRESHOLD;

	//Set the gap threshold
	(m_pTrackParas->m_pOptLayerParas)[0].m_nSafeGapNeighThresh = G_SAFE_GAP_NEIGHBOR_THRESHOLD[0];
	for (int i = 1; i < m_NumOfLayers; i++)
		(m_pTrackParas->m_pOptLayerParas)[i].m_nSafeGapNeighThresh = G_SAFE_GAP_NEIGHBOR_THRESHOLD[1];

	//Occlusion threshold to be neighborhoods.
	(m_pTrackParas->m_pOptLayerParas)[0].m_nOccNeighThresh = G_OCCRATIO_NEIGHBOR_THRESHOLD[0];
	for (int i = 1; i < m_NumOfLayers; i++)
		(m_pTrackParas->m_pOptLayerParas)[i].m_nOccNeighThresh = G_OCCRATIO_NEIGHBOR_THRESHOLD[1];

	//Target maximal velocity.
	for (int i = 0; i < m_NumOfLayers; i++)
		(m_pTrackParas->m_pOptLayerParas)[i].m_nMaxVelocity = G_TARGET_MAXVELOCITY;

	//Post processing type.
	(m_pTrackParas->m_pOptLayerParas)[0].m_nPostType = G_POSTPROCESS_TYPE[0];
	for (int i = 1; i < m_NumOfLayers; i++)
		(m_pTrackParas->m_pOptLayerParas)[i].m_nPostType = G_POSTPROCESS_TYPE[1];

	//Minimal track length.
	(m_pTrackParas->m_pOptLayerParas)[0].m_nMinTrackLen = G_TRACK_MINIMAL_LENGTH[0];
	for (int i = 1; i < m_NumOfLayers; i++)
		(m_pTrackParas->m_pOptLayerParas)[i].m_nMinTrackLen = G_TRACK_MINIMAL_LENGTH[1];

	//General maximal distance to be neighbor.
	(m_pTrackParas->m_pOptLayerParas)[0].m_nGenTrackMaxDistNeigh = G_GENERAL_TRACK_MAXIMAL_DISTANCE_NEIGHBOR[0];
	for (int i = 1; i < m_NumOfLayers; i++)
		(m_pTrackParas->m_pOptLayerParas)[i].m_nGenTrackMaxDistNeigh = G_GENERAL_TRACK_MAXIMAL_DISTANCE_NEIGHBOR[1];

	//General maximal segment to be neighbor.
	(m_pTrackParas->m_pOptLayerParas)[0].m_nGenTrackMaxSegNeigh = G_GENERAL_TRACK_MAXIMAL_SEGMENT_NEIGHBOR[0];
	for (int i = 1; i < m_NumOfLayers; i++)
		(m_pTrackParas->m_pOptLayerParas)[i].m_nGenTrackMaxSegNeigh = G_GENERAL_TRACK_MAXIMAL_SEGMENT_NEIGHBOR[1];

	//Move term sampling step.
	(m_pTrackParas->m_pOptLayerParas)[0].m_nTrackMoveSampleStep = G_TRACK_MOVE_SAMPLE_STEP[0];
	for (int i = 1; i < m_NumOfLayers; i++)
		(m_pTrackParas->m_pOptLayerParas)[i].m_nTrackMoveSampleStep = G_TRACK_MOVE_SAMPLE_STEP[1];

	//Smooth term sampling step.
	(m_pTrackParas->m_pOptLayerParas)[0].m_nTrackSmthSampleStep = G_TRACK_SMOOTH_SAMPLE_STEP[0];
	for (int i = 1; i < m_NumOfLayers; i++)
		(m_pTrackParas->m_pOptLayerParas)[i].m_nTrackSmthSampleStep = G_TRACK_SMOOTH_SAMPLE_STEP[1];

	//Appearance compensate factor.
	(m_pTrackParas->m_pOptLayerParas)[0].m_nAppCompFactor = G_TRACK_APP_COMPENSATE_FACTOR[0];
	(m_pTrackParas->m_pOptLayerParas)[1].m_nAppCompFactor = G_TRACK_APP_COMPENSATE_FACTOR[1];
	for (int i = 2; i < m_NumOfLayers; i++)
		(m_pTrackParas->m_pOptLayerParas)[i].m_nAppCompFactor = G_TRACK_APP_COMPENSATE_FACTOR[2];


	//Other information.
	m_pTrackParas->m_pExportInfos->m_nWidth = m_nWidth;
	m_pTrackParas->m_pExportInfos->m_nHeight = m_nHeight;

	//Some debug information.
	m_pTrackParas->m_pExportInfos->m_gDebugMark = G_DEBUG_MARK;
	m_pTrackParas->m_pExportInfos->m_gTracksImagePath_Debug = G_TRACKSIMAGEPATH_DEBUG.data();
	m_pTrackParas->m_pExportInfos->m_gTargetsImagePath_Debug = G_TARGETIMAGE_PATH_DEBUG.data();
	m_pTrackParas->m_pExportInfos->m_gOccupyImagePath_Debug = G_OCCUPYIMAGE_PATH_DEBUG.data();
	

	//*****************************************************************************************//
	//Set some default parameters.
	//Use the CVPR algorithm to complete the tracking task
	//****************************************************************************************************************//
	//Exhaust optimization, appearance alpha.
	((m_pTrackParas->m_pOptLayerParas)[0].m_pExhaustOptParas)->m_nAppAlpha = G_EXHAUST_GRAPH_APPALPHA[0];
	for (int i = 1; i < m_NumOfLayers; i++)
		((m_pTrackParas->m_pOptLayerParas)[i].m_pExhaustOptParas)->m_nAppAlpha = G_EXHAUST_GRAPH_APPALPHA[1];

	//Exhaust optimization, move alpha.
	((m_pTrackParas->m_pOptLayerParas)[0].m_pExhaustOptParas)->m_nMoveAlpha = G_EXHAUST_GRAPH_MOVEALPHA[0];
	for (int i = 1; i < m_NumOfLayers; i++)
		((m_pTrackParas->m_pOptLayerParas)[i].m_pExhaustOptParas)->m_nMoveAlpha = G_EXHAUST_GRAPH_MOVEALPHA[1];

	//Exhaust optimization, smooth alpha.
	((m_pTrackParas->m_pOptLayerParas)[0].m_pExhaustOptParas)->m_nSmthAlpha = G_EXHAUST_GRAPH_SMTHALPHA[0];
	for (int i = 1; i < m_NumOfLayers; i++)
		((m_pTrackParas->m_pOptLayerParas)[i].m_pExhaustOptParas)->m_nSmthAlpha = G_EXHAUST_GRAPH_SMTHALPHA[1];

	//Exhaust optimization, appearance sigma.
	((m_pTrackParas->m_pOptLayerParas)[0].m_pExhaustOptParas)->m_nAppSigma = G_EXHAUST_GRAPH_APPSIGMA[0];
	for (int i = 1; i < m_NumOfLayers; i++)
		((m_pTrackParas->m_pOptLayerParas)[i].m_pExhaustOptParas)->m_nAppSigma = G_EXHAUST_GRAPH_APPSIGMA[1];

	//Exhaust optimization, move sigma.
	((m_pTrackParas->m_pOptLayerParas)[0].m_pExhaustOptParas)->m_nMoveSigma = G_EXHAUST_GRAPH_MOVESIGMA[0];
	for (int i = 1; i < m_NumOfLayers; i++)
		((m_pTrackParas->m_pOptLayerParas)[i].m_pExhaustOptParas)->m_nMoveSigma = G_EXHAUST_GRAPH_MOVESIGMA[1];

	//Exhaust optimization, smooth sigma.
	((m_pTrackParas->m_pOptLayerParas)[0].m_pExhaustOptParas)->m_nSmthSigma = G_EXHAUST_GRAPH_SMTHSIGMA[0];
	for (int i = 1; i < m_NumOfLayers; i++)
		((m_pTrackParas->m_pOptLayerParas)[i].m_pExhaustOptParas)->m_nSmthSigma = G_EXHAUST_GRAPH_SMTHSIGMA[1];

	//Exhaust optimization, appearance bound.
	((m_pTrackParas->m_pOptLayerParas)[0].m_pExhaustOptParas)->m_nAppUpperBound = G_EXHAUST_GRAPH_APPUPPERBOUND[0];
	for (int i = 1; i < m_NumOfLayers; i++)
		((m_pTrackParas->m_pOptLayerParas)[i].m_pExhaustOptParas)->m_nAppUpperBound = G_EXHAUST_GRAPH_APPUPPERBOUND[1];

	//Exhaust optimization, move bound.
	((m_pTrackParas->m_pOptLayerParas)[0].m_pExhaustOptParas)->m_nMoveUpperBound = G_EXHAUST_GRAPH_MOVEUPPERBOUND[0];
	for (int i = 1; i < m_NumOfLayers; i++)
		((m_pTrackParas->m_pOptLayerParas)[i].m_pExhaustOptParas)->m_nMoveUpperBound = G_EXHAUST_GRAPH_MOVEUPPERBOUND[1];

	//Exhaust optimization, smooth bound.
	((m_pTrackParas->m_pOptLayerParas)[0].m_pExhaustOptParas)->m_nSmthUpperBound = G_EXHAUST_GRAPH_SMTHUPPERBOUND[0];
	for (int i = 1; i < m_NumOfLayers; i++)
		((m_pTrackParas->m_pOptLayerParas)[i].m_pExhaustOptParas)->m_nSmthUpperBound = G_EXHAUST_GRAPH_SMTHUPPERBOUND[1];

	//Exhaust optimization, mixture bound.
	((m_pTrackParas->m_pOptLayerParas)[0].m_pExhaustOptParas)->m_nMixUpperBound = G_EXHAUST_GRAPH_MIXUPPERBOUND[0];
	for (int i = 1; i < m_NumOfLayers; i++)
		((m_pTrackParas->m_pOptLayerParas)[i].m_pExhaustOptParas)->m_nMixUpperBound = G_EXHAUST_GRAPH_MIXUPPERBOUND[1];

	//Exhaust optimization, appearance threshold.
	((m_pTrackParas->m_pOptLayerParas)[0].m_pExhaustOptParas)->m_nAppThreshold = G_EXHAUST_GRAPH_APPTHRESHOLD[0];
	for (int i = 1; i < m_NumOfLayers; i++)
		((m_pTrackParas->m_pOptLayerParas)[i].m_pExhaustOptParas)->m_nAppThreshold = G_EXHAUST_GRAPH_APPTHRESHOLD[1];

	//Exhaust optimization, move threshold.
	((m_pTrackParas->m_pOptLayerParas)[0].m_pExhaustOptParas)->m_nMoveThreshold = G_EXHAUST_GRAPH_MOVETHRESHOLD[0];
	for (int i = 1; i < m_NumOfLayers; i++)
		((m_pTrackParas->m_pOptLayerParas)[i].m_pExhaustOptParas)->m_nMoveThreshold = G_EXHAUST_GRAPH_MOVETHRESHOLD[1];

	//Exhaust optimization, smooth threshold.
	((m_pTrackParas->m_pOptLayerParas)[0].m_pExhaustOptParas)->m_nSmthThreshold = G_EXHAUST_GRAPH_SMTHTHRESHOLD[0];
	for (int i = 1; i < m_NumOfLayers; i++)
		((m_pTrackParas->m_pOptLayerParas)[i].m_pExhaustOptParas)->m_nSmthThreshold = G_EXHAUST_GRAPH_SMTHTHRESHOLD[1];

	//Exhaust optimization, appearance weight.
	((m_pTrackParas->m_pOptLayerParas)[0].m_pExhaustOptParas)->m_nAppWeight = G_EXHAUST_GRAPH_APPWEIGHT[0];
	for (int i = 1; i < m_NumOfLayers; i++)
		((m_pTrackParas->m_pOptLayerParas)[i].m_pExhaustOptParas)->m_nAppWeight = G_EXHAUST_GRAPH_APPWEIGHT[1];

	//Exhaust optimization, move weight.
	((m_pTrackParas->m_pOptLayerParas)[0].m_pExhaustOptParas)->m_nMoveWeight = G_EXHAUST_GRAPH_MOVEWEIGHT[0];
	for (int i = 1; i < m_NumOfLayers; i++)
		((m_pTrackParas->m_pOptLayerParas)[i].m_pExhaustOptParas)->m_nMoveWeight = G_EXHAUST_GRAPH_MOVEWEIGHT[1];

	//Exhaust optimization, smooth weight.
	((m_pTrackParas->m_pOptLayerParas)[0].m_pExhaustOptParas)->m_nSmthWeight = G_EXHAUST_GRAPH_SMTHWEIGHT[0];
	for (int i = 1; i < m_NumOfLayers; i++)
		((m_pTrackParas->m_pOptLayerParas)[i].m_pExhaustOptParas)->m_nSmthWeight = G_EXHAUST_GRAPH_SMTHWEIGHT[1];


	//--- Initialize the detection variable.
	m_pDetector = new CDetector(m_nWidth, m_nHeight);


	return true;
}

