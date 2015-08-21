#pragma once
class FileUtils
{
public:
	FileUtils();
	~FileUtils();

	static void SaveMatrixToFile(char* fileName, double** natrix, int width, int height);
};

