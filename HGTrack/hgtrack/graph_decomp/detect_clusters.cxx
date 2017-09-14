///#include "stdafx.h" //MCC

#include <algorithm> //for max()
#include <cmath> // for abs()
#include "detect_clusters.h"

/*
  [Support,Prob,FunValue] = DetectingClusters(Indices,Weight,StartPoint,Para)
*/

//Help Function.
void QuickSortL(IDVector& data,int left,int right)
{
	int l_hold, r_hold, mid;
	IntDouble pivot;		
	l_hold = left;
	r_hold = right;
	mid=(left+right)/2;	
	pivot = data[left];

	while (left < right)
	{
		while ((data[right].D<pivot.D) && (left < right))
			right--;
		if (left != right)
		{
			data[left] = data[right];		
			left++;
		}
		while ((data[left].D>= pivot.D) && (left < right))
			left++;
		if (left != right)
		{
			data[right] = data[left];		 
			right--;
		}
	}

	data[left] = pivot;	

	mid=left;
	left = l_hold;
	right = r_hold;
	if (left < mid)
		QuickSortL(data, left, mid-1);
	if (right > mid)
		QuickSortL(data, mid+1, right);
}

//----------------------------------------------- MaxIntHeap Class -------------------------------------------------------//
MaxIntHeap::MaxIntHeap()
{
	m_vIndexedData.clear();
	m_vRevMapping.clear();
	m_pValue = 0;
	m_pOptProb = 0;	
	m_MinProb = 0;
	m_MinValue = 0;
	m_Epsilon = 0;
	m_Epsilon1 = 0;
    m_Len = 0;
	m_Tail = 0;
}

MaxIntHeap::~MaxIntHeap()
{
	m_vIndexedData.clear();
	m_vRevMapping.clear();
	m_pValue = 0;
	m_pOptProb = 0;	
	m_MinProb = 0;
	m_MinValue = 0;
	m_Epsilon = 0;
	m_Epsilon1 = 0;
	m_Len = 0;
	m_Tail = 0;
}


void MaxIntHeap::SetBufferLen(int nLen)
{
	m_Len = nLen;
	m_Tail = 0;
	m_vIndexedData.clear();
	m_vRevMapping.clear();
	for(int i = 0; i < m_Len; i++)
	{
		m_vIndexedData.push_back(-1);
		m_vRevMapping.push_back(-1);
	}
}

void MaxIntHeap::SetPara(double Mp,double Mv,double Ep)
{	
	m_MinProb = Mp;
	m_MinValue = Mv;
	m_Epsilon = Ep;
	m_Epsilon1 = m_Epsilon - m_MinProb;	
}

void MaxIntHeap::SetData(DoubleVector* pv,DoubleVector* pp)
{
	m_pValue = pv;
	m_pOptProb  = pp;		
}

void MaxIntHeap::ClearDataBuffer(void)
{
	for(int i=0;i<m_Tail;i++)			
		(*m_pValue)[m_vIndexedData[i]] = 0;		
}

IntVector MaxIntHeap::LevelSet(double Value)
{
	IntVector IndexVector;
	for(int i = 0; i < m_Tail; i++)
	{
		int CurIndex = m_vIndexedData[i];
		if(abs((*m_pValue)[CurIndex]-Value) < m_MinValue)
			IndexVector.push_back(CurIndex);
	}
	return IndexVector;
}

void MaxIntHeap::Reset(void)
{	
	for(int i=0;i<m_Tail;i++)
	{		
		m_vRevMapping[m_vIndexedData[i]] = -1;
		m_vIndexedData[i] = -1;		
	}
	m_Tail = 0;
}

bool MaxIntHeap::ShiftDown(int HeapIndex)
{
	bool IsShift = false;
	int Root = HeapIndex;
	double RootValue = (*m_pValue)[m_vIndexedData[Root]];
	int Child = 2*Root+1;	
	while(Child<m_Tail)
	{
		double ChildValue = (*m_pValue)[m_vIndexedData[Child]];
		if((Child<(m_Tail-1))&&(ChildValue<(*m_pValue)[m_vIndexedData[Child+1]]))
		{
			Child += 1;
			ChildValue = (*m_pValue)[m_vIndexedData[Child]];
		}

		if(RootValue>=ChildValue)
			break;

		IsShift = true;
		//swap data
		m_vRevMapping[m_vIndexedData[Root]]  = Child;
		m_vRevMapping[m_vIndexedData[Child]] = Root;
		int Temp = m_vIndexedData[Root];
		m_vIndexedData[Root] = m_vIndexedData[Child];
		m_vIndexedData[Child] = Temp;
		Root = Child;
		Child = 2*Root + 1;
		RootValue = (*m_pValue)[m_vIndexedData[Root]];
	}

	return IsShift;
}

