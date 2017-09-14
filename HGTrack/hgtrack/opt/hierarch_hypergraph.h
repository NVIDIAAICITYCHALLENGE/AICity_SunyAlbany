/*
 * GraphMultiCarTracker
 * Do not distribute
 *  Created on: Nov 08, 2015
 *      Author: Longyin Wen
 */
#ifndef _HIERARCHHYPERGRAPH_H
#define _HIERARCHHYPERGRAPH_H

#include <util/defs.h>
#include "video_segment_graph.h"


class CHierarchHyperGraph
{
public:
	CHierarchHyperGraph();
	virtual ~CHierarchHyperGraph();

	//------------------------------------------------------------- Functions --------------------------------------------------------------//
	//------------------------------------------------ Get the Similarity Between the K-ary ------------------------------------------------//
	//Utilized to construct the average affinity matrix.
public:
	//------------------------------------------------------------- Main Function ---------------------------------------------------------//
	//Initialize the hierarchical cluster optimization part
	bool InitOptimizer(LayerGraphOptPara* pPara, int nLayer, pImageSet* pFrames, CExport* pExport);

	//Hierarchical optimization to generate the tracklets.
	bool HierarchOptimize();

	//Get final trajectories.
	pTrackSet* GetOptTracks() {
		return m_pFinOptTracks;
	}
  
public:
	pImageMap* m_pSeqFrameMap;
	pImageSet* m_pSeqFrameSet;

	//Number of layers
	int m_nAllOptLayers;

	//Main optimization variable.
	CSegmentGraph* m_pSegmentGraphOpt;

	//Layer parameters.
	LayerGraphOptPara* m_pLayerOptParas;

	//The tracklets in previous optimization iteration.
	pSegTrackSet* m_pPreSegTracks;
	//The tracklets in current optimization iteration.
	pSegTrackSet* m_pCurSegTracks;

	//The optimized tracklet set.
	pTrackSet* m_pFinOptTracks;

	//The current optimization layer.
	int m_nCurOptLayer;
	
	//Tracking Results Handling
	CExport* m_pExpoter;

};

#endif