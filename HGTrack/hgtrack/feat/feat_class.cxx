/*
 * GraphMultiCarTracker
 * Do not distribute
 *  Created on: Nov 08, 2015
 *      Author: Longyin Wen
 */
#include "feat_class.h"
#include <cstring> // for memcpy

CHistogram::CHistogram()
{
	//The dimension of the histogram
	m_nDim = 0;
	m_pHist = 0;
}

CHistogram::CHistogram(int nDim)
{
	//The dimension of the histogram
	m_nDim = nDim;
	//The data term of the histogram
	m_pHist = new double[m_nDim];
	//Set zeros.
	for (int i = 0; i < m_nDim; i++) {
		m_pHist[i] = 0;
	}
}

CHistogram::CHistogram(const CHistogram* pFeat)
{
	m_nDim = pFeat->m_nDim;

	m_pHist = new double[m_nDim];
	for (int i = 0; i < m_nDim; i++) {
		m_pHist[i] = 0;
	}
	//Copy the data values.
	memcpy((void*)m_pHist, (void*)(pFeat->m_pHist), sizeof(double) * m_nDim);
}

CHistogram::~CHistogram()
{
	if (m_pHist) {
		RELEASEARRAY(m_pHist);
		m_pHist = 0;
	}
	m_nDim = 0;
}

//Reset the feature vector of the object
bool CHistogram::Reset()
{
	for (int i = 0; i < m_nDim; i++)
		m_pHist[i] = 0;
	return true;
}

//Copy the feature vector of the object
bool CHistogram::Copy(CHistogram *pFeat)
{
	if (m_nDim != pFeat->m_nDim) {
		//Copy the feature dimension.
		m_nDim = pFeat->m_nDim;
		//Release previous memories.
		if (m_pHist) {
			RELEASEARRAY(m_pHist);
			m_pHist = 0;
		}
		m_pHist = new double[m_nDim];
		for (int i = 0; i < m_nDim; i++)
			m_pHist[i] = 0;
		memcpy((void*)m_pHist, (void*)(pFeat->m_pHist), sizeof(double) * m_nDim);
	}
	else {
		memcpy((void*)m_pHist, (void*)(pFeat->m_pHist), sizeof(double) * m_nDim);
	}

	return true;
}

bool CHistogram::Copy(CHistogram &pFeat)
{
	if (m_nDim != pFeat.m_nDim) {
		//Copy the feature dimension.
		m_nDim = pFeat.m_nDim;
		//Release previous memories.
		if (m_pHist) {
			RELEASEARRAY(m_pHist);
			m_pHist = 0;
		}
		m_pHist = new double[m_nDim];
		for (int i = 0; i < m_nDim; i++)
			m_pHist[i] = 0;
		memcpy((void*)m_pHist, (void*)(pFeat.m_pHist), sizeof(double) * m_nDim);
	}
	else {
		memcpy((void*)m_pHist, (void*)(pFeat.m_pHist), sizeof(double) * m_nDim);
	}
	return true;
}



//************************************************************************************************************************//
//************************************************************************************************************************//
CFeature::CFeature()
{
	//RGB color histogram features.
	m_pColorFeat = new CHistogram(COLOR_DIMENSION);
	//HOG shape histogram features.
	m_pShapeFeat = new CHistogram(SHAPE_DIMENSION);
	//LBP texture histogram features.
	m_pTextFeat = new CHistogram(TEXTURE_DIMENSION);
}

CFeature::CFeature(const CFeature* pFeat)
{
	//RGB color histogram features.
	m_pColorFeat = new CHistogram(pFeat->m_pColorFeat);
	//Elliptical shape histogram features.
	m_pShapeFeat = new CHistogram(pFeat->m_pShapeFeat);
	//Texture SPM histogram features.
	m_pTextFeat = new CHistogram(pFeat->m_pTextFeat);
}

CFeature::~CFeature()
{
	if (m_pColorFeat) {
		RELEASE(m_pColorFeat);
		m_pColorFeat = 0;
	}
	if (m_pShapeFeat) {
		RELEASE(m_pShapeFeat);
		m_pShapeFeat = 0;
	}
	if (m_pTextFeat) {
		RELEASE(m_pTextFeat);
		m_pTextFeat = 0;
	}
}


//Reset the feature vector of the object
bool CFeature::Reset()
{
	//Reset the color features.
	m_pColorFeat->Reset();
	//Reset the elliptical shape features.
	m_pShapeFeat->Reset();
	//Reset the texture bags of local feature.
	m_pTextFeat->Reset();

	return true;
}

bool CFeature::Copy(CFeature* pFeat)
{
	//Copy the color features.
	m_pColorFeat->Copy(pFeat->m_pColorFeat);
	//Copy the shape features.
	m_pShapeFeat->Copy(pFeat->m_pShapeFeat);
	//Copy the texture features.
	m_pTextFeat->Copy(pFeat->m_pTextFeat);

	return true;
}

bool CFeature::Copy(CFeature& pFeat)
{
	//Copy the color features.
	m_pColorFeat->Copy(pFeat.m_pColorFeat);
	//Copy the shape features.
	m_pShapeFeat->Copy(pFeat.m_pShapeFeat);
	//Copy the texture features.
	m_pTextFeat->Copy(pFeat.m_pTextFeat);

	return true;
}




