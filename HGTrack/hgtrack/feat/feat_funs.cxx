#include "feat_funs.h"

//**************************************************************************************************************//
//Chi-Square similarity.
double CalcChiSimilarity(const CHistogram& pFeat1, const CHistogram& pFeat2)
{
	double nSim = 0;
	//check the dimension is equal or not.
	if (pFeat1.m_nDim != pFeat2.m_nDim) {
		fprintf(stderr, "The dimension is not matched in CalcChiSimilarity!\n");
		return 0;
	}

	nSim = 0;
	double v1 = 0;
	double v2 = 0;
	for(int i = 0; i < pFeat1.m_nDim; ++i) {
		v1 = (pFeat1.m_pHist)[i];
		v2 = (pFeat2.m_pHist)[i];
		if(v1 + v2 != 0)
			nSim += 2 * v1 * v2 /(v1 + v2);
	}
	return nSim;
}

double CalcChiSimilarity(const CHistogram* pFeat1, const CHistogram* pFeat2)
{
	double nSim = 0;
	//check the dimension is equal or not.
	if (pFeat1->m_nDim != pFeat2->m_nDim) {
		fprintf(stderr, "The dimension is not matched in CalcChiSimilarity!\n");
		return 0;
	}

	nSim = 0;
	double v1 = 0;
	double v2 = 0;
	for(int i = 0; i < pFeat1->m_nDim; ++i) {
		v1 = (pFeat1->m_pHist)[i];
		v2 = (pFeat2->m_pHist)[i];
		if(v1 + v2 != 0)
			nSim += 2 * v1 * v2 /(v1 + v2);
	}
	return nSim;
}


//Bhattacharyya similarity.
double CalcBhatSimilarity(const CHistogram& pFeat1, const CHistogram& pFeat2)
{
	//Calculate the dimension is matched or not.
	if (pFeat1.m_nDim != pFeat2.m_nDim) {
		fprintf(stderr, "The dimension is not matched in CalcBhatSimilarity!\n");
		return 0;
	}
	double nSim = 0;
	for (int i = 0; i < pFeat1.m_nDim; i++) {
		nSim += sqrt(pFeat1.m_pHist[i] * pFeat2.m_pHist[i]);
	}
	return nSim;
}

double CalcBhatSimilarity(const CHistogram* pFeat1, const CHistogram* pFeat2)
{
	//Calculate the dimension is matched or not.
	if (pFeat1->m_nDim != pFeat2->m_nDim) {
		fprintf(stderr, "The dimension is not matched in CalcBhatSimilarity!\n");
		return 0;
	}
	double nSim = 0;
	for (int i = 0; i < pFeat1->m_nDim; i++) {
		nSim += sqrt(pFeat1->m_pHist[i] * pFeat2->m_pHist[i]);
	}
	return nSim;
}



//Get the Similarity Between the Two Features.
double CalcFeatSimilarity(const CFeature* pFeat1, const CFeature* pFeat2)
{
	double nConf = 0;
	//Color, shape, and texture scores.
	double nColor, nShape, nText;

	//Get the color score of the features.
	nColor = CalcBhatSimilarity(pFeat1->m_pColorFeat, pFeat2->m_pColorFeat);
	nColor = G_APP_COLOR_WEIGHT * nColor;

	//Get the shape score of the features.
	nShape = CalcBhatSimilarity(pFeat1->m_pShapeFeat, pFeat2->m_pShapeFeat);
	nShape = G_APP_SHAPE_WEIGHT * nShape;

	//Get the texture score of the features.
	nText = CalcBhatSimilarity(pFeat1->m_pTextFeat, pFeat2->m_pTextFeat);
	nText = G_APP_TEXTURE_WEIGHT * nText;

	nConf = nColor + nShape + nText;

	return nConf;
}

