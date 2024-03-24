#include "Window.h"

namespace WindowApplication
{
	Window::Window(HINSTANCE hInstance, const wchar_t* title, const wchar_t* className, unsigned int width, unsigned int height, WNDPROC windowProcedure)
	{
		// Setup window class
		WNDCLASSEX wndClassEx;

		wndClassEx.hInstance = hInstance;
		wndClassEx.lpszClassName = className;
		wndClassEx.lpfnWndProc = windowProcedure;
		wndClassEx.style = CS_DBLCLKS;
		wndClassEx.cbSize = sizeof(WNDCLASSEX);
		wndClassEx.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
		wndClassEx.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
		wndClassEx.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wndClassEx.lpszMenuName = nullptr;
		wndClassEx.cbClsExtra = 0;
		wndClassEx.cbWndExtra = 0;
		wndClassEx.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));

		if (!RegisterClassEx(&wndClassEx))
		{
			return;
		}

		// Make window handle
		hWnd = CreateWindowEx(WS_EX_LAYERED, className, title, WS_CAPTION | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, width, height, HWND_DESKTOP, nullptr, hInstance, nullptr);
	}

	void Window::Show(int cmdShow)
	{
		ShowWindow(hWnd, cmdShow);
	}

	void Window::MessageLoop()
	{
		while (GetMessage(&message, nullptr, 0, 0))
		{
			TranslateMessage(&message);

			DispatchMessage(&message);
		}
	}

	HWND Window::GetHWND()
	{
		return hWnd;
	}
}