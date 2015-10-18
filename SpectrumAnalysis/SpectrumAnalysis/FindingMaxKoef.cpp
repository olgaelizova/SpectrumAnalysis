#include "stdafx.h"

#include "FindingMaxKoef.h"

using namespace std;

void maxKoef(char** etalons, double* koeffs, int efilescounter)
{
	int i=0;
	int etcounter = 0;
	double max = 0;

	for (i = 0; i < efilescounter; i++)
		if (koeffs[i]>max)
		{
			max = koeffs[i];
			etcounter = i;
		}

	cout << "Etalon is " << etalons[etcounter] << " with koefficient " << max << endl;

}