#pragma once

class DefaultWndClass : public WNDCLASS
{
public:
	DefaultWndClass(WNDPROC wndProc, HINSTANCE hInst, LPCWSTR name);
	~DefaultWndClass();
};