bool MaxIntHeap::ShiftUp(int HeapIndex)
{
	bool IsShift = false;
	int Child = HeapIndex;
	int Par   = (Child-1)/2;
	double ChildValue = (*m_pValue)[m_vIndexedData[Child]];
	while(Par>=0)
	{
		if(Child==0)
			break;
		double ParValue = (*m_pValue)[m_vIndexedData[Par]];
		if(ParValue>=ChildValue)
			break;

		IsShift = true;
		m_vRevMapping[m_vIndexedData[Par]] = Child;
		m_vRevMapping[m_vIndexedData[Child]] = Par;
		int Temp = m_vIndexedData[Par];
		m_vIndexedData[Par] = m_vIndexedData[Child];
		m_vIndexedData[Child] = Temp;
		Child = Par;
		Par = (Child-1)/2;
		ChildValue = (*m_pValue)[m_vIndexedData[Child]];
	}

	return IsShift;
}

void MaxIntHeap::RePos(int DataIndex)
{
	bool CanInc = true;
	if((*m_pOptProb)[DataIndex]>=m_Epsilon1)
		CanInc = false;
	bool TooSmall = false;
	if((*m_pValue)[DataIndex]<m_MinValue)
	{
		(*m_pValue)[DataIndex] = 0;
		TooSmall = true;
	}

	int HeapIndex = m_vRevMapping[DataIndex];
	if(HeapIndex==-1)
	{
		if(CanInc&&(!TooSmall))
		{
			m_vIndexedData[m_Tail] = DataIndex;
			m_vRevMapping[DataIndex] = m_Tail;
			m_Tail++;
			ShiftUp(m_Tail-1);
		}	
	}
	else
	{
		if((!CanInc)||TooSmall)
		{
			//copy the last item with this
			m_Tail--;
			m_vRevMapping[m_vIndexedData[m_Tail]] = HeapIndex;
			m_vRevMapping[DataIndex] = -1;
			m_vIndexedData[HeapIndex] = m_vIndexedData[m_Tail];	
			m_vIndexedData[m_Tail] = -1;
		}

		if(m_Tail==HeapIndex)
			return;		
		if(!ShiftUp(HeapIndex))
			ShiftDown(HeapIndex);	
	}
}

int MaxIntHeap::GetMax(void)
{
	if(m_Tail==0)
		return -1;
	else
		return m_vIndexedData[0];
}

int MaxIntHeap::GetSize(void)
{
	return m_Tail;
}


//------------------------------------------------------------------- Affinity Graph Class -------------------------------------------------------------//
//Initialize the variables.
CAffinityGraph::CAffinityGraph()
{
	m_vStartCoords.clear();
	m_vSProb.clear();
	m_Para.m_K = 0;
	m_Para.m_MaxProb = 0;
	m_Para.m_MinProb = 0;
	m_Para.m_MinRewardDiff = 0;
	m_Para.m_MinValue = 0;
	m_Para.m_MinVertexOfCluster = 0;
	m_Para.m_NumOfVertices = 0;

	//temporal use
	m_Vertex2Edge.clear();	
	m_WS.m_vVd.clear();
	m_WS.m_vInvVd.clear();
	m_WS.m_nv = 0;
	m_WS.m_vProb.clear();
	m_WS.m_vReward.clear();
	m_WS.m_vEdgeCount.clear();
	m_WS.m_vVertexMark.clear();
	m_WS.m_vInvVertexMark.clear();
	m_WS.m_nm = 0;

	m_vProb.clear();
	m_vSup.clear();
	m_vFunValue.clear();

	//Initialize the self-cycle mark.
	m_CycleLabel = false;
}

//Release the memory
CAffinityGraph::~CAffinityGraph()
{
	m_vStartCoords.clear();
	m_vSProb.clear();
	m_Para.m_K = 0;
	m_Para.m_MaxProb = 0;
	m_Para.m_MinProb = 0;
	m_Para.m_MinRewardDiff = 0;
	m_Para.m_MinValue = 0;
	m_Para.m_MinVertexOfCluster = 0;
	m_Para.m_NumOfVertices = 0;

	//temporal use
	m_Vertex2Edge.clear();	
	m_WS.m_vVd.clear();
	m_WS.m_vInvVd.clear();
	m_WS.m_nv = 0;
	m_WS.m_vProb.clear();
	m_WS.m_vReward.clear();
	m_WS.m_vEdgeCount.clear();
	m_WS.m_vVertexMark.clear();
	m_WS.m_vInvVertexMark.clear();
	m_WS.m_nm = 0;

	m_vProb.clear();
	m_vSup.clear();
	m_vFunValue.clear();

	//Initialize the self-cycle mark.
	m_CycleLabel = false;
}


void CAffinityGraph::BuildVertexEdgeRelation(void)
{
	m_Vertex2Edge.clear();
	IntVector Iv;
	for(int i = 0; i < m_Para.m_NumOfVertices; i++)
		m_Vertex2Edge.push_back(Iv);
	for(int i = 0; i < (int)(m_vHyperEdges.size()); i++)	
		for(int j = 0; j < m_Para.m_K; j++)			
			m_Vertex2Edge[m_vHyperEdges[i].Ivs[j]].push_back(i);	
}

