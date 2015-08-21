#include "stdafx.h"
#include "zapis_etalona.h"

#define _CRT_SECURE_NO_WARNINGS

using namespace std;

double* zapis_etalona(char name[])
{
	int **drobnya = new int*[1];

	ifstream input(name, ios_base::in);
	if (!input.is_open()) printf("Error input file\n");;

	double* drob = new double[595];
	double* cel = new double[595];
	int l = 0;

	int* znack = new int[595];
	int j = 0;

	while (!input.eof())
	{
		string value;
		input >> value;
		double z = 0.0;
		if (!value.empty())
		{
			int len = value.length();
			//cout << len << endl;
			for (int i = 0; i<1; i++)
			{
				drobnya[i] = new int[len - 3];
				z = 0.0;
				for (int k = 3; k<len; k++)
				{
					char s = value[k];
					drobnya[i][k - 3] = int(s - '0');
					z += double(drobnya[i][k - 3]) / pow(10, k - 2);
					//printf("%d", drobnya[i][k-3]);
					//printf("%.16f\t", z/pow(10,k-2));
				}
			}

			char znackstr = value[0];
			//cout << j <<"\t" << z << endl;
			if (znackstr == '-')
				znack[j] = 1;
			else
				znack[j] = 0;
			j++;


			//printf("%.16f", z);
			drob[l] = z;
			//printf("%.16f", drob[l]);
			//printf("\n");
			cel[l] = atof(value.c_str());
			//printf("%f\n", cel[l]);
		}
		l++;
	}
	input.close();

	//for(int i=0; i<595; i++)
	//cout << znack[i] << endl;

	double* massiv = new double[595];
	for (int i = 0; i<595; i++)
	{
		if (cel[i] <= 0 && znack[i] == 1)
		{
			massiv[i] = cel[i] - drob[i];
		}
		else
		{
			massiv[i] = cel[i] + drob[i];
		}
		//printf("%d\t%.14f\n", i,massiv[i]);
	}

	delete[] znack;
	delete[] cel;
	delete[] drob;
	for (int i = 0; i<1; i++)
		delete[] drobnya[i];
	delete[] drobnya;
	return massiv;
}
