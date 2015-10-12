#include "stdafx.h"
#include "VectorTransform.h"

void substraction_vec(double* vec, int len, double s) // vichitanie chisla iz vectora
{
	for (int i = 0; i<len; i++)
	{
		vec[i] = vec[i] - s;
	}

}

double* division_vec(double* vec, int len, double p) // delenie vectora na chislo
{
	for (int i = 0; i<len; i++)
		vec[i] = vec[i] / p;

	return vec;
}

double* ones_vec(int len)
{
	double* vec = new double[len];
	for (int i = 0; i<len; i++)
	{
		vec[i] = 1;
	}

	return vec;
}