void CAffinityGraph::InitWorkSet(void)
{
	for(int i = 0; i < m_Para.m_NumOfVertices; i++)
	{
		m_WS.m_vVd.push_back(-1);		
		m_WS.m_vProb.push_back(0);		
	}
	m_WS.m_vInvVd = m_WS.m_vVd;
	m_WS.m_vReward = m_WS.m_vProb;
	m_WS.m_vVertexMark = m_WS.m_vVd;
	m_WS.m_vInvVertexMark = m_WS.m_vVd;
    
	for(int i = 0; i < (int)(m_vHyperEdges.size()); i++)
		m_WS.m_vEdgeCount.push_back(0);

	m_WS.m_nv = 0;
	m_WS.m_nm = 0;
}

void CAffinityGraph::InitDataStructure(void)
{
	InitWorkSet();
	BuildVertexEdgeRelation();
}

void CAffinityGraph::GetEdgeSet(int VIdx1,int VIdx2,IntVector& C,IntVector& S1,IntVector& S2)
{
	IntVector& T1 = m_Vertex2Edge[VIdx1];
	IntVector& T2 = m_Vertex2Edge[VIdx2];
	
	int L = 0;
	if(m_WS.m_vProb[VIdx1] < m_Para.m_MinProb)
		L = 1;

	int Num1 = (int)(T1.size());
	int Num2 = (int)(T2.size());
	int p1 = 0;
	int p2 = 0;
	while((p1<Num1)&&(p2<Num2))
	{
		if(T1[p1]==T2[p2])
		{
			int EIdx = T1[p1];
			int Ec = m_WS.m_vEdgeCount[EIdx];

			if(Ec>=(m_Para.m_K-1-L))
				C.push_back(EIdx);

			p1++;
			p2++;
		}
		else if(T1[p1]<T2[p2])
		{
			int EIdx = T1[p1];
			int Ec = m_WS.m_vEdgeCount[EIdx];
			if(Ec>=(m_Para.m_K-1-L))
				S1.push_back(EIdx);
			p1++;
		}
		else
		{
			int EIdx = T2[p2];
			int Ec = m_WS.m_vEdgeCount[EIdx];
			if(Ec>=(m_Para.m_K-1))
				S2.push_back(EIdx);
			p2++;
		}
	}

	while(p1<Num1)
	{
		int EIdx = T1[p1];
		int Ec = m_WS.m_vEdgeCount[EIdx];
		if(Ec>=(m_Para.m_K-1-L))
			S1.push_back(EIdx);
		p1++;
	}

	while(p2<Num2)
	{
		int EIdx = T2[p2];
		int Ec = m_WS.m_vEdgeCount[EIdx];
		if(Ec>=(m_Para.m_K-1))
			S2.push_back(EIdx);
		p2++;
	}
}

