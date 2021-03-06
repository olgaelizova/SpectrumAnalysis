#include "stdafx.h"
#include "SpectrumMathFunctions.h"
#include "My_FFT.h"
#include "FileUtils.h"
#include "SpectrumMathSupport.h"

extern etalon_			etalon;
extern wav_header_t		header;
extern chunk_t			chunk;





double fix(double Number) // function for get num without .xxx part
{
	bool Negative = Number < 0 ? true : false;
	Number = fabs(Number);

	if (Number > floor(Number) + 0.5)
		Number = floor(Number); //ceil
	else
		Number = floor(Number); //floor

	return Negative ? Number * (-1) : Number;
}

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

int nextpow2(int b)  // poluchenie pokazatelya stepeni
{
	int x = ceil(log(b) / log(2));
	return x;
}

double* feval(int NFFT, char* window, int Lfrm)  // function for evaluation window parameters
{
	double* win = new double[Lfrm];
	//double* multiplier = new double [NFFT];
	double pi = 3.14159;
	if (strcmp(window, "hann") == 0)
	{
		for (int i = 0; i<Lfrm; i++)
		{
			win[i] = 0.5*(1 - (cos(2 * pi*i / (Lfrm - 1))));
		}

	}

	if (strcmp(window, "hamming") == 0)
	{
		for (int i = 0; i<Lfrm; i++)
		{
			win[i] = 0.54 - (0.46*(cos(2 * pi*i / (NFFT - 1))));
		}
	}
	return win;
}

double sum(double* mas, int size)
{
	double sum = 0.0;

	for (int i = 0; i<size; i++)
	{
		sum += mas[i];
	}

	return sum;
}

