/*
 * GraphMultiCarTracker
 * Do not distribute
 *  Created on: Nov 08, 2015
 *      Author: Longyin Wen
 */
#include "hierarch_hypergraph.h"

CHierarchHyperGraph::CHierarchHyperGraph()
{
	m_pSeqFrameMap = new pImageMap;
	m_pLayerOptParas = 0;
	m_nAllOptLayers = 0;

	m_pExpoter = 0;
	m_pSeqFrameSet = 0;

	//The current optimization layer.
	m_nCurOptLayer = 0;
	//Initialize the hierarchical optimization variable.
	m_pSegmentGraphOpt = new CSegmentGraph;

	//Some tracklets variables.
	m_pPreSegTracks = new pSegTrackSet;
	m_pPreSegTracks->clear();

	m_pCurSegTracks = new pSegTrackSet;
	m_pCurSegTracks->clear();

	m_pFinOptTracks = new pTrackSet;
	m_pFinOptTracks->clear();
}

CHierarchHyperGraph::~CHierarchHyperGraph()
{
	m_nAllOptLayers = 0;
	m_nCurOptLayer = 0;

	//The content of this variable is just the pointer.
	if (m_pSeqFrameMap) {
		m_pSeqFrameMap->clear();
		RELEASE(m_pSeqFrameMap);
		m_pSeqFrameMap = 0;
	}

	//Release the clustering optimization variable.
	if (m_pLayerOptParas) {
		RELEASEARRAY(m_pLayerOptParas);
		m_pLayerOptParas = 0;
	}

	//Release the clustering variable.
	if (m_pSegmentGraphOpt) {
		RELEASE(m_pSegmentGraphOpt);
		m_pSegmentGraphOpt = 0;
	}
	
	//Release the tracklets set variables.
    if (m_pPreSegTracks) {
		for (int i = 0; i < (int)(m_pPreSegTracks->size()); i++) {
			RELEASE((*m_pPreSegTracks)[i]);
			(*m_pPreSegTracks)[i] = 0;
		}
		m_pPreSegTracks->clear();
		RELEASE(m_pPreSegTracks);
		m_pPreSegTracks = 0;
    }

	if (m_pCurSegTracks) {
		for (int i = 0; i < (int)(m_pCurSegTracks->size()); i++) {
			RELEASE((*m_pCurSegTracks)[i]);
			(*m_pCurSegTracks)[i] = 0;
		}
		m_pCurSegTracks->clear();
		RELEASE(m_pCurSegTracks);
		m_pCurSegTracks = 0;
	}

	//Release the final optimized tracklets.
	if (m_pFinOptTracks) {
		for (int i = 0; i < (int)(m_pFinOptTracks->size()); i++) {
			RELEASE((*m_pFinOptTracks)[i]);
			(*m_pFinOptTracks)[i] = 0;
		}
		m_pFinOptTracks->clear();
		RELEASE(m_pFinOptTracks);
		m_pFinOptTracks = 0;
	}

	m_pExpoter = 0;

}

bool CHierarchHyperGraph::InitOptimizer(LayerGraphOptPara* pPara, int nLayer, pImageSet* pFrames, CExport* pExport)
{
	//Construct the optimization parameters variable.
	m_pLayerOptParas = new LayerGraphOptPara[nLayer];

	for (int i = 0; i < nLayer; i++)
		m_pLayerOptParas[i].Set(pPara[i]);

	m_nAllOptLayers = nLayer;

	m_pExpoter = pExport;
	m_pSeqFrameSet = pFrames;
	
	m_pSeqFrameMap->clear();
	//Construct the frame map, it is just a pointer, used for searching.
	for (int i = 0; i < (int)(pFrames->size()); i++)
		m_pSeqFrameMap->insert(make_pair((*pFrames)[i]->m_nFrameIndex, (*pFrames)[i]));

	return true;
}


