//Define some important classes. 

#ifndef _DEFS_H
#define _DEFS_H

#include <vector>
#include <set>
using namespace std;


#include <stdlib.h>
#include <stdio.h> 
#include <math.h>
#include <string>
#include <iostream>
#include <fstream>
#include <float.h>
#include <time.h>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <iterator>


///#include <graph_decomp/graph_decomp.h>
///#include <config/param.h>


using namespace std;


#define PI  3.1415926
#define DBL_E  2.718281828459
#define DBL_MINVAL         1.00000000000000000e-16
#define DBL_MAXVAL         1.00000000000000000e16

//A reasonable minimal value.
#define MIN_EPS 0.0001

//PI variable.
#define ERF_PI 3.1415926


//
// Basics
//

#ifndef VOID
#define VOID void
typedef char CHAR;
typedef short SHORT;
typedef long LONG;
#if !defined(MIDL_PASS)
typedef int INT;
#endif
#endif


typedef vector<int> IDSet;




typedef vector<int> IntVector;
typedef vector<IntVector> IntArray;
typedef vector<double> DoubleVector;
typedef vector<DoubleVector> DoubleArray;


//==========

typedef struct tagRECT
{
    LONG    left;
    LONG    top;
    LONG    right;
    LONG    bottom;
} RECT; /// , *PRECT, NEAR *NPRECT, FAR *LPRECT;

///typedef const RECT FAR* LPCRECT;

typedef struct _RECTL       /* rcl */
{
    LONG    left;
    LONG    top;
    LONG    right;
    LONG    bottom;
} RECTL; /// , *PRECTL, *LPRECTL;

///typedef const RECTL FAR* LPCRECTL;

typedef struct tagPOINT
{
    LONG  x;
    LONG  y;
} POINT; /// , *PPOINT, NEAR *NPPOINT, FAR *LPPOINT;

typedef struct _POINTL      /* ptl  */
{
    LONG  x;
    LONG  y;
} POINTL, *PPOINTL;

typedef struct tagSIZE
{
    LONG        cx;
    LONG        cy;
} SIZE; /// , *PSIZE, *LPSIZE;

///typedef SIZE               SIZEL;
///typedef SIZE               *PSIZEL, *LPSIZEL;

typedef struct tagPOINTS
{
#ifndef _MAC
    SHORT   x;
    SHORT   y;
#else
    SHORT   y;
    SHORT   x;
#endif
} POINTS; /// , *PPOINTS, *LPPOINTS;







typedef vector<POINT> vPointSet;


#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifndef MAX
#define MAX(x,y) ((x > y)? x : y)
#endif
#ifndef MIN
#define MIN(x,y) ((x < y)? x : y)
#endif
#ifndef ABS
#define ABS(x) ((x < 0)? -x : x)
#endif

//some macro definition
#define RELEASEARRAY(a) if (a) delete[] a
#define RELEASE(a) if (a) delete a



//*****************************************//
//***** Color Histogram Feature ***********//
#define RBIN_NUMBER 8
#define GBIN_NUMBER 8
#define BBIN_NUMBER 8

//Used for color features.
#define COLOR_DIMENSION 512 

#define RBIN_STEP 32
#define GBIN_STEP 32
#define BBIN_STEP 32

//****************************************//
//Used for shape features.
#define SHAPE_DIMENSION 18

//****************************************//
//Used for LBP features.
#define TEXTURE_DIMENSION 256


//-------------------------------------------------- Some Parameters Used for Clustering -----------------------------------------//
typedef set<int> IntSet;
//Used for Random Consensus Graph Converted to Pairwise Graph.
typedef map<int, int> I2IMap;

//Used for Random Consensus Graph Converted to Pairwise Graph.
typedef map<int, double> I2DMap;


typedef struct IntIntDouble_TYP {
	int I[2];
	double D;
}IntIntDouble;
typedef vector<IntIntDouble> IIDVector;

///typedef vector<IplImage*> pIplImageSet;


//**********************************************************************************************************************//

class CMask
{
	//Constructor function.
public:
	CMask() {
		m_nWidth = 320;
		m_nHeight = 240;
		m_pMask = new bool*[m_nHeight];
		for (int i = 0; i < m_nHeight; i++)
			m_pMask[i] = new bool[m_nWidth];

		for (int i = 0; i < m_nHeight; i++) {
			for (int j = 0; j < m_nWidth; j++) {
				m_pMask[i][j] = true;
			}
		}
	}

	CMask(int height, int width) {
		m_nWidth = width;
		m_nHeight = height;
		m_pMask = new bool*[m_nHeight];
		for (int i = 0; i < m_nHeight; i++)
			m_pMask[i] = new bool[m_nWidth];

		for (int i = 0; i < m_nHeight; i++) {
			for (int j = 0; j < m_nWidth; j++) {
				m_pMask[i][j] = true;
			}
		}
	}

