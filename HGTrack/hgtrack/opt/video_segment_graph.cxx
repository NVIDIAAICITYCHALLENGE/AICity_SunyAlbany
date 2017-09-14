/*
 * GraphMultiCarTracker
 * Do not distribute
 *  Created on: Nov 08, 2015
 *      Author: Longyin Wen
 */
#include "video_segment_graph.h"


//******************************************************************************************************//

CSegmentGraph::CSegmentGraph()
{
	//Apply the memory.
	m_pCurInitTracks = new pTrackSet;
	m_pCurInitTracks->clear();

	m_pCurOptTracks = new pTrackSet;
	m_pCurOptTracks->clear();

	m_nCurSegIndex = 0;

	//Just a pointer of the frame map.
	m_pSeqFrameMap = 0;
	
	//--------------------------------------------------------------------------//
	//------- Tracklets in 2D image plane.
	//The initial tracklets for each segment.
	m_pProSegTracks = new pSegTrackSet;
	m_pProSegTracks->clear();

	//The optimized tracklets after clustering.
	m_pOptSegTracks = new pSegTrackSet;
	m_pOptSegTracks->clear();

	//The optimized tracklet set in the current segment.
	m_pCurOptTracks = 0;

	//The inner contents are just a pointer.
	m_pCurInitTracks = new pTrackSet;
	m_pCurInitTracks->clear();

	//The output of the tracklets after clustering.
	m_pCurOptTracks = new pTrackSet;
	m_pCurOptTracks->clear();

	//Number of segments to be processed.
	m_nNumSegs = 0;
	//Begin segment index of current optimization.
	m_nBegSegIdx = 0;
	//End segment index of current optimization.
	m_nEndSegIdx = 0;

	//Appearance similarity array of the detections.
	m_pNodeAppSims = new DoubleArray;
	m_pNodeAppSims->clear();

	//Get the variables for clustering.
	m_pGraphDecOpt = GraphDecGreate_API();

	//Optimization parameters.
	m_pOptParas = 0;

	//Hyperedge or edge weight of the graph.
	m_vEdgeWeights.clear();

	//Node weight of the graph.
	m_vNodeWeights.clear();

	//Edge index.
	m_vEdgeManIdx.clear();

	
	//--- Parameters for the candidate K-Ary of the tracklets.
	m_pCandKAry = new IntArray;
	m_pCandKAry->clear();

	//------------------------------------- Used for Graph Optimization -------------------------------------------//
	//Some variables used for cluster optimization.
	m_pOptSupports = new IntArray;
	m_pOptProb = new DoubleArray;
	m_pOptValue = new DoubleVector;

	//Cluster results after post-process
	m_pOptClusters = new IntArray;

	m_pSplines = new spline1dinterpolant;
	m_nBoundType = 2;

	
	//--------------------------------------------------------------------//
	//Affinity value of the edge smooth aspect.
	m_pAppAffinity = new IVectorDVector;
	m_pAppAffinity->clear();

	//Affinity value of the edge move aspect.
	m_pMoveAffinity = new IVectorDVector;
	m_pMoveAffinity->clear();

	//Affinity value of the edge smoothness aspect.
	m_pSmthAffinity = new IVectorDVector;
	m_pSmthAffinity->clear();
	
	//Distance matrix between pairwise tracklets.
	m_p2DDistance = 0;
	//Neighboring mark matrix of the tracklets for general using.
	m_pNodeNeighMarks = 0;
  
	//Parameters for clustering.
	m_pClusterParas = new DecomposePara;

	//The export pointer, just used for debug.
	m_pExpoter = 0;

}

CSegmentGraph::~CSegmentGraph()
{
	m_nCurSegIndex = 0;
	m_pSeqFrameMap = 0;

	//Release the initialize tracklets for each segment.
	if (m_pProSegTracks) {
		for (int i = 0; i < (int)(m_pProSegTracks->size()); i++) {
			if ((*m_pProSegTracks)[i]) {
				RELEASE((*m_pProSegTracks)[i]);
				(*m_pProSegTracks)[i] = 0;
			}
		}
		m_pProSegTracks->clear();
		RELEASE(m_pProSegTracks);
		m_pProSegTracks = 0;
	}

	//Release the optimized tracklets for each segment.
	if (m_pOptSegTracks) {
		for (int i = 0; i < (int)(m_pOptSegTracks->size()); i++) {
			if ((*m_pOptSegTracks)[i]) {
				RELEASE((*m_pOptSegTracks)[i]);
				(*m_pOptSegTracks)[i] = 0;
			}
		}
		m_pOptSegTracks->clear();
		RELEASE(m_pOptSegTracks);
		m_pOptSegTracks = 0;
	}

	//The optimized tracklet set in the current segment.
	m_pCurSegTrack = 0;

	if (m_pCurInitTracks) {
		m_pCurInitTracks->clear();
		RELEASE(m_pCurInitTracks);
		m_pCurInitTracks = 0;
	}

	if (m_pCurOptTracks) {
		m_pCurOptTracks->clear();
		RELEASE(m_pCurOptTracks);
		m_pCurOptTracks = 0;
	}

	//Number of segments to be processed.
	m_nNumSegs = 0;
    //Begin segment index of current optimization.
	m_nBegSegIdx = 0;
	//End segment index of current optimization.
	m_nEndSegIdx = 0;

	//Release the node appearance similarity.
	if (m_pNodeAppSims) {
		m_pNodeAppSims->clear();
		RELEASE(m_pNodeAppSims);
		m_pNodeAppSims = 0;
	}


	//Release the cluster variable.
	GraphDecFree_API(m_pGraphDecOpt);

	m_pOptParas = 0;

	//Used for hypergraph or graph weights.
	m_vEdgeWeights.clear();

	//Used for hypergraph weights.
	m_vNodeWeights.clear();

    //Edge index.
	m_vEdgeManIdx.clear();


	//Parameters for the candidate K-Ary of the tracklets.
    if (m_pCandKAry) {
		m_pCandKAry->clear();
		RELEASE(m_pCandKAry);
		m_pCandKAry = 0;
    }

	//Release HyperGraph Clustering Variables.
	if (m_pOptSupports) {
		m_pOptSupports->clear();
		RELEASE(m_pOptSupports);
		m_pOptSupports = 0;
	}

	if (m_pOptProb) {
		m_pOptProb->clear();
		RELEASE(m_pOptProb);
		m_pOptProb = 0;
	}

	if (m_pOptValue) {
		m_pOptValue->clear();
		RELEASE(m_pOptValue);
		m_pOptValue = 0;
	}

	if (m_pOptClusters) {
		m_pOptClusters->clear();
		RELEASE(m_pOptClusters);
		m_pOptClusters = 0;
	}

	if (m_pSplines) {
		RELEASE(m_pSplines);
		m_pSplines = 0;
	}
	m_nBoundType = 0;

	//Appearance affinity
	if (m_pAppAffinity) {
		m_pAppAffinity->clear();
		RELEASE(m_pAppAffinity);
		m_pAppAffinity = 0;
	}
	//Motion affinity.
	if (m_pMoveAffinity) {
		m_pMoveAffinity->clear();
		RELEASE(m_pMoveAffinity);
		m_pMoveAffinity = 0;
	}
	//Smoothness affinity.
	if (m_pSmthAffinity) {
		m_pSmthAffinity->clear();
		RELEASE(m_pSmthAffinity);
		m_pSmthAffinity = 0;
	}
	
	//--------------------------------- Global Appearance Model ------------------------------------------------//
	//Tracklet neighboring analysis
	if (m_pNodeNeighMarks) {
		RELEASE(m_pNodeNeighMarks);
		m_pNodeNeighMarks = 0;
	}
	

	if (m_p2DDistance) {
		RELEASE(m_p2DDistance);
		m_p2DDistance = 0;
	}

	//Cluster variables.
	if (m_pClusterParas) {
		RELEASE(m_pClusterParas);
		m_pClusterParas = 0;
	}

	//The export pointer, just used for debug.
	m_pExpoter = 0;

}


bool CSegmentGraph::GraphOptimize(pSegTrackSet* pInit, LayerGraphOptPara* pPara, pImageMap* pFrame, CExport* pExport, pSegTrackSet* pOpt)
{

	//***************************************************************************//
	//Check whether the initial segment tracklet set is empty or not.
	if (pInit->empty())
		return false;

	//----------------------------- Connecting multiple low level tracklets into middle level tracklets --------------------------//
	int nTracks = 0;
	//Construct the segments according to the low level tracklets.
	//---------------------------------------------------------------------------------//
	//Release the previous memory of the initial segment tracks.
	for (int i = 0; i < (int)(m_pProSegTracks->size()); i++) {
		if ((*m_pProSegTracks)[i]) {
			RELEASE((*m_pProSegTracks)[i]);
			(*m_pProSegTracks)[i] = 0;
		}
	}
	m_pProSegTracks->clear();
	//Release the previous memory of the optimal segment tracks.
	//Release the previous optimized tracklets.
	for (int i = 0; i < (int)(m_pOptSegTracks->size()); i++) {
		if ((*m_pOptSegTracks)[i]) {
			RELEASE((*m_pOptSegTracks)[i]);
			(*m_pOptSegTracks)[i] = 0;
		}
	}
	m_pOptSegTracks->clear();

	//***********************************************************************************************************************//
	//***********************************************************************************************************************//
	//***********************************************************************************************************************//
	//------------------------------------------------------------------------------//
    //Get the optimization parameters.
	m_pOptParas = pPara;
	//Export variables.
	m_pExpoter = pExport;

	//Get the frame map variables.
	m_pSeqFrameMap = pFrame;

	//Set the current segment to zero.
	//Initialization.
	m_nCurSegIndex = 0;

	//Number of segments to be processed.
	m_nNumSegs = 0;
	//Get the segment index.
	m_nBegSegIdx = (*pInit)[0]->m_nSegIndex;
	m_nEndSegIdx = (*pInit)[0]->m_nSegIndex;
	
	//Get the segment tracks.
	for (int i = 0; i < (int)(pInit->size()); i++) {
		m_nBegSegIdx = min((*pInit)[i]->m_nSegIndex, m_nBegSegIdx);
		m_nEndSegIdx = max((*pInit)[i]->m_nSegIndex, m_nEndSegIdx);
	}
	m_nNumSegs = m_nEndSegIdx - m_nBegSegIdx + 1;

	//Subcontract the segments, first get the segment frame images.
	int nPreIdx = 0;
	int nCurIdx = 0;

	//Calculate the segment size.
	int nSegs = m_nNumSegs / m_pOptParas->m_nSegSize;
    nSegs = max(nSegs, 1);
	
	//Get the current process tracklets.
	for (int i = 0; i < nSegs; i++) {
		CSegTracks* pNewSegTk = new CSegTracks;
		pNewSegTk->m_bActMark = true;
		pNewSegTk->m_nSegIndex = i;
		m_pProSegTracks->push_back(pNewSegTk);
	}

	//------------------------------------------------------------------------------//
	//Copy the initialize tracks.
	for (int i = 0; i < (int)(pInit->size()); i++) {
		//Get the previous segment index.
		nPreIdx = (*pInit)[i]->m_nSegIndex;
		nPreIdx = nPreIdx / m_pOptParas->m_nSegSize;
		//Get the next segment index.
		nCurIdx = (nPreIdx + 1) * m_pOptParas->m_nSegSize;
		if (nCurIdx > m_nNumSegs)
			nPreIdx = nSegs - 1;

		for (int j = 0; j < (int)((*pInit)[i]->m_pTracks->size()); j++) {
			CTrack* pNewTrack = new CTrack(((*(*pInit)[i]->m_pTracks))[j]);
			(*m_pProSegTracks)[nPreIdx]->m_pTracks->push_back(pNewTrack);
		}
	}

	//Clear the vector pointer.
	//Release the previous pointer to be zero.
	m_pCurSegTrack = 0;
    //Count of the segment.
	int nCount = 0;
	//Handle multiple segments.
	for (int i = 0; i < (int)(m_pProSegTracks->size()); i++) {
		//Pass the value of the tracks in segment to the current variable (similar as the role of detection).
		//Get the current process segment tracklet.
		//Current process segment index.
		m_nCurSegIndex = i;
		//Process segment pointer.
		m_pCurSegTrack = (*m_pProSegTracks)[i];

		//Clear the previous memory of the initialize tracklets.
		m_pCurInitTracks->clear();
		//----------------------------- Get the tracklet list -------------------------------------------//
	    for (int j = 0; j < (int)(m_pCurSegTrack->m_pTracks->size()); j++)
			m_pCurInitTracks->push_back((*(m_pCurSegTrack->m_pTracks))[j]);

		//-----------------------------------------------------------------------------------------------------------//
		//----------------------------------- Get Some Help Information ---------------------------------------------//
		//Construct the neighboring relationships between tracklets.
		GetTrackNeighbors();
		//Construct the similarities matrix of the tracklets
		GetTrackAppSims();
		//----------------------------------------------------------------------------------------------------------//

		//-------------------- Conduct the Dense Neighborhoods Revealing Directly on the Graph --------------------//
		//Generate the candidate k-ary.
		GetTrackKAry();
		//Get the weight pairwise graph.
		GraphDecompose_ExhaustOpt();
		//Set the optimization parameters.
		SetClusterPara_Exhaust();

		//Main optimization function.
		DecomposeGraph_API(m_pGraphDecOpt, m_vEdgeWeights, m_vNodeWeights, *m_pClusterParas, *m_pOptSupports, *m_pOptProb, *m_pOptValue);

		//Generate the Final Level Tracklets According to the Clustering Results.
		GetClusterTracks();

		//Print the information of the tracking process.
		printf("\r");
		printf("Finish Handling %d th Segment!", nCount);
		nCount++;
	}

	//Pass the tracklet values to the outer class.
	for (int i = 0; i < (int)(pOpt->size()); i++) {
		if ((*pOpt)[i]) {
			RELEASE((*pOpt)[i]);
			(*pOpt)[i] = 0;
		}
	}
	pOpt->clear();

	//Pass the values.
	for (int i = 0; i < (int)(m_pOptSegTracks->size()); i++) {
		CSegTracks* pNewTrack = new CSegTracks((*m_pOptSegTracks)[i]);
		pOpt->push_back(pNewTrack);
	}

	return true;
}


