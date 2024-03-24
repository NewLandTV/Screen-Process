#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <Windows.h>
#include "Dll.h"

namespace WindowApplication
{
	class DLL_DEFINE Window
	{
	private:
		HWND hWnd;
		MSG message;

	public:
		Window(HINSTANCE hInstance, const wchar_t* title, const wchar_t* className, unsigned int width, unsigned int height, WNDPROC windowProcedure);

		void Show(int cmdShow);
		void MessageLoop();

		HWND GetHWND();
	};
}

#endif