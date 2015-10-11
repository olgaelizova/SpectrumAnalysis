#include "stdafx.h"
#include "RegrKoefAnalysis.h"

int koef_of_regr(double* buffer, double* etalon, int usenoise, int col, int len)
{
	double** X = new double*[len];

	double* shum;
	double* constanta;
	constanta = ones_vec(len);  // constanta - vector iz edinic
	shum = ones_vec(len);  // na dannom etape shum - vector iz edinic

	double norm_const = norm(constanta, len, 2.0);

	double std_shum = std_vec(shum, len);

	double mean_etalon = mean(etalon, len);

	double* subst = substraction_vec(etalon, len, mean_etalon); //etalon[i]-mean_etalon
	double std_subst = std_vec(subst, len);

	for (int i = 0; i<len; i++)
	{
		X[i] = new double[col];	// na shum, etalon, raspozn_slovo

		X[i][0] = constanta[i] / norm_const; //normirovannye t.k. v matlab norm(x)=norm(x,2)

		X[i][1] = subst[i] / std_subst;	// etalon

		if (col == 3)
		{
			X[i][2] = shum[i] / std_shum;	// pomexa
		}
	}

	double** proizv;
	double** quotient;
	double* koef;

	proizv = transpONmatr(transp_matr(X, len, col), X, len, col);

	quotient = matrONmatr(obr_matr(proizv, col), transp_matr(X, len, col), len, col);

	koef = matrONvec(quotient, buffer, len, col);

	printf("koef: %.4f", koef[1]);

	// free() problem here
	//delete[] koef;
	//delete[] quotient;
	//delete[] proizv;
	delete[] shum;
	delete[] constanta;
	//delete[] subst;

	for (int i = 0; i<len; i++)
		delete[] X[i];
	delete[] X;

	return 0;
}