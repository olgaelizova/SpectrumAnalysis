#include "stdafx.h"
#include "SpectrumTypeTransf.h"

double hz2mel(double Fo)  // perevod hz v mel
{
	double F = 0.0;
	F = 2595 * log10(1.0 + Fo / 700);
	return F;
}

double mel2hz(double Fo)  // perevod mel v hz
{
	double F = 0.0;
	double F1 = pow(10, (Fo / 2595));
	F = 700 * (F1 - 1.0);
	return F;
}