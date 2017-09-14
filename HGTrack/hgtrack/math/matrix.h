//
//  matrix.h
//  matrix
//
//  Created by Longyin Wen on 11-5-25.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#ifndef _MATRIX_H
#define _MATRIX_H

#include <util/defs.h>
#include "vector.h"

class CMatrix {
public:
	int m_nRow; //行数
	int m_nCol; //列数

	double** m_pMatrix;
	
public:
	CMatrix();
	CMatrix(int cntrow, int cnrcol);

	const CMatrix& operator=(const CMatrix& m);

    bool transform(CMatrix* pMatTrans); //矩阵转置
	double trace();

    bool set_value(CMatrix* pMat); //矩阵元素赋值, 由外到内
	bool offer_value(double** pArray, int row, int col);
    bool set_value(double** pArray, int row, int col);
	bool set_value(double data); //所有元素赋值相同

	double get_max_value(); //get the max value of the matrix.
	bool normalize(); //normalized the matrix. 0-1

	//normalized to >1 直接除以最大的数
	bool normalize_max();
	//normalize each row.
	bool normalize_row_sum();
	//normalize each column.
	bool normalize_col_sum();

	bool set_identity();
	bool set_zero(); //将矩阵元素置零
	void matrix_swap_columns(int xid, int yid);
	void matrix_swap_rows(int xid, int yid);

	~CMatrix();
};


//*************************************Useful function of matrix*****************************************//
bool matrix_init(CMatrix& M, double* val, int n); //n为val的维数
bool matrix_init(CMatrix& M, double** val, int row, int col); //row为val的行数，col为val的列数

CMatrix* matrix_create(int row, int col);

//matrix component process.
//这三个函数为重新申请的内存空间函数
CVector* matrix_get_row(CMatrix* A, int id);
CVector* matrix_get_column(CMatrix* A, int id);
CMatrix* matrix_get_submatrix(CMatrix* m, int i, int j, int n1, int n2);

//set the matrix to identity
bool matrix_set_identity(CMatrix* A);

//function for array.
double** CreateArray( int, int );
void FreeArray( double**, int );

#endif