	CMask(int height, int width, bool sign) {
		m_nWidth = width;
		m_nHeight = height;
		m_pMask = new bool*[m_nHeight];
		for (int i = 0; i < m_nHeight; i++)
			m_pMask[i] = new bool[m_nWidth];

		for (int i = 0; i < m_nHeight; i++) {
			for (int j = 0; j < m_nWidth; j++) {
				m_pMask[i][j] = sign;
			}
		}
	}

	virtual ~CMask() {
		for (int i = 0; i < m_nHeight; i++) {
			RELEASEARRAY(m_pMask[i]);
			m_pMask[i] = 0;
		}
		RELEASEARRAY(m_pMask);
		m_pMask = 0;
		m_nWidth = 0;
		m_nHeight = 0;
	}
	//Some variables
public:
	int m_nWidth;
	int m_nHeight;
	bool** m_pMask;
};



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

class CMSS
{
public:
	//The index set constructing the MSS.
	IntVector m_vIdx;
	double m_nAppConf;
	double m_nMoveConf;
	double m_nSmthConf;
	double m_nMixConf;

public:
	CMSS () {
		m_vIdx.clear();
		m_nAppConf = 0;
		m_nMoveConf = 0;
		m_nSmthConf = 0;
		m_nMixConf = 0;
	}
	~CMSS () {
		m_vIdx.clear();
		m_nAppConf = 0;
		m_nMoveConf = 0;
		m_nSmthConf = 0;
		m_nMixConf = 0;
	}
};

typedef vector<CMSS> MSSVector;


typedef class GraphExhaustOptParameter_TYP
{
public:
    //----------- Alpha value for each term in weight ---------//
	//Alpha value for appearance term.
	double m_nAppAlpha;
	//Alpha value for move term.
	double m_nMoveAlpha;
	//Alpha value for smooth term.
	double m_nSmthAlpha;
	
	//----------- Sigma value for each term in weight ---------//
	//Sigma value for appearance term.
	double m_nAppSigma;
	//Sigma value for smooth term.
	double m_nSmthSigma;
	//Sigma value for move term.
	double m_nMoveSigma;
	
	//----------- Upper bound for each terms in weight --------//
	//Appearance upper bound.
	double m_nAppUpperBound;
	//Smoothness upper bound.
	double m_nSmthUpperBound;
	//Move upper bound.
	double m_nMoveUpperBound;
	//Pairwise graph upper bound.
	double m_nMixUpperBound;

	//------------ Threshold parameters of each term ----------//
	//Appearance threshold.
	double m_nAppThreshold;
	//Smooth threshold.
	double m_nSmthThreshold;
	//Move threshold.
	double m_nMoveThreshold;
	
	//----------- Weight of the graph edges -----------------//
	//Appearance weight
	double m_nAppWeight;
	//Smoothness weight.
	double m_nSmthWeight;
	//Move weight.
	double m_nMoveWeight;
	
	//---------------------------------------------------------------------------------------------------//
	//---------------------------------------------------------------------------------------------------//
	//--- Useful functions
	//---------------------------------------------------------------------------------------------------//
public:
	GraphExhaustOptParameter_TYP () {
		//Alpha values.
		m_nAppAlpha = 0;
        m_nMoveAlpha = 0;
		m_nSmthAlpha = 0;
		
		//Sigma values
		m_nAppSigma = 0;
		m_nSmthSigma = 0;
		m_nMoveSigma = 0;

		//Bound values.
		m_nAppUpperBound = 0;
		m_nSmthUpperBound = 0;
		m_nMoveUpperBound = 0;
		m_nMixUpperBound = 0;

		//Threshold values.
		m_nAppThreshold = 0;
		m_nSmthThreshold = 0;
		m_nMoveThreshold = 0;
		
		//Weight values.
		m_nAppWeight = 0;
		m_nSmthWeight = 0;
		m_nMoveWeight = 0;
	}

	//Construction function with input.
	GraphExhaustOptParameter_TYP (GraphExhaustOptParameter_TYP* pParas) {
		//Alpha values
		m_nAppAlpha = pParas->m_nAppAlpha;
        m_nMoveAlpha = pParas->m_nMoveAlpha;
		m_nSmthAlpha = pParas->m_nSmthAlpha;
		
		//Sigma values
		m_nAppSigma = pParas->m_nAppSigma;
		m_nSmthSigma = pParas->m_nSmthSigma;
		m_nMoveSigma = pParas->m_nMoveSigma;
		
		//Bound values.
		m_nAppUpperBound = pParas->m_nAppUpperBound;
		m_nSmthUpperBound = pParas->m_nSmthUpperBound;
		m_nMoveUpperBound = pParas->m_nMoveUpperBound;
		m_nMixUpperBound = pParas->m_nMixUpperBound;

		//Threshold values.
		m_nAppThreshold = pParas->m_nAppThreshold;
		m_nSmthThreshold = pParas->m_nSmthThreshold;
		m_nMoveThreshold = pParas->m_nMoveThreshold;

		//Weight values.
		m_nAppWeight = pParas->m_nAppWeight;
		m_nSmthWeight = pParas->m_nSmthWeight;
		m_nMoveWeight = pParas->m_nMoveWeight;
	}

	~GraphExhaustOptParameter_TYP () {
		//Alpha values
        m_nAppAlpha = 0;
		m_nMoveAlpha = 0;
		m_nSmthAlpha = 0;
		
		//Sigma values
		m_nAppSigma = 0;
		m_nSmthSigma = 0;
		m_nMoveSigma = 0;
		
		//Bound values.
		m_nAppUpperBound = 0;
		m_nSmthUpperBound = 0;
		m_nMoveUpperBound = 0;
		m_nMixUpperBound = 0;

		//Threshold values.
		m_nAppThreshold = 0;
		m_nSmthThreshold = 0;
		m_nMoveThreshold = 0;

		//Weight values.
		m_nAppWeight = 0;
		m_nSmthWeight = 0;
		m_nMoveWeight = 0;
	}

	//Parameter set function.
	bool Set(GraphExhaustOptParameter_TYP& pParas) {
		//Alpha values
        m_nAppAlpha = pParas.m_nAppAlpha;
		m_nMoveAlpha = pParas.m_nMoveAlpha;
		m_nSmthAlpha = pParas.m_nSmthAlpha;

		//Sigma values
		m_nAppSigma = pParas.m_nAppSigma;
		m_nSmthSigma = pParas.m_nSmthSigma;
		m_nMoveSigma = pParas.m_nMoveSigma;

		//Bound values.
		m_nAppUpperBound = pParas.m_nAppUpperBound;
		m_nSmthUpperBound = pParas.m_nSmthUpperBound;
		m_nMoveUpperBound = pParas.m_nMoveUpperBound;
		m_nMixUpperBound = pParas.m_nMixUpperBound;

		//Threshold values.
		m_nAppThreshold = pParas.m_nAppThreshold;
		m_nSmthThreshold = pParas.m_nSmthThreshold;
		m_nMoveThreshold = pParas.m_nMoveThreshold;

		//Weight values.
		m_nAppWeight = pParas.m_nAppWeight;
		m_nSmthWeight = pParas.m_nSmthWeight;
		m_nMoveWeight = pParas.m_nMoveWeight;
		return true;
	}

}GraphExhaustOptPara;


