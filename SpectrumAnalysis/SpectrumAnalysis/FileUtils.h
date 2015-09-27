#pragma once
class FileUtils
{
public:
	FileUtils();
	~FileUtils();

	static void SaveMatrixToFile(char* fileName, double** natrix, int width, int height);
	static bool CheckExtension(char* toCheck, char* etalonExt, int len);
	static void CheckFolderSlash( char* &path);
	static void AppendCharsAtEnd( char* &dst, char* src, int n);
	static void CpyCharStr(char * &dest, const char* src);

};

