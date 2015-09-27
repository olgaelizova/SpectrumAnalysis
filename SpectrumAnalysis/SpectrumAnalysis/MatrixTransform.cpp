#include "stdafx.h"
#include "MatrixTransform.h"

double* matrINvect(double** matr, int N, int M)
{
	double* vec = new double[N*M];

	for (int i = 0; i< M; i++)
	{
		for (int j = 0; j<N; j++)
		{
			vec[i*N + j] = matr[j][i];

			//printf("%d\t%.4f\n",i*M+j, vec[i*M+j]);
		}
		//printf("\n");
	}

	for (int i = 0; i< M; i++)
	{
		for (int j = 0; j<N; j++)
		{
			vec[i*N + j] = matr[j][i];

			//printf("%d\t%.4f\n",i*M+j, vec[i*M+j]);
		}
		//printf("\n");
	}


	return vec;
}

double** transp_matr(double** matr, int N, int M)  // N=len, M=col
{

	double** t_matr = new double*[M];

	for (int i = 0; i<M; i++)
	{
		t_matr[i] = new double[N];
		for (int j = 0; j<N; j++)
		{
			t_matr[i][j] = matr[j][i];
			//cout << matr[j][i] << endl;
		}
	}

	return t_matr;
}

double** matrONmatr(double** matr1/*kvadr*/, double** matr2/*pryamoug*/, int N/*len*/, int M/*col*/)
{

	double** mul = new double*[M];

	for (int i = 0; i<M; i++)//stroki
	{
		mul[i] = new double[N];
		for (int j = 0; j<N; j++)//stolbcy
		{
			mul[i][j] = 0;
			for (int t = 0; t<2; t++)
			{
				mul[i][j] += matr1[i][t] * matr2[t][j];
			}

			//std::cout << mul[i][j] << "\n";
		}
		//	std::cout << "\n\n";
	}

	return mul;
}

double** transpONmatr(double** transp_matr, double** matr, int N, int M) // N=len, M=col
{
	int size = 0, l = 0;
	if (N <= M) size = N, l = M;
	else size = M, l = N;

	double** mul = new double*[size];

	for (int i = 0; i<size; i++)
	{
		mul[i] = new double[size];
		for (int j = 0; j<size; j++)
		{
			mul[i][j] = 0;
			for (int k = 0; k < l; k++)
			{
				mul[i][j] += transp_matr[i][k] * matr[k][j];
			}

			//cout << mul[i][j]<<endl;
		}
	}

	return mul;
}

double* matrONvec(double** matr, double* vec, int N, int M)
{
	int size_i = 0;
	int size_j = 0;
	if (N >= M)
	{
		size_i = M;
		size_j = N;
	}
	else
	{
		size_i = N;
		size_j = M;
	}

	double* mul = new double[size_i];

	for (int i = 0; i<size_i; i++)
	{
		mul[i] = 0;

		for (int j = 0; j<size_j; j++)
		{
			mul[i] += matr[i][j] * vec[j];
		}
		//cout << mul[i] << endl;
	}

	return mul;
}

double** obr_matr(double **A, int N)
{
	double temp;

	double **E = new double *[N];

	for (int i = 0; i < N; i++)
		E[i] = new double[N];

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
			E[i][j] = 0.0;

			if (i == j)
				E[i][j] = 1.0;
		}

	for (int k = 0; k < N; k++)
	{
		temp = A[k][k];

		for (int j = 0; j < N; j++)
		{
			A[k][j] /= temp;
			E[k][j] /= temp;
		}

		for (int i = k + 1; i < N; i++)
		{
			temp = A[i][k];

			for (int j = 0; j < N; j++)
			{
				A[i][j] -= A[k][j] * temp;
				E[i][j] -= E[k][j] * temp;
			}
		}
	}

	for (int k = N - 1; k > 0; k--)
	{
		for (int i = k - 1; i >= 0; i--)
		{
			temp = A[i][k];

			for (int j = 0; j < N; j++)
			{
				A[i][j] -= A[k][j] * temp;
				E[i][j] -= E[k][j] * temp;
			}
		}
	}

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
			A[i][j] = E[i][j];
			//cout << A[i][j] <<endl;
		}

	for (int i = 0; i < N; i++)
		delete[] E[i];

	delete[] E;

	return A;
}