void CAffinityGraph::DetectingCluster(int Idx)
{
	//Some temporal variable.
	IntVector Common;
	Common.clear();
	//Prepare some variable.
	MaxIntHeap Mih;
	Mih.SetBufferLen(m_Para.m_NumOfVertices);
	Mih.SetData(&m_WS.m_vReward,&m_WS.m_vProb);
	Mih.SetPara(m_Para.m_MinProb,m_Para.m_MinValue,m_Para.m_MaxProb);

	m_WS.m_nv = (int)(m_vStartCoords[Idx].size());
	for(int  i= 0; i < m_WS.m_nv; i++) {
		int t = m_vStartCoords[Idx][i];
		m_WS.m_vVd[i] = t;
		m_WS.m_vInvVd[t] = i;
		m_WS.m_vProb[t] = m_vSProb[Idx][i];
	}

	IntSet NSet;
	ComputeRewards(NSet);

	double fVal = ComputeFunValue();
	for(int i = 0; i < m_WS.m_nv; i++)
		Mih.RePos(m_WS.m_vVd[i]);

	IntSet::iterator pos = NSet.begin();
	while(pos != NSet.end()) {
		Mih.RePos(*pos);
		pos++;
	}

	while(true) {
		//select a pair for updating
		int Id1,Id2;
		Id1 = Mih.GetMax();
		if(Id1==-1)
			break;

		int Pos2 = 0;
		double MinRw = m_WS.m_vReward[m_WS.m_vVd[Pos2]];
		for(int i = 1; i < m_WS.m_nv; i++) {
			int VIdx = m_WS.m_vVd[i];
			if(m_WS.m_vReward[VIdx] < MinRw) {
				Pos2 = i;
				MinRw = m_WS.m_vReward[VIdx];
			}
		}

		Id2 = m_WS.m_vVd[Pos2];

		double Rd = m_WS.m_vReward[Id1] - m_WS.m_vReward[Id2];

		if(Rd < m_Para.m_MinRewardDiff) {
			//Handle the self-cycle edges.
			//not considering self loop, then stop
			if(!m_CycleLabel)
				break;

			bool IsFind = false;
			double Pivot = m_WS.m_vReward[Id1];
			IntVector S1 = Mih.LevelSet(Pivot);

			IntVector S2;
			for(int i = 1; i < m_WS.m_nv; i++) {
				if(abs(m_WS.m_vReward[m_WS.m_vVd[i]]-Pivot) < m_Para.m_MinValue)
					S2.push_back(m_WS.m_vVd[i]);
			}

			for(int i = 0; i < (int)(S1.size()); i++) {
				double Mii = m_vCycleEdges[S1[i]].D;
				for(int j = 0; j < (int)(S2.size()); j++) {
					if(S1[i] == S2[j])
						continue;

					double Mjj = m_vCycleEdges[S2[j]].D;

					//get important edgeset
					IntVector D1,D2;
					D1.clear();
					D2.clear();
					Common.clear();
					GetEdgeSet(Id1,Id2,Common,D1,D2);

					//compute Rij
					double Mij = 0;
					for(int k = 0; k < (int)(Common.size()); k++) {
						int EIdx = Common[k];
						double w = m_vHyperEdges[EIdx].D;
						for(int m = 0; m < m_Para.m_K; m++) {
							int VIdx = m_vHyperEdges[EIdx].Ivs[m] ;
							if(VIdx == Id1)
								continue;
							if(VIdx == Id2)
								continue;
							w *= m_WS.m_vProb[VIdx];
						}
						Mij += w;
					}

					//find a pair, then update index1, index2, RewardsDiff, Pos2 for late use
					if((Mii+Mjj) > 2*Mij) {
						IsFind = true;
						Id1 = S1[i];
						Id2 = S2[j];
						Rd = m_WS.m_vReward[Id1] - m_WS.m_vReward[Id2];

						int Pos2 = -1;
						for(int m = 1; m < m_WS.m_nv; m++) {
							if(m_WS.m_vVd[m] == Id2) {
								Pos2 = m;
								break;
							}
						}
						break;
					}
				}
				if(IsFind)
					break;
			}
			//not find a pair, then stop
			if(!IsFind)
				break;
		}
			
		double Alpha = m_Para.m_MaxProb - m_WS.m_vProb[Id1];
		if(m_WS.m_vProb[Id2] < Alpha)
			Alpha = m_WS.m_vProb[Id2];

		//get important edgeset
		IntVector S1, S2;
		S1.clear();
		S2.clear();
		Common.clear();
		GetEdgeSet(Id1,Id2,Common,S1,S2);

		//compute Rij
		double Rij = 0;
		for(int i = 0; i < (int)(Common.size()); i++) {
			int EIdx = Common[i];
			double w = m_vHyperEdges[EIdx].D;
			for(int j = 0; j < m_Para.m_K; j++) {
				int VIdx = m_vHyperEdges[EIdx].Ivs[j] ;
				if(VIdx==Id1)
					continue;
				if(VIdx==Id2)
					continue;
				w *= m_WS.m_vProb[VIdx];
			}
			Rij += w;
		}

		//Temporal variables.
		double T = -2*Rij;
		if (m_CycleLabel)
			T += m_vCycleEdges[Id1].D + m_vCycleEdges[Id2].D;

		if (T < 0) {
			double TempAlpha = - Rd / T;
			if (Alpha > TempAlpha)
				Alpha = TempAlpha;
		}

		if(Alpha < m_Para.m_MinProb)
			break;

		//update function value
		double Delta = (Rd-Rij*Alpha)*Alpha;
		fVal += Delta;			

		//update prob
		m_WS.m_vProb[Id1] += Alpha;
		m_WS.m_vProb[Id2] -= Alpha;

		//update Vd and InvVd
		int Pos1 = m_WS.m_vInvVd[Id1];
		if(m_WS.m_vProb[Id2] < m_Para.m_MinProb) {
			m_WS.m_vProb[Id2] = 0;
			m_WS.m_vInvVd[Id2] = -1;
			if(Pos1 == -1) {
				m_WS.m_vVd[Pos2] = Id1;
				m_WS.m_vInvVd[Id1] = Pos2;
			}
			else if(Pos2 == (m_WS.m_nv-1)) {
				m_WS.m_nv--;
				m_WS.m_vVd[m_WS.m_nv] = -1;
			}
			else {
				m_WS.m_nv--;
				int VIdx = m_WS.m_vVd[m_WS.m_nv];
				m_WS.m_vVd[Pos2] = VIdx;
				m_WS.m_vInvVd[VIdx] = Pos2;
				m_WS.m_vVd[m_WS.m_nv] = -1;
			}
		}
		else {
			if(Pos1==-1) {
				m_WS.m_vVd[m_WS.m_nv] = Id1;
				m_WS.m_vInvVd[Id1] = m_WS.m_nv;
				m_WS.m_nv++;
			}
		}

		//update edgecount
		if(m_WS.m_vProb[Id2]<m_Para.m_MinProb) {
			for(int i = 0; i < (int)(m_Vertex2Edge[Id2].size()); i++) {
				int EIdx = m_Vertex2Edge[Id2][i];
				m_WS.m_vEdgeCount[EIdx]--;
			}
		}
		if(Pos1==-1) {
			for(int i = 0; i < (int)(m_Vertex2Edge[Id1].size()); i++) {
				int EIdx = m_Vertex2Edge[Id1][i];
				m_WS.m_vEdgeCount[EIdx]++;
			}
		}

		//update reward
		m_WS.m_vVertexMark[0] = Id1;
		m_WS.m_vVertexMark[1] = Id2;
		m_WS.m_vInvVertexMark[Id1] = 0;
		m_WS.m_vInvVertexMark[Id2] = 1;
		m_WS.m_nm = 2;

		for(int i = 0; i < (int)(S1.size()); i++) {
			int EIdx = S1[i];

			double w = m_vHyperEdges[EIdx].D;
			int Ec = m_WS.m_vEdgeCount[EIdx];
			if(Ec == m_Para.m_K) {
				for(int j = 0; j < m_Para.m_K; j++) {
					int VIdx = m_vHyperEdges[EIdx].Ivs[j];
					if(VIdx==Id1)
						continue;
					w *= m_WS.m_vProb[VIdx];
				}

				for(int j = 0; j < m_Para.m_K; j++) {
					int VIdx = m_vHyperEdges[EIdx].Ivs[j];
					if(VIdx == Id1)
						continue;
					double tw = w/m_WS.m_vProb[VIdx];
					m_WS.m_vReward[VIdx] += tw*Alpha;

					int MIdx = m_WS.m_vInvVertexMark[VIdx];
					if(MIdx < 0) {
						m_WS.m_vVertexMark[m_WS.m_nm] = VIdx;
						m_WS.m_vInvVertexMark[VIdx] = m_WS.m_nm++;
					}										
				}
			}
			else {
				int ZeroIndex;
				for(int j = 0; j < m_Para.m_K; j++) {
					int VIdx = m_vHyperEdges[EIdx].Ivs[j];
					if(VIdx==Id1)
						continue;
					if(m_WS.m_vProb[VIdx] < m_Para.m_MinProb) {
						ZeroIndex = VIdx;
						continue;
					}
					w *= m_WS.m_vProb[VIdx];
				}

				m_WS.m_vReward[ZeroIndex] += w*Alpha;
				int MIdx = m_WS.m_vInvVertexMark[ZeroIndex];
				if(MIdx < 0) {
					m_WS.m_vVertexMark[m_WS.m_nm] = ZeroIndex;
					m_WS.m_vInvVertexMark[ZeroIndex] = m_WS.m_nm++;
				}										
			}
		}

		for(int i = 0; i < (int)(S2.size()); i++) {
			int EIdx = S2[i];

			int L = 0;
			if(m_WS.m_vProb[Id2]<m_Para.m_MinProb)
				L = 1;
			double w = m_vHyperEdges[EIdx].D;
			int Ec = m_WS.m_vEdgeCount[EIdx];
			if(Ec == (m_Para.m_K-L)) {
				for(int j = 0; j < m_Para.m_K; j++) {
					int VIdx = m_vHyperEdges[EIdx].Ivs[j];
					if(VIdx==Id2)
						continue;
					w *= m_WS.m_vProb[VIdx];
				}

				for(int j = 0; j < m_Para.m_K; j++) {
					int VIdx = m_vHyperEdges[EIdx].Ivs[j];
					if(VIdx==Id2)
						continue;
					double tw = w/m_WS.m_vProb[VIdx];
					m_WS.m_vReward[VIdx] -= tw*Alpha;
					
					int MIdx = m_WS.m_vInvVertexMark[VIdx];
					if(MIdx < 0) {
						m_WS.m_vVertexMark[m_WS.m_nm] = VIdx;
						m_WS.m_vInvVertexMark[VIdx] = m_WS.m_nm++;
					}										
				}
			}
			else {
				int ZeroIndex;
				for(int j = 0; j < m_Para.m_K; j++) {
					int VIdx = m_vHyperEdges[EIdx].Ivs[j];
					if(VIdx == Id2)
						continue;
					if(m_WS.m_vProb[VIdx] < m_Para.m_MinProb) {
						ZeroIndex = VIdx;
						continue;
					}
					w *= m_WS.m_vProb[VIdx];
				}

				m_WS.m_vReward[ZeroIndex] -= w*Alpha;

				int MIdx = m_WS.m_vInvVertexMark[ZeroIndex];
				if(MIdx < 0) {
					m_WS.m_vVertexMark[m_WS.m_nm] = ZeroIndex;
					m_WS.m_vInvVertexMark[ZeroIndex] = m_WS.m_nm++;
				}		
			}
		}

		for(int i = 0; i < (int)(Common.size()); i++) {
			int EIdx = Common[i];
			int Ec = m_WS.m_vEdgeCount[EIdx];
			int L = 0;
			if(m_WS.m_vProb[Id2]<m_Para.m_MinProb)
				L = 1;

			double w = m_vHyperEdges[EIdx].D;
			double factor = Alpha*(m_WS.m_vProb[Id2]-m_WS.m_vProb[Id1]+Alpha);
			if(Ec == (m_Para.m_K-L)) {
				for(int j = 0; j < m_Para.m_K; j++) {
					int VIdx = m_vHyperEdges[EIdx].Ivs[j];
					if(VIdx == Id1)
						continue;
					if(VIdx == Id2)
						continue;					
					w *= m_WS.m_vProb[VIdx];
				}

				for(int j = 0; j < m_Para.m_K; j++) {
					int VIdx = m_vHyperEdges[EIdx].Ivs[j];
					if(VIdx == Id1)						
						m_WS.m_vReward[VIdx] -= w*Alpha;
					else if(VIdx == Id2)
						m_WS.m_vReward[VIdx] += w*Alpha;
					else {
						double tw = w/m_WS.m_vProb[VIdx];
						m_WS.m_vReward[VIdx] += tw*factor;
						
						int MIdx = m_WS.m_vInvVertexMark[VIdx];
						if(MIdx < 0) {
							m_WS.m_vVertexMark[m_WS.m_nm] = VIdx;
							m_WS.m_vInvVertexMark[VIdx] = m_WS.m_nm++;
						}										
					}
				}
			}
			else {
				int ZeroIndex;
				for(int j = 0; j < m_Para.m_K; j++) {
					int VIdx = m_vHyperEdges[EIdx].Ivs[j];
					if(VIdx == Id1)
						continue;
					if(VIdx == Id2)
						continue;
					if(m_WS.m_vProb[VIdx] < m_Para.m_MinProb) {
						ZeroIndex = VIdx;
						continue;
					}
					w *= m_WS.m_vProb[VIdx];
				}

				m_WS.m_vReward[ZeroIndex] += w*factor;
				
				int MIdx = m_WS.m_vInvVertexMark[ZeroIndex];
				if(MIdx < 0) {
					m_WS.m_vVertexMark[m_WS.m_nm] = ZeroIndex;
					m_WS.m_vInvVertexMark[ZeroIndex] = m_WS.m_nm++;
				}		
			}
		}

		//Use the self-cylces.
		if (m_CycleLabel) {
			m_WS.m_vReward[Id1] += Alpha * m_vCycleEdges[Id1].D;
			m_WS.m_vReward[Id2] -= Alpha * m_vCycleEdges[Id2].D;
		}

		//update heap
		for(int i = 0; i < m_WS.m_nm; i++)
			Mih.RePos(m_WS.m_vVertexMark[i]);

		//clear mark 
		for(int i = 0; i < m_WS.m_nm; i++) {
			m_WS.m_vInvVertexMark[m_WS.m_vVertexMark[i]] = -1;
			m_WS.m_vVertexMark[i] = -1;
		}
		m_WS.m_nm = 0;
	}

	//save result
	IntVector CurSup;
	DoubleVector CurProb;
	double TotalProb = 0;
	for(int i = 0; i < m_WS.m_nv; i++) {
		int VIdx = m_WS.m_vVd[i];
		if(m_WS.m_vProb[VIdx] > m_Para.m_MinProb) {
			CurSup.push_back(VIdx);
			CurProb.push_back(m_WS.m_vProb[VIdx]);
			TotalProb += m_WS.m_vProb[VIdx];
		}
	}
	for(int i = 0; i < (int)(CurProb.size()); i++)
		CurProb[i] /= TotalProb;

	m_vSup.push_back(CurSup);
	m_vProb.push_back(CurProb);
	m_vFunValue.push_back(fVal);


	//clear 
	Mih.ClearDataBuffer();
	Mih.Reset();
	for(int i = 0; i < m_WS.m_nv; i++) {
		int VIdx = m_WS.m_vVd[i];
		m_WS.m_vProb[VIdx] = 0;
		m_WS.m_vReward[VIdx] = 0;
		m_WS.m_vInvVd[VIdx] = -1;
		m_WS.m_vVd[i] = -1;	
	}

	for(int i = 0; i < (int)(m_vHyperEdges.size()); i++)
		m_WS.m_vEdgeCount[i] = 0;

	m_WS.m_nv = 0;	
}