typedef class GraphRandomOptParameter_TYP
{
public:
	//***************************** Parameters for MSS Generation.
    //Alpha Parameters for MSS Confidence.
	//MSS appearance alpha.
	double m_nMSSAppConfAlpha;
	//MSS move alpha.
	double m_nMSSMoveConfAlpha;
	//MSS smooth alpha.
	double m_nMSSSmthConfAlpha;
	
	//Sigma Parameters for MSS Confidence.
	//MSS appearance sigma.
	double m_nMSSAppConfSigma;
	//MSS move sigma.
	double m_nMSSMoveConfSigma;
	//MSS smooth sigma.
	double m_nMSSSmthConfSigma;

	//Weight Parameters for MSS Confidence.
	//MSS appearance weight.
	double m_nMSSAppConfWeight;
	//MSS move weight.
	double m_nMSSMoveConfWeight;
	//MSS smooth weight.
	double m_nMSSSmthConfWeight;

	//Threshold Parameters for MSS Confidence.
	//MSS appearance confidence threshold.
	double m_nMSSAppConfThresh;
	//MSS move confidence threshold.
	double m_nMSSMoveConfThresh;
	//MSS smooth confidence threshold.
	double m_nMSSSmthConfThresh;
	
	//-------------------------------------------------------------------------------------//
	//------------ Parameters for consensus graph construction.
	//Weight of the consensus graph confidence.
	//Appearance term.
	double m_nRCGAppConfWeight;
	//Move term.
	double m_nRCGMoveConfWeight;
	//Smooth term.
	double m_nRCGSmthConfWeight;

	//Threshold of the consensus graph confidence.
	//Appearance term.
	double m_nRCGAppConfThresh;
	//Move term.
	double m_nRCGMoveConfThresh;
	//Smooth term.
	double m_nRCGSmthConfThresh;
	
	//Mixture term.
	double m_nRCGMixConfThresh;

	//Upper bound of the consensus graph confidence.
	//Appearance term.
	double m_nRCGAppConfBound;
	//Move term.
	double m_nRCGMoveConfBound;
	//Smooth term.
	double m_nRCGSmthConfBound;
	//Mixture term.
	double m_nRCGMixConfBound;

	//Alpha value of the consensus graph confidence.
	//Appearance term.
	double m_nRCGAppConfAlpha;
	//Move term.
	double m_nRCGMoveConfAlpha;
	//Smooth term.
	double m_nRCGSmthConfAlpha;
	
	//Sigma value of the consensus graph confidence.
	//Appearance term.
	double m_nRCGAppConfSigma;
	//Move term.
	double m_nRCGMoveConfSigma;
	//Smooth term.
	double m_nRCGSmthConfSigma;
	
	//-------------------------------------------------------------------------------------//
	//------------ Parameters for pairwise graph construction.
	//Alpha value for the pairwise graph.
	double m_nPairGraphAlpha;
	//Sigma value for the pairwise graph.
	double m_nPairGraphSigma;
	//Upper bound value for the pairwise graph 
	double m_nPairGraphBound;
	//Threshold value for the pairwise graph
	double m_nPairGraphThresh;

	//---------------------------------------------------------------------------------------//
	//------------- Parameters for generating the MSSs.
	//Number of random samples for each node.
	int m_nNodeNumRandIters;
	//Number of iterations used to generate the MSSs.
	int m_nMaxMSSNumRandIters;
	//Number of random hyperedges.
	int m_nMaxNumHypMSSs;

	//**************************************************************************************//
	//******* Useful functions
public:
	GraphRandomOptParameter_TYP () {
        //Alpha parameters for MSS confidence.
		m_nMSSAppConfAlpha = 0;
		m_nMSSMoveConfAlpha = 0;
		m_nMSSSmthConfAlpha = 0;
		
		//Sigma parameters for MSS confidence.
		m_nMSSAppConfSigma = 0;
		m_nMSSMoveConfSigma = 0;
		m_nMSSSmthConfSigma = 0;

		//Weight parameters for MSS confidence.
		m_nMSSAppConfWeight = 0;
		m_nMSSMoveConfWeight = 0;
		m_nMSSSmthConfWeight = 0;

		//Threshold parameters for MSS confidence.
		m_nMSSAppConfThresh = 0;
		m_nMSSMoveConfThresh = 0;
		m_nMSSSmthConfThresh = 0;

		//Weight parameters for RCG.
		m_nRCGAppConfWeight = 0;
		m_nRCGMoveConfWeight = 0;
		m_nRCGSmthConfWeight = 0;

		//Threshold parameters for RCG.
		m_nRCGAppConfThresh = 0;
		m_nRCGMoveConfThresh = 0;
		m_nRCGSmthConfThresh = 0;
		m_nRCGMixConfThresh = 0;

		//Upper bound parameters for RCG.
		m_nRCGAppConfBound = 0;
		m_nRCGMoveConfBound = 0;
		m_nRCGSmthConfBound = 0;
		m_nRCGMixConfBound = 0;

		//Alpha parameters for RCG.
		m_nRCGAppConfAlpha = 0;
		m_nRCGMoveConfAlpha = 0;
		m_nRCGSmthConfAlpha = 0;

		//Sigma parameters for RCG.
		m_nRCGAppConfSigma = 0;
		m_nRCGMoveConfSigma = 0;
		m_nRCGSmthConfSigma = 0;
		
		//Parameters for pairwise graph.
		m_nPairGraphAlpha = 0;
		m_nPairGraphSigma = 0;
		m_nPairGraphBound = 0;
		m_nPairGraphThresh = 0;

		//Parameters for generating the MSSs.
		m_nMaxMSSNumRandIters = 0;
		m_nNodeNumRandIters = 0;
		m_nMaxNumHypMSSs = 0;
	}

	//Construction function with the input
	GraphRandomOptParameter_TYP (GraphRandomOptParameter_TYP* pParas) {
		//Alpha parameters for MSS confidence.
        m_nMSSAppConfAlpha = pParas->m_nMSSAppConfAlpha;
		m_nMSSMoveConfAlpha = pParas->m_nMSSMoveConfAlpha;
		m_nMSSSmthConfAlpha = pParas->m_nMSSSmthConfAlpha;
		
		//Sigma parameters for MSS confidence.
		m_nMSSAppConfSigma = pParas->m_nMSSAppConfSigma;
		m_nMSSMoveConfSigma = pParas->m_nMSSMoveConfSigma;
		m_nMSSSmthConfSigma = pParas->m_nMSSSmthConfSigma;
		
		//Weight parameters for MSS confidence.
		m_nMSSAppConfWeight = pParas->m_nMSSAppConfWeight;
		m_nMSSMoveConfWeight = pParas->m_nMSSMoveConfWeight;
		m_nMSSSmthConfWeight = pParas->m_nMSSSmthConfWeight;
		
		//Threshold parameters for MSS confidence.
		m_nMSSAppConfThresh = pParas->m_nMSSAppConfThresh;
		m_nMSSMoveConfThresh = pParas->m_nMSSMoveConfThresh;
		m_nMSSSmthConfThresh = pParas->m_nMSSSmthConfThresh;
	
		//Weight parameters for RCG.
		m_nRCGAppConfWeight = pParas->m_nRCGAppConfWeight;
		m_nRCGMoveConfWeight = pParas->m_nRCGMoveConfWeight;
		m_nRCGSmthConfWeight = pParas->m_nRCGSmthConfWeight;

		//Threshold parameters for RCG.
		m_nRCGAppConfThresh = pParas->m_nRCGAppConfThresh;
		m_nRCGMoveConfThresh = pParas->m_nRCGMoveConfThresh;
		m_nRCGSmthConfThresh = pParas->m_nRCGSmthConfThresh;
		m_nRCGMixConfThresh = pParas->m_nRCGMixConfThresh;

		//Upper bound parameters for RCG.
		m_nRCGAppConfBound = pParas->m_nRCGAppConfBound;
		m_nRCGMoveConfBound = pParas->m_nRCGMoveConfBound;
		m_nRCGSmthConfBound = pParas->m_nRCGSmthConfBound;
		m_nRCGMixConfBound = pParas->m_nRCGMixConfBound;

		//Alpha parameters for RCG.
		m_nRCGAppConfAlpha = pParas->m_nRCGAppConfAlpha;
		m_nRCGMoveConfAlpha = pParas->m_nRCGMoveConfAlpha;
		m_nRCGSmthConfAlpha = pParas->m_nRCGSmthConfAlpha;

		//Sigma parameters for RCG.
		m_nRCGAppConfSigma = pParas->m_nRCGAppConfSigma;
		m_nRCGMoveConfSigma = pParas->m_nRCGMoveConfSigma;
		m_nRCGSmthConfSigma = pParas->m_nRCGSmthConfSigma;

		//Parameters for pairwise graph.
		m_nPairGraphAlpha = pParas->m_nPairGraphAlpha;
		m_nPairGraphSigma = pParas->m_nPairGraphSigma;
		m_nPairGraphBound = pParas->m_nPairGraphBound;
		m_nPairGraphThresh = pParas->m_nPairGraphThresh;

		//Parameters for generating the MSSs.
		m_nMaxMSSNumRandIters = pParas->m_nMaxMSSNumRandIters;
		m_nNodeNumRandIters = pParas->m_nNodeNumRandIters;
		m_nMaxNumHypMSSs = pParas->m_nMaxNumHypMSSs;
	}

	~GraphRandomOptParameter_TYP () {
		//Alpha parameters for MSS confidence.
        m_nMSSAppConfAlpha = 0;
		m_nMSSMoveConfAlpha = 0;
		m_nMSSSmthConfAlpha = 0;

		//Sigma parameters for MSS confidence.
		m_nMSSAppConfSigma = 0;
		m_nMSSMoveConfSigma = 0;
		m_nMSSSmthConfSigma = 0;

		//Weight parameters for MSS confidence.
		m_nMSSAppConfWeight = 0;
		m_nMSSMoveConfWeight = 0;
		m_nMSSSmthConfWeight = 0;
	
		//Threshold parameters for MSS confidence.
		m_nMSSAppConfThresh = 0;
		m_nMSSMoveConfThresh = 0;
		m_nMSSSmthConfThresh = 0;
	
		//Weight parameters for RCG.
		m_nRCGAppConfWeight = 0;
		m_nRCGMoveConfWeight = 0;
		m_nRCGSmthConfWeight = 0;

		//Threshold parameters for RCG.
		m_nRCGAppConfThresh = 0;
		m_nRCGMoveConfThresh = 0;
		m_nRCGSmthConfThresh = 0;
		m_nRCGMixConfThresh = 0;

		//Upper bound parameters for RCG.
		m_nRCGAppConfBound = 0;
		m_nRCGMoveConfBound = 0;
		m_nRCGSmthConfBound = 0;
		m_nRCGMixConfBound = 0;

		//Alpha parameters for RCG.
		m_nRCGAppConfAlpha = 0;
		m_nRCGMoveConfAlpha = 0;
		m_nRCGSmthConfAlpha = 0;

		//Sigma parameters for RCG.
		m_nRCGAppConfSigma = 0;
		m_nRCGMoveConfSigma = 0;
		m_nRCGSmthConfSigma = 0;

		//Parameters for pairwise graph.
		m_nPairGraphAlpha = 0;
		m_nPairGraphSigma = 0;
		m_nPairGraphBound = 0;
		m_nPairGraphThresh = 0;

		//Parameters for generating the MSSs.
		m_nMaxMSSNumRandIters = 0;
		m_nNodeNumRandIters = 0;
		m_nMaxNumHypMSSs = 0;
	}

	//Parameter set function.
	//Construction function with the input
	bool Set(GraphRandomOptParameter_TYP& pParas) {
        //Alpha parameters for MSS confidence.
		m_nMSSAppConfAlpha = pParas.m_nMSSAppConfAlpha;
		m_nMSSMoveConfAlpha = pParas.m_nMSSMoveConfAlpha;
		m_nMSSSmthConfAlpha = pParas.m_nMSSSmthConfAlpha;
		
		//Sigma parameters for MSS confidence.
		m_nMSSAppConfSigma = pParas.m_nMSSAppConfSigma;
		m_nMSSMoveConfSigma = pParas.m_nMSSMoveConfSigma;
		m_nMSSSmthConfSigma = pParas.m_nMSSSmthConfSigma;

		//Weight parameters for MSS confidence.
		m_nMSSAppConfWeight = pParas.m_nMSSAppConfWeight;
		m_nMSSMoveConfWeight = pParas.m_nMSSMoveConfWeight;
		m_nMSSSmthConfWeight = pParas.m_nMSSSmthConfWeight;
	
		//Threshold parameters for MSS confidence.
		m_nMSSAppConfThresh = pParas.m_nMSSAppConfThresh;
		m_nMSSMoveConfThresh = pParas.m_nMSSMoveConfThresh;
		m_nMSSSmthConfThresh = pParas.m_nMSSSmthConfThresh;
	
		//Weight parameters for RCG.
		m_nRCGAppConfWeight = pParas.m_nRCGAppConfWeight;
		m_nRCGMoveConfWeight = pParas.m_nRCGMoveConfWeight;
		m_nRCGSmthConfWeight = pParas.m_nRCGSmthConfWeight;

		//Threshold parameters for RCG.
		m_nRCGAppConfThresh = pParas.m_nRCGAppConfThresh;
		m_nRCGMoveConfThresh = pParas.m_nRCGMoveConfThresh;
		m_nRCGSmthConfThresh = pParas.m_nRCGSmthConfThresh;
		m_nRCGMixConfThresh = pParas.m_nRCGMixConfThresh;

		//Upper bound parameters for RCG.
		m_nRCGAppConfBound = pParas.m_nRCGAppConfBound;
		m_nRCGMoveConfBound = pParas.m_nRCGMoveConfBound;
		m_nRCGSmthConfBound = pParas.m_nRCGSmthConfBound;
		m_nRCGMixConfBound = pParas.m_nRCGMixConfBound;

		//Alpha parameters for RCG.
		m_nRCGAppConfAlpha = pParas.m_nRCGAppConfAlpha;
		m_nRCGMoveConfAlpha = pParas.m_nRCGMoveConfAlpha;
		m_nRCGSmthConfAlpha = pParas.m_nRCGSmthConfAlpha;

		//Sigma parameters for RCG.
		m_nRCGAppConfSigma = pParas.m_nRCGAppConfSigma;
		m_nRCGMoveConfSigma = pParas.m_nRCGMoveConfSigma;
		m_nRCGSmthConfSigma = pParas.m_nRCGSmthConfSigma;
		
		//Parameters for pairwise graph.
		m_nPairGraphAlpha = pParas.m_nPairGraphAlpha;
		m_nPairGraphSigma = pParas.m_nPairGraphSigma;
		m_nPairGraphBound = pParas.m_nPairGraphBound;
		m_nPairGraphThresh = pParas.m_nPairGraphThresh;

		//Parameters for generating the MSSs.
		m_nMaxMSSNumRandIters = pParas.m_nMaxMSSNumRandIters;
		m_nNodeNumRandIters = pParas.m_nNodeNumRandIters;
		m_nMaxNumHypMSSs = pParas.m_nMaxNumHypMSSs;
		return true;
	}

}GraphRandomOptPara;



