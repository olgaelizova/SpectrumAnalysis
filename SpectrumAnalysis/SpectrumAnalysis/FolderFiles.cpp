#include "stdafx.h"
#include "FolderFiles.h"
#include "FileUtils.h"

char** extendmas(char** mas, int newsize)
{
	char** buf = new char*[newsize*2];
	
	for (int i = 0; i < newsize; i++)
	{
		buf[i] = new char[260];
		memcpy(buf[i], mas[i], 260);
	}

	for (int i = 0; i < newsize; i++)
	{
		delete[] mas[i];
	}

	delete[] mas;

	return buf;
}

char** folderfiles(const char* path, int &setsize, int &filescounter, char* extension) // tolko dlya WINDOWS!
{
	char **nameset = new char*[setsize];
	int pos = 0;
	//Вспомогательные структуры и дескрипторы
	struct _finddata_t file;
	intptr_t hFile;

	//Открываем выбранную директорию
	if ((hFile = _findfirst(path, &file)) == -1L)
	{
		cerr << "Can not open catalog!" << endl;
	}
	else
	{
		//В цикле обходим файлы директории
		cout << "Files in folder " << path << " are: " << endl;
		while (_findnext(hFile, &file) == 0)
		{
			//if ((file.name != ".") || (file.name != ".."))
			if (FileUtils::CheckExtension(file.name, extension, 3))
			{
				nameset[pos] = new char[260];
				memcpy(nameset[pos], &file.name, 260);
				cout << nameset[pos] << endl;
				pos++;

				if (pos == setsize)
				{
					nameset = extendmas(nameset, setsize);
					setsize *= 2;
				}

				filescounter++;
			}
		}
		cout << endl;

		//Закрываем директорию
		_findclose(hFile);
	}

	return nameset;
}