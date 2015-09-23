#include "stdafx.h"
#include "FolderFiles.h"

char** extendmas(char** mas, int newsize)
{
	cout << "bad" << endl;

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

	/*
	mas = new char*[newsize * 2];

	for (int i = 0; i < newsize; i++)
	{
		mas[i] = new char[260];
		memcpy(mas[i], buf[i], 260);
	}
	
	for (int i = 0; i < newsize; i++)
	{
		delete[] buf[i];
	}
	delete[] buf;
	
	*/
	return buf;
}

char** folderfiles(const char* path, int &setsize)
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
		do
		{
			if (file.name != "." && file.name != "..")
			{
				//cout << file.name << endl;

				nameset[pos] = new char[260];
				memcpy(nameset[pos], &file.name, 260);
				cout << nameset[pos] << endl;
				pos++;

				if (pos == setsize )
				{
					nameset = extendmas(nameset, setsize);
					setsize *= 2;
				}
			}

		} while (_findnext(hFile, &file) == 0);

		//Закрываем директорию
		_findclose(hFile);

	}


	return nameset;


}