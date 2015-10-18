#include "stdafx.h"

//// libs for input wav files and txt files for etalons
#include "FileUtils.h"
#include "WavReader.h"
#include "FolderFiles.h"

#include <iostream>
#include <fstream>
#include <string>
////

char** inputWavFile(int setsize, int &wavfilescounter, string &strpath, string &folderpath, string &str);

char** inputEtalons(int setsize, int &efilescounter, string &estrpath, string &efolderpath, string &estr);