#include "stdafx.h"

#include "Win64Window.h"
#include "DefaultWndClass.h"

#include "graphics/Context.h"

namespace
{
	PIXELFORMATDESCRIPTOR GetPixelFormat()
	{
		PIXELFORMATDESCRIPTOR pfd = {};
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 24;
		pfd.cStencilBits = 8;
		pfd.cAuxBuffers = 0;
		pfd.iLayerType = PFD_MAIN_PLANE;
		return pfd;
	}
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Win64Window::Win64Window(unsigned int width, unsigned int height, const std::string& title) :
	Window(title, width, height),
	m_hinstance(GetModuleHandle(0))
{
	std::wstring wide_title(title.begin(), title.end());

	const std::wstring window_class_name = L"DirectXWindowClass";
	DefaultWndClass wnd_class((WNDPROC)WindowProcdureStatic, m_hinstance, window_class_name.c_str());

	RECT r = { 0,0, (LONG)width, (LONG)height };
	AdjustWindowRect(&r, WS_OVERLAPPED, false);
	int win_width = r.right - r.left;
	int win_height = r.bottom - r.top;

	m_handle = CreateWindowExW(
		0L,
		window_class_name.c_str(),
		wide_title.c_str(),
		WS_OVERLAPPED | WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		win_width,
		win_height,
		NULL,			//no parent window
		nullptr,		//not using menus
		m_hinstance,
		this
	);

	if (!m_handle)
	{
		DWORD err = GetLastError();
		ThrowIfFailedWindow(HRESULT_FROM_WIN32(err));
	}

	m_hdc = GetDC(m_handle);
	PIXELFORMATDESCRIPTOR pfd = GetPixelFormat();
	int pixel_format = ChoosePixelFormat(m_hdc, &pfd); // let windows choose pixel format.
	if (pixel_format)
	{
		if (!SetPixelFormat(m_hdc, pixel_format, &pfd))
			ThrowException("Failed to set pixel format!");
	}
	else
		ThrowException("Failed to choose pixel format!");

	ShowWindow(m_handle, SW_SHOWDEFAULT);

	ImGui::CreateContext();
	ImGui_ImplWin32_Init(m_handle);
}

Win64Window::~Win64Window()
{
	ImGui_ImplWin32_Shutdown();
}

LRESULT Win64Window::WindowProcdureStatic(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_CREATE)
	{
		CREATESTRUCT* pCs = reinterpret_cast<CREATESTRUCT*>(lParam);
		SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCs->lpCreateParams));
	}
	else
	{
		Win64Window* pThis = reinterpret_cast<Win64Window*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
		if (pThis)
			return pThis->HandleEvent(hwnd, message, wParam, lParam);
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

LRESULT Win64Window::HandleEvent(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, message, wParam, lParam))
		return true;

	switch (message)
	{
		//Is sent when the window is being destroyed
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_ACTIVATE:
		if (wParam == WA_ACTIVE || wParam == WA_CLICKACTIVE)
			m_is_paused = false;
		else
			m_is_paused = true;
		return 0;
	case WM_RBUTTONDOWN:
		//OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_RBUTTONUP:
		//OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_MOUSEMOVE:
		//OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
}

int Win64Window::EventLoop()
{
	MSG message = { 0 };

	while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);

		if (message.message == WM_QUIT)
			return message.message;
	}

	return message.message;
}

void* Win64Window::GetHandle() const
{
	return m_handle;
}


void Win64Window::Present()
{
	Context::GetCurrent()->Present();
}