//Set the clustering parameters.
bool CSegmentGraph::SetClusterPara_Exhaust()
{
	m_pClusterParas->m_K = m_pOptParas->m_nTrackNumK;
	m_pClusterParas->m_NumOfVertices = (int)(m_pCurInitTracks->size());
	m_pClusterParas->m_MaxProb = 1.0 / m_pOptParas->m_nTrackMinVertices;
	m_pClusterParas->m_MinProb = 0.0001;
	m_pClusterParas->m_MinRewardDiff = 0.00001;
	m_pClusterParas->m_MinValue = 0.0000000000001;
	m_pClusterParas->m_MinVertexOfCluster = max(2, m_pOptParas->m_nTrackMinVertices);

	return true;
}


double CSegmentGraph::GetSmthDevs(pTrackSet* pTracks)
{
	int nSampleStep = m_pOptParas->m_nTrackSmthSampleStep;
	double nDevs = 0;

	pDetBoxSet* pBoxes = new pDetBoxSet;
	pBoxes->clear();

	CTrack* pTk = 0;
	CDetBBox* pBx = 0;
	//Get the box set in the tracklets.
	for (int i = 0; i < (int)(pTracks->size()); i++) {
		pTk = (*pTracks)[i];
		for (int j = 0; j < (int)(pTk->m_pDetPoints->size()); j++) {
			pBx = (*(pTk->m_pDetPoints))[j];
			pBoxes->push_back(pBx);
		}
	}

	//Sort the detection boxes in order according to the frame index.
	for (int i = 0; i < (int)(pBoxes->size() - 1); i++) {
		for (int j = i + 1; j < (int)(pBoxes->size()); j++) {
			//Check whether exchange the box.
			if ((*pBoxes)[i]->m_nFrameIndex > (*pBoxes)[j]->m_nFrameIndex) {
				pBx = (*pBoxes)[i];
				(*pBoxes)[i] = (*pBoxes)[j];
				(*pBoxes)[j] = pBx;
			}
		}
	}

	//Calculate the smooth term of the detection set.
	int nPoints = (int)(pBoxes->size());
	int nFitts = nPoints / nSampleStep + 1;
	int nTests = nPoints - nFitts;
	double nDx, nDy, nDw, nDh;

	if ((nFitts < 2)||(nTests < 1)) {
		nDevs = 0;
		return nDevs;
	}

	//Apply the point variable.
	double* pFxs = new double[nFitts];
	double* pFys = new double[nFitts];
	double* pFws = new double[nFitts];
	double* pFhs = new double[nFitts];
	double* pFsign = new double[nFitts];

	double* pPxs = new double[nTests];
	double* pPys = new double[nTests];
	double* pPws = new double[nTests];
	double* pPhs = new double[nTests];

	double* pTxs = new double[nTests];
	double* pTys = new double[nTests];
	double* pTws = new double[nTests];
	double* pThs = new double[nTests];
	double* pTsign = new double[nTests];

	bool* pTypes = new bool[nPoints];
	for (int j = 0; j < nPoints; j++)
		pTypes[j] = false;

	//Construct the types array.
	pTypes[0] = true;
	pTypes[nPoints - 1] = true;
	for (int j = 0; j < nFitts - 2; j++) 
		pTypes[(j + 1) * nSampleStep] = true;

	int nFitCt = 0;
	int nTestCt = 0;
	//Set the fitting set and test set according to the types set.
	for (int j = 0; j < nPoints; j++) {
		if (pTypes[j]) {
			pFxs[nFitCt] = (*pBoxes)[j]->m_n2Dcx;
			pFys[nFitCt] = (*pBoxes)[j]->m_n2Dcy;
			pFws[nFitCt] = (*pBoxes)[j]->m_nWidth;
			pFhs[nFitCt] = (*pBoxes)[j]->m_nHeight;
			pFsign[nFitCt] = (*pBoxes)[j]->m_nFrameIndex;
			nFitCt++;
		}
		else {
			pTxs[nTestCt] = (*pBoxes)[j]->m_n2Dcx;
			pTys[nTestCt] = (*pBoxes)[j]->m_n2Dcy;
			pTws[nTestCt] = (*pBoxes)[j]->m_nWidth;
			pThs[nTestCt] = (*pBoxes)[j]->m_nHeight;
			pTsign[nTestCt] = (*pBoxes)[j]->m_nFrameIndex;
			nTestCt++;
		}
	}

	//Set the spline values.
	real_1d_array algcx;
	real_1d_array algcy;
	real_1d_array algcw;
	real_1d_array algch;
	real_1d_array algct;

	algcx.setcontent(nFitts, pFxs);
	algcy.setcontent(nFitts, pFys);
	algcw.setcontent(nFitts, pFws);
	algch.setcontent(nFitts, pFhs);
	algct.setcontent(nFitts, pFsign);

	//Get the cubic spline of x coordinate.
	spline1dbuildcubic(algct, algcx, nFitts, m_nBoundType, 0.0, m_nBoundType, 0.0, *m_pSplines);
	for (int j = 0; j < nTests; j++)
		pPxs[j] = spline1dcalc(*m_pSplines, pTsign[j]);

	//Get the cubic spline of y coordinate.
	spline1dbuildcubic(algct, algcy, nFitts, m_nBoundType, 0.0, m_nBoundType, 0.0, *m_pSplines);
	for (int j = 0; j < nTests; j++)
		pPys[j] = spline1dcalc(*m_pSplines, pTsign[j]);

	//Get the cubic spline of size w.
	spline1dbuildcubic(algct, algcw, nFitts, m_nBoundType, 0.0, m_nBoundType, 0.0, *m_pSplines);
	for (int j = 0; j < nTests; j++)
		pPws[j] = spline1dcalc(*m_pSplines, pTsign[j]);

	//Get the cubic spline of size h.
	spline1dbuildcubic(algct, algch, nFitts, m_nBoundType, 0.0, m_nBoundType, 0.0, *m_pSplines);
	for (int j = 0; j < nTests; j++)
		pPhs[j] = spline1dcalc(*m_pSplines, pTsign[j]);

	double nBias = 0;
	for (int j = 0; j < nTests; j++) {
		nDx = pPxs[j] - pTxs[j];
		nDy = pPys[j] - pTys[j];
		nDw = pPws[j] - pTws[j];
		nDh = pPhs[j] - pThs[j];
		nBias += sqrt(nDx*nDx + nDy*nDy + nDw*nDw + nDh*nDh);
	}

	if (nTests > 0)
		nDevs = nBias / nTests;

	//Release the applied memory.
	if (pFxs) {
		RELEASEARRAY(pFxs);
		pFxs = 0;
	}
	if (pFys) {
		RELEASEARRAY(pFys);
		pFys = 0;
	}
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
	if (pTxs) {
		RELEASEARRAY(pTxs);
		pTxs = 0;
	}
	if (pTys) {
		RELEASEARRAY(pTys);
		pTys = 0;
	}
	if (pTws) {
		RELEASEARRAY(pTws);
		pTws = 0;
	}
	if (pThs) {
		RELEASEARRAY(pThs);
		pThs = 0;
	}
	if (pTsign) {
		RELEASEARRAY(pTsign);
		pTsign = 0;
	}
	if (pPxs) {
		RELEASEARRAY(pPxs);
		pPxs = 0;
	}
	if (pPys) {
		RELEASEARRAY(pPys);
		pPys = 0;
	}
	if (pPws) {
		RELEASEARRAY(pPws);
		pPws = 0;
	}
	if (pPhs) {
		RELEASEARRAY(pPhs);
		pPhs = 0;
	}
	if (pTypes) {
		RELEASEARRAY(pTypes);
		pTypes = 0;
	}
	if (pBoxes) {
		pBoxes->clear();
		RELEASE(pBoxes);
		pBoxes = 0;
	}

	return nDevs;
}

