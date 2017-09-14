//
//  vector.h
//  vector
//
//  Created by Longyin Wen on 11-5-25.
//  Copyright 2011Äê __MyCompanyName__. All rights reserved.
//
#ifndef _VECTOR_H
#define _VECTOR_H

#include <util/defs.h>

class CVector
{
public:
	int m_nLength; //The length of the vector.
	double* m_pData;

	CVector();
	CVector(int length);

	virtual ~CVector();

	inline void vector_swap_elements(int xid, int yid) {
		if ((xid > m_nLength - 1)||(yid > m_nLength - 1)) {
			fprintf(stderr, "The swap id exceeds the length of the vector!");
			return;
		}
		double swapv = 0;
		swapv = m_pData[xid];
		m_pData[xid] = m_pData[yid];
		m_pData[yid] = swapv;
	}

	/****************************************************************/
	bool set_value(double data);
	bool set_value(CVector* pv);
	bool set_zero();
	bool normalize();
};

//some useful vector function.
CVector* vector_create(int length);
CVector* vector_get_subvector(CVector* v, int sid, int length);

double vector_blas_dnrm2(CVector* v);
bool vector_blas_dscal(double alpha, CVector* v);


#endif