//Define some important classes. 

#ifndef _DEFS_H
#define _DEFS_H

#include <vector>
#include <set>
using namespace std;

///#include "graph_decomp.h"

//********************************************************************//
//** Define some 



typedef vector<int> IntVector;
typedef vector<IntVector> IntArray;
typedef vector<double> DoubleVector;
typedef vector<DoubleVector> DoubleArray;


typedef set<int> IntSet;

typedef struct IntIntDouble_TYP {
	int I[2];
	double D;
}IntIntDouble;
typedef vector<IntIntDouble> IIDVector;

typedef struct WorkSet_TYP
{
	IntVector m_vVd;
	IntVector m_vInvVd;
	int m_nv;
	DoubleVector m_vProb;
	DoubleVector m_vReward;
	IntVector m_vEdgeCount;
	IntVector m_vVertexMark;
	IntVector m_vInvVertexMark;
	int m_nm;
}WorkSet;



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


#endif