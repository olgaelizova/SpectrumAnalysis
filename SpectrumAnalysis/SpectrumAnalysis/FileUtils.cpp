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
	const char * chars = new char[256];
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

		delete[] chars;

		fclose(file);
		
	}
}