//Size is treated as the other two dimension of the points.
double CSegmentGraph::GetMoveDevs(pTrackSet* pTracks)
{
	double nDevs = 0;
	CDetBBox* pBx1 = 0;
	CDetBBox* pBx2 = 0;
	CTrack* pTk1 = 0;
	CTrack* pTk2 = 0;
	//First decide the parameters.
	int nSample = m_pOptParas->m_nTrackMoveSampleStep;

	//Check whether the tracklet set is validate to get the move deviations.
	if ((int)(pTracks->size()) <= 1)
		return 0;

	//Counter.
	int nCount1 = 0;
	int nCount2 = 0;
	//Check whether the tracklets are degenerated or not.
	for (int i = 0; i < (int)(pTracks->size()); i++) {
		if ((int)((*pTracks)[i]->m_pDetPoints->size()) <= 1)
			nCount1++;
		else
			nCount2++;
	}

	int nCase = 0;
	if (nCount1 == (int)(pTracks->size()))
		nCase = 1;
	else if (nCount2 == (int)(pTracks->size()))
		nCase = 3;
	else
		nCase = 2;

	switch (nCase) {
	case 1: 
		{
			//Use new strategy to calculate the motion term.
			//Temporal variables.
			pTrackSet* pTempTracks = new pTrackSet;
			pTempTracks->clear();

			for (int i = 0; i < (int)(pTracks->size()); i++)
				pTempTracks->push_back((*pTracks)[i]);

			//Get the Head and Tail of the tracklets.
			//Because there is no overlap between the tracklets, so we just need order the tracklets according to the head frame index
			pTk1 = 0;
			pTk2 = 0;
			for (int i = 0; i < (int)(pTempTracks->size() - 1); i++) {
				for (int j = i + 1; j < (int)(pTempTracks->size()); j++) {
					if ((*pTempTracks)[i]->m_nBegFrmIdx > (*pTempTracks)[j]->m_nBegFrmIdx) {
						pTk1 = (*pTempTracks)[i];
						(*pTempTracks)[i] = (*pTempTracks)[j];
						(*pTempTracks)[j] = pTk1;
					}
				}
			}

			//Some key points.
			vector<double> vKeyPxs;
			vector<double> vKeyPys;
			
			//Clear the previous memory.
			vKeyPxs.clear();
			vKeyPys.clear();

			//Get the cont term in motion part.
			for (int i = 0; i < (int)(pTempTracks->size()); i++) {
				pTk1 = (*pTempTracks)[i];
				pBx1 = (*(pTk1->m_pDetPoints))[0];
				//Get x coordinate.
				vKeyPxs.push_back(pBx1->m_n2Dcx);
				//Get y coordinate.
				vKeyPys.push_back(pBx1->m_n2Dcy);
			}

			//Temporal variable.
			double nDx = 0;
			double nDy = 0;

			double nConts = 0;
			//Get the cont term in motion deviation.
			for (int i = 0; i < (int)(pTempTracks->size()) - 1; i++) {	
				nDx = vKeyPxs[i] - vKeyPxs[i+1];
				nDy = vKeyPys[i] - vKeyPys[i+1];
				nConts = max(nConts, sqrt(nDx*nDx + nDy*nDy));
			}

			double nCurvs = 0;
			//Get the curv term in motion deviation.
			for (int i = 0; i < (int)(pTempTracks->size()) - 2; i++) {
				nDx = vKeyPxs[i] - 2 * vKeyPxs[i+1] + vKeyPxs[i+2];
				nDy = vKeyPys[i] - 2 * vKeyPys[i+1] + vKeyPys[i+2];
				nCurvs = max(nCurvs, sqrt(nDx*nDx + nDy*nDy));
			}

			nDevs = nConts + nCurvs;

			//Release the applied memory.
			//Release the temporal variable.
			if (pTempTracks) {
				pTempTracks->clear();
				RELEASE(pTempTracks);
				pTempTracks = 0;
			}

			//Tracklet variables.
			pTk1 = 0;
			pTk2 = 0;
			//Box variables.
			pBx1 = 0;
			pBx2 = 0;
			break;
		}
	case 2: 
		{
			if ((int)(pTracks->size()) > 2) {
				//Temporal variables.
				pTrackSet* pTempTracks = new pTrackSet;
				pTempTracks->clear();

				for (int i = 0; i < (int)(pTracks->size()); i++)
					pTempTracks->push_back((*pTracks)[i]);

				//Get the Head and Tail of the tracklets.
				//Because there is no overlap between the tracklets, so we just need order the tracklets according to the head frame index
				pTk1 = 0;
				pTk2 = 0;
				for (int i = 0; i < (int)(pTempTracks->size() - 1); i++) {
					for (int j = i + 1; j < (int)(pTempTracks->size()); j++) {
						if ((*pTempTracks)[i]->m_nBegFrmIdx > (*pTempTracks)[j]->m_nBegFrmIdx) {
							pTk1 = (*pTempTracks)[i];
							(*pTempTracks)[i] = (*pTempTracks)[j];
							(*pTempTracks)[j] = pTk1;
						}
					}
				}

				//Apply the memory for the difference vector.
				double* pDxs = new double[(int)(pTempTracks->size() - 1)];
				double* pDys = new double[(int)(pTempTracks->size() - 1)];
				
				//Initialization.
				for (int i = 0; i < (int)(pTempTracks->size() - 1); i++) {
					pDxs[i] = 0;
					pDys[i] = 0;
				}

				//Check the motion case, if one of the tracklet has just one detection, case1, otherwise case two.
				for (int i = 0; i < (int)(pTempTracks->size() - 1); i++) {
					//Previous track.
					pTk1 = (*pTempTracks)[i];
					//Current track.
					pTk2 = (*pTempTracks)[i+1];
					//Get the bounding box.
					pBx1 = (*(pTk1->m_pDetPoints))[(int)(pTk1->m_pDetPoints->size() - 1)];
					pBx2 = (*(pTk2->m_pDetPoints))[0];
					//Get the information.
					pDxs[i] = (pBx2->m_n2Dcx - pBx1->m_n2Dcx) / fabs((double)(pBx1->m_nFrameIndex - pBx2->m_nFrameIndex));
					pDys[i] = (pBx2->m_n2Dcy - pBx1->m_n2Dcy) / fabs((double)(pBx1->m_nFrameIndex - pBx2->m_nFrameIndex));
				}

				double nDist1 = 0;
				double nDist2 = 0;
				double nTheta = 0;
				nDevs = 0;

				for (int i = 0; i < (int)(pTempTracks->size() - 2); i++) {
					nDist1 = sqrt(pDxs[i]*pDxs[i]+pDys[i]*pDys[i]);
					nDist2 = sqrt(pDxs[i+1]*pDxs[i+1]+pDys[i+1]*pDys[i+1]);
					nTheta = (pDxs[i]*pDxs[i+1]+pDys[i]*pDys[i+1])/(nDist1*nDist2);
					nDevs = max(nDevs, abs(nDist1 - nDist2)*(1 - nTheta));
				}

				//Release the variables.
				if (pDxs) {
					RELEASEARRAY(pDxs);
					pDxs = 0;
				}
				if (pDys) {
					RELEASEARRAY(pDys);
					pDys = 0;
				}
				//Release the temporal variable.
				if (pTempTracks) {
					pTempTracks->clear();
					RELEASE(pTempTracks);
					pTempTracks = 0;
				}

				//Tracklet variables.
				pTk1 = 0;
				pTk2 = 0;
				//Box variables.
				pBx1 = 0;
				pBx2 = 0;
			}
			else {
				nDevs = 0;
			}
			break;
		}
	case 3: 
		{
			//Use new strategy to calculate the motion term.
			//Temporal variables.
			pTrackSet* pTempTracks = new pTrackSet;
			pTempTracks->clear();

			for (int i = 0; i < (int)(pTracks->size()); i++)
				pTempTracks->push_back((*pTracks)[i]);

			//Get the Head and Tail of the tracklets.
			//Because there is no overlap between the tracklets, so we just need order the tracklets according to the head frame index
			pTk1 = 0;
			pTk2 = 0;
			for (int i = 0; i < (int)(pTempTracks->size() - 1); i++) {
				for (int j = i + 1; j < (int)(pTempTracks->size()); j++) {
					if ((*pTempTracks)[i]->m_nBegFrmIdx > (*pTempTracks)[j]->m_nBegFrmIdx) {
						pTk1 = (*pTempTracks)[i];
						(*pTempTracks)[i] = (*pTempTracks)[j];
						(*pTempTracks)[j] = pTk1;
					}
				}
			}

			int nLen1 = 0;
			int nLen2 = 0;
			int nPred1 = 0;
			int nPred2 = 0;
			double nErrs = 0;

			double nVx, nVy;
			nDevs = 0;
			//Get the prediction error of the tracklet set.
			for (int i = 0; i < (int)(pTempTracks->size() - 1); i++) {
				pTk1 = (*pTempTracks)[i];
				nLen1 = (int)(pTk1->m_pDetPoints->size());
				nPred1 = min(nLen1, nSample);

				pTk2 = (*pTempTracks)[i+1];
				nLen2 = (int)(pTk2->m_pDetPoints->size());
				nPred2 = min(nLen2, nSample);

				nErrs = 0;
				for (int j = 0; j < nPred1 - 1; j++) {
					for (int k = j + 1; k < nPred1; k++) {
						//Get the velocity.
						pBx1 = (*(pTk1->m_pDetPoints))[(int)(pTk1->m_pDetPoints->size() - 1) - k];
						pBx2 = (*(pTk1->m_pDetPoints))[(int)(pTk1->m_pDetPoints->size() - 1) - j];

						nVx = (pBx2->m_n2Dcx - pBx1->m_n2Dcx) / (fabs((double)(pBx2->m_nFrameIndex - pBx1->m_nFrameIndex)));
						nVy = (pBx2->m_n2Dcy - pBx1->m_n2Dcy) / (fabs((double)(pBx2->m_nFrameIndex - pBx1->m_nFrameIndex)));
						
						pBx1 = (*(pTk1->m_pDetPoints))[(int)(pTk1->m_pDetPoints->size() - 1) - k];
						pBx2 = (*(pTk2->m_pDetPoints))[0];
						nDevs += CalcPosDeviate(pBx1, nVx, nVy, pBx2);
					}
				}
				nErrs = 2 * nDevs / (nPred1 * (nPred1 - 1));
				nDevs += nErrs;

				nErrs = 0;
				for (int j = 0; j < nPred2 - 1; j++) {
					for (int k = j + 1; k < nPred2; k++) {
						//Get the velocity.
						pBx1 = (*(pTk2->m_pDetPoints))[k];
						pBx2 = (*(pTk2->m_pDetPoints))[j];

						nVx = (pBx2->m_n2Dcx - pBx1->m_n2Dcx) / (fabs((double)(pBx2->m_nFrameIndex - pBx1->m_nFrameIndex)));
						nVy = (pBx2->m_n2Dcy - pBx1->m_n2Dcy) / (fabs((double)(pBx2->m_nFrameIndex - pBx1->m_nFrameIndex)));
						
						pBx1 = ((*pTk2->m_pDetPoints))[k];
						pBx2 = (*(pTk1->m_pDetPoints))[(int)(pTk1->m_pDetPoints->size() - 1)];
						nErrs += CalcPosDeviate(pBx1, nVx, nVy, pBx2);
					}
				}
				nErrs = 2 * nErrs / (nPred2 * (nPred2 - 1));
				nDevs += nErrs;
			}

			//Release the temporal variable.
			if (pTempTracks) {
				pTempTracks->clear();
				RELEASE(pTempTracks);
				pTempTracks = 0;
			}

			//Tracklet variables.
			pTk1 = 0;
			pTk2 = 0;

			break;
		}
	default:
		nDevs = 0;
	}

	return nDevs;
}


