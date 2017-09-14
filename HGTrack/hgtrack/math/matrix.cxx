//
//  matrix.cpp
//
//  Created by steinbeck on 11-5-25.
//  Copyright 2011�� __MyCompanyName__. All rights reserved.
//

#include "matrix.h"

CMatrix::CMatrix()
{
	m_nRow = 0;
	m_nCol = 0;
	m_pMatrix = 0;
}

CMatrix::CMatrix(int cntrow, int cntcol)
{
	m_nRow = cntrow;
	m_nCol = cntcol;
	m_pMatrix = new double*[m_nRow];
	for(int i=0; i<m_nRow; i++) {
		m_pMatrix[i] = new double[m_nCol];
	}

}

const CMatrix& CMatrix::operator=(const CMatrix& m)
{
	if(m_nRow!=m.m_nRow || m_nCol!=m.m_nCol) {
		for(int i=0; i<m_nRow; i++) {
			delete [] m_pMatrix[i];
		} 
		delete [] m_pMatrix;

		m_nRow = m.m_nRow;
		m_nCol = m.m_nCol;
		m_pMatrix = new double*[m_nRow];

		for(int i=0; i<m_nRow; i++) {
			m_pMatrix[i] = new double[m_nCol];
		}
	}

	for(int i=0; i<m_nRow; i++) {
		for(int j=0; j<m_nCol; j++) {
			m_pMatrix[i][j] = m.m_pMatrix[i][j];
		}
	}
	return *this;
}


//transform of the matrix.
bool CMatrix::transform(CMatrix* pMatTrans)
{
	if (!pMatTrans) {
		fprintf(stderr, "The matrix is not exist!\n");
		return false;
	}
	if ((m_nRow != pMatTrans->m_nCol)||(m_nCol != pMatTrans->m_nRow)) {
		fprintf(stderr, "The matrix is not matched!\n");
		return false;
	}
	for (int i = 0; i < m_nRow; i++) {
		for (int j = 0; j < m_nCol; j++) {
			pMatTrans->m_pMatrix[j][i] = m_pMatrix[i][j];
		}
	}
	return true;
}

//get the trace of the matrix.
double CMatrix::trace()
{
	double tra = 0;
    if (m_nCol != m_nRow) {
		fprintf(stderr, "The matrix is not the square!\n");
		return 0;
	}
	for (int i = 0; i < m_nRow; i++) {
		tra += m_pMatrix[i][i];
	}
	return tra;
}

//matrix element copy.
bool CMatrix::set_value(CMatrix* pMat)
{
	if (!pMat) {
		fprintf(stderr, "The matrix is not exist!\n");
		return false;
	}
	if ((m_nRow != pMat->m_nRow)||(m_nCol != pMat->m_nCol)) {
		fprintf(stderr, "The matrix is not matching!\n");
		return false;
	}
	for (int i = 0; i < m_nRow; i++) {
		for (int j = 0; j < m_nCol; j++) {
			m_pMatrix[i][j] = pMat->m_pMatrix[i][j];
		}
	}
	return true;
}


bool CMatrix::offer_value(double** pArray, int row, int col)
{
	if (!pArray) {
		fprintf(stderr, "The array is not exist!\n");
		return false;
	}
	if ((m_nRow != row)||(m_nCol != col)) {
		fprintf(stderr, "The matrix is not matching!\n");
		return false;
	}

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			pArray[i][j] = m_pMatrix[i][j];
		}
	}
	return true;
}

bool CMatrix::set_value(double** pArray, int row, int col)
{
	if (!pArray) {
		fprintf(stderr, "The array is not exist!\n");
		return false;
	}
	if ((m_nRow != row)||(m_nCol != col)) {
		fprintf(stderr, "The matrix is not matching!\n");
		return false;
	}

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			m_pMatrix[i][j] = pArray[i][j];
		}
	}
	return true;
}

bool CMatrix::set_value(double data)
{
	for (int i = 0; i < m_nRow; i++) {
		for (int j = 0; j < m_nCol; j++) {
			m_pMatrix[i][j] = data;
		}
	}
	return true;
}

bool CMatrix::set_identity()
{ 
	if (!m_pMatrix) {
		fprintf(stderr, "The matrix is null when setting identity!\n");
		return false;
	}
	set_zero();

	int minl = min(m_nRow, m_nCol);
	for (int i = 0; i < minl; i++) m_pMatrix[i][i] = 1.0f;
	return false;
}

bool CMatrix::set_zero()
{
	if (!m_pMatrix) {
		fprintf(stderr, "The matrix is null when setting zeros!\n");
		return false;
	}
	for (int i = 0; i < m_nRow; i++) {
		for (int j = 0; j < m_nCol; j++) {
			m_pMatrix[i][j] = 0;
		}
	}
	return true;
}

