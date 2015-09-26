#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <memory.h>
#include <iostream>
#include <string>
using namespace std;

char** extendmas(char** mas, int newsize);
char** folderfiles(const char* path, int &setsize, int &filescounter);