//-------------------------------------------------------------------------------------------------//
//---------------- Get the appearance similarities between different tracklets ------------//
bool CSegmentGraph::GetTrackAppSims()
{
	//************************************************************//
	//**** Some important variables.
	CTrack* pTk1 = 0;
	CTrack* pTk2 = 0;

	//Appearance compensation factor.
	double nComps= m_pOptParas->m_nAppCompFactor;
	//Time interval variable.
	double nInterval = 0;
	//Confidence between two tracklets.
	double nConf = 0;

	//*********** Calculate the similarity between different tracklets (nodes).
	m_pNodeAppSims->clear();
	//Number of tracks.
	int nTracks = (int)(m_pCurInitTracks->size());
    //Basic items in tracklet similarity.
	//Construct the similarities.
	DoubleVector vSims;
	vSims.clear();
	for (int i = 0; i < nTracks; i++)
		vSims.push_back(0);
	for (int i = 0; i < nTracks; i++)
		m_pNodeAppSims->push_back(vSims);

	for (int i = 0; i < nTracks - 1; i++) {
		pTk1 = (*m_pCurInitTracks)[i];
		for (int j = i + 1; j < nTracks; j++) {
			pTk2 = (*m_pCurInitTracks)[j];
			//If the two tracklets are not the neighborhoods, continue.
			if (!(m_pNodeNeighMarks->m_pMask[i][j]))
				continue;
			//Compensate factor.
			nInterval = fabs((double)(pTk1->m_nSegIndex - pTk2->m_nSegIndex)) - 1;
            if (pTk1->m_nSegIndex > pTk2->m_nSegIndex) {
				//Get the similarities between two features.
				nConf = max(CalcFeatSimilarity(pTk1->m_pHeadAvgFeat, pTk2->m_pTailAvgFeat), 
					CalcFeatSimilarity(pTk1->m_pHeadDetFeat, pTk2->m_pTailDetFeat));
				(*m_pNodeAppSims)[i][j] = nConf * exp(nInterval * nComps);
            }
			else {
				//Get the similarities between two features.
				nConf = max(CalcFeatSimilarity(pTk1->m_pTailAvgFeat, pTk2->m_pHeadAvgFeat),
					CalcFeatSimilarity(pTk1->m_pTailDetFeat, pTk2->m_pHeadDetFeat));
				(*m_pNodeAppSims)[i][j] = nConf * exp(nInterval * nComps);
			}
		}
	}

	//Get the right half area of the array.
	for (int i = 0; i < nTracks - 1; i++) {
		for (int j = i + 1; j < nTracks; j++) {
			(*m_pNodeAppSims)[j][i] = (*m_pNodeAppSims)[i][j];
		}
	}

	return true;
}

//-------------------------------------------------------------------------------------------------//
//---------------- Get the neighboring system of different tracklets ------------//
bool CSegmentGraph::GetTrackNeighbors()
{
	//Release the previous memory of the tracklet neighboring system.
	if (m_pNodeNeighMarks) {
		RELEASE(m_pNodeNeighMarks);
		m_pNodeNeighMarks = 0;
	}
	
	//Release the distance matrix.
	if (m_p2DDistance) {
		RELEASE(m_p2DDistance);
		m_p2DDistance = 0;
	}

	//-----------------------------------------------------------------------------//
	//------ Initialize the tracklet neighboring system according to the spatial relationships
	//Number of tracklets.
	int nTracks = (int)(m_pCurInitTracks->size());

	//General neighborhood marks.
	m_pNodeNeighMarks = new CMask(nTracks, nTracks, false);
	
	//Distance matrix.
	m_p2DDistance = matrix_create(nTracks, nTracks);

	
	//Just a pointer.
	CTrack* pTrack1 = 0;
	CTrack* pTrack2 = 0;
	for (int i = 0; i < nTracks - 1; i++) {
		for (int j = i + 1; j < nTracks; j++) {
			pTrack1 = (*m_pCurInitTracks)[i];
			pTrack2 = (*m_pCurInitTracks)[j];
			m_p2DDistance->m_pMatrix[i][j] = CalcTrack2DDist(pTrack1, pTrack2);
		}
	}
	//Symmetric the matrix.
	for (int i = 0; i < nTracks - 1; i++) {
		for (int j = i + 1; j < nTracks; j++) {
			m_p2DDistance->m_pMatrix[j][i] = m_p2DDistance->m_pMatrix[i][j];
		}
	}

	int nGaps = 0;
	bool bConf = true;
	int nDiff = 0;

	bool bOccs = true;

	//***********************************************************************************************************************//
	//Get the general neighboring system and the MSS neighboring system of the tracklets.
	for (int i = 0; i < nTracks - 1; i++) {
		for (int j = i + 1; j < nTracks; j++) {
			pTrack1 = (*m_pCurInitTracks)[i];
			pTrack2 = (*m_pCurInitTracks)[j];
			nGaps = abs(pTrack1->m_nSegIndex - pTrack2->m_nSegIndex);
			bConf = CheckTrackOverlap(pTrack1, pTrack2);
			nDiff = min(abs(pTrack1->m_nBegFrmIdx - pTrack2->m_nEndFrmIdx), abs(pTrack1->m_nEndFrmIdx - pTrack2->m_nBegFrmIdx));

			//Get the occlusion reasoning constraints of the two tracklets.
			bOccs = CheckOccReasons(pTrack1, pTrack2);

			//Get the general neighboring system.
			if ((m_p2DDistance->m_pMatrix[i][j] < m_pOptParas->m_nMaxVelocity * nDiff)
				&&(nGaps <= m_pOptParas->m_nGenTrackMaxSegNeigh)&&(bOccs)
				&&(m_p2DDistance->m_pMatrix[i][j] < m_pOptParas->m_nGenTrackMaxDistNeigh)) {
					//Get the node neighboring marks.
					m_pNodeNeighMarks->m_pMask[i][j] = true;
			}
		}
	}

    //**********************************************************************************************************************//
	//Make these two neighboring matrix to be symmetry
	for (int i = 0; i < nTracks - 1; i++) {
		for (int j = i + 1; j < nTracks; j++) {
			m_pNodeNeighMarks->m_pMask[j][i] = m_pNodeNeighMarks->m_pMask[i][j];
		}
	}

	return true;
}

bool CSegmentGraph::CheckOccReasons(CTrack* pTk1, CTrack* pTk2)
{
	bool bOccs = true;
	//Get the parameters according to the level value.
	if (CheckTrackOverlap(pTk1, pTk2)) {
		bOccs = false;
		return bOccs;
	}

	int nGaps = min(abs(pTk1->m_nBegFrmIdx - pTk2->m_nEndFrmIdx), abs(pTk1->m_nEndFrmIdx - pTk2->m_nBegFrmIdx));
	if (nGaps < m_pOptParas->m_nSafeGapNeighThresh) {
		bOccs = true;
		return bOccs;
	}

	double nRatio = 0;

	//First get the sequence detection boxes of the tracklets.
	pDetBoxSet* pBoxes = new pDetBoxSet;
	pBoxes->clear();

	CTrack* pTk = 0;
	CDetBBox* pBx1 = 0;
	CDetBBox* pBx2 = 0;

	//Push the boxes in the first tracklets.
	for (int i = 0; i < (int)(pTk1->m_pDetPoints->size()); i++) {
		pBx1 = (*(pTk1->m_pDetPoints))[i];
		pBoxes->push_back(pBx1);
	}
	//Push the boxes in the second tracklets.
	for (int i = 0; i < (int)(pTk2->m_pDetPoints->size()); i++) {
		pBx1 = (*(pTk2->m_pDetPoints))[i];
		pBoxes->push_back(pBx1);
	}

	//Sort the detection boxes in order according to the frame index.
	for (int i = 0; i < (int)(pBoxes->size() - 1); i++) {
		for (int j = i + 1; j < (int)(pBoxes->size()); j++) {
			//Check whether exchange the box.
			if ((*pBoxes)[i]->m_nFrameIndex > (*pBoxes)[j]->m_nFrameIndex) {
				pBx1 = (*pBoxes)[i];
				(*pBoxes)[i] = (*pBoxes)[j];
				(*pBoxes)[j] = pBx1;
			}
		}
	}

	int nGap = 0;
	int nFid1, nFid2;
	CDetBBox* pVirtBox = new CDetBBox;
	CTImage* pFrame = 0;
	pImageMap::iterator iter_frame;

	int nCount = 0;
	//Check whether there exists a time gap.
	for (int i = 0; i < (int)(pBoxes->size() - 1); i++) {
		pBx1 = (*pBoxes)[i];
		pBx2 = (*pBoxes)[i+1];
		nFid1 = pBx1->m_nFrameIndex;
		nFid2 = pBx2->m_nFrameIndex;
		nGap = nFid2 - nFid1 - 1;
		if (nGap > 0) {
			for (int j = 0; j < nGap; j++) {
				pVirtBox->m_bVirtMark = true;
				pVirtBox->m_nTrackID = (*pBoxes)[0]->m_nTrackID;

				pVirtBox->m_nFrameIndex = nFid1 + j + 1;
				pVirtBox->m_nConf = 0;
				pVirtBox->m_nDetBoxID = -1;
				pVirtBox->m_n2Dcx = pBx1->m_n2Dcx + (pBx2->m_n2Dcx - pBx1->m_n2Dcx) * (j + 1) / (nGap + 1);
				pVirtBox->m_n2Dcy = pBx1->m_n2Dcy + (pBx2->m_n2Dcy - pBx1->m_n2Dcy) * (j + 1) / (nGap + 1);
				pVirtBox->m_nWidth = pBx1->m_nWidth + (pBx2->m_nWidth - pBx1->m_nWidth) * (j + 1) / (nGap + 1);
				pVirtBox->m_nHeight = pBx1->m_nHeight + (pBx2->m_nHeight - pBx1->m_nHeight) * (j + 1) / (nGap + 1);

				iter_frame = m_pSeqFrameMap->find(pVirtBox->m_nFrameIndex);
				if (iter_frame != m_pSeqFrameMap->end()) {
					pFrame = iter_frame->second;
					nRatio += CalcOccRatio(pVirtBox, pFrame);
				}
				nCount++;
			}
		}
	}

	if (nCount > 0)
		nRatio = nRatio / nCount;

	if (nRatio > m_pOptParas->m_nOccNeighThresh)
		bOccs = true;
	else
		bOccs = false;

	//Release the memory.
	if (pVirtBox) {
		RELEASE(pVirtBox);
		pVirtBox = 0;
	}
	if (pBoxes) {
		RELEASE(pBoxes);
		pBoxes = 0;
	}

	pTk = 0;
	pBx1 = 0;
	pBx2 = 0;

	return bOccs;
}



//************************************************************
//Get the candidate K-Ary, mainly used for the general pairwise graph.
bool CSegmentGraph::GetTrackKAry()
{
	//Release the previous memory.
	m_pCandKAry->clear();

	//Check the parameters.
	if (m_pNodeNeighMarks->m_nWidth != m_pNodeNeighMarks->m_nHeight) {
		fprintf(stderr, "Parameters Error in Variable m_pNodeNeighMarks!\n");
		return false;
	}

	//Get the number of nodes in neighborhing system.
	int nTracks = m_pNodeNeighMarks->m_nWidth;

	switch (m_pOptParas->m_nTrackNumK) {
	case 2:
		Get2Ary(m_pNodeNeighMarks, m_pCandKAry);
		break;
	case 3:
		Get3Ary(m_pNodeNeighMarks, m_pCandKAry);
		break;
	case 4:
		Get4Ary(m_pNodeNeighMarks, m_pCandKAry);
		break;
	case 5:
		Get5Ary(m_pNodeNeighMarks, m_pCandKAry);
		break;
	case 6:
		Get6Ary(m_pNodeNeighMarks, m_pCandKAry);
		break;
	case 7:
		Get7Ary(m_pNodeNeighMarks, m_pCandKAry);
		break;
	case 8:
		Get8Ary(m_pNodeNeighMarks, m_pCandKAry);
		break;
	case 9:
		Get9Ary(m_pNodeNeighMarks, m_pCandKAry);
		break;
	case 10:
		Get10Ary(m_pNodeNeighMarks, m_pCandKAry);
		break;
	default:
		break;
	}

	return true;
}