void CAffinityGraph::ComputeRewards(IntSet& NSet)
{	
	IntVector ESet;
	for(int i=0;i<m_WS.m_nv;i++) {
		int VIdx = m_WS.m_vVd[i];
		for(int j = 0; j < (int)(m_Vertex2Edge[VIdx].size()); j++) {
			int EIdx = m_Vertex2Edge[VIdx][j];
			m_WS.m_vEdgeCount[EIdx]++;
			if(m_WS.m_vEdgeCount[EIdx]==1)
				ESet.push_back(EIdx);
		}
	}

	for(int i = 0; i < (int)(ESet.size()); i++) {
		int EIdx = ESet[i];
		int Ec = m_WS.m_vEdgeCount[EIdx];
		if(Ec<(m_Para.m_K-1))
			continue;

		double w = m_vHyperEdges[EIdx].D;
		if(Ec == m_Para.m_K) {
			for(int j=0;j<m_Para.m_K;j++)
				w *= m_WS.m_vProb[m_vHyperEdges[EIdx].Ivs[j]];

			for(int j=0;j<m_Para.m_K;j++) {
				int VIdx = m_vHyperEdges[EIdx].Ivs[j];
				m_WS.m_vReward[VIdx] += w/m_WS.m_vProb[VIdx];
			}
		}
		else {
			int ZeroIndex=-1;
			for(int j = 0; j < m_Para.m_K; j++) {
				int VIdx = m_vHyperEdges[EIdx].Ivs[j];
				int MIdx = m_WS.m_vInvVd[VIdx];
				if(MIdx>=0)
					w *= m_WS.m_vProb[VIdx];
				else								
					ZeroIndex = VIdx;				
			}

			NSet.insert(ZeroIndex);
			m_WS.m_vReward[ZeroIndex] += w;
		}		
	}	

	if (m_CycleLabel) {
		//Check whether it contains a self-cycles.
		for(int i = 0; i < m_WS.m_nv; i++) {
			int Index = m_WS.m_vVd[i];
			m_WS.m_vReward[Index] += m_vCycleEdges[Index].D * m_WS.m_vProb[Index];
		}

	}
}

