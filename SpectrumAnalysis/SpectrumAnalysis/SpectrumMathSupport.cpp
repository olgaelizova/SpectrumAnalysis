#include "stdafx.h"
#include "SpectrumMathSupport.h"

double* zeros_for_buffer(double* buffer, int num_zero, int L)  //dobavlenie nulei v konec buffera
{
	double* buf = new double[L + num_zero]; //

	//for (int i = 0; i<L; i++) // end of array has been violated!
	for (int i = 0; i<L + num_zero; i++) // end of array has been violated!
	{
		buf[i] = buffer[i];
	}

	for (int i = 0; i<num_zero; i++)
	{
		buf[L + i] = 0;
	}

	return buf;
}

double** zeros(int m, int n)  //formirovanie dvumernogo massiva MxN iz nulei
{
	double** mas = new double*[m];

	for (int i = 0; i<m; i++)
	{
		mas[i] = new double[n];
	}

	for (int i = 0; i<m; i++)
	{
		for (int j = 0; j<n; j++)
		{
			mas[i][j] = (double)0;
		}
	}

	return mas;
}