//object features: v.01: just utilize the RGB color features, 
//HOG gradient features, and LBP texture features..
#ifndef _FEATCLASS_H
#define _FEATCLASS_H

#include <util/defs.h>
//****************************************************************************************************//
class CHistogram
{
public:
	CHistogram();
	CHistogram(int nDim);
	CHistogram(const CHistogram* pFeat);
	virtual ~CHistogram();

	//Reset the variables.
	bool Reset();
	//Copy the variables.
	bool Copy(CHistogram* pFeat);
	bool Copy(CHistogram& pFeat);

public:
	//The dimension of the histogram.
	int m_nDim;
	//The histogram features.
	double* m_pHist;
};


//**************************************************************************************************//
//feature class, used for matching and other scenes.
class CFeature
{
public:
	CFeature();
	CFeature(const CFeature* pFeat);
	virtual ~CFeature();
	//Reset the feature of the object
	bool Reset();
	//Copy the feature of the object.
	bool Copy(CFeature* pFeat);
	bool Copy(CFeature& pFeat);

public:
	//--------------- RGB color features --------------//
	//-------- Including both the face and torso of the person.
	//RGB Color Histogram
	CHistogram* m_pColorFeat;
	//Histogram of Gradient Features
	CHistogram* m_pShapeFeat;
	//Local Binary Pattern Texture Features
	CHistogram* m_pTextFeat;
};


#endif