double CAffinityGraph::ComputeFunValue(void)
{
	double fv = 0;
	for(int i=0;i<m_WS.m_nv;i++)
	{
		int VIdx = m_WS.m_vVd[i];
		fv += m_WS.m_vReward[VIdx]*m_WS.m_vProb[VIdx];
	}

	return fv/m_Para.m_K;
}

void CAffinityGraph::ComputeClusters(void)
{
	for(int i = 0; i < (int)(m_vStartCoords.size()); i++)
		DetectingCluster(i);
}


//reset the variables of the affinity graph
bool CAffinityGraph::Reset()
{
	for (int i = 0; i < (int)(m_vHyperEdges.size()); i++)
		m_vHyperEdges[i].Ivs.clear();
	m_vHyperEdges.clear();


	for (int i = 0; i < (int)(m_vStartCoords.size()); i++)
		m_vStartCoords[i].clear();
	m_vStartCoords.clear();

	for (int i = 0; i < (int)(m_vSProb.size()); i++)
		m_vSProb[i].clear();
	m_vSProb.clear();

	for (int i = 0; i < (int)(m_Vertex2Edge.size()); i++)
		m_Vertex2Edge[i].clear();
	m_Vertex2Edge.clear();

	m_WS.m_nm = 0;
	m_WS.m_nv = 0;
	m_WS.m_vEdgeCount.clear();
	m_WS.m_vInvVd.clear();
	m_WS.m_vInvVertexMark.clear();
	m_WS.m_vProb.clear();
	m_WS.m_vReward.clear();
	m_WS.m_vVd.clear();
	m_WS.m_vVertexMark.clear();

	for (int i = 0; i < (int)(m_vProb.size()); i++)
		m_vProb[i].clear();
	m_vProb.clear();

	for (int i = 0; i < (int)(m_vSup.size()); i++)
		m_vSup[i].clear();
	m_vSup.clear();

	m_vFunValue.clear();
	return true;
}


