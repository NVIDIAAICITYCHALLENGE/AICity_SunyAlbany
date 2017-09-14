#include "vector.h"

CVector::CVector()
{
	m_nLength = 0;
	m_pData = 0;
}

CVector::CVector(int length)
{
	m_nLength = length;
	m_pData = new double[length];
}

CVector::~CVector() {
	delete[] m_pData;
}

bool CVector::set_zero()
{
	if (!m_pData) {
		fprintf(stderr, "The vector is not initialized in setzero function!\n");
		return false;
	}
	for (int i = 0; i < m_nLength; i++) {
		m_pData[i] = 0;
	}
	return true;
}

bool CVector::set_value(double data)
{
	if (!m_pData) {
		fprintf(stderr, "The vector is not initialized in setvalue function!\n");
		return false;
	}
	for (int i = 0; i < m_nLength; i++) {
		m_pData[i] = data;
	}
	return true;
}

//��������������ȱȸ�ֵ�����������ȡ��ǰ��Ԫ�أ������̣�������Ԫ�ظ���
bool CVector::set_value(CVector *pv)
{
	if (!pv) {
		fprintf(stderr, "The vector is not initialized in set_value function!\n");
		return false;
	}
	if (pv->m_nLength > m_nLength) {
		for (int i = 0; i < m_nLength; i++) {
			m_pData[i] = pv->m_pData[i];
		}
		for (int i = m_nLength; i < pv->m_nLength; i++) {
			m_pData[i] = 0;
		}
	}
	else if (pv->m_nLength == m_nLength) {
		for (int i = 0; i < m_nLength; i++) {
			m_pData[i] = pv->m_pData[i];
		}
	}
	else if (pv->m_nLength < m_nLength) {
		for (int i = 0; i < pv->m_nLength; i++) {
			m_pData[i] = pv->m_pData[i];
		}
	}
	return true;
}

bool CVector::normalize()
{
	//normalize and modified the data cost term.
	double maxv = - DBL_MAXVAL;
	double minv = DBL_MAXVAL;
	//find the minimal cost
	for (int i = 0; i < m_nLength; i++) {
		if (m_pData[i] < minv) {
			minv = m_pData[i];
		}
	}
	for (int i = 0; i < m_nLength; i++)
		m_pData[i] = m_pData[i] - minv;

	//find the maximal cost
	for (int i = 0; i < m_nLength; i++) {
		if (m_pData[i] > maxv) {
			maxv = m_pData[i];
		}
	}

	if (maxv != 0) {
		for (int i = 0; i < m_nLength; i++)
			m_pData[i] = m_pData[i] / maxv;
	}
	return true;
}


//**************************** Useful vector functions *****************************************//
CVector* vector_create(int length)
{
	CVector* v = new CVector(length);
	v->set_zero();
	return v;
}

CVector* vector_get_subvector(CVector* v, int sid, int length)
{
	if (!v) {
		fprintf(stderr, "The vector is not initialized!\n");
		return false;
	}
	if ((length == 0)||(sid + length > v->m_nLength)) {
		fprintf(stderr, "The parameter error in get sub-matrix!\n");
		return 0;
	}

	CVector* subv = vector_create(length);
	for (int i = 0; i < length; i++) {
		subv->m_pData[i] = v->m_pData[i + sid];
	}
	return subv;
}

double vector_blas_dnrm2(CVector* v)
{
	if (!v) {
		fprintf(stderr, "The vector is not initialized!\n");
		return -1;
	}
	double norm = 0;
	for (int i = 0; i < v->m_nLength; i++) {
		norm = v->m_pData[i] * v->m_pData[i];
	}
	norm = sqrt(norm);
	return norm;
}

bool vector_blas_dscal(double alpha, CVector* v)
{
	if (!v) {
		fprintf(stderr, "The vector is not initialized!\n");
		return false;
	}
	int length = v->m_nLength;
	for (int i = 0; i < length; i++) {
		v->m_pData[i] = alpha * v->m_pData[i];
	}
	return true;
}