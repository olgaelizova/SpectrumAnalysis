#include "stdafx.h"
#include "MatlabFunctions.h"

double fix(double Number) // function for get num without .xxx part
{
	bool Negative = Number < 0 ? true : false;
	Number = fabs(Number);

	if (Number > floor(Number) + 0.5)
		Number = floor(Number); //ceil
	else
		Number = floor(Number); //floor

	return Negative ? Number * (-1) : Number;
}

int nextpow2(int b)  // poluchenie pokazatelya stepeni
{
	int x = ceil(log(b) / log(2));
	return x;
}

double* feval(int NFFT, char* window, int Lfrm)  // function for evaluation window parameters
{
	double* win = new double[Lfrm];
	double pi = 3.14159;

	if (strcmp(window, "hann") == 0)
	{
		for (int i = 0; i<Lfrm; i++)
		{
			win[i] = 0.5*(1 - (cos(2 * pi*i / (Lfrm - 1))));
		}

	}

	if (strcmp(window, "hamming") == 0)
	{
		for (int i = 0; i<Lfrm; i++)
		{
			win[i] = 0.54 - (0.46*(cos(2 * pi*i / (NFFT - 1))));
		}
	}
	return win;
}

double sum(double* mas, int size)
{
	double sum = 0.0;

	for (int i = 0; i<size; i++)
	{
		sum += mas[i];
	}

	return sum;
}

double mean(double* vec, int len)  //++
{
	double m = 0;

	for (int i = 0; i<len; i++)
		m += vec[i];

	return (m / (double)len);
}

double std_vec(double* vec, int len)
{
	double S = 0.0;

	double m = mean(vec, len);

	double p = 1.0 / ((double)len - 1.0);

	double S_pr = 0.0;

	for (int i = 0; i<len; i++)
		S_pr += pow(vec[i] - m, 2.0);

	S = pow(S_pr*p, 0.5);

	return S;
}

double* pow_vec(double* vec, int len, double n)  // ne rabotaet pri pereprisvaivanii
{
	double* v = new double[len];
	for (int i = 0; i<len; i++)
	{
		v[i] = pow(vec[i], n);
		//printf(" %.4f ",v[i]); //++
	}
	//cout << endl << endl;
	return v;
}

double* abs_vec(double* vec, int len)
{
	for (int i = 0; i<len; i++)
	{
		if (vec[i] < 0)
			vec[i] = vec[i] * (-1.0);
		//cout << vec[i] << endl; //++
	}
	//cout << endl << endl;
	return vec;
}

double norm(double* v, int len, double p)
{
	double n = 0;

	n = pow(sum(pow_vec(abs_vec(v, len), len, p), len), (1 / p));

	return n;
}

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