// The standard method of macro

#ifndef _GRAPHDECOMPOSE_H
#define _GRAPHDECOMPOSE_H

//MCCTODO: should remove
//#include <Windows.h>

#include <vector>

//Declare the handle variable.
///DECLARE_HANDLE(GRAPHDECOMPOSE_HANDLE);


#include "detect_clusters.h"

using namespace std;


//Create the graph decomposition component

CAffinityGraph* GraphDecGreate_API();

//Graph decomposition.
bool DecomposeGraph_API(CAffinityGraph* handle, IVectorDVector& vHyperEdges, IDVector& vCycleEdges, DecomposePara& Paras,
	IntArray& vSupport, DoubleArray& vProb, DoubleVector& vFValue);

//Free the graph decomposition component
void GraphDecFree_API(CAffinityGraph* handle);



typedef vector<int> IntVector;
typedef vector<IntVector> IntArray;
typedef vector<double> DoubleVector;
typedef vector<DoubleVector> DoubleArray;
/* 
// YI: remove the duplicated definition from defs.h
typedef struct IntDouble_TYP {
	int I;
	double D;
}IntDouble;
typedef vector<IntDouble> IDVector;

typedef struct IntVectorDouble_TYP {
	IntVector Ivs;
	double D;
}IntVectorDouble;

typedef vector<IntVectorDouble> IVectorDVector;


typedef struct DecomposeParameter_TYP {	
	int m_NumOfVertices; // Number of vertices in the graph, must provided, since the affinity matrix is in sparse form	
	int m_MinVertexOfCluster; //minimal number of vertices in a cluster. 
	int m_K;
	double m_MinValue;
	double m_MaxProb;
	double m_MinProb;
	double m_MinRewardDiff;
}DecomposePara;
*/


#endif