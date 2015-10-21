#include "stdafx.h"

#include "PushButtonHandler.h"

void pushButtonHandler(int &flag)
{
	char x;

	x = _getch();
	switch (x) //перед этим было switch(_getch())
	{
	case 32:
		//cout << "Space button" << endl;
		cout << endl;
		flag = 32;
		break;
	case 13:
		//cout << "Enter button" << endl;
		cout << endl;
		flag = 13;
		break;
	case 8:
		//cout << "Backspace button" << endl;
		cout << endl;
		flag = 8;
		break;
	}
}