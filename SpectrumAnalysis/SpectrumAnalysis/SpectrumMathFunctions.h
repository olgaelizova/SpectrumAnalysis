#pragma once

typedef unsigned short	WORD;
typedef unsigned int	DWORD;
typedef short			INT16;

struct wav_header_t  // wav header
{
	char rId[4]; //"RIFF" = 0x46464952
	DWORD rLen; //28 [+ sizeof(wExtraFormatBytes) + wExtraFormatBytes] + sum(sizeof(chunk.id) + sizeof(chunk.size) + chunk.size)
	char wId[4]; //"WAVE" = 0x45564157
	char fId[4]; //"fmt " = 0x20746D66
	DWORD fLen; //16 [+ sizeof(wExtraFormatBytes) + wExtraFormatBytes]
	WORD wFormatTag;
	WORD nChannels;
	DWORD nSamplesPerSec;
	DWORD nAvgBytesPerSec;
	WORD nBlockAlign;
	WORD wBitsPerSample;
	//[WORD wExtraFormatBytes;]
	//[Extra format bytes]
};

struct chunk_t  // dannye wav
{
	char id[4]; //"data" = 0x61746164
	DWORD size;
	//Chunk data bytes
};

struct etalon_
{
	double* pilotag;
	double* mashtab;
	double* navigacia;
	
};

double fix(double Number); // function for get num without .xxx part

double hz2mel(double Fo);  // perevod hz v mel

double mel2hz(double Fo);  // perevod mel v hz

int nextpow2(int b);  // poluchenie pokazatelya stepeni

double* feval(int NFFT, char* window, int Lfrm);  // function for evaluation window parameters

double sum(double* mas, int size);

double** speval_eq(double* buffer, int Nfrm, double overlap, int Fs, int Nfrb, char* window, char* type); // function for spectral analys

double* matrINvect(double** matr, int N, int M);

double** transp_matr(double** matr, int N, int M);

double** matrONmatr(double** matr1/*kvadr*/, double** matr2/*pryamoug*/, int N/*len*/, int M/*col*/);

double** transpONmatr(double** transp_matr, double** matr, int N, int M); // N=len, M=col

double* matrONvec(double** matr, double* vec, int N, int M);

double** obr_matr(double **A, int N); // proverit' pravilnost

double* substraction_vec(double* vec, int len, double s); // vichitanie chisla iz vectora

double* division_vec(double* vec, int len, double p); // delenie vectora na chislo

double* ones_vec(int len);

double mean(double* vec, int len);  //++

double std_vec(double* vec, int len);

double* pow_vec(double* vec, int len, double n);  // ne rabotaet pri pereprisvaivanii

double* abs_vec(double* vec, int len);

double norm(double* v, int len, double p);

int koef_of_regr_korr(double* buffer, double* etalon, int usenoise, int col, int len);