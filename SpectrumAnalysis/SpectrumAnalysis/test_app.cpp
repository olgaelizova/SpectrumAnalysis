#include "stdafx.h"
#include <memory.h>  // analog mem.h in WinVS  // <memory.h> in unix
#include <stdio.h>

#include <math.h>
//#include "My_FFT.cpp"

#define _CRT_SECURE_NO_WARNINGS

using namespace std;

#include "My_FFT.h"
#include "zapis_etalona.h"
#include <iostream>
#include <fstream>
#include <string>
#include "FileUtils.h"
#include "SpectrumMathFunctions.h"
#include "FolderFiles.h"

etalon_			etalon;
wav_header_t	header;
chunk_t			chunk;

int main(void)
{
	//// schityvaem wav faily
	string strpath;
	string folderpath;
	string str;

	//cout << "Input path to directory with wav files!" << endl << "Directory with wav files is: ";
	//cin >> str;
	str = "C:\\test\\wavfiles\\";
	folderpath = str;

	str = str + "*.*";
	const char* path = str.c_str();
	int setsize = 2;

	char** wavfiles = folderfiles(path, setsize);
	////

	/// schityvaem faily etalonov
	string estrpath;
	string efolderpath;
	string estr;
	
	//cout << "Input path to directory with etalon files!" << endl << "Directory with etalons is: ";
	//cin >> estr;
	estr = "C:\\test\\etalons\\";
	efolderpath = estr;

	estr = estr + "*.*";
	const char* epath = estr.c_str();

	char** etalons = folderfiles(epath, setsize);
	///

	int n = 1; // tk pervie dva faila est . i ..

	cout << "Start computing... " << endl << endl;

	FILE* f = 0;

	for (int i = 0+n; i < 3+n; i++) // i=0
	{
		strpath = folderpath + wavfiles[i];
		const char* fullpath = strpath.c_str();

		cout << "Fullpath is: " << fullpath << endl;
		cout << "Wav-file is: " << wavfiles[i] << endl << endl; // wavfile[i]

		f = fopen(fullpath, "rb"); // wavfile[i]

		if (f == NULL)
		{
			printf("File no open\n");
			system("pause");
			return 0;
		}

		///chistim stroky s putem k faily
		strpath.clear();
		///

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
	//spectral analysis

	int Fs = 22050;
	double overlap = 0.0;
	double NFFT = 512.0;
	double nfrm = samples_count / (NFFT*(1 - overlap)); // chunk.samples_count iz-za WavRead
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
	/*
	char name1[40] = { "etalon_pilotag.txt" }; //20
	char name2[40] = { "etalon_mashtab.txt" }; //20
	char name3[44] = { "etalon_navigacia.txt" }; //22
	*/
	/*
	//etalon.pilotag = zapis_etalona(name1);		// gives mean value -3.1386171643444776
	etalon.pilotag		= readDoubles(name1);		// gives mean value -3.1406766853184722
	etalon.mashtab		= readDoubles(name2);
	etalon.navigacia	= readDoubles(name3);
	*/
	for (int j = 0 + n; j < 3 + n; j++)
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
		//int len_etalona = 595;
		int len_etalona = len;
		//cout << len_etalona << endl;
		int col_vect = 2; // skolko vectorov: (shum), etalon, sravn_zapis

		///// neverno tak kak dolgen bit k2 max dlya etalona so slovom

		/*	// dlya resultatov iz matlab
			double alpha = 0.95;
			double* tmp = new double[len_etalona];
			for (int i = 0; i < len_etalona; i++)
			{
			if (i == 0)
			{
			tmp[i] = 0;
			}
			else
			{
			tmp[i] = lin_spectr[i-1] * alpha;
			}
			//cout << tmp[1] << endl;
			}
			for (int i = 0; i < len_etalona; i++)
			{
			if (i != 0)
			{
			lin_spectr[i] = lin_spectr[i] - tmp[i];
			}
			}
			delete[] tmp;
			////
			*/

/*		int lDataOutput = 0;
		cout << "Etalon is: " << endl << endl;
		cout << "pilotag\n" << endl;
		koef_of_regr_korr(lin_spectr, etalon.pilotag, 1, col_vect, len_etalona); // (lens)5 to fix

		lDataOutput++;
		cout << endl;

		cout << "mashtab\n" << endl;
		koef_of_regr_korr(lin_spectr, etalon.mashtab, 1, col_vect, len_etalona);

		lDataOutput++;
		cout << endl;

		cout << "navigacia\n" << endl;
		koef_of_regr_korr(lin_spectr, etalon.navigacia, 1, col_vect, len_etalona);
		cout << endl << endl;
*/
		cout << "Etalon is: " << etalons[j] << endl;

		koef_of_regr_korr(lin_spectr, etalon, 1, col_vect, len_etalona); 

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
