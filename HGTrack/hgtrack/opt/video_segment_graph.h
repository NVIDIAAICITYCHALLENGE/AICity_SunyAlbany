/*
 * GraphMultiCarTracker
 * Do not distribute
 *  Created on: Nov 08, 2015
 *      Author: Longyin Wen
 */
#ifndef _SEGMENTGRAPH_H_
#define _SEGMENTGRAPH_H_
/*
#include "../Defs.h"
#include "GraphDecompose.h"
//Clique searching class.
#include "../Clique/cliquer.h"
#include "../Types/Tracklet.h"
#include "../Feature/FeatFuns.h"
#include "../Math/Algebra.h"
#include "../Export/Export.h"
#include "../Types/TFrame.h"
#include "../Configuration/Parameter.h"
*/
#include <util/defs.h>
#include <graph_decomp/graph_decomp.h>
#include <clique/cliquer.h>
#include <util/tracklet.h>
#include <util/tframe.h>
#include <feat/feat_funs.h>
#include <math/algebra.h>
#include <config/param.h>
#include <export/export.h>


class CSegmentGraph
{
public:
	CSegmentGraph();
	virtual ~CSegmentGraph();
	//-------------------- Variables -----------------------------------------//
public:

	//------------------------------------------------------------------------//
	//Current processed index.
	int m_nCurSegIndex;

	//Just a pointer of the frame map.
	pImageMap* m_pSeqFrameMap;

	//----------------------------------------------------------------------------------//
	//------- Tracklets in 2D image plane.
    //The initial tracklets for each segment.
	pSegTrackSet* m_pProSegTracks;
	//The optimized tracklets after clustering.
	pSegTrackSet* m_pOptSegTracks;

	//The optimized tracklet set in the current segment
	CSegTracks* m_pCurSegTrack;

	//********If the input is the tracklets, active the tracklet variable
	//The inner contents are just a pointer
	pTrackSet* m_pCurInitTracks;
	//********The output of the tracklets after clustering optimization.
	pTrackSet* m_pCurOptTracks;

	//Number of segments to be processed.
	int m_nNumSegs;
	//Begin segment index of current optimization.
	int m_nBegSegIdx;
	//End segment index of current optimization.
	int m_nEndSegIdx;

	//Clustering class.
	CAffinityGraph* m_pGraphDecOpt;

	//Appearance similarity array of the detections.
	DoubleArray* m_pNodeAppSims;

	//It is just a pointer.
	LayerGraphOptPara* m_pOptParas;

	//Used for graph decomposition (edge weight).
	IVectorDVector m_vEdgeWeights;

	//Used for graph decomposition (node weight).
	IDVector m_vNodeWeights;

	//Input Edge Index.
	IntArray m_vEdgeManIdx;

	
	//-----------------------------------------------------------------------------//
	//----- Parameters for the candidate K-Ary of the tracklets.
	IntArray* m_pCandKAry;

	IntArray* m_pOptSupports;
	DoubleArray* m_pOptProb;
	DoubleVector* m_pOptValue;

	//Cluster results after post-process
	IntArray* m_pOptClusters;

	spline1dinterpolant* m_pSplines;
	ae_int_t m_nBoundType;

	
	//--------------------------------------------------------------------------------------//
	//********** Used for Confidence of Hypergraph or Graph Edges.
	//Affinity Value of the Edge Appearance Aspects of Pairwise Edge.
	IVectorDVector* m_pAppAffinity;
	//Affinity Value of the Edge Move Aspects of Pairwise Edge.
	IVectorDVector* m_pMoveAffinity;
	//Affinity Value of the Edge Smooth Aspects of Pairwise Edge.
	IVectorDVector* m_pSmthAffinity;
	

	//Distance matrix in 2D space used to get the neighbor system marks matrix.
	CMatrix* m_p2DDistance;
	
	//Neighbor system marks of the tracklets or detections.
	//There exists mainly two neighboring system, one is for pairwise edge generation, and the other one is for MSS generation. 
	//This neighborhood is the general neighborhoods
	CMask* m_pNodeNeighMarks;

	//Parameters of the clustering.
	DecomposePara* m_pClusterParas;
	
	//The export variable, mainly used for debug.
	CExport* m_pExpoter;

	//--------------------- Functions -----------------------------------------//
public:
	//Set the parameters of the clustering for the CVPR algorithm.
	bool SetClusterPara_Exhaust();

    //Main optimization function of the whole segment.
	bool GraphOptimize(pSegTrackSet* pInit, LayerGraphOptPara* pPara, pImageMap* pFrame, CExport* pExport, pSegTrackSet* pOpt);
	//Get the clustering results.
	bool GetClusterTracks();

	//***************************************************
	//** Two types to get the current optimized tracklets.
	bool GetCurSegTracks_TYP1();
	bool GetCurSegTracks_TYP2();

	//***************************************************
	//Get the neighboring systems of the tracklets.
	bool GetTrackNeighbors();
	//Get the similarity between different vertex, used to reduce the computation complexity.
	bool GetTrackAppSims();

	//Get the parameters according to the level
	//Used for Tracklets Deviation. (The detections can be regarded as the specific tracklets)
	double GetMoveDevs(pTrackSet* pTracks);
	//Used for Tracklets Deviation. (The detections can be regarded as the specific tracklets)
	double GetSmthDevs(pTrackSet* pTracks);

	//Calculate the occlusion reasoning term between two tracklets.
	bool CheckOccReasons(CTrack* pTk1, CTrack* pTk2);

public:
	//-------------------------------------------------------------------------------------------------------------------------//
	//------------------------------------------------------------------------------------------------------------------------//
	//***** CVPR2014 algorithm: directly decompose the hypergraph.
	//Handle the General Graph Dense Neighborhoods Searching Problem
	//Generate the tracklet k-ary.
	bool GetTrackKAry();
	//Search the dense neighborhoods on the graph/hyerpgraph using exhaust strategy
	bool GraphDecompose_ExhaustOpt();

	
	//-------------------------------------------------------------------------------------------------------------------------//
	//------------------------------------------------------------------------------------------------------------------------//
	//--------------------------------------------------- Some Help Functions --------------------------------------------------//
	//**************************************************************************************************************************//
	//Get candidate 2-Ary.
	bool Get2Ary(CMask* pNeighMarks, IntArray* p2Ary);
	//Get candidate 3-Ary.
	bool Get3Ary(CMask* pNeighMarks, IntArray* p3Ary);
	//Get candidate 4-Ary.
	bool Get4Ary(CMask* pNeighMarks, IntArray* p4Ary);
	//Get candidate 5-Ary.
	bool Get5Ary(CMask* pNeighMarks, IntArray* p5Ary);
	//Get candidate 6-Ary.
	bool Get6Ary(CMask* pNeighMarks, IntArray* p6Ary);
	//Get candidate 7-Ary.
	bool Get7Ary(CMask* pNeighMarks, IntArray* p7Ary);
	//Get candidate 8-Ary.
	bool Get8Ary(CMask* pNeighMarks, IntArray* p8Ary);
	//Get candidate 9-Ary.
	bool Get9Ary(CMask* pNeighMarks, IntArray* p9Ary);
	//Get candidate 10-Ary.
	bool Get10Ary(CMask* pNeighMarks, IntArray* p10Ary);
};


#endif