double CMatrix::get_max_value()
{
	double maxv = - DBL_MAXVAL;
	for (int i = 0; i < m_nRow; i++) {
		for (int j = 0; j < m_nCol; j++) {
			if (m_pMatrix[i][j] > maxv) {
				maxv = m_pMatrix[i][j];
			}
		}
	}
	return maxv;
}

bool CMatrix::normalize()
{
	//get the minimal value of the matrix.
    double maxv = - DBL_MAXVAL;
	double minv = DBL_MAXVAL;
	for (int i = 0; i < m_nRow; i++) {
		for (int j = 0; j < m_nCol; j++) {
			if (m_pMatrix[i][j] < minv) {
				minv = m_pMatrix[i][j];
			}
		}
	}
	//minus the minimal value
	for (int i = 0; i < m_nRow; i++) {
		for (int j = 0; j < m_nCol; j++) {
			m_pMatrix[i][j] = m_pMatrix[i][j] - minv;
		}
	}
	//get the maximal value of the matrix.
	for (int i = 0; i < m_nRow; i++) {
		for (int j = 0; j < m_nCol; j++) {
			if (m_pMatrix[i][j] > maxv) {
				maxv = m_pMatrix[i][j];
			}
		}
	}

	if (fabs(maxv) > 0.0001) {
		//get the normalization of the matrix.
		for (int i = 0; i < m_nRow; i++) {
			for (int j = 0; j < m_nCol; j++) {
				m_pMatrix[i][j] = m_pMatrix[i][j] / maxv;
			}
		}
	}
	return true;
}

//Normalize each row.
bool CMatrix::normalize_row_sum()
{
	double sv = 0; 
	for (int i = 0; i < m_nRow; i++) {
		sv = 0;
		for (int j = 0; j < m_nCol; j++)
			sv += m_pMatrix[i][j];
		if (fabs(sv) > DBL_MINVAL) {
			for (int j = 0; j < m_nCol; j++)
				m_pMatrix[i][j] = m_pMatrix[i][j] / sv;
		}
	}
	return true;
}

//Normalize each column.
bool CMatrix::normalize_col_sum()
{
	double sv = 0;
	for (int i = 0; i < m_nCol; i++) {
		sv = 0;
		for (int j = 0; j < m_nRow; j++)
			sv += m_pMatrix[j][i];
		if (fabs(sv) > DBL_MINVAL) {
			for (int j = 0; j < m_nRow; j++)
				m_pMatrix[j][i] = m_pMatrix[j][i] / sv;
		}
	}
	return true;
}


bool CMatrix::normalize_max()
{
	//get the maximal value of the matrix.
	double maxv = - DBL_MAXVAL;
	for (int i = 0; i < m_nRow; i++) {
		for (int j = 0; j < m_nCol; j++) {
			if (m_pMatrix[i][j] > maxv) {
				maxv = m_pMatrix[i][j];
			}
		}
	}
	
	if (maxv != 0) {
		//get the normalization of the matrix.
		for (int i = 0; i < m_nRow; i++) {
			for (int j = 0; j < m_nCol; j++) {
				m_pMatrix[i][j] = m_pMatrix[i][j] / maxv;
			}
		}
	}
	else {
		set_zero();
	}

	return true;
}

void CMatrix::matrix_swap_columns(int xid, int yid)
{
	//check the id.
	if ((xid > m_nCol - 1)||(yid > m_nCol - 1)) {
		fprintf(stderr, "The swap id is out of range in swapping columns!\n");
		return;
	}
	double swapv = 0;
	for (int i = 0; i < m_nRow; i++) {
		swapv = m_pMatrix[i][xid];
		m_pMatrix[i][xid] = m_pMatrix[i][yid];
		m_pMatrix[i][yid] = swapv;
	}
}


void CMatrix::matrix_swap_rows(int xid, int yid)
{
	//check the id.
	if ((xid > m_nRow - 1)||(yid > m_nRow - 1)) {
		fprintf(stderr, "The swap id is out of range in swapping rows!\n");
		return;
	}
	double swapv = 0;
	for (int i = 0; i < m_nCol; i++) {
		swapv = m_pMatrix[xid][i];
		m_pMatrix[xid][i] = m_pMatrix[yid][i];
		m_pMatrix[yid][i] = swapv;
	}
}

CMatrix::~CMatrix()
{
	for(int i=0; i<m_nRow; i++) {
		delete [] m_pMatrix[i];
	} 
	delete [] m_pMatrix;
}