//************************************************************
//Search the dense neighborhoods using exhaust optimization.
bool CSegmentGraph::GraphDecompose_ExhaustOpt()
{
	//Number of 2-ary.
	int nCandAry = (int)(m_pCandKAry->size());
	//Some parameters used globally.
	pTrackSet* pTempTracks = new pTrackSet;
	pTempTracks->clear();

	//------------------------------------------------------------//
	//----- Get the four aspects of the candidate 2-ary affinity calculation.
    //Bound of the four terms.
	double nAppBound = m_pOptParas->m_pExhaustOptParas->m_nAppUpperBound;
	double nMoveBound = m_pOptParas->m_pExhaustOptParas->m_nMoveUpperBound;
	double nSmthBound = m_pOptParas->m_pExhaustOptParas->m_nSmthUpperBound;
	
	//Alpha for four terms.
	double nAppAlpha = m_pOptParas->m_pExhaustOptParas->m_nAppAlpha;
	double nMoveAlpha = m_pOptParas->m_pExhaustOptParas->m_nMoveAlpha;
	double nSmthAlpha = m_pOptParas->m_pExhaustOptParas->m_nSmthAlpha;
	
	//Sigma of the four terms.
	double nAppSigma = m_pOptParas->m_pExhaustOptParas->m_nAppSigma;
    double nMoveSigma = m_pOptParas->m_pExhaustOptParas->m_nMoveSigma;
	double nSmthSigma = m_pOptParas->m_pExhaustOptParas->m_nSmthSigma;
	
    //Clear the previous memory.
	m_pAppAffinity->clear();
	m_pMoveAffinity->clear();
	m_pSmthAffinity->clear();
	m_vEdgeWeights.clear();

	//Initialize the memory.
	for (int i = 0; i < nCandAry; i++) {
		IntVectorDouble Affinity;
		Affinity.Ivs.clear();
		Affinity.D = 0;
		for (int j = 0; j < (int)((*m_pCandKAry)[i].size()); j++)
			Affinity.Ivs.push_back((*m_pCandKAry)[i][j]);

		//Appearance terms.
		m_pAppAffinity->push_back(Affinity);
		//Move terms.
		m_pMoveAffinity->push_back(Affinity);
		//Smooth term.
		m_pSmthAffinity->push_back(Affinity);
		//Hypergraph or graph edges weight.
		m_vEdgeWeights.push_back(Affinity);
	}

	double nDevs = 0;
	double nConf = 0;
	int nAdds = 0;
	pTempTracks->clear();
	for (int i = 0; i < nCandAry; i++) {
		//Get the tracklet set of the k-ary.
		//Sort the tracklet set.
		sort((*m_pCandKAry)[i].begin(), (*m_pCandKAry)[i].end());
		pTempTracks->clear();
		for (int j = 0; j < (int)((*m_pCandKAry)[i].size()); j++)
			pTempTracks->push_back((*m_pCurInitTracks)[(*m_pCandKAry)[i][j]]);

		//--------------------------------------------------//
		//Calculate the appearance term.
		nConf = 0;
		nAdds = 0;
		for (int j = 0; j < (int)((*m_pCandKAry)[i].size() - 1); j++) {
			for (int k = j + 1; k < (int)((*m_pCandKAry)[i].size()); k++) {
                nConf += (*m_pNodeAppSims)[(*m_pCandKAry)[i][j]][(*m_pCandKAry)[i][k]];
				nAdds++;
			}
		}
        if (nAdds > 0)
          nConf = nConf / (double)nAdds;
		 (*m_pAppAffinity)[i].D = nAppAlpha * exp(nAppSigma * nConf);

		//--------------------------------------------------//
		//Calculate the move term.
		nDevs = 0;
		nConf = 0;
		nDevs = GetMoveDevs(pTempTracks);
		nConf = nMoveAlpha * exp(-nMoveSigma * nDevs);
		(*m_pMoveAffinity)[i].D = nConf;
		//--------------------------------------------------//
		//Calculate the smooth term.
		nDevs = 0;
		nConf = 0;
		nDevs = GetSmthDevs(pTempTracks);
		nConf = nSmthAlpha * exp(-nSmthSigma * nDevs);
		(*m_pSmthAffinity)[i].D = nConf;
	}

	//Threshold of the four aspects.
    double nAppThreshold = m_pOptParas->m_pExhaustOptParas->m_nAppThreshold;
	double nMoveThreshold = m_pOptParas->m_pExhaustOptParas->m_nMoveThreshold;
	double nSmthThreshold = m_pOptParas->m_pExhaustOptParas->m_nSmthThreshold;
	
	IntVector vDeleteIdxs;
	vDeleteIdxs.clear();

	//Check whether the 2-ary satisfied the four aspects threshold.
	for (int i = 0; i < nCandAry; i++) {
		if (((*m_pAppAffinity)[i].D < nAppThreshold)||((*m_pMoveAffinity)[i].D < nMoveThreshold)
			||((*m_pSmthAffinity)[i].D < nSmthThreshold)) {
				//Delete the three aspects affinity value.
				//Appearance term.
				(*m_pAppAffinity)[i].D = 0;
				//Move term.
				(*m_pMoveAffinity)[i].D = 0;
				//Smooth term.
				(*m_pSmthAffinity)[i].D = 0;
				//Hypergraph and graph edges weights.
				m_vEdgeWeights[i].D = 0;
                //Delete index.
				vDeleteIdxs.push_back(i);
		}
	}
	sort(vDeleteIdxs.begin(), vDeleteIdxs.end());

	IVectorDVector::iterator iter_app;
	IVectorDVector::iterator iter_move;
	IVectorDVector::iterator iter_smth;
	IVectorDVector::iterator iter_edge;
	for (int i = (int)(vDeleteIdxs.size() - 1); i >= 0; i--) {
		//Delete appearance term.
		iter_app = m_pAppAffinity->begin();
		for (int j = 0; j < vDeleteIdxs[i]; j++)
			iter_app++;
		iter_app = m_pAppAffinity->erase(iter_app);
		//Delete move term.
		iter_move = m_pMoveAffinity->begin();
		for (int j = 0; j < vDeleteIdxs[i]; j++)
			iter_move++;
		iter_move = m_pMoveAffinity->erase(iter_move);
		//Delete smooth term.
		iter_smth = m_pSmthAffinity->begin();
		for (int j = 0; j < vDeleteIdxs[i]; j++)
			iter_smth++;
		iter_smth = m_pSmthAffinity->erase(iter_smth);
		//Delete pairwise term.
		iter_edge = m_vEdgeWeights.begin();
		for (int j = 0; j < vDeleteIdxs[i]; j++)
			iter_edge++;
		iter_edge = m_vEdgeWeights.erase(iter_edge);
	}

	//Set the bound of the appearance term.
	for (int i = 0; i < (int)(m_pAppAffinity->size()); i++)
		(*m_pAppAffinity)[i].D = (*m_pAppAffinity)[i].D < nAppBound ? (*m_pAppAffinity)[i].D : nAppBound;

	//Set the bound of the move term.
	for (int i = 0; i < (int)(m_pMoveAffinity->size()); i++)
		(*m_pMoveAffinity)[i].D = (*m_pMoveAffinity)[i].D < nMoveBound ? (*m_pMoveAffinity)[i].D : nMoveBound;

	//Set the bound of the smooth term.
	for (int i = 0; i < (int)(m_pSmthAffinity->size()); i++)
		(*m_pSmthAffinity)[i].D = (*m_pSmthAffinity)[i].D < nSmthBound ? (*m_pSmthAffinity)[i].D : nSmthBound;

	//Some variables.
	double nAppTerm = 0;
	double nMoveTerm = 0;
	double nSmthTerm = 0;
	//Combine these three terms.
	for (int i = 0; i < (int)(m_vEdgeWeights.size()); i++) {
		nAppTerm = m_pOptParas->m_pExhaustOptParas->m_nAppWeight * (*m_pAppAffinity)[i].D;
		nMoveTerm = m_pOptParas->m_pExhaustOptParas->m_nMoveWeight * (*m_pMoveAffinity)[i].D;
		nSmthTerm = m_pOptParas->m_pExhaustOptParas->m_nSmthWeight * (*m_pSmthAffinity)[i].D;
		m_vEdgeWeights[i].D = nAppTerm + nMoveTerm + nSmthTerm;
	}

	double nEdgeBound = m_pOptParas->m_pExhaustOptParas->m_nMixUpperBound;
	//Set the upper bound of the mixture.
	for (int i = 0; i < (int)(m_vEdgeWeights.size()); i++)
		m_vEdgeWeights[i].D = m_vEdgeWeights[i].D < nEdgeBound ? m_vEdgeWeights[i].D : nEdgeBound;

	//Release the memory.
	if (pTempTracks) {
		pTempTracks->clear();
		RELEASE(pTempTracks);
		pTempTracks = 0;
	}

	return true;
}



//************************************************************
//Get cluster tracks.
bool CSegmentGraph::GetClusterTracks()
{
	switch (m_pOptParas->m_nPostType) {
		case 1:
			GetCurSegTracks_TYP1();
			break;
		case 2:
			GetCurSegTracks_TYP2();
			break;
		default:
			GetCurSegTracks_TYP2();
			break;
	}
	return true;
}

//Two types of strategy to get the current optimized tracklets
bool CSegmentGraph::GetCurSegTracks_TYP1()
{
	//Get the small tracklets according to the clustering results.
	//Overlap items.
	IntVector vOverlap;
	IntVector::iterator vIter;
	//Copy of the supports.
	IntArray vSupports;
	//Copy the support variable to the copy.
	for (int i = 0; i < (int)(m_pOptSupports->size()); i++) {
		IntVector vNew;
		for (int j = 0; j < (int)((*m_pOptSupports)[i].size()); j++)
			vNew.push_back((*m_pOptSupports)[i][j]);
		vSupports.push_back(vNew);
	}

	m_pOptClusters->clear();
	//First sort the clusters in the support according to the FValue.
	IntVector vIndexSet;
	DoubleVector vFValueSet;
	vIndexSet.clear();
	for (int i = 0; i < (int)(m_pOptSupports->size()); i++)
		vIndexSet.push_back(i);

	vFValueSet.clear();
	for (int i = 0; i < (int)(m_pOptValue->size()); i++)
		vFValueSet.push_back((*m_pOptValue)[i]);

	SortDescend(&vFValueSet, &vIndexSet);

	//Add the conflicts handle between different clusters.(Important)
	for (int i = 0; i < (int)(vIndexSet.size()); i++) {
		//Check whether it has overlap components with the existing clusters.
		//Jump over the null set
		if (vSupports[vIndexSet[i]].empty())
			continue;

		//First clear the overlap elements of the other items.
		for (int j = i + 1; j < (int)(vIndexSet.size()); j++) {
			vOverlap.clear();
			sort(vSupports[vIndexSet[i]].begin(), vSupports[vIndexSet[i]].end());
			sort(vSupports[vIndexSet[j]].begin(), vSupports[vIndexSet[j]].end());
			set_intersection(vSupports[vIndexSet[i]].begin(), vSupports[vIndexSet[i]].end(), vSupports[vIndexSet[j]].begin(), vSupports[vIndexSet[j]].end(), back_inserter(vOverlap));
			//Clear the items in the j 
			//Sort the overlap value.
			sort(vOverlap.begin(), vOverlap.end());
			for (int k = 0; k < (int)(vOverlap.size()); k++) {
				for (vIter = vSupports[vIndexSet[j]].begin(); vIter != vSupports[vIndexSet[j]].end(); vIter++) {
					if (vOverlap[k] == *vIter) {
						vSupports[vIndexSet[j]].erase(vIter);
						break;
					}
				}
			}
		}
		//Second move the items to the cluster variable.
		IntVector vNew;
		for (int j = 0; j < (int)(vSupports[vIndexSet[i]].size()); j++)
			vNew.push_back(vSupports[vIndexSet[i]][j]);

		m_pOptClusters->push_back(vNew);
	}

	//***********************************************************************************************************//
	//Get the trajectories in previous iteration.
	//**********************************************************************************************************//
	//*** Release the previous memory of the tracklets.
	m_pCurOptTracks->clear();

	//Detection box temporal variable.
	CDetBBox* pTempBox = 0;
	//Construct the tracklets according to the clustering results.
	int nCount = 0;
	for (int i = 0; i < (int)(m_pOptClusters->size()); i++) {
		if ((*m_pOptClusters)[i].empty())
			continue;

		CTrack* pNewTrack = new CTrack;
		pNewTrack->m_nSegIndex = m_nCurSegIndex;
		pNewTrack->m_nVirtIndex = nCount;
		
		for (int j = 0; j < (int)((*m_pOptClusters)[i].size()); j++)
			pNewTrack->Add((*m_pCurInitTracks)[(*m_pOptClusters)[i][j]]);

		//Check whether the tracklet is null or not
		if ((int)(pNewTrack->m_pDetPoints->size()) >= m_pOptParas->m_nMinTrackLen) {
			pNewTrack->m_bActMark = true;
			pNewTrack->m_nTrackID = nCount;
            //Set the virtual index of all boxes in the tracklet.
			for (int j = 0; j < (int)(pNewTrack->m_pDetPoints->size()); j++) {
				pTempBox = (*(pNewTrack->m_pDetPoints))[j];
				pTempBox->m_nVirtIndex = nCount;
			}
			m_pCurOptTracks->push_back(pNewTrack);
			nCount++;
		}
		else {
			if (pNewTrack) {
				RELEASE(pNewTrack);
				pNewTrack = 0;
			}
		}
	}

	//Modified the trajectories.
	for (int i = 0; i < (int)(m_pCurOptTracks->size()); i++) {
		(*m_pCurOptTracks)[i]->SplineAdjust();
		(*m_pCurOptTracks)[i]->SplineFitting();
		(*m_pCurOptTracks)[i]->GetAppearance();
	}

	//-------------------------------------------- Save the debug informations ---------------------------------------------------//
	//m_pExpoter->ExportSegTrackImages_Debug(m_pCurOptTracks, m_pOptParas->m_nCurLayer);
    
	//-------------------------------------------------------------------------------------------------------------------//
	//------- Put the Optimized Tracklets into a Segment Tracklet Variable.
	//Collect the tracklets in the current window.
	//Initialize a new segment tracks.
	CSegTracks* pNewSegTrack = new CSegTracks;
	pNewSegTrack->m_nSegIndex = m_nCurSegIndex;
	pNewSegTrack->m_bActMark = true;
	for (int i = 0; i < (int)(m_pCurOptTracks->size()); i++)
		pNewSegTrack->m_pTracks->push_back((*m_pCurOptTracks)[i]);

	pNewSegTrack->Update();
	
	//Add it to the final optimized tracklet set.
	m_pOptSegTracks->push_back(pNewSegTrack);

	return true;
}


