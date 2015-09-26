#include "stdafx.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

double* zapis_etalona(char name[]);

double * readDoubles(char* name, int &len);

int row_count(const char* filename);