//Get the Similarity Between the Two Features.
double CalcFeatSimilarity(const CFeature& pFeat1, const CFeature& pFeat2)
{
	double nConf = 0;
	//Color, shape, and texture scores.
	double nColor, nShape, nText;

	//Get the color score of the features.
	nColor = CalcBhatSimilarity(pFeat1.m_pColorFeat, pFeat2.m_pColorFeat);
	nColor = G_APP_COLOR_WEIGHT * nColor;

	//Get the shape score of the features.
	nShape = CalcBhatSimilarity(pFeat1.m_pShapeFeat, pFeat2.m_pShapeFeat);
	nShape = G_APP_SHAPE_WEIGHT * nShape;

	//Get the texture score of the features.
	nText = CalcBhatSimilarity(pFeat1.m_pTextFeat, pFeat2.m_pTextFeat);
	nText = G_APP_TEXTURE_WEIGHT * nText;

	nConf = nColor + nShape + nText;

	return nConf;
}


//-----------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------//
//Color Features.
//-----------------------------------------------------------------------------------------------------------------------------//
bool ExtractColorFeature(CDetBBox* pBox)
{
	double nVotes;
	int nIdxR;
	int nIdxG;
	int nIdxB;
	CHistogram* pFeat = pBox->m_pDetFeat->m_pColorFeat;
	pFeat->Reset();

	//Size of the feature extraction region.
	int nWidth = pBox->m_pFeatPict->width;
	int nHeight = pBox->m_pFeatPict->height;

	//Extract the RGB histogram of the box.
	//Extract the Features in Each Cell
	for (int i = 0; i < nWidth; i++) {
		for (int j = 0; j < nHeight; j++) {
			//Red channel
			nIdxR = (int)CV_IMAGE_ELEM(pBox->m_pFeatPict, uchar, j, i * 3 + 2) / RBIN_STEP;
			//Green channel
			nIdxG = (int)CV_IMAGE_ELEM(pBox->m_pFeatPict, uchar, j, i * 3 + 1) / GBIN_STEP;
			//Blue channel
			nIdxB = (int)CV_IMAGE_ELEM(pBox->m_pFeatPict, uchar, j, i * 3) / BBIN_STEP;
			//Get the bin of voting.
			(pFeat->m_pHist)[nIdxR * GBIN_NUMBER * BBIN_NUMBER + nIdxG * BBIN_NUMBER + nIdxB] += 1;
		}
	}

	//Normalize the histogram.
	nVotes = 0;
	for (int i = 0; i < pFeat->m_nDim; i++)
		nVotes += (pFeat->m_pHist)[i];
	if (nVotes > 0) {
		for (int i = 0; i < pFeat->m_nDim; i++)
			(pFeat->m_pHist)[i] /= nVotes;
	}
	
	//Set zeros.
	pFeat = 0;

	return true;
}

