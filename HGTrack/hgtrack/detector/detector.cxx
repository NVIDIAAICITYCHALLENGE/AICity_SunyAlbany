/*
 * GraphMultiCarTracker
 * Do not distribute
 *  Created on: Nov 08, 2015
 *      Author: Longyin Wen
 */
//detector.cpp

#include "detector.h"

CDetector::CDetector()
{
	m_pDetections = new map<int, CFrameDets*>;
	m_pDetections->clear();

	m_nWidth = 320;
	m_nHeight = 240;
}

CDetector::CDetector(int nWidth, int nHeight)
{
	m_pDetections = new map<int, CFrameDets*>;
	m_pDetections->clear();

	m_nWidth = nWidth;
	m_nHeight = nHeight;
}

CDetector::~CDetector()
{
	if (m_pDetections) {
		map<int, CFrameDets*>::iterator iter_det;
        for (iter_det = m_pDetections->begin(); iter_det != m_pDetections->end(); iter_det++) {
			RELEASE(iter_det->second);
			iter_det->second = 0;
        }
		m_pDetections->clear();
		RELEASE(m_pDetections);
		m_pDetections = 0;
	}

	m_nWidth = 0;
	m_nHeight = 0;
}


bool CDetector::LoadDetections(const char *pDetPath)
{
	fstream fdetfile;
	fdetfile.open(pDetPath, ios::in);
	if (fdetfile.fail()) {
		fprintf(stderr, "Errors in opening the input detection results\n");
		return false;
	}

	vector<string> cfgVariable;
	string temp;

	while(getline(fdetfile, temp)) {
		if (temp.size() != 0) 
			cfgVariable.push_back(temp);
	}
	fdetfile.close();

	double* boxvalue = new double[7];
	for (int i = 0; i < 7; i++)
		boxvalue[i] = 0;

	//some temporal variables.
	double left, right, top, bottom;

	double xc, yc;
	int width, height;
	double conf;

	//map frame index.
	int mapidx = 0;

	//get the detection results.
	char seps[] = ",";
	char* pToken;
	char pLine[1024];
	int nct = 0;
	int thisfid = -1;
	int nextfid = -1;
	CFrameDets* thisfdets = 0;

	vector<string>::iterator iter;
	for (iter = cfgVariable.begin(); iter != cfgVariable.end(); iter++) {
		//YI: no _Copy_s in linux
		//iter->_Copy_s(pLine, 1024, iter->length());
		iter->copy(pLine, iter->length());
		pToken = strtok(pLine, seps);
		nct = 0;
		while(pToken != NULL) {
			boxvalue[nct] = atof(pToken);
			pToken = strtok(NULL, seps);
			nct++;
		}

		nextfid = (int)(boxvalue[0]);

		left = boxvalue[2] - 1;
		top = boxvalue[3] - 1;
		right = boxvalue[4] - 1;
		bottom = boxvalue[5] - 1;
		conf = boxvalue[6];

		//check whether the detection box satisfied the requirements. ��֤����������ͼ��ƽ����
		left = max((double)(0), left);
		right = min((double)(m_nWidth - 1), right);
		top = max((double)(0), top);
		bottom = min((double)(m_nHeight - 1), bottom);

		xc = (right + left) / 2;
		yc = (top + bottom) / 2;
		width = (int)(right - left);
		height = (int)(bottom - top);

		for (int i = 0; i < 7; i++) 
			boxvalue[i] = 0;

		//Check the detection response satisfied the size requirement.
		if ((width >= G_CAR_MIN_WIDTH)&&(height >= G_CAR_MIN_HEIGHT)) {
			CDetBBox* detbox = new CDetBBox;
			detbox->m_nFrameIndex = mapidx;

			detbox->m_n2Dcx = xc;
			detbox->m_n2Dcy = yc;
			detbox->m_nWidth = width;
			detbox->m_nHeight = height;

			detbox->m_nConf = conf;

			//check the frame id
			if (nextfid != thisfid) {
				//get the detection results in a frame.
				if (thisfdets) {
					thisfdets->m_nNum = (int)((thisfdets->m_pBoxes)->size());
					m_pDetections->insert(make_pair(mapidx, thisfdets));
					mapidx = mapidx + nextfid - thisfid;
					thisfdets = 0;
					thisfdets = new CFrameDets;
					thisfdets->m_nFrameIndex = mapidx;
					//push in the objects in the frame.
					(thisfdets->m_pBoxes)->push_back(detbox);
				}
				else {
					thisfdets = new CFrameDets;
					thisfdets->m_nFrameIndex = mapidx;
					//push in the objects in the frame.
					(thisfdets->m_pBoxes)->push_back(detbox);
				}
			}
			else {
				//push in the objects in the frame.
				(thisfdets->m_pBoxes)->push_back(detbox);
			}
			thisfid = nextfid;
		}
	}
	//Push the final frame responses.
	thisfdets->m_nNum = (int)((thisfdets->m_pBoxes)->size());
	m_pDetections->insert(make_pair(mapidx, thisfdets));

	if (boxvalue) {
		RELEASEARRAY(boxvalue);
		boxvalue = 0;
	}

	return true;
}
