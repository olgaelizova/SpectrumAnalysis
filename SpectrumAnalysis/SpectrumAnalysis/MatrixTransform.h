#include "stdafx.h"

double* matrINvect(double** matr, int N, int M);

double** transp_matr(double** matr, int N, int M);

double** matrONmatr(double** matr1/*kvadr*/, double** matr2/*pryamoug*/, int N/*len*/, int M/*col*/);

double** transpONmatr(double** transp_matr, double** matr, int N, int M); // N=len, M=col

double* matrONvec(double** matr, double* vec, int N, int M);

double** obr_matr(double **A, int N); 