#include <util/defs.h>
#include "algebra.h"

double matrix_vectorDoubleMultiply(CVector* vector, CMatrix* A)
{
	double temp = 0, result = 0;
	int row = A->m_nRow;
	int col = A->m_nCol;
	for(int i=0; i< row; i++) {
		temp = 0;
		for(int j=0; j<col; j++) {
			temp += vector->m_pData[j] * A->m_pMatrix[i][j];
		}
		result += temp * vector->m_pData[i];
	}
	return result;
}

bool matrix_vectorMulti(CVector *x, CMatrix* A, CVector* vector)  //vector=A*x
{
	if ((A->m_nCol != x->m_nLength)||(A->m_nRow != vector->m_nLength)) {
		fprintf(stderr, "The vector size is not matched!\n");
		return false;
	}
	
	double temp;

	vector->set_zero();
	int row = A->m_nRow;
	int col = A->m_nCol;
	for(int i=0; i<row; i++) {
		temp = 0;
		for(int j=0; j<col; j++) {
			temp += A->m_pMatrix[i][j] * x->m_pData[j];
		}
		vector->m_pData[i] = temp;
	}
	return true;
}

bool matrix_cholesky(CMatrix* A, CMatrix* L)
{

	if (A->m_nRow != A->m_nCol) {
		fprintf(stderr, "The matrix A is not the square matrix in cholesky!\n");
		return false;
	}
	if (L->m_nRow != L->m_nCol) {
		fprintf(stderr, "The matrix L is not the square matrix in cholesky!\n");
		return false;
	}
	
	int n = A->m_nRow;
	

	L->set_zero();

	for(int k=0;k<n;k++) {
		L->m_pMatrix[k][k] = A->m_pMatrix[k][k];
		for(int m=0;m<k;m++)
			L->m_pMatrix[k][k] = L->m_pMatrix[k][k]-L->m_pMatrix[k][m] * L->m_pMatrix[k][m];
		L->m_pMatrix[k][k] = sqrt(L->m_pMatrix[k][k]);

		for(int i=k+1;i<n;i++) {
			L->m_pMatrix[i][k] = A->m_pMatrix[i][k];
			for(int m=0;m<k;m++)
				L->m_pMatrix[i][k] = L->m_pMatrix[i][k]-L->m_pMatrix[i][m] * L->m_pMatrix[k][m];
			L->m_pMatrix[i][k] = L->m_pMatrix[i][k]/(L->m_pMatrix[k][k] + DBL_MINVAL);
		}
	}
	return true;
}

//matrix A multiple matrix B to get matrix A*B
bool matrix_matrixMulti(CMatrix* A, CMatrix* B, CMatrix* AmultiB)
{
	if (A->m_nCol != B->m_nRow) {
		fprintf(stderr, "The matrix is not matched in multiply!\n");
		return false;
	}
	if (!AmultiB) {
		fprintf(stderr, "The matrix space is not applied!\n");
		return false;
	}

	int m = A->m_nRow;
	int n = A->m_nCol;
	int p = B->m_nCol;

	AmultiB->set_zero();

	for (int i = 0; i < m; i++) {
		for (int j = 0; j < p; j++) {
			for (int k = 0; k < n; k++) {
				AmultiB->m_pMatrix[i][j] += + A->m_pMatrix[i][k] * B->m_pMatrix[k][j];
			}
		}
	}
	return true;
}
