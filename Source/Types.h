/*
  ==============================================================================

    Types.h
    Created: 31 Oct 2018 1:21:26pm
    Author:  Tyler

  ==============================================================================
*/

#pragma once

#ifdef _WIN32
#include <windows.h>
	//typedef DWORD UINT32;
	typedef unsigned long UINT32; //should be UL *****fix
#else 
	typedef unsigned int UINT32;
#endif