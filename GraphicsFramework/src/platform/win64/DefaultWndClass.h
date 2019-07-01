#pragma once

class DefaultWndClass : public WNDCLASSW
{
public:
	DefaultWndClass(WNDPROC wndProc, HINSTANCE hInst, LPCWSTR name);
	~DefaultWndClass();
};