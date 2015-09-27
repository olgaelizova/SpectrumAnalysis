#include "stdafx.h"
#include "FileUtils.h"


FileUtils::FileUtils()
{
}


FileUtils::~FileUtils()
{
}

void FileUtils::SaveMatrixToFile(char* f, double** m, int w, int h)
{
	FILE * file = 0;
	const char * chars;
	std::string str;

	file = fopen(f, "wb");

	if (file)
	{
		std::ostringstream out;

		for (int b = 0; b < h; b++)
		{
			out << "Row number:";
			out << b + 1;
			out << "\n";

			for (int a = 0; a < w; a++)
			{
				out << m[a][b];
				out << "\n";

				str = out.str();
				chars = str.c_str();

				fwrite(chars, str.length(), 1, file );

				out.str("");
			}

			out << "\r\n";

		}

		fclose(file);
		
	}
}

bool FileUtils::CheckExtension(char *a, char* b, int l)
{
	bool res = true;
	int pos = 0;

	for (;;)
	{
		if (a[pos] == '\0')
			break;

		pos++;
	}

	pos -= l;

	for (int i = 0; i < l; i++)
	{
		if (b[i] != a[pos + i])
			return false;
	}
}

void FileUtils::CheckFolderSlash( char* &path)
{
	int pos = 0;

	for (;;)
	{
		if (path[pos] == '\0')
			break;

		pos++;
	}

	if (path[pos - 1] == '\\' )
		return;

	char * newPath = new char[pos+2];
	memcpy(newPath, path, pos + 1);

	newPath[pos] = '\\';
	newPath[pos + 1] = '\0';

	path = newPath;

}

void FileUtils::AppendCharsAtEnd( char* &d, char* s, int n)
{
	int pos = 0;

	for (;;)
	{
		if (d[pos] == '\0')
			break;

		pos++;
	}

	char * newD = new char[pos + n];
	memcpy(newD, d, pos);
	memcpy(&newD[pos], s, n);

	newD[pos + n - 1] = '\0';

	d = newD;
}