bool CSegmentGraph::GetCurSegTracks_TYP2()
{
	//Get the small tracklets according to the clustering results.
	//Overlap items.
	IntVector vOverlap;
	IntVector vHighDiff;
	IntVector vLowDiff;
	IntVector vDiff;

	//The ignored tracklet set.
	IntVector vIgnored;
	vIgnored.clear();
	for (int i = 0; i < (int)(m_pCurInitTracks->size()); i++)
		vIgnored.push_back(i);

	CTrack* pTrack1 = 0;
	CTrack* pTrack2 = 0;

	IntVector::iterator vIter;

	//Conflict mark.
	bool bConf = false;
	bool fBreak = false;

	//Copy of the supports.
	IntArray vSupports;
	//Copy the support variable to the copy.
	for (int i = 0; i < (int)(m_pOptSupports->size()); i++) {
		IntVector vNew;
		for (int j = 0; j < (int)((*m_pOptSupports)[i].size()); j++)
			vNew.push_back((*m_pOptSupports)[i][j]);
		vSupports.push_back(vNew);
	}
	//Copy of the function value.
	IntVector vIndexSet;
	DoubleVector vFValueSet;
	vIndexSet.clear();
	for (int i = 0; i < (int)(m_pOptSupports->size()); i++)
		vIndexSet.push_back(i);

	vFValueSet.clear();
	for (int i = 0; i < (int)(m_pOptValue->size()); i++)
		vFValueSet.push_back((*m_pOptValue)[i]);

	//Sort the order according to the function value.
	SortDescend(&vFValueSet, &vIndexSet);

	int cIndex = 0;
	//Add the conflicts handle between different clusters.(Important)
	for (int i = 0; i < (int)(vIndexSet.size()); i++) {
		//Check whether it has overlap components with the existing clusters.
		//Jump over the null set
		if (vSupports[vIndexSet[i]].empty())
			continue;

		cIndex = i + 1;
		//While iteration, when the set i updated, just check from j=i+1;
		while (1) {
			if (cIndex >= (int)(vIndexSet.size()))
				break;

			if (vSupports[vIndexSet[cIndex]].empty()) {
				cIndex++;
				continue;
			}

			vOverlap.clear();
			vHighDiff.clear();
			vLowDiff.clear();
			sort(vSupports[vIndexSet[i]].begin(), vSupports[vIndexSet[i]].end());
			sort(vSupports[vIndexSet[cIndex]].begin(), vSupports[vIndexSet[cIndex]].end());
			set_intersection(vSupports[vIndexSet[i]].begin(), vSupports[vIndexSet[i]].end(), 
				vSupports[vIndexSet[cIndex]].begin(), vSupports[vIndexSet[cIndex]].end(), back_inserter(vOverlap));
			set_difference(vSupports[vIndexSet[i]].begin(), vSupports[vIndexSet[i]].end(), 
				vSupports[vIndexSet[cIndex]].begin(), vSupports[vIndexSet[cIndex]].end(), back_inserter(vHighDiff));
			set_difference(vSupports[vIndexSet[cIndex]].begin(), vSupports[vIndexSet[cIndex]].end(), 
				vSupports[vIndexSet[i]].begin(), vSupports[vIndexSet[i]].end(), back_inserter(vLowDiff));

			sort(vOverlap.begin(), vOverlap.end());
			sort(vHighDiff.begin(), vHighDiff.end());
			sort(vLowDiff.begin(), vLowDiff.end());
			//Check whether these two tracklet conflicts with each other. vHighDiff and vLowDiff conflict.

			if (!(vOverlap.empty())) {
				bConf = false;
				fBreak = false;
				for (int k = 0; k < (int)(vHighDiff.size()); k++) {
					for (int m = 0; m < (int)(vLowDiff.size()); m++) {
						pTrack1 = (*m_pCurInitTracks)[vHighDiff[k]];
						pTrack2 = (*m_pCurInitTracks)[vLowDiff[m]];
						fBreak = CheckTrackOverlap(pTrack1, pTrack2);
						if (fBreak) {
							bConf = true;
							break;
						}
					}
					if (bConf)
						break;
				}

				//Update the tracklet i.
				if (bConf) {
					//Clear the overlap area of the tracklet in j.
					for (int k = 0; k < (int)(vOverlap.size()); k++) {
						for (vIter = vSupports[vIndexSet[cIndex]].begin(); vIter != vSupports[vIndexSet[cIndex]].end(); vIter++) {
							if (vOverlap[k] == *vIter) {
								vSupports[vIndexSet[cIndex]].erase(vIter);
								break;
							}
						}
					}
				}
				else {
					//These two tracklets are not conflict with each other and merge them.
					vSupports[vIndexSet[i]].clear();
					for (int k = 0; k < (int)(vOverlap.size()); k++)
						vSupports[vIndexSet[i]].push_back(vOverlap[k]);
					for (int k = 0; k < (int)(vHighDiff.size()); k++)
						vSupports[vIndexSet[i]].push_back(vHighDiff[k]);
					for (int k = 0; k < (int)(vLowDiff.size()); k++)
						vSupports[vIndexSet[i]].push_back(vLowDiff[k]);

					//Clear the jth tracklet.
					vSupports[vIndexSet[cIndex]].clear();
					cIndex = i + 1;
				}
			}
			else {
				cIndex++;
			}
		}

		//Modified the ignored set.
		vDiff.clear();
		sort(vSupports[vIndexSet[i]].begin(), vSupports[vIndexSet[i]].end());
		sort(vIgnored.begin(), vIgnored.end());
		set_difference(vIgnored.begin(), vIgnored.end(), vSupports[vIndexSet[i]].begin(), vSupports[vIndexSet[i]].end(), back_inserter(vDiff));
		//Clear the ignored set
		vIgnored.clear();
		//Add the difference components.
		for (int j = 0; j < (int)(vDiff.size()); j++)
			vIgnored.push_back(vDiff[j]);
	}

	m_pOptClusters->clear();
	//Second move the items to the cluster variable.
	for (int i = 0; i < (int)(vSupports.size()); i++) {
		if (vSupports[i].empty())
			continue;
		IntVector vNew;
		for (int j = 0; j < (int)(vSupports[i].size()); j++)
			vNew.push_back(vSupports[i][j]);
		m_pOptClusters->push_back(vNew);
	}

	//***********************************************************************************************************//
	//Get the trajectories in space.
	//**********************************************************************************************************//
	//*** Release the previous memory of the tracklets.
	m_pCurOptTracks->clear();

	//Construct the tracklets according to the clustering results.
	//Temporal detection box variable.
	CDetBBox* pTempBox = 0;
	//Just a pointer.
	int nCount = 0;
	for (int i = 0; i < (int)(m_pOptClusters->size()); i++) {
		CTrack* pNewTrack = new CTrack;
		pNewTrack->m_nSegIndex = m_nCurSegIndex;
		pNewTrack->m_nVirtIndex = nCount;

		for (int j = 0; j < (int)((*m_pOptClusters)[i].size()); j++)
			pNewTrack->Add((*m_pCurInitTracks)[(*m_pOptClusters)[i][j]]);

		//Check whether the tracklet is null or not
		if ((int)(pNewTrack->m_pDetPoints->size()) > m_pOptParas->m_nMinTrackLen) {
			pNewTrack->m_bActMark = true;
			pNewTrack->m_nTrackID = nCount;
			//Set the virtual index of all boxes in the tracklet.
			for (int j = 0; j < (int)(pNewTrack->m_pDetPoints->size()); j++) {
				pTempBox = (*(pNewTrack->m_pDetPoints))[j];
				pTempBox->m_nVirtIndex = nCount;
			}
			m_pCurOptTracks->push_back(pNewTrack);
			nCount++;
		}
		else {
			if (pNewTrack) {
				RELEASE(pNewTrack);
				pNewTrack = 0;
			}
		}
	}

	//Modified the trajectories.
	for (int i = 0; i < (int)(m_pCurOptTracks->size()); i++) {
		(*m_pCurOptTracks)[i]->SplineAdjust();
		(*m_pCurOptTracks)[i]->SplineFitting();
		(*m_pCurOptTracks)[i]->GetAppearance();
	}

	//Check whether ignored set have conflicts with the clustering results, if not, add it, otherwise, ignored.
	IntVector vAdd;
	vAdd.clear();
	bConf = false;
	if (!(vIgnored.empty())) {
		for (int i = 0; i < (int)(vIgnored.size()); i++) {
			pTrack1 = (*m_pCurInitTracks)[vIgnored[i]];
			bConf = false;
			for (int j = 0; j < (int)(m_pCurOptTracks->size()); j++) {
				pTrack2 = (*m_pCurOptTracks)[j];
				bConf = CheckConflict(pTrack1, pTrack2, m_pOptParas->m_nOccConflictThresh);
				if (bConf)
					break;
			}
			//Add the ignored tracklet
			if (!bConf)
				vAdd.push_back(vIgnored[i]);
		}
	}

	if (!vAdd.empty()) {
		nCount = (int)(m_pCurOptTracks->size());
		for (int i = 0; i < (int)(vAdd.size()); i++) {
			CTrack* pNewTrack = new CTrack((*m_pCurInitTracks)[vAdd[i]]);
			pNewTrack->m_nSegIndex = m_nCurSegIndex;
			if ((int)(pNewTrack->m_pDetPoints->size()) > m_pOptParas->m_nMinTrackLen) {
				pNewTrack->m_bActMark = true;
				pNewTrack->m_nTrackID = nCount;
				pNewTrack->SplineAdjust();
				pNewTrack->SplineFitting();
				pNewTrack->GetAppearance();
				//Set the virtual index of all boxes in the tracklet.
				for (int j = 0; j < (int)(pNewTrack->m_pDetPoints->size()); j++) {
					pTempBox = (*(pNewTrack->m_pDetPoints))[j];
					pTempBox->m_nVirtIndex = nCount;
				}
				m_pCurOptTracks->push_back(pNewTrack);
				nCount++;
			}
			else {
				if (pNewTrack) {
					RELEASE(pNewTrack);
					pNewTrack = 0;
				}
			}
		}
	}

	//-------------------------------------------- Save the debug informations ---------------------------------------------------//
	//m_pExpoter->ExportSegTrackImages_Debug(m_pCurOptTracks, m_pOptParas->m_nCurLayer);

	//-------------------------------------------------------------------------------------------------------------------//
	//------- Put the Optimized Tracklets into a Segment Tracklet Variable.
	//Collect the tracklets in the current window.
	//Initialize a new segment tracks.
	CSegTracks* pNewSegTrack = new CSegTracks;
	pNewSegTrack->m_nSegIndex = m_nCurSegIndex;
	pNewSegTrack->m_bActMark = true;
	for (int i = 0; i < (int)(m_pCurOptTracks->size()); i++)
		pNewSegTrack->m_pTracks->push_back((*m_pCurOptTracks)[i]);

	pNewSegTrack->Update();

	//Add it to the final optimized tracklet set.
	m_pOptSegTracks->push_back(pNewSegTrack);

	return true;
}