double** speval_eq(double* buffer, int Nfrm, double overlap, int Fs, int Nfrb, char* window, char* type) // function for spectral analys
{

	// TODO: THIS IS THE START (lens)
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

double* matrINvect(double** matr, int N, int M)
{
	double* vec = new double[N*M];
	/*
	for (int i = 0; i< N; i++)
	{
		for (int j = 0; j<M; j++)
		{
			vec[i*M + j] = matr[i][j];

			//printf("%d\t%.4f\n",i*M+j, vec[i*M+j]);
		}
		//printf("\n");
	}
	*/

	for (int i = 0; i< M; i++)
	{
		for (int j = 0; j<N; j++)
		{
			vec[i*N + j] = matr[j][i];

			//printf("%d\t%.4f\n",i*M+j, vec[i*M+j]);
		}
		//printf("\n");
	}

	return vec;
}

double** transp_matr(double** matr, int N, int M)  // N=len, M=col
{

	
	double** t_matr = new double*[M];

	for (int i = 0; i<M; i++)
	{
		t_matr[i] = new double[N];
		for (int j = 0; j<N; j++)
		{
			//matr[i][j] += matr[j][i];
			//matr[j][i] = matr[i][j] - matr[j][i];
			//matr[i][j] -= matr[j][i];
			t_matr[i][j] = matr[j][i];
		}
	}

	

	// change N and M
	/*
	double** t_matr = new double*[N];

	for (int i = 0; i<N; i++)
	{
		t_matr[i] = new double[M];
		for (int j = 0; j<M; j++)
		{
			//matr[i][j] += matr[j][i];
			//matr[j][i] = matr[i][j] - matr[j][i];
			//matr[i][j] -= matr[j][i];
			t_matr[i][j] = matr[j][i];
		}
	}
	*/
	/*	for(int i=0; i<M; ++i)
	{
	for(int j=0; j<N; ++j)
	{
	printf("%d\t%d\t%.4f\t",i,j,t_matr[i][j]);
	}
	printf("\n\n");
	}
	*/

	return t_matr;
}

double** matrONmatr(double** matr1/*kvadr*/, double** matr2/*pryamoug*/, int N/*len*/, int M/*col*/)
{

	double** mul = new double*[M];

	for (int i = 0; i<M; i++)//stroki
	{
		mul[i] = new double[N];
		for (int j = 0; j<N; j++)//stolbcy
		{
			mul[i][j] = 0;
			for (int t = 0; t<2; t++)
			{
				mul[i][j] += matr1[i][t] * matr2[t][j];
			}

			//std::cout << mul[i][j] << "\n";
		}
	//	std::cout << "\n\n";
	}


	/*
	double** mul = new double*[N];

	for (int i = 0; i<N; i++)//stroki
	{
		mul[i] = new double[M];
		for (int j = 0; j<M; j++)//stolbcy
		{
			mul[i][j] = 0;
			for (int t = 0; t<2; t++)
			{
				mul[i][j] += matr1[t][i] * matr2[j][t];
			}

			std::cout << mul[i][j] << "\n";
		}
		std::cout << "\n\n";
	}

	*/

	return mul;
}

double** transpONmatr(double** transp_matr, double** matr, int N, int M) // N=len, M=col
{
	int size = 0, l = 0;
	if (N <= M) size = N, l = M;
	else size = M, l = N;

	//cout << size << endl; ==2
	//cout << l << endl; ==595

	double** mul = new double*[size];

	for (int i = 0; i<size; i++)
	{
		mul[i] = new double[size];
		for (int j = 0; j<size; j++)
		{
			mul[i][j] = 0;
			for (int k = 0; k < l; k++)
			{
				//mul[i][j] += transp_matr[i][k] * matr[k][j]; // some strange :(
				mul[i][j] += transp_matr[i][k] * matr[k][j];

				//cout << transp_matr[i][k] << endl;
				//if ((k % 100) == 0)
				//	bool stop = true;
				//cout << matr[k][j] << endl;
				//printf("\n transp");
				//cout << transp_matr[i][k] << endl;
			}

			//	cout << mul[i][j]<<endl;
		}
	}

	return mul;
}

double* matrONvec(double** matr, double* vec, int N, int M)
{
	int size_i = 0;
	int size_j = 0;
	if (N >= M)
	{
		size_i = M;
		size_j = N;
	}
	else
	{
		size_i = N;
		size_j = M;
	}

	//cout << size_i << "\t" << size_j << endl; // 2 595

	double* mul = new double[size_i];

	for (int i = 0; i<size_i; i++)
	{
		mul[i] = 0;

		for (int j = 0; j<size_j; j++)
		{
			mul[i] += matr[i][j] * vec[j];
		}
		//cout << mul[i] << endl;
	}

	return mul;
}

double** obr_matr(double **A, int N) // proverit' pravilnost
{
	double temp;

	double **E = new double *[N];

	for (int i = 0; i < N; i++)
		E[i] = new double[N];

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
		E[i][j] = 0.0;

		if (i == j)
			E[i][j] = 1.0;
		}

	for (int k = 0; k < N; k++)
	{
		temp = A[k][k];

		for (int j = 0; j < N; j++)
		{
			A[k][j] /= temp;
			E[k][j] /= temp;
		}

		for (int i = k + 1; i < N; i++)
		{
			temp = A[i][k];

			for (int j = 0; j < N; j++)
			{
				A[i][j] -= A[k][j] * temp;
				E[i][j] -= E[k][j] * temp;
			}
		}
	}

	for (int k = N - 1; k > 0; k--)
	{
		for (int i = k - 1; i >= 0; i--)
		{
			temp = A[i][k];

			for (int j = 0; j < N; j++)
			{
				A[i][j] -= A[k][j] * temp;
				E[i][j] -= E[k][j] * temp;
			}
		}
	}

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
		A[i][j] = E[i][j];
		//cout << A[i][j] <<endl;
		}

	for (int i = 0; i < N; i++)
		delete[] E[i];

	delete[] E;

	return A;
}

double* substraction_vec(double* vec, int len, double s) // vichitanie chisla iz vectora
{
	for (int i = 0; i<len; i++)
	{
		vec[i] = vec[i] - s;
		//cout << vec[i]<< endl;
	}

	return vec;
}

double* division_vec(double* vec, int len, double p) // delenie vectora na chislo
{
	for (int i = 0; i<len; i++)
		vec[i] = vec[i] / p;

	return vec;
}

double* ones_vec(int len)
{
	double* vec = new double[len];
	for (int i = 0; i<len; i++)
	{
		vec[i] = 1;
		//cout << vec[i] << endl;  //++
	}

	return vec;
}

