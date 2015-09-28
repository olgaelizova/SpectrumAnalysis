#include "stdafx.h"
#include "WavReader.h"
#include "MatlabFunctions.h"
#include "SpectrumTypeTransf.h"
#include "FFTAnalysis.h"
#include "FileUtils.h"

double** speval_eq(double* buffer, int Nfrm, double overlap, int Fs, int Nfrb, char* window, char* type); // function for spectral analys