//reset the variables of the affinity graph with parameters.
bool CAffinityGraph::Reset(DecomposePara& Para)
{
	for (int i = 0; i < (int)(m_vHyperEdges.size()); i++)
		m_vHyperEdges[i].Ivs.clear();
	m_vHyperEdges.clear();

	for (int i = 0; i < (int)(m_vStartCoords.size()); i++)
		m_vStartCoords[i].clear();
	m_vStartCoords.clear();

	for (int i = 0; i < (int)(m_vSProb.size()); i++)
		m_vSProb[i].clear();
	m_vSProb.clear();

	for (int i = 0; i < (int)(m_Vertex2Edge.size()); i++)
		m_Vertex2Edge[i].clear();
	m_Vertex2Edge.clear();

	m_WS.m_nm = 0;
	m_WS.m_nv = 0;
	m_WS.m_vEdgeCount.clear();
	m_WS.m_vInvVd.clear();
	m_WS.m_vInvVertexMark.clear();
	m_WS.m_vProb.clear();
	m_WS.m_vReward.clear();
	m_WS.m_vVd.clear();
	m_WS.m_vVertexMark.clear();

	for (int i = 0; i < (int)(m_vProb.size()); i++)
		m_vProb[i].clear();
	m_vProb.clear();

	for (int i = 0; i < (int)(m_vSup.size()); i++)
		m_vSup[i].clear();
	m_vSup.clear();

	m_vFunValue.clear();

	//set the parameter.
	m_Para.m_K = Para.m_K;
	m_Para.m_NumOfVertices = Para.m_NumOfVertices;
	m_Para.m_MinVertexOfCluster = max(Para.m_K, Para.m_MinVertexOfCluster);
	m_Para.m_MinValue = Para.m_MinValue;
	m_Para.m_MaxProb = Para.m_MaxProb;
	m_Para.m_MinProb = Para.m_MinProb;
	m_Para.m_MinRewardDiff = Para.m_MinRewardDiff;
	
	return true;
}