//---------------------------------------------------------------------------------------------------------//
//Extract the HOG gradient features in the region.
bool ExtractShapeFeature(CDetBBox* pBox)
{
	int nWidth = pBox->m_pFeatPict->width;
	int nHeight = pBox->m_pFeatPict->height;

	CHistogram* pFeat = pBox->m_pDetFeat->m_pShapeFeat;
	//Reset previous memory.
	pFeat->Reset();

	//Get the gray scale image of the feature extraction region.
	IplImage* pGray = cvCreateImage(cvGetSize(pBox->m_pFeatPict), 8, 1);
	cvCvtColor(pBox->m_pFeatPict, pGray, CV_RGB2GRAY);

	//Extract the Gradient Map of the Image.
	float** pDx = new float*[nHeight];
	for (int i = 0; i < nHeight; i++)
		pDx[i] = new float[nWidth];


	float** pDy = new float*[nHeight];
	for (int i = 0; i < nHeight; i++)
		pDy[i] = new float[nWidth];

	for (int i = 0; i < nHeight; i++) {
		for (int j = 0; j < nWidth; j++) {
			pDx[i][j] = 0;
			pDy[i][j] = 0;
		}
	}

	//Get the X Ordinate Gradient Map
	for (int i = 0; i < nHeight; i++) {
		for (int j = 1; j < nWidth - 1; j++) {
			pDx[i][j] = (float)CV_IMAGE_ELEM(pGray, uchar, i, j + 1) 
				- (float)CV_IMAGE_ELEM(pGray, uchar, i, j - 1);
		}
	}

	for (int i = 0; i < nHeight; i++) {
		pDx[i][0] = (float)CV_IMAGE_ELEM(pGray, uchar, i, 1);
	}

	for (int i = 0; i < nHeight; i++) {
		pDx[i][nWidth - 1] = - (float)CV_IMAGE_ELEM(pGray, uchar, i, nWidth - 2);
	}

	//Get the Y Ordinate Gradient Map
	for (int i = 1; i < nHeight - 1; i++) {
		for (int j = 0; j < nWidth; j++) {
			pDy[i][j] = (float)CV_IMAGE_ELEM(pGray, uchar, i + 1, j) 
				- (float)CV_IMAGE_ELEM(pGray, uchar, i - 1, j);
		}
	}

	for (int i = 0; i < nWidth; i++) {
		pDy[0][i] = (float)CV_IMAGE_ELEM(pGray, uchar, 1, i);
	}

	for (int i = 0; i < nWidth; i++) {
		pDy[nHeight - 1][i] = - (float)CV_IMAGE_ELEM(pGray, uchar, nHeight - 2, i);
	}

	float nMagn;
	float nSum;
	float nOrient;
	int nIdx;
	float nRatio;
	float nDiff;

	//Extract the HoG Features.
	for (int x = 0; x < nWidth; x++) {
		for (int y = 0; y < nHeight; y++) {
			nMagn = sqrtf(pDx[y][x]*pDx[y][x]+pDy[y][x]*pDy[y][x]);
			//Get the angle of the points
			nOrient = atan2(pDy[y][x], pDx[y][x]);
			nOrient = max(nOrient, -(float)(ERF_PI));
			nOrient = min(nOrient, (float)(ERF_PI));
			nOrient = (nOrient + (float)(ERF_PI)) * 180 / (float)(ERF_PI);

			nIdx = (int)(nOrient / 20); ///20 = 360 degree/18 bin, then ceil it to 1-18
			nIdx = min(nIdx, 17);
			nIdx = max(nIdx, 0);

			//Vote for two neighbor bins.
			nDiff = nOrient - (nIdx + 0.5f) * 20;

			if (nDiff < 0) {
				if (nIdx > 0) {
					nRatio = fabs(nDiff) / 20;
					pFeat->m_pHist[nIdx] += nMagn * (1 - nRatio);
					pFeat->m_pHist[nIdx - 1] += nMagn * nRatio;
				}
				else {
					pFeat->m_pHist[nIdx] += nMagn;
				}
			}
			else {
				if (nIdx < pFeat->m_nDim - 1) {
					nRatio = nDiff / 20;
					pFeat->m_pHist[nIdx] += nMagn * (1 - nRatio);
					pFeat->m_pHist[nIdx + 1] += nMagn * nRatio;

				}
				else {
					pFeat->m_pHist[nIdx] += nMagn;
				}
			}
		}
	}
	//Normalize the histogram of the HoG feature.
	nSum = 0;
	for (int x = 0; x < pFeat->m_nDim; x++)
		nSum += (float)(pFeat->m_pHist[x] * pFeat->m_pHist[x]);

	nSum = sqrt(nSum);
	if (nSum > 0) {
		for (int x = 0; x < pFeat->m_nDim; x++)
			pFeat->m_pHist[x] = pFeat->m_pHist[x] / (nSum + 0.01f);
	}

	//Release the memory.
	if (pDx) {
		for (int j = 0; j < nHeight; j++) {
			if (pDx[j]) {
				RELEASEARRAY(pDx[j]);
				pDx[j] = 0;
			}
		}
		RELEASEARRAY(pDx);
		pDx = 0;
	}
	if (pDy) {
		for (int j = 0; j < nHeight; j++) {
			if (pDy[j]) {
				RELEASEARRAY(pDy[j]);
				pDy[j] = 0;
			}
		}
		RELEASEARRAY(pDy);
		pDy = 0;
	}
	//Release the gray scale image.
	if (pGray) {
		cvReleaseImage(&pGray);
		pGray = 0;
	}
	return true;
}


