#include "../WindowApplication/Window.h"

#pragma comment(lib, "WindowApplication.lib")

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR args, int nCmdShow)
{
	WindowApplication::Window window(hInstance, L"End", L"EndWindowApp", 768, 432, WindowProcedure);

	window.Show(nCmdShow);
	window.MessageLoop();

	return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// Variables
	static int xPosition = -32;

	// Flags
	static bool myOwn = false;

	// Structs
	RECT clientRect;
	HDC hdc;
	HDC hMemoryDC;
	HDC hTempDC;
	HBITMAP hBitmap;
	HBITMAP hOldBitmap;
	PAINTSTRUCT paintStruct;
	COPYDATASTRUCT copyDataStruct;
	PCOPYDATASTRUCT receiveCopyDataStruct;

	switch (message)
	{
	case WM_CREATE:
		MoveWindow(hWnd, 752, 0, 768, 432, TRUE);

		break;
	case WM_CHAR:
		myOwn = xPosition >= 0;

		if (!myOwn)
		{
			InvalidateRect(hWnd, nullptr, false);
		}
		else
		{
			switch (wParam)
			{
			case 'A':
			case 'a':
				xPosition -= 32;

				break;
			case 'D':
			case 'd':
				xPosition += 32;

				break;
			}

			InvalidateRect(hWnd, nullptr, false);

			break;
		}

		copyDataStruct.dwData = 1;
		copyDataStruct.cbData = 2;

		switch (wParam)
		{
		case 'A':
		case 'a':
			copyDataStruct.lpData = (void*)"L";

			break;
		case 'D':
		case 'd':
			copyDataStruct.lpData = (void*)"R";

			break;
		}

		SendMessage(FindWindow(L"StartWindowApp", L"Start"), WM_COPYDATA, NULL, (LPARAM)&copyDataStruct);

		break;
	case WM_COPYDATA:
		receiveCopyDataStruct = reinterpret_cast<PCOPYDATASTRUCT>(lParam);

		if (receiveCopyDataStruct->dwData == 1)
		{
			if (strcmp(reinterpret_cast<const char*>(receiveCopyDataStruct->lpData), "L") == 0)
			{
				xPosition -= 32;
			}
			else if (strcmp(reinterpret_cast<const char*>(receiveCopyDataStruct->lpData), "R") == 0)
			{
				xPosition += 32;
			}
		
			InvalidateRect(hWnd, nullptr, false);
		}

		myOwn = xPosition >= 0;

		break;
	case WM_PAINT:
		// Double buffering begin process
		GetClientRect(hWnd, &clientRect);

		hdc = BeginPaint(hWnd, &paintStruct);
		hMemoryDC = CreateCompatibleDC(hdc);
		hBitmap = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
		hOldBitmap = (HBITMAP)SelectObject(hMemoryDC, hBitmap);

		PatBlt(hMemoryDC, 0, 0, clientRect.right, clientRect.bottom, BLACKNESS);

		hTempDC = hdc;
		hdc = hMemoryDC;
		hMemoryDC = hTempDC;

		// Draw
		if (myOwn)
		{
			Rectangle(hdc, xPosition, (clientRect.bottom >> 1) - 32, xPosition + 64, (clientRect.bottom >> 1) + 32);
		}

		// Double buffering end process
		hTempDC = hdc;
		hdc = hMemoryDC;
		hMemoryDC = hTempDC;

		GetClientRect(hWnd, &clientRect);
		BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, hMemoryDC, 0, 0, SRCCOPY);
		SelectObject(hMemoryDC, hOldBitmap);
		DeleteObject(hOldBitmap);
		DeleteDC(hMemoryDC);
		EndPaint(hWnd, &paintStruct);

		break;
	case WM_DESTROY:
		PostQuitMessage(0);

		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}