bool CSegmentGraph::Get2Ary(CMask* pNeighMarks, IntArray* p2Ary)
{
	//Check the parameters.
	if (pNeighMarks->m_nWidth != pNeighMarks->m_nHeight) {
		fprintf(stderr, "Parameters Error in Function Get2Ary!\n");
		return false;
	}
	//First clear the previous memory.
	p2Ary->clear();
	for (int v1 = 0; v1 < pNeighMarks->m_nWidth - 1; v1++) {
		for (int v2 = v1 + 1; v2 < pNeighMarks->m_nWidth; v2++) {
			if (pNeighMarks->m_pMask[v1][v2]) {
				IntVector vAry;
				vAry.push_back(v1);
				vAry.push_back(v2);
				sort(vAry.begin(), vAry.end());
				p2Ary->push_back(vAry);
			}
		}
	}
	return true;
}

bool CSegmentGraph::Get3Ary(CMask* pNeighMarks, IntArray* p3Ary)
{
	//Check the parameters.
	if (pNeighMarks->m_nWidth != pNeighMarks->m_nHeight) {
		fprintf(stderr, "Parameters Error in Function Get3Ary!\n");
		return false;
	}
	//First clear the previous memory.
	p3Ary->clear();
	for (int v1 = 0; v1 < pNeighMarks->m_nWidth - 2; v1++) {
		for (int v2 = v1 + 1; v2 < pNeighMarks->m_nWidth - 1; v2++) {
			for (int v3 = v2 + 1; v3 < pNeighMarks->m_nWidth; v3++) {
				if (pNeighMarks->m_pMask[v1][v2]&&pNeighMarks->m_pMask[v1][v3]&&pNeighMarks->m_pMask[v2][v3]) {
					IntVector vAry;
					vAry.push_back(v1);
					vAry.push_back(v2);
					vAry.push_back(v3);
					sort(vAry.begin(), vAry.end());
					p3Ary->push_back(vAry);
				}
			}
		}
	}
	return true;
}

bool CSegmentGraph::Get4Ary(CMask* pNeighMarks, IntArray* p4Ary)
{
	//Check the parameters.
	if (pNeighMarks->m_nWidth != pNeighMarks->m_nHeight) {
		fprintf(stderr, "Parameters Error in Function Get4Ary!\n");
		return false;
	}
	//First clear the previous memory.
	p4Ary->clear();
	for (int v1 = 0; v1 < pNeighMarks->m_nWidth - 3; v1++) {
		for (int v2 = v1 + 1; v2 < pNeighMarks->m_nWidth - 2; v2++) {
			for (int v3 = v2 + 1; v3 < pNeighMarks->m_nWidth - 1; v3++) {
				for (int v4 = v3 + 1; v4 < pNeighMarks->m_nWidth; v4++) {
					if (pNeighMarks->m_pMask[v1][v2]&&pNeighMarks->m_pMask[v1][v3]&&pNeighMarks->m_pMask[v1][v4]&&
						pNeighMarks->m_pMask[v2][v3]&&pNeighMarks->m_pMask[v2][v4]&&pNeighMarks->m_pMask[v3][v4]) {
							IntVector vAry;
							vAry.push_back(v1);
							vAry.push_back(v2);
							vAry.push_back(v3);
							vAry.push_back(v4);
							sort(vAry.begin(), vAry.end());
							p4Ary->push_back(vAry);
					}
				}
			}
		}
	}
	return true;
}

bool CSegmentGraph::Get5Ary(CMask* pNeighMarks, IntArray* p5Ary)
{
	//Check the parameters.
	if (pNeighMarks->m_nWidth != pNeighMarks->m_nHeight) {
		fprintf(stderr, "Parameters Error in Function Get5Ary!\n");
		return false;
	}
	//First clear the previous memory.
	p5Ary->clear();
	for (int v1 = 0; v1 < pNeighMarks->m_nWidth - 4; v1++) {
		for (int v2 = v1 + 1; v2 < pNeighMarks->m_nWidth - 3; v2++) {
			for (int v3 = v2 + 1; v3 < pNeighMarks->m_nWidth - 2; v3++) {
				for (int v4 = v3 + 1; v4 < pNeighMarks->m_nWidth - 1; v4++) {
					for (int v5 = v4 + 1; v5 < pNeighMarks->m_nWidth; v5++) {
						if (pNeighMarks->m_pMask[v1][v2]&&pNeighMarks->m_pMask[v1][v3]&&pNeighMarks->m_pMask[v1][v4]&&pNeighMarks->m_pMask[v1][v5]&&
							pNeighMarks->m_pMask[v2][v3]&&pNeighMarks->m_pMask[v2][v4]&&pNeighMarks->m_pMask[v2][v5]&&pNeighMarks->m_pMask[v3][v4]&&
							pNeighMarks->m_pMask[v3][v5]&&pNeighMarks->m_pMask[v4][v5]) {
								IntVector vAry;
								vAry.push_back(v1);
								vAry.push_back(v2);
								vAry.push_back(v3);
								vAry.push_back(v4);
								vAry.push_back(v5);
								sort(vAry.begin(), vAry.end());
								p5Ary->push_back(vAry);
						}
					}
				}
			}
		}
	}
	return true;
}

bool CSegmentGraph::Get6Ary(CMask* pNeighMarks, IntArray* p6Ary)
{
	//Check the parameters.
	if (pNeighMarks->m_nWidth != pNeighMarks->m_nHeight) {
		fprintf(stderr, "Parameters Error in Function Get6Ary!\n");
		return false;
	}
	//First clear the previous memory.
	p6Ary->clear();
	for (int v1 = 0; v1 < pNeighMarks->m_nWidth - 5; v1++) {
		for (int v2 = v1 + 1; v2 < pNeighMarks->m_nWidth - 4; v2++) {
			for (int v3 = v2 + 1; v3 < pNeighMarks->m_nWidth - 3; v3++) {
				for (int v4 = v3 + 1; v4 < pNeighMarks->m_nWidth - 2; v4++) {
					for (int v5 = v4 + 1; v5 < pNeighMarks->m_nWidth - 1; v5++) {
						for (int v6 = v5 + 1; v6 < pNeighMarks->m_nWidth; v6++) {
							if (pNeighMarks->m_pMask[v1][v2]&&pNeighMarks->m_pMask[v1][v3]&&pNeighMarks->m_pMask[v1][v4]&&pNeighMarks->m_pMask[v1][v5]&&
								pNeighMarks->m_pMask[v1][v6]&&pNeighMarks->m_pMask[v2][v3]&&pNeighMarks->m_pMask[v2][v4]&&pNeighMarks->m_pMask[v2][v5]&&
								pNeighMarks->m_pMask[v2][v6]&&pNeighMarks->m_pMask[v3][v4]&&pNeighMarks->m_pMask[v3][v5]&&pNeighMarks->m_pMask[v3][v6]&&
								pNeighMarks->m_pMask[v4][v5]&&pNeighMarks->m_pMask[v4][v6]&&pNeighMarks->m_pMask[v5][v6]) {
									IntVector vAry;
									vAry.push_back(v1);
									vAry.push_back(v2);
									vAry.push_back(v3);
									vAry.push_back(v4);
									vAry.push_back(v5);
									vAry.push_back(v6);
									sort(vAry.begin(), vAry.end());
									p6Ary->push_back(vAry);
							}
						}
					}
				}
			}
		}
	}

	return true;
}

bool CSegmentGraph::Get7Ary(CMask* pNeighMarks, IntArray* p7Ary)
{
	//Check the parameters.
	if (pNeighMarks->m_nWidth != pNeighMarks->m_nHeight) {
		fprintf(stderr, "Parameters Error in Function Get7Ary!\n");
		return false;
	}
	//First clear the previous memory.
	p7Ary->clear();
	for (int v1 = 0; v1 < pNeighMarks->m_nWidth - 6; v1++) {
		for (int v2 = v1 + 1; v2 < pNeighMarks->m_nWidth - 5; v2++) {
			for (int v3 = v2 + 1; v3 < pNeighMarks->m_nWidth - 4; v3++) {
				for (int v4 = v3 + 1; v4 < pNeighMarks->m_nWidth - 3; v4++) {
					for (int v5 = v4 + 1; v5 < pNeighMarks->m_nWidth - 2; v5++) {
						for (int v6 = v5 + 1; v6 < pNeighMarks->m_nWidth - 1; v6++) {
							for (int v7 = v6 + 1; v7 < pNeighMarks->m_nWidth; v7++) {
								if (pNeighMarks->m_pMask[v1][v2]&&pNeighMarks->m_pMask[v1][v3]&&pNeighMarks->m_pMask[v1][v4]&&pNeighMarks->m_pMask[v1][v5]&&
									pNeighMarks->m_pMask[v1][v6]&&pNeighMarks->m_pMask[v1][v7]&&pNeighMarks->m_pMask[v2][v3]&&pNeighMarks->m_pMask[v2][v4]&&
									pNeighMarks->m_pMask[v2][v5]&&pNeighMarks->m_pMask[v2][v6]&&pNeighMarks->m_pMask[v2][v7]&&pNeighMarks->m_pMask[v3][v4]&&
									pNeighMarks->m_pMask[v3][v5]&&pNeighMarks->m_pMask[v3][v6]&&pNeighMarks->m_pMask[v3][v7]&&pNeighMarks->m_pMask[v4][v5]&&
									pNeighMarks->m_pMask[v4][v6]&&pNeighMarks->m_pMask[v4][v7]&&pNeighMarks->m_pMask[v5][v6]&&pNeighMarks->m_pMask[v5][v7]&&
									pNeighMarks->m_pMask[v6][v7]) {
										IntVector vAry;
										vAry.push_back(v1);
										vAry.push_back(v2);
										vAry.push_back(v3);
										vAry.push_back(v4);
										vAry.push_back(v5);
										vAry.push_back(v6);
										vAry.push_back(v7);
										sort(vAry.begin(), vAry.end());
										p7Ary->push_back(vAry);
								}
							}
						}
					}
				}
			}
		}
	}
	return true;
}


