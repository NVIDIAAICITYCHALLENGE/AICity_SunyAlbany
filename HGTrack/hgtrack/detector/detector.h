/*
 * GraphMultiCarTracker
 * Do not distribute
 *  Created on: Nov 08, 2015
 *      Author: Longyin Wen
 */
//detector.h  
//************************************************************************//
//getting the detection results.
//************************************************************************//

#include <util/tracklet.h>
#include <config/config.h>

struct CFrameDets
{
	int m_nFrameIndex;
	int m_nNum;
	pDetBoxSet* m_pBoxes;
	CFrameDets() {
		m_nFrameIndex = -1;
		m_nNum = 0;
		m_pBoxes = new pDetBoxSet;
		m_pBoxes->clear();
	}
	~CFrameDets() {
		if (m_pBoxes) {
			for (int i = 0; i < (int)(m_pBoxes->size()); i++) {
				RELEASE((*m_pBoxes)[i]);
				(*m_pBoxes)[i] = 0;
			}
			m_pBoxes->clear();
			RELEASE(m_pBoxes);
			m_pBoxes = 0;
		}
		m_nFrameIndex = -1;
		m_nNum = 0;
	}
};

//reading the existing detection results or utilize the detector to get the results.
class CDetector
{
public:
	CDetector();
	CDetector(int nWidth, int nHeight);
	virtual ~CDetector();
public:
	//Detection results in our own format
	bool LoadDetections(const char *pDetPath);

	//image size of the detection.
	int m_nWidth;
	int m_nHeight;

public:
	map<int, CFrameDets*>* m_pDetections;
};