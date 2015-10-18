#include "stdafx.h"
#include <stdio.h>

#include <math.h>

#include "FFTAnalysis.h"
#include "ReadEtalons.h"
#include "SpectrumAnalysis.h"
#include "RegrKoefAnalysis.h"

#include "InputParameters.h"

using namespace std;

#define _CRT_SECURE_NO_WARNINGS

int main(void)
{
	//// default values for spectrum analysis
	int samples_count = 0;
	int Fs = 22050;   // default - ==header.nSamplesPerSec
	double overlap = 0.0;
	double NFFT = 512.0;
	char win[] = "hann";  // okno hanna, vtoroi variant s oknom hamminga
	char type[] = "hz";
	////

	//// tut znachenia takie, chto bu poluchit odinakovuy matricy s etalonom
	int Nfrb = 35;
	int Nfrm = 17;
	////

	//// tut parametry dlya etalonov i evaluate koef of regression
	int len = 0;
	int len_etalona = 0;
	int Nfrm_etalona = 17;
	int Nfrb_etalona = 35;
	int col_vect = 2; // skolko vectorov: (shum), etalon, sravn_zapis
	////

	//// for input files functions
	int setsize = 2;

	int wavfilescounter = 0;
	int efilescounter = 0;

	string strpath;
	string folderpath;
	string str;

	string estrpath;
	string efolderpath;
	string estr;
	////

	char** wavfiles = 0;
	char** etalons = 0;

	wavfiles = inputWavFile(setsize, wavfilescounter, strpath, folderpath, str);

	etalons = inputEtalons(setsize, efilescounter, estrpath, efolderpath, estr);

	cout << "Start computing... " << endl << endl;

	for (int i = 0 ; i < wavfilescounter; i++) // i=0
	{
		strpath = folderpath + wavfiles[i];
		const char* fullpath = strpath.c_str();

		cout << "Wav-file is: " << wavfiles[i] << endl << endl; // wavfile[i]

		double* wavdata;

		wavdata = wavread(fullpath, samples_count);

		///chistim stroky s putem k faily
		strpath.clear();
		///

	//spectral analysis
	double **spectr;
	spectr = speval_eq(wavdata, Nfrm, overlap, Fs, Nfrb, win, type);  //my_function speval_eq s oknom hanna v hz

	//chtenie etalonov iz txt faila
	for (int j = 0; j < efilescounter; j++)
	{

		estrpath = efolderpath + etalons[j];

		const char* efullpath = estrpath.c_str();

		double* etalon = readDoubles((char*)efullpath, len);

		//////vityagivanie matrici spectralnogo analiza v vector
		double* lin_spectr;

		lin_spectr = matrINvect(spectr, Nfrm, Nfrb);

		//sravnenie faila s etalonami
		len_etalona = len;

		cout << "Etalon is: " << etalons[j] << endl;

		koef_of_regr(lin_spectr, etalon, 1, col_vect, len_etalona); 
		
		cout << endl;

		///chistim stroky s putem k faily
		estrpath.clear();
		///

		/// ochistka pamyaty
		delete[] etalon;
		delete[] lin_spectr;
		///
	}
	cout << endl;

	/////// ochistka pamyaty
	delete[] spectr;  
	delete[] wavdata;  
	}

	system("pause");

	return 0;
}
