#include "stdafx.h"
#include "DefaultWndClass.h"

DefaultWndClass::DefaultWndClass(WNDPROC wndProc, HINSTANCE hInst, LPCWSTR name)
{
	style = CS_HREDRAW | CS_VREDRAW;
	lpfnWndProc = wndProc;
	hInstance = hInst;
	hCursor = LoadCursor(NULL, IDC_ARROW);
	lpszClassName = name;

	// Set other values to 0
	hIcon = 0;
	cbClsExtra = 0;
	cbWndExtra = 0;
	hbrBackground = 0;
	lpszMenuName = 0;

	RegisterClass(this);
}

DefaultWndClass::~DefaultWndClass()
{
	UnregisterClass(lpszClassName, hInstance);
}