//Main Function to Cluster the detection results.
bool CAffinityGraph::DecomposeGraph(IVectorDVector& vHyperEdges, IDVector& vCycleEdges, DecomposePara& Paras, IntArray& vSupport, DoubleArray& vProb, DoubleVector& vFValue)
{
	//reset the variables of the class.
	if (!Reset(Paras))
		return false;
	//clear the previous values in the output variable.
	for (int i = 0; i < (int)(vSupport.size()); i++)
		vSupport[i].clear();
	vSupport.clear();

	for (int i = 0; i < (int)(vProb.size()); i++)
		vProb[i].clear();
	vProb.clear();

	vFValue.clear();

	//************************************************************************************************************//
	//Passing the variables for the hyperedges.
	//vEdges variable
	int NumOfEntries = (int)(vHyperEdges.size());
	if (NumOfEntries == 0)
		return false;

	//���ǵļ�Ԫ��ϵ
	int Dimensions = Paras.m_K;
	//check the parameters.
	if (Dimensions != (int)((vHyperEdges[0]).Ivs.size())) {
		fprintf(stderr, "Dimension Errors in DecomposeGraph!\n");
		return false;
	}

	for(int i = 0; i < NumOfEntries; i++) {
		IntVectorDouble Ivd;
		Ivd.Ivs.clear();
		for(int j = 0; j < Dimensions; j++)
			Ivd.Ivs.push_back((vHyperEdges[i]).Ivs[j]);
		Ivd.D = vHyperEdges[i].D;
		m_vHyperEdges.push_back(Ivd);
	}	

    //Check the status of the graph, check whether cycle edge is empty or not.

	if (vCycleEdges.empty())
		m_CycleLabel = false; 
	else
		m_CycleLabel = true;

	if (m_CycleLabel) {
		//Case 1: with the cycle label.
		//Set the parameters for the self-cycle edges.
		//Number of nodes in the graph.
		int NumOfNodes = (int)(vCycleEdges.size());
		for (int i = 0; i < NumOfNodes; i++) 
			m_vCycleEdges.push_back(vCycleEdges[i]);
	}
	
	//*********************************************************************************************************************//
	//Generate the starting points.
	GenStartCoords(vHyperEdges);

	//run the clustering.
	InitDataStructure();
	ComputeClusters();

	//output the data.
	//get the support variables.
	int NumR = (int)(m_vSup.size());
	int NumC = 0;
	for (int i = 0; i < NumR; i++) {
		IntVector vS;
		NumC = (int)(m_vSup[i].size());
		for (int j = 0; j < NumC; j++)
			vS.push_back(m_vSup[i][j]);

		vSupport.push_back(vS);
	}

	NumR = (int)(m_vProb.size());
	for (int i = 0; i < NumR; i++) {
		DoubleVector vP;
		NumC = (int)(m_vProb[i].size());
		for (int j = 0; j < NumC; j++)
			vP.push_back(m_vProb[i][j]);
		vProb.push_back(vP);
	}

	NumR = (int)(m_vFunValue.size());
	for (int i = 0; i < NumR; i++)
		vFValue.push_back(m_vFunValue[i]);

	return true;
}


bool CAffinityGraph::GenStartCoords(IVectorDVector& AffMatrix)
{
	int NumOfEntry = (int)(AffMatrix.size());
	bool QuitMark = false;
	bool Indicator = true;
	for(int i = 0; i < m_Para.m_NumOfVertices; i++) {
		IDVector Idv;
		for(int j = 0; j < NumOfEntry; j++) {
			Indicator = false;
            for (int k = 0; k < (int)(AffMatrix[j].Ivs.size()); k++) {
				if ((AffMatrix[j].Ivs)[k] == i) {
					Indicator = true;
					break;
				}
            }

			if(Indicator) {
				IntDouble Id;
				Id.I = j;
				Id.D = AffMatrix[j].D;
				Idv.push_back(Id);
			}
		}

		//If one vertex is not appearing in the set, continue.
		if ((int)(Idv.size()) == 0)
			continue;

		QuickSortL(Idv,0,(int)(Idv.size()-1));

		IntVector Mark;
		IntVector NeighborIndex;
		for(int j = 0; j < m_Para.m_NumOfVertices; j++)
			Mark.push_back(0);

		NeighborIndex.push_back(i);
		Mark[i] = 1;

		QuitMark = false;
		for(int j = 0; j < (int)(Idv.size()); j++) {
			for (int k = 0; k < (int)(AffMatrix[j].Ivs.size()); k++) {
				int Index = Idv[j].I;
				if(Mark[(AffMatrix[Index]).Ivs[k]]==0) {
					NeighborIndex.push_back((AffMatrix[Index]).Ivs[k]);
					Mark[(AffMatrix[Index]).Ivs[k]] = 1;
				}
				if((int)(NeighborIndex.size()) >= m_Para.m_MinVertexOfCluster) {
					QuitMark = true;
					break;
				}
			}
			if (QuitMark)
				break;
		}
		m_vStartCoords.push_back(NeighborIndex);
	}

	//start point probability.
	int NumOfStartPoints = (int)(m_vStartCoords.size());
	DoubleVector vPorb;
	double belta = 1.0/m_Para.m_MinVertexOfCluster;

	for (int i = 0; i < m_Para.m_MinVertexOfCluster; i++)
		vPorb.push_back(belta);

	for(int i = 0; i < NumOfStartPoints; i++)	
		m_vSProb.push_back(vPorb);
	
	return true;
}