typedef struct LayerGraphOptParameter_TYP
{
public:
	//The current layer of the graph optimization.
	int m_nCurLayer;
    //The post-processing of the clusters.
	int m_nPostType;
	//Maximum speed of the target in the scene between two continuous frames.
	double m_nMaxVelocity;
	//Occlusion ratio threshold to decide the conflict
	double m_nOccConflictThresh;

	//Safe gap threshold to use the occlusion ratio in neighborhoods determining.
	int m_nSafeGapNeighThresh;

	//Occlusion ratio to decide whether the tracklets are neighborhoods.
	double m_nOccNeighThresh;
	
	//Number of object per-segment.
	int m_nSegSize;
	//Minimum length of the tracks in each segment;
	int m_nMinTrackLen;

	//General distance threshold for the tracklet K-Ary.
	double m_nGenTrackMaxDistNeigh;
	//General maximum frame constraint of the tracklet to be neighbor
	int m_nGenTrackMaxSegNeigh;

	//MSS distance threshold for the tracklet K-Ary.
	double m_nMSSTrackMaxDistNeigh;
	//MSS maximum frame constraint of the tracklet to be neighbor
	int m_nMSSTrackMaxSegNeigh;

	//---------------------------- Parameters for HyperGraph Clustering -----------------------------------------------//
	//Number of object in the K-Ary.
	int m_nTrackNumK;
	//Minimal number of nodes in a cluster of tracks clustering.
	int m_nTrackMinVertices;
   
	//The sample step to get the smooth of the tracklets constraints.
	int m_nTrackSmthSampleStep;
	//The sample step to get the move of the tracklets constraints.
	int m_nTrackMoveSampleStep;

	//Parameters for random optimization.
	GraphRandomOptPara* m_pRandomOptParas;

	//Parameters for exhaust optimization.
	GraphExhaustOptPara* m_pExhaustOptParas;

	//-------------------------------------------------------------------------------------------------------------------//
	//-------------------------------------- Compensate Factor of Tracklet Layer ---------------------------------------//
	double m_nAppCompFactor;

	//********* Useful functions
	//----------------------------------------------------------------------------------//
	//----------------------------------------------------------------------------------//
	//------ Construction function.
	LayerGraphOptParameter_TYP() {
		m_nCurLayer = 0;

		m_nPostType = 1;
		m_nMaxVelocity = 100;
		m_nOccConflictThresh = 0.4;

		m_nSafeGapNeighThresh = 5;
		m_nOccNeighThresh = 0.2;

		m_nSegSize = 0;
		m_nMinTrackLen = 1;

		m_nGenTrackMaxDistNeigh = 200;
		m_nGenTrackMaxSegNeigh = 5;

		m_nMSSTrackMaxDistNeigh = 0;
		m_nMSSTrackMaxSegNeigh = 0;

		m_nTrackNumK = 0;
		m_nTrackMinVertices = 0;
		//Sampling variables for smooth and moving aspects.
		m_nTrackSmthSampleStep = 0;
	    m_nTrackMoveSampleStep = 0;
		//Parameters for hypergraph.
	    m_pRandomOptParas = new GraphRandomOptPara;
		//Parameters for pairwise graph.
	    m_pExhaustOptParas = new GraphExhaustOptPara;
		//Compensate factor for current tracklet.
		m_nAppCompFactor = 0;
	}

	LayerGraphOptParameter_TYP(LayerGraphOptParameter_TYP* pParas) {
		m_nCurLayer = pParas->m_nCurLayer;

		m_nPostType = pParas->m_nPostType;
		m_nMaxVelocity = pParas->m_nMaxVelocity;

		m_nOccConflictThresh = pParas->m_nOccConflictThresh;
		m_nSafeGapNeighThresh = pParas->m_nSafeGapNeighThresh;
		m_nOccNeighThresh = pParas->m_nOccNeighThresh;

		m_nSegSize = pParas->m_nSegSize;
		m_nMinTrackLen = pParas->m_nMinTrackLen;

		m_nGenTrackMaxDistNeigh = pParas->m_nGenTrackMaxDistNeigh;
		m_nGenTrackMaxSegNeigh = pParas->m_nGenTrackMaxSegNeigh;

		m_nMSSTrackMaxDistNeigh = pParas->m_nMSSTrackMaxDistNeigh;
		m_nMSSTrackMaxSegNeigh = pParas->m_nMSSTrackMaxSegNeigh;

		m_nTrackNumK = pParas->m_nTrackNumK;
		m_nTrackMinVertices = pParas->m_nTrackMinVertices;
		//Sampling variables for smooth and moving aspects.
		m_nTrackSmthSampleStep = pParas->m_nTrackSmthSampleStep;
		m_nTrackMoveSampleStep = pParas->m_nTrackMoveSampleStep;
		//Parameters for hypergraph.
		m_pRandomOptParas = new GraphRandomOptPara(pParas->m_pRandomOptParas);
		//Parameters for pairwise graph.
		m_pExhaustOptParas = new GraphExhaustOptPara(pParas->m_pExhaustOptParas);
		//Compensate factor for current tracklet.
		m_nAppCompFactor = pParas->m_nAppCompFactor;
	}

	~LayerGraphOptParameter_TYP() {
		m_nCurLayer = 0;
	
		m_nPostType = 0;
		m_nMaxVelocity = 0;
		
		m_nOccConflictThresh = 0;
        m_nSafeGapNeighThresh = 0;
		m_nOccNeighThresh = 0;

		m_nSegSize = 0;
		m_nMinTrackLen = 0;

		m_nGenTrackMaxDistNeigh = 0;
		m_nGenTrackMaxSegNeigh = 0;

		m_nMSSTrackMaxDistNeigh = 0;
		m_nMSSTrackMaxSegNeigh = 0;

		m_nTrackNumK = 0;
		m_nTrackMinVertices = 0;
		m_nTrackSmthSampleStep = 0;
		m_nTrackMoveSampleStep = 0;
		//Parameters for hypergraph.
		if (m_pRandomOptParas) {
			RELEASE(m_pRandomOptParas);
			m_pRandomOptParas = 0;
		}
		//Parameters for pairwise graph.
		if (m_pExhaustOptParas) {
			RELEASE(m_pExhaustOptParas);
			m_pExhaustOptParas = 0;
		}
		//Compensate factor for current tracklet.
		m_nAppCompFactor = 0;
	}

	//Parameter set function.
	bool Set(LayerGraphOptParameter_TYP& pParas) {
		m_nCurLayer = pParas.m_nCurLayer;

		m_nPostType = pParas.m_nPostType;
		m_nMaxVelocity = pParas.m_nMaxVelocity;

		m_nOccConflictThresh = pParas.m_nOccConflictThresh;
        m_nSafeGapNeighThresh = pParas.m_nSafeGapNeighThresh;
		m_nOccNeighThresh = pParas.m_nOccNeighThresh;

		m_nSegSize = pParas.m_nSegSize;
		m_nMinTrackLen = pParas.m_nMinTrackLen;

		m_nGenTrackMaxDistNeigh = pParas.m_nGenTrackMaxDistNeigh;
		m_nGenTrackMaxSegNeigh = pParas.m_nGenTrackMaxSegNeigh;

		m_nMSSTrackMaxDistNeigh = pParas.m_nMSSTrackMaxDistNeigh;
		m_nMSSTrackMaxSegNeigh = pParas.m_nMSSTrackMaxSegNeigh;

		m_nTrackNumK = pParas.m_nTrackNumK;
		m_nTrackMinVertices = pParas.m_nTrackMinVertices;
		//Sampling variables for smooth and moving aspects.
		m_nTrackSmthSampleStep = pParas.m_nTrackSmthSampleStep;
		m_nTrackMoveSampleStep = pParas.m_nTrackMoveSampleStep;
		//Parameters for hypergraph.
		m_pRandomOptParas->Set(*(pParas.m_pRandomOptParas));
		//Parameters for pairwise graph.
		m_pExhaustOptParas->Set(*(pParas.m_pExhaustOptParas));
		//Compensate factor for current tracklet.
		m_nAppCompFactor = pParas.m_nAppCompFactor;
		return true;
	}

}LayerGraphOptPara;

