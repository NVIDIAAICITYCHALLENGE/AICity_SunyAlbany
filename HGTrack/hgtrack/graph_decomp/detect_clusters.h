#ifndef _DETECTCLUSTERS_H
#define _DETECTCLUSTERS_H
#include <util/defs.h>

//-------------------------------------------- Help Function ---------------------------------------------------------------//
void QuickSortL(IDVector& data,int left,int right);

//-------------------------------------------- MaxIntHeap Class -----------------------------------------------------------//
class MaxIntHeap
{
public:
	IntVector m_vIndexedData;
	IntVector m_vRevMapping;
	DoubleVector* m_pValue;
	DoubleVector* m_pOptProb;	
	double m_MinProb;
	double m_MinValue;
	double m_Epsilon;
	double m_Epsilon1;

	int m_Len;
	int m_Tail;
	void SetData(DoubleVector* pv,DoubleVector* pp);	
	void SetBufferLen(int Len);
	void SetPara(double Mp,double Mv,double Ep);

	bool ShiftDown(int HeapIndex); //Assumption: the downward part is already in heap state
	bool ShiftUp(int HeapIndex);   //Assumption: other part is already in heap state
	void RePos(int DataIndex);	
	int GetMax(void);
	int GetSize(void);
	void Reset(void);	
	void ClearDataBuffer(void);
	IntVector LevelSet(double Value);

	//Constructive function.
public:
	MaxIntHeap();
	virtual ~MaxIntHeap();
};

//------------------------------------------------- Affinity Graph Class -----------------------------------------------------//

class CAffinityGraph
{
public:
	CAffinityGraph();
	virtual ~CAffinityGraph();
public:
	//input data
	//Self-cycle label
	bool m_CycleLabel;
	//Hyperedge data.
	IVectorDVector m_vHyperEdges;
	//Self-cycle edge data
	IDVector m_vCycleEdges;
	//Start indices.
	IntArray m_vStartCoords;
	DoubleArray m_vSProb;

	
    //Clustering parameters.
	DecomposePara m_Para;

	//temporal use
	IntArray m_Vertex2Edge;	
	WorkSet m_WS;

	DoubleArray m_vProb;
	IntArray m_vSup;
	DoubleVector m_vFunValue;

	void BuildVertexEdgeRelation(void);
	void InitWorkSet(void);
	void InitDataStructure(void);
	void DetectingCluster(int Idx);
	void ComputeClusters(void);
	void ComputeRewards(IntSet& NSet);
	double ComputeFunValue(void);
	void GetEdgeSet(int VIdx1,int VIdx2,IntVector& C,IntVector& S1,IntVector& S2);	
public:
	//reset the variables in the class.
	bool Reset();
	//reset the class with the new parameters.
	bool Reset(DecomposePara& Para);
	bool GenStartCoords(IVectorDVector& AffMatrix);
	//Dense neighborhoods revealing for the graph with the self cycle edges.
	bool DecomposeGraph(IVectorDVector& vHyperEdges, IDVector& vCycleEdges, DecomposePara& Paras, IntArray& vSupport, DoubleArray& vProb, DoubleVector& vFValue);
};


#endif