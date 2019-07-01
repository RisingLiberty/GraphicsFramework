#pragma once

#include "graphics/window.h"

class Win64Window : public Window
{
public: 
	Win64Window(unsigned int width, unsigned int height, const std::wstring& title);
	virtual ~Win64Window();

	static LRESULT CALLBACK WindowProcdureStatic(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT HandleEvent(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	int EventLoop() override;
	void* GetHandle() const override;
private:
	HWND m_handle;
	HINSTANCE m_hinstance;
	HDC m_hdc;
};