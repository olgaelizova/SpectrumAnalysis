#include "stdafx.h"
#include "zapis_etalona.h"

#define _CRT_SECURE_NO_WARNINGS
#define DEFAULT_ETALON_PILOTAG_SIZE	596

using namespace std;

double * readDoubles(char* name, int &len)
{
	////
	int default_size = row_count(name)+1;
	//int default_size = DEFAULT_ETALON_PILOTAG_SIZE;
	//cout << "Row count is: " << row_count(name) << endl;
	////

	int pos = 0;
	//double * result = new double[DEFAULT_ETALON_PILOTAG_SIZE];
	double * result = new double[default_size];

	ifstream input(name, ios_base::in);

	if (!input.is_open()) printf("Error input file\n");

	while (!input.eof())
	{
		result[pos] = 0;

		string value;
		input >> value;
		double z = 0.0;

		int		sign = 1;
		bool	point = false;
		int		decPosition = 0;

		for (int a = 0; a < value.length(); a++) // find the point
		{

			if ( (value[a] == ',') || (value[a] == '.') )
			{
				point		= true;
				decPosition = a;
				break;
			}
		}

		if (point)
		for (int a = value.length() - 1; a > decPosition; a--) // calculate decimal part
		{
			if  ( (value[a] >= '0') && (value[a] <= '9') )
			{
				double digit = value[a] - '0';
				result[pos] += (double) 1 / pow(10, ( a - decPosition ) ) * digit;
			}
		}

		int decPower = 0;

		for (int a = decPosition - 1; a >= 0; a--) // calculate natural part
		{
			if ((value[a] >= '0') && (value[a] <= '9'))
			{
				double digit = value[a] - '0';
				result[pos] += pow(10, decPower) * digit;
				decPower++;
			}
		}
		if (value[0] == '-')
		{
			result[pos] *= -1; 
		}
/*		else
		{
			bool stop = true;
		}
*/
		pos++;
		len++;
	}

	input.clear();
	input.close();

	return result;
}

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

int row_count(const char* filename) 
{
/*	
// tolko dlya failov s yavnym perexodom na druguy stroku
	FILE* fp;
	int cnt = 0;

	if ((fp = fopen(filename, "r")) == NULL)
		return 0;

	while (!feof(fp) && !ferror(fp)) 
	{
		fscanf(fp, "%*[^\n]%*c");
		cnt++;
		//cout << cnt << endl;
	}

	fclose(fp);
	//cout << cnt << endl;
	return cnt;
*/

// dlya lubyx failov
	int len = 0;

	ifstream input(filename, ios_base::in);

	if (!input.is_open()) printf("Error input file\n");

	while (!input.eof())
	{
		string value;
		input >> value;

		len++;
	}

	input.clear();
	input.close();

	return len;
}