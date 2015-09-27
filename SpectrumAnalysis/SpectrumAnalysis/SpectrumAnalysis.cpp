#include "stdafx.h"
#include "SpectrumAnalysis.h"

extern wav_header_t		header;
extern chunk_t			chunk;

double** speval_eq(double* buffer, int Nfrm, double overlap, int Fs, int Nfrb, char* window, char* type) // function for spectral analys
{
	int L = chunk.size * 8 / header.wBitsPerSample;  // razmer massiva
	//std::cout << L << std::endl;
	double lfrm = double(L) / double(Nfrm*(1 - overlap) + overlap);
	int Lfrm = fix(lfrm);  //myfunction
	//printf("Lfrm %d\n",Lfrm);//++
	int noverlap = fix(overlap*Lfrm); //myfunction
	//printf("noverlap %d\n",noverlap);//++
	int h = Lfrm - noverlap; // why is h so big(858), when the whole array is 1024 and less ? (lens_comment)
	//printf("h %d\n",h);//++

	int NFFT = pow(2, nextpow2(Lfrm));  //myfunction
	//int np2 = nextpow2(Lfrm);
	//printf("nextpow2(Lfrm) %d\n",np2);//++
	//int pw = pow(2,9);
	//printf("pow(2,9) %d\n",pw);//++
	//printf("NFFT %d\n",NFFT);//++

	double* win = new double[Lfrm];
	win = feval(NFFT, window, Lfrm); //myfunction

	/// zapis v fail znachenii win
	///

	// formirovanie dopolneya massiva 0 do stepeni dvoiki
	int num_zero = noverlap + Nfrm*h - L;
	//int np = nextpow2(num_zero+L);
	//printf("nextpow2(zeros) %d\n",np); //++
	int bufsize = L;// + num_zero;
	double* buf = new double[bufsize];
	//buf = zeros_for_buffer(buffer, num_zero, L); // myfunction
	//buf = zeros_for_buffer(buffer, num_zero, L); // (lens)3
	memcpy(buf, buffer, sizeof(double) * L);
	///zapis v fail znachenyi zeros_for_buffer
	///

	int Fmin = 0;
	int Fmax = fix(header.nSamplesPerSec / 2);
	//printf("Fmax %d\n",Fmax); //++

	//delenie na chastotnye polosy
	int* iend = new int[Nfrb];
	int* ibeg = new int[Nfrb];
	double* freq = new double[Nfrb];

	if (strcmp(type, "hz") == 0)
	{
		double Fstep = (double)(Fmax - Fmin) / (double)Nfrb;
		//printf("Fstep %.4f\n",Fstep); //+
		double* Fbins = new double[Nfrb + 1];

		for (int i = 0; i<Nfrb + 1; i++)
		{
			Fbins[i] = (double)Fmin + (double)i*Fstep / 10000.;
			if (i == Nfrb + 1)
			{
				Fbins[i] = double(Fmax);
			}
		}


		for (int i = 0; i<Nfrb; i++)
		{
			if (i == 0)
			{
				freq[i] = ((double)Fmin + (double)Fstep / 2.) / 10000.;
			}
			else
			{
				freq[i] = freq[i - 1] + (double)Fstep / 10000.;
			}
		}

		for (int i = 0; i<Nfrb; i++)
		{
			if (i == 0)
			{
				ibeg[i] = floor(NFFT*Fbins[i] * 10000. / Fs) + 1;  //why not ceil? magic:(
				//ibeg[i]=ceil(NFFT*Fbins[i]*10000./Fs);
			}
			else
			{
				ibeg[i] = ceil(NFFT*Fbins[i] * 10000. / Fs) + 1; // 1 ( !!!!! ) (lens)(MISTAKE FOUND) //why not floor? magic:(
				//ibeg[i] = floor(NFFT*Fbins[i]*10000./Fs)+1;
			}
		}

		for (int i = 0; i<Nfrb; i++)
		{
			//iend[i] = ceil(NFFT*Fbins[i+1]*10000./Fs);
			iend[i] = floor(NFFT*Fbins[i + 1] * 10000. / Fs) + 1;
		}



	}
	//////////////////
	int* ind = new int[Nfrb];
	int* freqInt = new int[Nfrb - 2]; // freq cganged to freqInt

	if (strcmp(type, "mel") == 0)  // utochnit pravilnost vichislenia i ciklov !!!!!
	{
		int MFmin = (int)hz2mel((double)Fmin);
		int MFmax = (int)hz2mel((double)Fmax);
		int MFstep = (MFmax - MFmin) / (Nfrb + 1);

		int* Fbins = new int[Nfrb - 1];
		for (int i = 0; i<Nfrb - 1; i++)
		{
			double F = MFmin + i*MFstep;
			Fbins[i] = (int)mel2hz(F);
		}

		for (int i = 0; i<Nfrb; i++)
		{
			if (i == 0)
			{
				ind[i] = ceil(NFFT*Fmin / Fs);
			}
			if (i == Nfrb)
			{
				ind[i] = floor(NFFT*Fmax / Fs);
			}
			else
			{
				ind[i] = round(NFFT*Fbins[i] / Fs);
			}
		}

		for (int i = 2; i<Nfrb - 2; i++)
		{
			freqInt[i] = Fbins[i];
		}

	}
	//////////////////
	int end_beg_frm = (Nfrm - 1)*h + 1;

	int* beg_frm = new int[Nfrm];

	for (int i = 0; i<Nfrm; i++)
	{
		if (i == 0)
		{
			beg_frm[i] = 0;
		}
		else
		{
			beg_frm[i] = beg_frm[i - 1] + h;
		}
		//printf("%d\n", beg_frm[i]); //+
	}

	int end_end_frm = Lfrm + (Nfrm - 1)*h;
	int* end_frm = new int[Nfrm];
	for (int i = 0; i<Nfrm; i++)
	{
		if (i == 0)
		{
			end_frm[i] = Lfrm;
		}
		else
		{
			end_frm[i] = end_frm[i - 1] + h;
		}
		//printf("%d\n", end_frm[i]);  //+
	}

	double** sp;
	sp = zeros(Nfrm, Nfrb);  // myfunction - mas[m][n]=0;

	//bufsize = 14624;  Lfrm = 499; Nfrm = 30;

	double* xfrm = new double[NFFT];
	double* X = new double[NFFT];
	double* X_vrem = new double[NFFT];
	double sp_local = 0.0;
	double val = 0.0;
	//FILE *f;
	//f = fopen("xfrm_data.dat", "w");
	//f = fopen("fft_data.dat", "w");
	//f = fopen("sp_local_data.dat", "w");

	for (int k = 0; k<Nfrm; k++) //Nfrm
	{
		//fprintf(f,"%d\n\n",k);
		//printf("%d\n\n",k);  // ot 0 do 113 ++
		//printf("%d\t%d\n",beg_frm[k],end_frm[k]);
		int i = 0;
		for (int j = beg_frm[k]; j <= end_frm[k]; j++)
		{
			val = buf[j];  // viborka ++ po 499 znachenyi
			//fprintf(f,"%.4f\n",val);

			xfrm[i] = val; // pishem viborku dlya kagdogo k

			xfrm[i] = xfrm[i] * win[i]; //++
			//fprintf(f,"%.4f\n",xfrm[i]);  //++ po 499 znach 114 raz


			i++;
		}
		//fprintf(f,"\n");

		xfrm = zeros_for_buffer(xfrm, NFFT - Lfrm, Lfrm);

		X = FFTAnalysis(xfrm, X, NFFT, NFFT);  // problem solved (lens)4 problem solved

		for (int i = 0; i<NFFT; i++)
		{
			X[i] = fabs(X[i]);
		}


		//for(int i=0; i<NFFT; i++)
		//	fprintf(f,"%.4f\n",X[i]); //++

		//printf("%d\n",NFFT);  // NFFT = 512

		if (strcmp(type, "hz") == 0)
		{
			for (int l = 0; l<Nfrb; l++)  //Nfrb
			{
				int j = 0;
				if (l != 0 && l != 15)  // stranno s 15, tk tam v matlabe 8 znach a ne 9
				{
					for (int s = ibeg[l] - 1; s<iend[l]; s++) // s < -1 ????????
					{
						val = X[s];
						X_vrem[j] = val;
						//fprintf(f,"%.4f\n",val); //++
						j++;
					}
				}
				else
				{
					for (int s = ibeg[l] - 1; s<iend[l]; s++)
					{
						val = X[s];
						X_vrem[j] = val;
						//fprintf(f,"%.4f\n",val); //++
						j++;
					}
				}
				// tak kak j tochnyi schetchik shaga, a po iena-ibeg poluchaem menshe
				sp_local = sum(X_vrem, j); //iend[l]-ibeg[l]
				//printf("%d\n",j);
				//sp_local /= (iend[l]-ibeg[l]);
				sp_local /= (j);
				sp[k][l] = sp_local;
				//printf("%.4f\n", sp_local); //++
			}
		}
		//fclose(f);
	}
	//fclose(f);

	for (int i = 0; i< Nfrm; i++)  // dlya etalona Nfrm=35
	{
		for (int j = 0; j<Nfrb; j++)  // dlya etalona Nfrb = 17
		{
			sp[i][j] = log(sp[i][j]);
			//printf(" %.4f ",sp[i][j]);  //++
		}
		//printf("\n"); 
	}

	//printf("Nfrm = %d\t Nfrb = %d\n",Nfrm, Nfrb); // nachalnyi variant po speval_eq Nfrm=114,Nfrb=30

	/////// ochistka pamyaty
	delete[]X_vrem;
	delete[]X;
	delete[]xfrm;
	delete[]end_frm;
	delete[]beg_frm;
	delete[]freqInt;
	delete[]ind;
	delete[]ibeg;
	delete[]iend;
	delete[]buf;
	delete[]win;
	delete[] freq;
	//////

	FileUtils::SaveMatrixToFile("SpectrumData.txt", sp, Nfrm, Nfrb);

	return sp;
}