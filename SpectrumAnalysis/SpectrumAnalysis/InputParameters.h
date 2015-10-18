#include "stdafx.h"

/// default values for spectrum analysis
int samples_count = 0;
int Fs = 22050;   // default - ==header.nSamplesPerSec
double overlap = 0.0;
double NFFT = 512.0;
char win[] = "hann";  // okno hanna, vtoroi variant s oknom hamminga
char type[] = "hz";

//// tut znachenia takie, chto bu poluchit odinakovuy matricy s etalonom
int Nfrb = 35;
int Nfrm = 17;

////////
int len = 0;
int len_etalona = 0;
int Nfrm_etalona = 17;
int Nfrb_etalona = 35;
int col_vect = 2; // skolko vectorov: (shum), etalon, sravn_zapis
//////