bool CHierarchHyperGraph::HierarchOptimize()
{
	//set the seeds of the random function.
	srand((unsigned)time(NULL));
	//------------------------------------------------- Main Part of the Tracker ------------------------------------------------------//
	//--------------------------------------------------------------------------------------------------------------------------------//
	//---------------------------------------------------------------- Step 1 -------------------------------------------------------//
	m_nCurOptLayer = 0;
	//Initialize the optimization of the first layer according to the detection results.
    //Release the previous values.
	for (int i = 0; i < (int)(m_pPreSegTracks->size()); i++) {
		if ((*m_pPreSegTracks)[i]) {
			RELEASE((*m_pPreSegTracks)[i]);
			(*m_pPreSegTracks)[i] = 0;
		}
	}
	m_pPreSegTracks->clear();

	//Release the current values.
	for (int i = 0; i < (int)(m_pCurSegTracks->size()); i++) {
		if ((*m_pCurSegTracks)[i]) {
			RELEASE((*m_pCurSegTracks)[i]);
			(*m_pCurSegTracks)[i] = 0;
		}
	}
	m_pCurSegTracks->clear();


	CTImage* pFrame = 0;
	CTrack* pTrack = 0;
	CDetBBox* pBox = 0;
	for (int i = 0; i < (int)(m_pSeqFrameSet->size()); i++) {
		pFrame = (*m_pSeqFrameSet)[i];
		CSegTracks* pNewSegTrack = new CSegTracks;
		pNewSegTrack->m_nSegIndex = pFrame->m_nFrameIndex;
		for (int j = 0; j < (int)(pFrame->m_pBoxes->size()); j++) {
			pBox = (*(pFrame->m_pBoxes))[j];
			CTrack* pNewTrack = new CTrack(pBox);
			pNewTrack->m_nSegIndex = pBox->m_nFrameIndex;
            pNewSegTrack->m_pTracks->push_back(pNewTrack);
		}
		pNewSegTrack->Update();
		m_pPreSegTracks->push_back(pNewSegTrack);
	}

	cout<<endl;
	m_nCurOptLayer = 0;
	//The iteration of each layer.
	while (1) {
		//Check the break conditions.
		if (m_nCurOptLayer >= m_nAllOptLayers)
			break;

		cout<<"Begin to Handle Step #"<<m_nCurOptLayer<<",Cluster the Tracklets in Each Window!"<<endl;

		//Get the optimization parameters of the k the layer
		m_pSegmentGraphOpt->GraphOptimize(m_pPreSegTracks, &(m_pLayerOptParas[m_nCurOptLayer]), m_pSeqFrameMap, m_pExpoter, m_pCurSegTracks);

		//Get the current optimized
		//Release the previous tracks and set it as the current one.
		for (int i = 0; i < (int)(m_pPreSegTracks->size()); i++) {
			RELEASE((*m_pPreSegTracks)[i]);
			(*m_pPreSegTracks)[i] = 0;
		}
		m_pPreSegTracks->clear();

		//Set the current values.
		for (int i = 0; i < (int)(m_pCurSegTracks->size()); i++) {
			CSegTracks* pNewTrack = new CSegTracks((*m_pCurSegTracks)[i]);
			m_pPreSegTracks->push_back(pNewTrack);
		}

		cout<<"Finish Step #"<<m_nCurOptLayer<<"!"<<endl;
		m_nCurOptLayer++;
	}

	//*******************************************************************************************//
	//******************************************************************************************//
	//** Warning!! This is the final process to ensure all tracklets are clustered.
	//******* Check whether finish the dense neighborhoods revealing based tracker, 
	//******* Otherwise iterated it with the last layer parameters.
	//Final count variables.
	int nCount = 0;
	//Get the final optimized parameters.
	LayerGraphOptPara* pOptParas = new LayerGraphOptPara(&(m_pLayerOptParas[m_nCurOptLayer - 1]));

	while (1) {
		if ((int)(m_pCurSegTracks->size()) == 1)
			break;
		else if ((int)(m_pCurSegTracks->size()) < 1) {
			fprintf(stderr, "There exists some errors in final processing of function: HierarchOptimize!\n");
			break;
		}

		cout<<"Begin to Handle Final #"<<nCount<<"Processing, Cluster the Detections in Each Window!"<<endl;

		//Get the current layer in optimization.
		pOptParas->m_nCurLayer = nCount + m_nCurOptLayer + 1;

		//Get the optimization parameters of the k the layer
		m_pSegmentGraphOpt->GraphOptimize(m_pPreSegTracks, pOptParas, m_pSeqFrameMap, m_pExpoter, m_pCurSegTracks);

		//Get the current optimized
		//Release the previous tracks and set it as the current one.
		for (int i = 0; i < (int)(m_pPreSegTracks->size()); i++) {
			RELEASE((*m_pPreSegTracks)[i]);
			(*m_pPreSegTracks)[i] = 0;
		}
		m_pPreSegTracks->clear();

		//Set the current values.
		for (int i = 0; i < (int)(m_pCurSegTracks->size()); i++) {
			CSegTracks* pNewTrack = new CSegTracks((*m_pCurSegTracks)[i]);
			m_pPreSegTracks->push_back(pNewTrack);
		}

		cout<<"Finish Final #"<<nCount<<"Processing!"<<endl;
		nCount++;
	}

	//Get the optimized tracklets.
  printf ("m_pCurSegTracks->size() = %d\n", m_pCurSegTracks->size());
	CSegTracks* pOptSegTracks = (*m_pCurSegTracks)[0];
	
	for (int i = 0; i < (int)(pOptSegTracks->m_pTracks->size()); i++) {
		CTrack* pNewTrack = new CTrack((*(pOptSegTracks->m_pTracks))[i]);
        pNewTrack->SplineAdjust();
        pNewTrack->SplineFitting();
		m_pFinOptTracks->push_back(pNewTrack);
	}

	//Release the optimization parameters.
	if (pOptParas) {
		RELEASE(pOptParas);
		pOptParas = 0;
	}

	return true;
};

