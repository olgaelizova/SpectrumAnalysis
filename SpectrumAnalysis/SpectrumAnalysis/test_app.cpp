#include "stdafx.h"
#include <memory.h>  // analog mem.h in WinVS  // <memory.h> in unix
#include <stdio.h>

#include <math.h>
//#include "My_FFT.cpp"
// and I forked the project, where will it save ?

#define _CRT_SECURE_NO_WARNINGS

using namespace std;

// types for convert WinVS format in typical unix standa
#include "My_FFT.h"
#include "zapis_etalona.h"

#include <iostream>
#include <fstream>
#include <string>
#include "FileUtils.h"
#include "SpectrumMathFunctions.h"


etalon_			etalon;
wav_header_t	header;
chunk_t			chunk;



int main(void)
{
	FILE *f = 0;

	f = fopen("pilotag_wav.wav", "rb");

	if (f == NULL)
	{
		printf("File no open\n");
	}

	// schitaem razmer faila
	int fileSize = 0;
	fseek(f, 0, SEEK_END);  // smeshaet kursor ot 0 do konza faila
	fileSize = ftell(f);	// vozvrash. tekushyu poziciu v potoke
	fseek(f, 0, SEEK_SET); // vozvrashaem ukazatel na nachalo faila
	//

	fread(&header, sizeof(header), 1, f); // chitaem inf v structuru header

	//// proverca na format PCM
	if (header.wFormatTag != 1)
		printf("Not PCM format\n");

	fseek(f, header.fLen - 16, SEEK_CUR); //skip wExtraFormatBytes & extra format bytes

	while (true) //go to data chunk //tut propuskaem polya LIST i INFO
	{
		fread(&chunk, sizeof(chunk), 1, f);
		if (*(DWORD *)&chunk.id == 0x61746164) break;
		fseek(f, chunk.size, SEEK_CUR); //skip chunk data bytes
	}

	// schitaem chislo samplov 
	int sample_size = header.wBitsPerSample / 8;
	int samples_count = chunk.size * 8 / header.wBitsPerSample;

	// pamyat pod massiv znacheniy iz data 
	DWORD *value = new DWORD[samples_count];
	memset(value, 0, sizeof(DWORD)* samples_count);  // dlya nee nygna memory.h

	for (int i = 0; i < samples_count; i++)
	{
		fread(&value[i], sample_size, 1, f);  // poluchili massiv
	}

	fclose(f);

	// poluchim massiv (double) nashix znachenyi
	double* data_chunk = new double[samples_count];

	for (int i = 0; i < samples_count; i++)
	{
		//for 16 bits per sample only: delim na 0x8000 chto ravno 32768.0;
		double x = (double)(INT16)value[i] / 32768.0;
		data_chunk[i] = x;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	//spectral analysis

	int Fs = 22050;
	double overlap = 0.0;
	double NFFT = 512.0;
	double nfrm = samples_count / (NFFT*(1 - overlap));
	//int Nfrb = 30;
	char win[] = "hann";
	char type[] = "hz";

	//int Nfrm = fix(nfrm); //my_function fix - otbros drobnoi chasti  ++
	//printf("Nfrm %d\n",Nfrm);//++
	/////////////////////////////// tut znachenia takie, chto bu poluchit odinakovuy matricy s etalonom
	int Nfrb = 35;
	int Nfrm = 17;
	//////////////////////////////
	double **spectr;
	spectr = speval_eq(data_chunk, Nfrm, overlap, Fs, Nfrb, win, type);  //my_function speval_eq s oknom hanna v hz
	////////////////////////////////////////////////////////////////////////////////////////////////
	//zapis' etalonov iz txt faila

	char name1[20] = { "etalon_pilotag.txt" };
	char name2[20] = { "etalon_mashtab.txt" };
	char name3[22] = { "etalon_navigacia.txt" };

	etalon.pilotag = zapis_etalona(name1); // gives mean value -3.1406766853184722
	//etalon.pilotag = readDoubles(name1); // gives mean value -3.1386171643444776
	etalon.mashtab = zapis_etalona(name2);
	etalon.navigacia = zapis_etalona(name3);
	//////////////////////////////
	//vityagivanie matrici spectralnogo analiza v vector

	double* lin_spectr;

	lin_spectr = matrINvect(spectr, Nfrm, Nfrb);

	/////////////////////////////
	//sravnenie faila s etalonom
	int Nfrm_etalona = 17;
	int Nfrb_etalona = 35;
	int len_etalona = 595;
	int col_vect = 2; // skolko vectorov: (shum), etalon, sravn_zapis

	///// neverno tak kak dolgen bit k2 max dlya etalona so slovom

	cout << "pilotag\n" << endl;
	koef_of_regr_korr(lin_spectr, etalon.pilotag, 1, col_vect, len_etalona); // (lens)5 to fix

	//cout << "mashtab\n" << endl;
	//koef_of_regr_korr (lin_spectr, etalon.mashtab, 1, col_vect, len_etalona);

	//cout << "navigacia\n" << endl;
	//koef_of_regr_korr (lin_spectr, etalon.navigacia, 1, col_vect, len_etalona);

	////////////////////////////////////////////////////////////////////////////////////////////////
	/// vivod v fail
	///////
	delete[]spectr;  // ochistka pamyaty
	delete[]data_chunk;  // ochistka pamyaty
	delete[]value;  // ochistka pamyaty
	//////


	_CrtDumpMemoryLeaks();

	return 0;
}
/*

int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}
*/
