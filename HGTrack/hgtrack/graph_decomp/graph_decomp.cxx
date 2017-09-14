// GraphDecompose.cpp : ���� DLL Ӧ�ó���ĵ���������
//

//MCC
///#include "stdafx.h"

#include "graph_decomp.h"
//#include "detect_clusters.h"

//Create the graph decompose variable.
CAffinityGraph* GraphDecGreate_API()
{
	CAffinityGraph* pDecompose = new CAffinityGraph;
	return pDecompose;
}


//Decompose the input graph
bool DecomposeGraph_API(CAffinityGraph* handle, IVectorDVector& vHyperEdges, IDVector& vCycleEdges, DecomposePara& Paras,
	IntArray& vSupport, DoubleArray& vProb, DoubleVector& vFValue)
{
	CAffinityGraph* pDecompose = handle;
	pDecompose->DecomposeGraph(vHyperEdges, vCycleEdges, Paras, vSupport, vProb, vFValue);
	return true;
}

//Free the graph decomposition component
void GraphDecFree_API(CAffinityGraph* handle)
{
	CAffinityGraph* pDecompose = handle;
	if (pDecompose) {
		delete pDecompose;
		pDecompose = 0;
	}
}