//--------------------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------------------//

typedef struct ExportParameter_TYP
{
	//-------------------------------------- Tracking Parameters -----------------------------------------------------//
	//detection file path of the tracker.
	const char* m_gImportDetFilePath;
	//input video file path.
 	const char* m_gImportVideoFilePath;
	//output tracking results path.
	const char* m_gExportVideoFilePath;
	//output tracking speed results path.
	const char* m_gExportSpeedFilePath;
	//output trajectory path.
	const char* m_gExportTrajFilePath;
	//name of the video data
	const char* m_gDataName;

	//image width.
	int m_nWidth;
	//image height.
	int m_nHeight;

	//---------------------- Used for Debug --------------------------//
	//Debug mark. 1:save debug; 0: do not save debug
	int m_gDebugMark;
	//Tracklets image
	const char* m_gTracksImagePath_Debug;
	//Target image
	const char* m_gTargetsImagePath_Debug;
	//Occupy image
	const char* m_gOccupyImagePath_Debug;

}ExportPara;


//parameter class.
typedef struct GraphTrackerParameter_TYP
{
	//The version of the optimization algorithms.
	int m_nVersion;

	//Number of layers used in optimization.
	int m_nOptLayers;
    //Number of layers for hypergraph.
	int m_nHypLayers;
	//Number of layers for pairwise graph.
	int m_nPairLayers;

	//Optimization Parameters.
	LayerGraphOptPara* m_pOptLayerParas;
	//Tracking Path Parameters.
	ExportPara* m_pExportInfos;
	
	GraphTrackerParameter_TYP(int nHypers, int nPairs) {
        //Number of optimization layers.
		m_nOptLayers = nHypers + nPairs;
		//Number of layers for hypergraph.
		m_nHypLayers = nHypers;
		//Number of layers for pairwise graph.
		m_nPairLayers = nPairs;
        //Apply the memory for the layer parameters.
		m_pOptLayerParas = new LayerGraphOptPara[m_nOptLayers];
		m_pExportInfos = new ExportPara;
	}

	~GraphTrackerParameter_TYP() {
		if (m_pOptLayerParas) {
			RELEASEARRAY(m_pOptLayerParas);
			m_pOptLayerParas = 0;
		}
		if (m_pExportInfos) {
			RELEASE(m_pExportInfos);
			m_pExportInfos = 0;
		}
		
		//Reset the parameter array.
		m_nOptLayers = 0;
		m_nHypLayers = 0;
		m_nPairLayers = 0;
	}

}CGraphTrackerPara;