bool CSegmentGraph::Get8Ary(CMask* pNeighMarks, IntArray* p8Ary)
{
	//Check the parameters.
	if (pNeighMarks->m_nWidth != pNeighMarks->m_nHeight) {
		fprintf(stderr, "Parameters Error in Function Get8Ary!\n");
		return false;
	}
	//First clear the previous memory.
	p8Ary->clear();
	for (int v1 = 0; v1 < pNeighMarks->m_nWidth - 7; v1++) {
		for (int v2 = v1 + 1; v2 < pNeighMarks->m_nWidth - 6; v2++) {
			for (int v3 = v2 + 1; v3 < pNeighMarks->m_nWidth - 5; v3++) {
				for (int v4 = v3 + 1; v4 < pNeighMarks->m_nWidth - 4; v4++) {
					for (int v5 = v4 + 1; v5 < pNeighMarks->m_nWidth - 3; v5++) {
						for (int v6 = v5 + 1; v6 < pNeighMarks->m_nWidth - 2; v6++) {
							for (int v7 = v6 + 1; v7 < pNeighMarks->m_nWidth - 1; v7++) {
								for (int v8 = v7 + 1; v8 < pNeighMarks->m_nWidth; v8++) {
									if (pNeighMarks->m_pMask[v1][v2]&&pNeighMarks->m_pMask[v1][v3]&&pNeighMarks->m_pMask[v1][v4]&&pNeighMarks->m_pMask[v1][v5]&&
										pNeighMarks->m_pMask[v1][v6]&&pNeighMarks->m_pMask[v1][v7]&&pNeighMarks->m_pMask[v1][v8]&&pNeighMarks->m_pMask[v2][v3]&&
										pNeighMarks->m_pMask[v2][v4]&&pNeighMarks->m_pMask[v2][v5]&&pNeighMarks->m_pMask[v2][v6]&&pNeighMarks->m_pMask[v2][v7]&&
										pNeighMarks->m_pMask[v2][v8]&&pNeighMarks->m_pMask[v3][v4]&&pNeighMarks->m_pMask[v3][v5]&&pNeighMarks->m_pMask[v3][v6]&&
										pNeighMarks->m_pMask[v3][v7]&&pNeighMarks->m_pMask[v3][v8]&&pNeighMarks->m_pMask[v4][v5]&&pNeighMarks->m_pMask[v4][v6]&&
										pNeighMarks->m_pMask[v4][v7]&&pNeighMarks->m_pMask[v4][v8]&&pNeighMarks->m_pMask[v5][v6]&&pNeighMarks->m_pMask[v5][v7]&&
										pNeighMarks->m_pMask[v5][v8]&&pNeighMarks->m_pMask[v6][v7]&&pNeighMarks->m_pMask[v6][v8]&&pNeighMarks->m_pMask[v7][v8]) {
											IntVector vAry;
											vAry.push_back(v1);
											vAry.push_back(v2);
											vAry.push_back(v3);
											vAry.push_back(v4);
											vAry.push_back(v5);
											vAry.push_back(v6);
											vAry.push_back(v7);
											vAry.push_back(v8);
											sort(vAry.begin(), vAry.end());
											p8Ary->push_back(vAry);
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return true;
}

bool CSegmentGraph::Get9Ary(CMask* pNeighMarks, IntArray* p9Ary)
{
	//Check the parameters.
	if (pNeighMarks->m_nWidth != pNeighMarks->m_nHeight) {
		fprintf(stderr, "Parameters Error in Function Get9Ary!\n");
		return false;
	}
	//First clear the previous memory.
	p9Ary->clear();
	for (int v1 = 0; v1 < pNeighMarks->m_nWidth - 8; v1++) {
		for (int v2 = v1 + 1; v2 < pNeighMarks->m_nWidth - 7; v2++) {
			for (int v3 = v2 + 1; v3 < pNeighMarks->m_nWidth - 6; v3++) {
				for (int v4 = v3 + 1; v4 < pNeighMarks->m_nWidth - 5; v4++) {
					for (int v5 = v4 + 1; v5 < pNeighMarks->m_nWidth - 4; v5++) {
						for (int v6 = v5 + 1; v6 < pNeighMarks->m_nWidth - 3; v6++) {
							for (int v7 = v6 + 1; v7 < pNeighMarks->m_nWidth - 2; v7++) {
								for (int v8 = v7 + 1; v8 < pNeighMarks->m_nWidth - 1; v8++) {
									for (int v9 = v8 + 1; v9 < pNeighMarks->m_nWidth; v9++) {
										if (pNeighMarks->m_pMask[v1][v2]&&pNeighMarks->m_pMask[v1][v3]&&pNeighMarks->m_pMask[v1][v4]&&pNeighMarks->m_pMask[v1][v5]&&
											pNeighMarks->m_pMask[v1][v6]&&pNeighMarks->m_pMask[v1][v7]&&pNeighMarks->m_pMask[v1][v8]&&pNeighMarks->m_pMask[v1][v9]&&
											pNeighMarks->m_pMask[v2][v3]&&pNeighMarks->m_pMask[v2][v4]&&pNeighMarks->m_pMask[v2][v5]&&pNeighMarks->m_pMask[v2][v6]&&
											pNeighMarks->m_pMask[v2][v7]&&pNeighMarks->m_pMask[v2][v8]&&pNeighMarks->m_pMask[v2][v9]&&pNeighMarks->m_pMask[v3][v4]&&
											pNeighMarks->m_pMask[v3][v5]&&pNeighMarks->m_pMask[v3][v6]&&pNeighMarks->m_pMask[v3][v7]&&pNeighMarks->m_pMask[v3][v8]&&
											pNeighMarks->m_pMask[v3][v9]&&pNeighMarks->m_pMask[v4][v5]&&pNeighMarks->m_pMask[v4][v6]&&pNeighMarks->m_pMask[v4][v7]&&
											pNeighMarks->m_pMask[v4][v8]&&pNeighMarks->m_pMask[v4][v9]&&pNeighMarks->m_pMask[v5][v6]&&pNeighMarks->m_pMask[v5][v7]&&
											pNeighMarks->m_pMask[v5][v8]&&pNeighMarks->m_pMask[v5][v9]&&pNeighMarks->m_pMask[v6][v7]&&pNeighMarks->m_pMask[v6][v8]&&
											pNeighMarks->m_pMask[v6][v9]&&pNeighMarks->m_pMask[v7][v8]&&pNeighMarks->m_pMask[v7][v9]&&pNeighMarks->m_pMask[v8][v9]) {
												IntVector vAry;
												vAry.push_back(v1);
												vAry.push_back(v2);
												vAry.push_back(v3);
												vAry.push_back(v4);
												vAry.push_back(v5);
												vAry.push_back(v6);
												vAry.push_back(v7);
												vAry.push_back(v8);
												vAry.push_back(v9);
												sort(vAry.begin(), vAry.end());
												p9Ary->push_back(vAry);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return true;
}


bool CSegmentGraph::Get10Ary(CMask* pNeighMarks, IntArray* p10Ary)
{
	//Check the parameters.
	if (pNeighMarks->m_nWidth != pNeighMarks->m_nHeight) {
		fprintf(stderr, "Parameters Error in Function Get10Ary!\n");
		return false;
	}
	//First clear the previous memory.
	p10Ary->clear();
	for (int v1 = 0; v1 < pNeighMarks->m_nWidth - 9; v1++) {
		for (int v2 = v1 + 1; v2 < pNeighMarks->m_nWidth - 8; v2++) {
			for (int v3 = v2 + 1; v3 < pNeighMarks->m_nWidth - 7; v3++) {
				for (int v4 = v3 + 1; v4 < pNeighMarks->m_nWidth - 6; v4++) {
					for (int v5 = v4 + 1; v5 < pNeighMarks->m_nWidth - 5; v5++) {
						for (int v6 = v5 + 1; v6 < pNeighMarks->m_nWidth - 4; v6++) {
							for (int v7 = v6 + 1; v7 < pNeighMarks->m_nWidth - 3; v7++) {
								for (int v8 = v7 + 1; v8 < pNeighMarks->m_nWidth - 2; v8++) {
									for (int v9 = v8 + 1; v9 < pNeighMarks->m_nWidth - 1; v9++) {
										for (int v10 = v9 + 1; v10 < pNeighMarks->m_nWidth; v10++) {
											if (pNeighMarks->m_pMask[v1][v2]&&pNeighMarks->m_pMask[v1][v3]&&pNeighMarks->m_pMask[v1][v4]&&pNeighMarks->m_pMask[v1][v5]&&
												pNeighMarks->m_pMask[v1][v6]&&pNeighMarks->m_pMask[v1][v7]&&pNeighMarks->m_pMask[v1][v8]&&pNeighMarks->m_pMask[v1][v9]&&
												pNeighMarks->m_pMask[v1][v10]&&pNeighMarks->m_pMask[v2][v3]&&pNeighMarks->m_pMask[v2][v4]&&pNeighMarks->m_pMask[v2][v5]&&
												pNeighMarks->m_pMask[v2][v6]&&pNeighMarks->m_pMask[v2][v7]&&pNeighMarks->m_pMask[v2][v8]&&pNeighMarks->m_pMask[v2][v9]&&
												pNeighMarks->m_pMask[v2][v10]&&pNeighMarks->m_pMask[v3][v4]&&pNeighMarks->m_pMask[v3][v5]&&pNeighMarks->m_pMask[v3][v6]&&
												pNeighMarks->m_pMask[v3][v7]&&pNeighMarks->m_pMask[v3][v8]&&pNeighMarks->m_pMask[v3][v9]&&pNeighMarks->m_pMask[v3][v10]&&
												pNeighMarks->m_pMask[v4][v5]&&pNeighMarks->m_pMask[v4][v6]&&pNeighMarks->m_pMask[v4][v7]&&pNeighMarks->m_pMask[v4][v8]&&
												pNeighMarks->m_pMask[v4][v9]&&pNeighMarks->m_pMask[v4][v10]&&pNeighMarks->m_pMask[v5][v6]&&pNeighMarks->m_pMask[v5][v7]&&
												pNeighMarks->m_pMask[v5][v8]&&pNeighMarks->m_pMask[v5][v9]&&pNeighMarks->m_pMask[v5][v10]&&pNeighMarks->m_pMask[v6][v7]&&
												pNeighMarks->m_pMask[v6][v8]&&pNeighMarks->m_pMask[v6][v9]&&pNeighMarks->m_pMask[v6][v10]&&pNeighMarks->m_pMask[v7][v8]&&
												pNeighMarks->m_pMask[v7][v9]&&pNeighMarks->m_pMask[v7][v10]&&pNeighMarks->m_pMask[v8][v9]&&pNeighMarks->m_pMask[v8][v10]&&
												pNeighMarks->m_pMask[v9][v10]) {
													IntVector vAry;
													vAry.push_back(v1);
													vAry.push_back(v2);
													vAry.push_back(v3);
													vAry.push_back(v4);
													vAry.push_back(v5);
													vAry.push_back(v6);
													vAry.push_back(v7);
													vAry.push_back(v8);
													vAry.push_back(v9);
													vAry.push_back(v10);
													sort(vAry.begin(), vAry.end());
													p10Ary->push_back(vAry);
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return true;
}