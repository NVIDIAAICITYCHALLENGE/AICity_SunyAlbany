//
//  matrix.h
//  matrix
//
//  Created by Longyin Wen on 11-5-25.
//  Copyright 2011�� __MyCompanyName__. All rights reserved.
//

#ifndef _MATRIX_H
#define _MATRIX_H

#include <util/defs.h>
#include "vector.h"

class CMatrix {
public:
	int m_nRow; //����
	int m_nCol; //����

	double** m_pMatrix;
	
public:
	CMatrix();
	CMatrix(int cntrow, int cnrcol);

	const CMatrix& operator=(const CMatrix& m);

    bool transform(CMatrix* pMatTrans); //����ת��
	double trace();

    bool set_value(CMatrix* pMat); //����Ԫ�ظ�ֵ, ���⵽��
	bool offer_value(double** pArray, int row, int col);
    bool set_value(double** pArray, int row, int col);
	bool set_value(double data); //����Ԫ�ظ�ֵ��ͬ

	double get_max_value(); //get the max value of the matrix.
	bool normalize(); //normalized the matrix. 0-1

	//normalized to >1 ֱ�ӳ���������
	bool normalize_max();
	//normalize each row.
	bool normalize_row_sum();
	//normalize each column.
	bool normalize_col_sum();

	bool set_identity();
	bool set_zero(); //������Ԫ������
	void matrix_swap_columns(int xid, int yid);
	void matrix_swap_rows(int xid, int yid);

	~CMatrix();
};


//*************************************Useful function of matrix*****************************************//
bool matrix_init(CMatrix& M, double* val, int n); //nΪval��ά��
bool matrix_init(CMatrix& M, double** val, int row, int col); //rowΪval��������colΪval������

CMatrix* matrix_create(int row, int col);

//matrix component process.
//����������Ϊ����������ڴ�ռ亯��
CVector* matrix_get_row(CMatrix* A, int id);
CVector* matrix_get_column(CMatrix* A, int id);
CMatrix* matrix_get_submatrix(CMatrix* m, int i, int j, int n1, int n2);

//set the matrix to identity
bool matrix_set_identity(CMatrix* A);

//function for array.
double** CreateArray( int, int );
void FreeArray( double**, int );

#endif