//************************* Useful function of matrix operator ******************//
//*******************************************************************************//
CMatrix* matrix_create(int row, int col)
{
	CMatrix* m = new CMatrix(row, col);
	m->set_zero();
	return m;
}

bool matrix_init(CMatrix& M, double* val, int n)
{
	int k;
	int row = M.m_nRow;
	int col = M.m_nCol;

	if(n > row * col) {
		fprintf(stderr, "Initial Matrix error!!!");
		return false;
	}

	for(int i=0; i<row; i++) {
		for(int j=0; j<col; j++) {
			k = i*col+j;
			if(k<n)
				M.m_pMatrix[i][j] = val[k];
			else 
				M.m_pMatrix[i][j] = 0;
		}
	}
	return true;
}

bool matrix_init(CMatrix& M, double** val, int row, int col)
{
	if (!val) {
		fprintf(stderr, "The value matrix is null!");
		return false;
	}
	if ((M.m_nRow != row)||(M.m_nCol != col)) {
		fprintf(stderr, "The size of the matrix is not matched!");
		return false;
	}
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			M.m_pMatrix[i][j] = val[i][j];
		}
	}
	return true;
}


CVector* matrix_get_row(CMatrix* A, int id)
{
	if (!A) {
		fprintf(stderr, "The matrix A is not initialized in matrix get row!");
		return 0;
	}
	if (id > (A->m_nRow - 1)) {
		fprintf(stderr, "The row id exceeds the range!");
		return 0;
	}

	int col = A->m_nCol;
	CVector* row = vector_create(col);
	for (int i = 0; i < col; i++) {
		row->m_pData[i] = A->m_pMatrix[id][i];
	}
	return row;
}

CVector* matrix_get_column(CMatrix* A, int id)
{
	if (!A) {
		fprintf(stderr, "The matrix A is not initialized in matrix get column!");
		return 0;
	}
	if (id > (A->m_nCol - 1)) {
		fprintf(stderr, "The column id exceeds the range!");
		return 0;
	}

	CVector* col = vector_create(A->m_nRow);
	for (int i = 0; i < A->m_nRow; i++) {
		col->m_pData[i] = A->m_pMatrix[i][id];
	}
	return col;
}

CMatrix* matrix_get_submatrix(CMatrix* m, int i, int j, int n1, int n2)
{
	if (!m) {
		fprintf(stderr, "The matrix A is not initialized in matrix get sub-matrix!");
		return 0;
	}
	if (i >= m->m_nRow) {
		fprintf(stderr, "row index is out of range");
		return 0;
	}
	else if (j >= m->m_nCol) {
		fprintf(stderr, "column index is out of range");
		return 0;
	}
	else if (n1 == 0) {
		fprintf(stderr, "first dimension must be non-zero");
		return 0;
	}
	else if (n2 == 0) {
		fprintf(stderr, "second dimension must be non-zero");
		return 0;
	}
	else if (i + n1 > m->m_nRow) {
		fprintf(stderr, "first dimension overflows matrix");
		return 0;
	}
	else if (j + n2 > m->m_nCol) {
		fprintf(stderr, "second dimension overflows matrix");
		return 0;
	}

	CMatrix* submatrix = matrix_create(n1, n2);

	for (int k = 0; k < n1; k++) {
		for (int l = 0; l < n2; l++)
		{
			submatrix->m_pMatrix[k][l] = m->m_pMatrix[k + i][l + j];
		}
	}

	return submatrix;
}

bool matrix_set_identity(CMatrix* A)
{
	if (!A) {
		fprintf(stderr, "The matrix A is null in set identity!");
		return false;
	}
	int row = A->m_nRow;
	int col = A->m_nCol;
	if (row != col) {
		fprintf(stderr, "The matrix A is not the square matrix!");
		return false;
	}
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (i == j) {
				A->m_pMatrix[i][i] = 1.0f;
			} 
			else {
				A->m_pMatrix[i][j] = 0;
			}
		}
	}
	return true;
}



//************************* Create a square matrix ************************//
// n: dimensionality
//*************************************************************************//
double** CreateArray( int m, int n )
{
	double** A;
	A = new double* [m];

	for( int i=0; i<m; i++ )
		A[i] = new double[n];

	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++){
			A[i][j] = 0;
		}
	}
	return A;
}

//***************************** Release memory ****************************//
// A: square matrix
// n: dimensionality
//*************************************************************************//
void FreeArray( double** A, int m )
{
	if (A) {
		for( int i=0; i<m; i++ ) {
			delete[] A[i]; 
			A[i] = 0;
		}
		delete[] A; 
		A = 0;
	}
}