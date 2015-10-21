#include "stdafx.h"

#ifdef _WIN32
#include <iostream>
//#include <windows.h>
//#include <mmsystem.h>
#include <fstream>

//#pragma comment(lib,"winmm.lib") 

#else
#ifdef __unix__
//TODO realization for UNIX
#else
#error Unsupported Implementation
#endif
#endif

double* VoiceWrite();