double mean(double* vec, int len)  //++
{
	double m = 0;

	for (int i = 0; i<len; i++)
		m += vec[i];

	return (m / (double)len);
}

double std_vec(double* vec, int len)
{
	double S = 0.0;

	double m = mean(vec, len);

	double p = 1.0 / ((double)len - 1.0);
	//cout << p << endl;

	double S_pr = 0.0;

	for (int i = 0; i<len; i++)
		S_pr += pow(vec[i] - m, 2.0);

	//cout << S_pr << endl;

	S = pow(S_pr*p, 0.5);
	//cout << S << endl;

	return S;
}

double* pow_vec(double* vec, int len, double n)  // ne rabotaet pri pereprisvaivanii
{
	double* v = new double[len];
	for (int i = 0; i<len; i++)
	{
		v[i] = pow(vec[i], n);
		//printf(" %.4f ",v[i]); //++
	}
	//cout << endl << endl;
	return v;
}

double* abs_vec(double* vec, int len)
{
	for (int i = 0; i<len; i++)
	{
		if (vec[i] < 0)
			vec[i] = vec[i] * (-1.0);
		//printf(" %.4f ",vec[i]); //++
	}
	//cout << endl << endl;
	return vec;
}

double norm(double* v, int len, double p)
{
	//cout << len << endl;
	double n = 0;
	n = pow(sum(pow_vec(abs_vec(v, len), len, p), len), (1 / p));
	//cout << n <<endl;
	return n;
}

int koef_of_regr_korr(double* buffer, double* etalon, int usenoise, int col, int len)
{
	/// to chto ponyala
	double** X = new double*[len];  // 595 - dlina vectora kak etalona tak i spectra

	double* shum;
	double* constanta;
	constanta = ones_vec(len);  // constanta - vector iz edinic
	shum = ones_vec(len);  // na dannom etape shum - vector iz edinic

	double norm_const = norm(constanta, len, 2.0);
	//cout << norm_const << endl;  //++

	//double std_shum = std_vec(shum,len);
	//cout << std_shum << endl; // =1?

	double mean_etalon = mean(etalon, len);
	//cout << mean_etalon << endl; // pravilno!

	double* subst = substraction_vec(etalon, len, mean_etalon); //etalon[i]-mean_etalon
	double std_subst = std_vec(subst, len);
	//cout << std_subst << endl; // tipa poxoge

	for (int i = 0; i<len; i++)
	{
		X[i] = new double[col];	// na shum, etalon, raspozn_slovo

		X[i][0] = constanta[i] / norm_const; //normirovannye t.k. v matlab norm(x)=norm(x,2)
		//cout << X[i][0] << endl; //++ tochno pravilno
		X[i][1] = subst[i] / std_subst;	// etalon
		//cout << X[i][1] << endl;

		if (col == 3)
		{
			//X[i][2] = shum[i]/std_shum;	// pomexa
			//cout << X[i][2] << endl;
		}
	}

	double** proizv;
	double** quotient;
	double* koef;

	proizv = transpONmatr(transp_matr(X, len, col), X, len, col);

	quotient = matrONmatr(obr_matr(proizv, col), transp_matr(X, len, col), len, col);

	koef = matrONvec(quotient, buffer, len, col);

	std::cout << koef[1] << std::endl;
	/*double** matr1 = new double*[2]; // test
	double** matr2 = new double*[2];

	for(int i=0; i<2; i++)
	{	matr1[i] = new double[2];
	for(int j=0; j<2; j++)
	{
	matr1[i][j] = rand()%100-50;
	cout << matr1[i][j] << "\t";
	}
	cout << endl;
	}
	cout << endl;
	for(int i=0; i<2; i++)
	{	matr2[i] = new double[5];
	for(int j=0; j<5; j++)
	{
	matr2[i][j] = rand()%100-50;
	cout << matr2[i][j] << "\t";
	}
	cout << endl;
	}

	matrONmatr(matr1,matr2,5,2);
	*/

	// free() problem here
	//delete[] koef;
	//delete[] quotient;
	//delete[] proizv;
	delete[] shum;
	delete[] constanta;
	//delete[] subst;

	for (int i = 0; i<len; i++)
		delete[] X[i];
	delete[] X;

	return 0;
}
