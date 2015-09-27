#include "stdafx.h"
#include <stdio.h>

#include <memory.h>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

#include "FFTAnalysis.h"
#include "ReadEtalons.h"
#include "WavReader.h"
#include "FolderFiles.h"
#include "SpectrumAnalysis.h"
#include "RegrKoefAnalysis.h"
#include "FileUtils.h"

using namespace std;

#define _CRT_SECURE_NO_WARNINGS

wav_header_t	header;
chunk_t			chunk;

int main(void)
{
	//// schityvaem wav faily
	string strpath;
	string folderpath;
	string str;

	int wavfilescounter = 0;
	int efilescounter = 0;

	cout << "Input path to directory with wav files!" << endl << "Directory with wav files is: ";
	cin >> str;

	folderpath = str;

	const char* path = str.c_str();
	char *path_ = 0;
	FileUtils::CpyCharStr(path_, path);
	FileUtils::CheckFolderSlash(path_);
	folderpath = path_;
	FileUtils::AppendCharsAtEnd(path_, "*.*", 3);
	int setsize = 2;

	char** wavfiles = folderfiles(path_, setsize, wavfilescounter, "wav");
	////

	/// schityvaem faily etalonov
	string estrpath;
	string efolderpath;
	string estr;
	
	cout << "Input path to directory with etalon files!" << endl << "Directory with etalons is: ";
	cin >> estr;

	efolderpath = estr;

	const char* epath = estr.c_str();
	char *epath_ = 0;
	FileUtils::CpyCharStr(epath_, epath);
	FileUtils::CheckFolderSlash(epath_);
	efolderpath = epath_;
	FileUtils::AppendCharsAtEnd(epath_, "*.*", 3);

	char** etalons = folderfiles(epath_, setsize, efilescounter, "txt");
	///

	cout << "Start computing... " << endl << endl;

	FILE* f = 0;

	for (int i = 0 ; i < wavfilescounter; i++) // i=0
	{
		strpath = folderpath + wavfiles[i];
		const char* fullpath = strpath.c_str();

		//cout << "Fullpath is: " << fullpath << endl;
		cout << "Wav-file is: " << wavfiles[i] << endl << endl; // wavfile[i]

		f = fopen(fullpath, "rb"); // wavfile[i]

		if (f == NULL)
		{
			printf("File no open\n");
			system("pause");
			return 0;
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
			//cout << data_chunk[i] << endl;
		}
	////////////////////////////////////////////////////////////////////////////////////////////////

		///chistim stroky s putem k faily
		strpath.clear();
		///

	//spectral analysis

	int Fs = 22050;
	double overlap = 0.0;
	double NFFT = 512.0;
	double nfrm = samples_count / (NFFT*(1 - overlap)); // chunk.samples_count iz-za WavRead
	//int Nfrb = 30;
	char win[] = "hann";
	char type[] = "hz";

	/////////////////////////////// tut znachenia takie, chto bu poluchit odinakovuy matricy s etalonom
	int Nfrb = 35;
	int Nfrm = 17;
	//////////////////////////////
	double **spectr;
	spectr = speval_eq(data_chunk, Nfrm, overlap, Fs, Nfrb, win, type);  //my_function speval_eq s oknom hanna v hz
	////////////////////////////////////////////////////////////////////////////////////////////////
	//chtenie etalonov iz txt faila

	for (int j = 0; j < efilescounter; j++)
	{
		int len = 0;
		estrpath = efolderpath + etalons[j];

		const char* efullpath = estrpath.c_str();

		double* etalon = readDoubles((char*)efullpath, len);
		//////////////////////////////
		//vityagivanie matrici spectralnogo analiza v vector

		double* lin_spectr;

		lin_spectr = matrINvect(spectr, Nfrm, Nfrb);

		/////////////////////////////
		//sravnenie faila s etalonom
		int Nfrm_etalona = 17;
		int Nfrb_etalona = 35;

		int len_etalona = len;

		int col_vect = 2; // skolko vectorov: (shum), etalon, sravn_zapis

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
	////////////////////////////////////////////////////////////////////////////////////////////////
	cout << endl;
	///////
	delete[] spectr;  // ochistka pamyaty
	delete[] data_chunk;  // ochistka pamyaty
	delete[]value;  // ochistka pamyaty
	//////
	}

	system("pause");

	/*_CrtDumpMemoryLeaks();*/

	return 0;
}