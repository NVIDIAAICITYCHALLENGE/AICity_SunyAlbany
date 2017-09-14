#ifndef _ALGEBRA_H
#define _ALGEBRA_H
#include "matrix.h"
#include "vector.h"


double matrix_vectorDoubleMultiply(CVector* vector, CMatrix* A); // a = xT * A * x
bool matrix_vectorMulti(CVector *x, CMatrix* A, CVector* vector); 
bool matrix_cholesky(CMatrix* A, CMatrix* L);
bool matrix_matrixMulti(CMatrix* A, CMatrix* B, CMatrix* AmultiB);


#endif