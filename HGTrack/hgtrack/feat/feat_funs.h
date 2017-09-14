//This file focuses on the function of feature class.
#ifndef _FEATFUNS_H
#define _FEATFUNS_H

#include <feat/feat_class.h>
#include <util/tracklet.h>
#include <config/config.h>

//----------------------------------------------------------------------------------------------------------------//
//------------------- Calculate the Similarity between the Histograms -------------------------------------------//
//Calculate the Chi-square similarity between two histograms.
double CalcChiSimilarity(const CHistogram* pFeat1, const CHistogram* pFeat2);
double CalcChiSimilarity(const CHistogram& pFeat1, const CHistogram& pFeat2);

//Calculate the Bhattacharyya similarity between two histograms.
double CalcBhatSimilarity(const CHistogram* pFeat1, const CHistogram* pFeat2);
double CalcBhatSimilarity(const CHistogram& pFeat1, const CHistogram& pFeat2);


//Calculate the Similarity Between Two Features.
double CalcFeatSimilarity(const CFeature* pFeat1, const CFeature* pFeat2);
double CalcFeatSimilarity(const CFeature& pFeat1, const CFeature& pFeat2);


//---------------------------------------------------------------------------------------------------------------//
//---------------------- Extract the Features of the Target ----------------------------------------------------//
//Extract the RGB color histogram.
bool ExtractColorFeature(CDetBBox* pBox);
//Extract the HOG shape histogram.
bool ExtractShapeFeature(CDetBBox* pBox);
//Extract the LBP texture histogram.
bool ExtractTextureFeature(CDetBBox* pBox);


#endif