typedef struct Palette_TYP
{
	int m_nR;
	int m_nG;
	int m_nB;
}Palette;


struct dRECT
{
	double m_nLeft;
	double m_nRight;
	double m_nTop;
	double m_nBottom;
};


//*********************************************************************************************************************//
//Detection ID Dispatcher Class
//*********************************************************************************************************************//

class CAllotID
{
private:
	//------------------------------------ Detection ID Dispatcher --------------------------------------//
	//id counter
	int m_nIDCount;
	//occupied ID set
	IDSet* m_pOccupyID;
	//free ID set
	IDSet* m_pFreeID;
public:
	//-------------------------------------------- ID generator -------------------------------------------------//
	CAllotID();
	virtual ~CAllotID();
	//This ID generator is mainly used for bounding box ID generation.
	//ID generator: add new ids to the free ID set, if it is less than a threshold.
	bool AppendIDs();
	//reclaim the old ID. ½«±»É¾³ýµÄÄ¿±êIDÖØÐÂ»ØÊÕÀûÓÃ
	bool ReclaimIDs(int id);
	//dispatch a ID to a target.
	int DispatchID();

	//Reset the ID allocator.
	bool Reset();
};



///#include "graph_decomp.h"

//********************************************************************//
//** Define some 


typedef set<int> IntSet;


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