//---------------------------------------------------------------------------------------------------------//
//Extract the LBP texture features in the region.
bool ExtractTextureFeature(CDetBBox* pBox)
{
	int nWidth = pBox->m_pFeatPict->width;
	int nHeight = pBox->m_pFeatPict->height;

	CHistogram* pFeat = pBox->m_pDetFeat->m_pTextFeat;
	//Reset previous memory.
	pFeat->Reset();

	//Get the gray scale image of the feature extraction region.
	IplImage* pGray = cvCreateImage(cvGetSize(pBox->m_pFeatPict), 8, 1);
	cvCvtColor(pBox->m_pFeatPict, pGray, CV_RGB2GRAY);

	int nIdx = 0;
	float nPxl1 = 0;
	float nPxl2 = 0;
	float* pValue = new float[8];
	for (int i = 0; i < 8; i++)
		pValue[i] = 0;

	for (int i = 1; i < nWidth - 1; i++) {
		for (int j = 1; j < nHeight - 1; j++) {
			nIdx = 0;
			nPxl1 = (float)CV_IMAGE_ELEM(pGray, uchar, j, i);

			nPxl2 = (float)CV_IMAGE_ELEM(pGray, uchar, j - 1, i - 1);
			pValue[0] = nPxl1 > nPxl2 ? 0.0f : 1.0f;

			nPxl2 = (float)CV_IMAGE_ELEM(pGray, uchar, j - 1, i);
			pValue[1] = nPxl1 > nPxl2 ? 0.0f : 1.0f;

			nPxl2 = (float)CV_IMAGE_ELEM(pGray, uchar, j - 1, i + 1);
			pValue[2] = nPxl1 > nPxl2 ? 0.0f : 1.0f;

			nPxl2 = (float)CV_IMAGE_ELEM(pGray, uchar, j, i + 1);
			pValue[3] = nPxl1 > nPxl2 ? 0.0f : 1.0f;

			nPxl2 = (float)CV_IMAGE_ELEM(pGray, uchar, j + 1, i + 1);
			pValue[4] = nPxl1 > nPxl2 ? 0.0f : 1.0f;

			nPxl2 = (float)CV_IMAGE_ELEM(pGray, uchar, j + 1, i);
			pValue[5] = nPxl1 > nPxl2 ? 0.0f : 1.0f;

			nPxl2 = (float)CV_IMAGE_ELEM(pGray, uchar, j + 1, i - 1);
			pValue[6] = nPxl1 > nPxl2 ? 0.0f : 1.0f;

			nPxl2 = (float)CV_IMAGE_ELEM(pGray, uchar, j, i - 1);
			pValue[7] = nPxl1 > nPxl2 ? 0.0f : 1.0f;

			for (int k = 0; k < 8; k++)
				nIdx += (int)(pow((double)2, (double)(7 - k)) * pValue[k]);
			
			//Votes the histogram.
			pFeat->m_pHist[nIdx]++;
		}
	}

	//Normalize the histogram.
	double nVotes = 0;
	for (int i = 0; i < pFeat->m_nDim; i++)
		nVotes += (pFeat->m_pHist)[i];
	if (nVotes > 0) {
		for (int i = 0; i < pFeat->m_nDim; i++)
			(pFeat->m_pHist)[i] /= nVotes;
	}

	//Set zeros.
	pFeat = 0;

	if (pGray) {
		cvReleaseImage(&pGray);
		pGray = 0;
	}
	return true;
}
