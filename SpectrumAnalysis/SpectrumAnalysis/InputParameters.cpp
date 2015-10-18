#include "stdafx.h"

#include "InputParameters.h"

char** inputWavFile(int setsize, int &wavfilescounter, string &strpath, string &folderpath, string &str)
{
	//cout << "Input path to directory with wav files!" << endl << "Directory with wav files is: ";
	//cin >> str;
	str = "C:\\test\\wavfiles\\";
	folderpath = str;

	const char* path = str.c_str();
	char *path_ = 0;
	FileUtils::CpyCharStr(path_, path);
	FileUtils::CheckFolderSlash(path_);
	folderpath = path_;
	FileUtils::AppendCharsAtEnd(path_, "*.*", 3);

	char** wavfiles = folderfiles(path_, setsize, wavfilescounter, "wav");

	return wavfiles;
}

char** inputEtalons(int setsize, int &efilescounter, string &estrpath, string &efolderpath, string &estr)
{
	//cout << "Input path to directory with etalon files!" << endl << "Directory with etalons is: ";
	//cin >> estr;
	estr = "C:\\test\\etalons\\";
	efolderpath = estr;

	const char* epath = estr.c_str();
	char *epath_ = 0;
	FileUtils::CpyCharStr(epath_, epath);
	FileUtils::CheckFolderSlash(epath_);
	efolderpath = epath_;
	FileUtils::AppendCharsAtEnd(epath_, "*.*", 3);

	char** etalons = folderfiles(epath_, setsize, efilescounter, "txt");

	return etalons;
}