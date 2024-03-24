#include <string>
#include "../WindowApplication/Window.h"
#include <gdiplus.h>

// Window
#define MOVEABLE_WINDOW FALSE
#define WINDOW_SIZE 144
#define WINDOW_MOVING_SPEED 72
// CHESS_GAME #define ON_COLOR RGB(200, 165, 40)
// CHESS_GAME #define OFF_COLOR RGB(80, 74, 0)
#define ON_COLOR RGB(255, 255, 255)
#define OFF_COLOR RGB(0, 0, 0)
#define TRANSPARENT_COLOR RGB(254, 251, 252)

// Path
#define ASSET_PATH L"E:\\Programming\\Languages\\Programming\\C++\\CPP_Lecture\\MultiScreen\\Assets\\"

#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "WindowApplication.lib")

// Global variables
HINSTANCE hInstance;
ULONG_PTR token;

unsigned short windowX;
unsigned short windowY;

// Functions
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void DrawImage(HINSTANCE hInstance, HWND hWnd, HDC hdc, const wchar_t* imagePath, unsigned int x, unsigned int y, int width, int height);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR args, int nCmdShow)
{
	::hInstance = hInstance;

	// Setup gdi+
	Gdiplus::GdiplusStartupInput input;

	Gdiplus::GdiplusStartup(&token, &input, nullptr);

	// Get starting x and y position
	char* context;

	ZeroMemory(&context, 0);

	char* command = strtok_s(args, " ", &context);
	unsigned short x = atoi(command);

	command = strtok_s(nullptr, " ", &context);

	unsigned short y = atoi(command);

	windowX = x * WINDOW_SIZE;
	windowY = y * WINDOW_SIZE;

	// Setup window
	std::wstring className(L"ScreenProcessWindowApp_");

	className.append(std::to_wstring(x));
	className.append(L"x");
	className.append(std::to_wstring(y));

	WindowApplication::Window window(hInstance, L"", className.c_str(), WINDOW_SIZE, WINDOW_SIZE, WindowProcedure);

	MoveWindow(window.GetHWND(), x * WINDOW_SIZE, y * WINDOW_SIZE, WINDOW_SIZE, WINDOW_SIZE, false);
	SetLayeredWindowAttributes(window.GetHWND(), TRANSPARENT_COLOR, 0, LWA_COLORKEY);
	SetMenu(window.GetHWND(), nullptr);

	window.Show(nCmdShow);
	window.MessageLoop();

	return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// Flags
	static bool draw = false;
	static bool transparent = false;

	// Structs
	RECT clientRect;
	HDC hdc;
	HDC hMemoryDC;
	HDC hTempDC;
	HBITMAP hBitmap;
	HBITMAP hOldBitmap;
	PAINTSTRUCT paintStruct;
	HPEN pen;
	HPEN oldPen;
	HBRUSH brush;
	HBRUSH oldBrush;
	PCOPYDATASTRUCT receiveCopyDataStruct;

	switch (message)
	{
	case WM_CREATE:
	{
		// Hide title bar
		long style = GetWindowLong(hWnd, GWL_STYLE);

		style &= ~WS_CAPTION;

		SetWindowLong(hWnd, GWL_STYLE, style);

		break;
	}
	case WM_KEYDOWN:
		if (!MOVEABLE_WINDOW)
		{
			break;
		}

		switch (wParam)
		{
		case VK_UP:
			MoveWindow(hWnd, windowX, (windowY -= WINDOW_MOVING_SPEED), WINDOW_SIZE, WINDOW_SIZE, false);

			break;
		case VK_LEFT:
			MoveWindow(hWnd, (windowX -= WINDOW_MOVING_SPEED), windowY, WINDOW_SIZE, WINDOW_SIZE, false);

			break;
		case VK_DOWN:
			MoveWindow(hWnd, windowX, (windowY += WINDOW_MOVING_SPEED), WINDOW_SIZE, WINDOW_SIZE, false);

			break;
		case VK_RIGHT:
			MoveWindow(hWnd, (windowX += WINDOW_MOVING_SPEED), windowY, WINDOW_SIZE, WINDOW_SIZE, false);

			break;
		}

		break;
	case WM_COPYDATA:
		receiveCopyDataStruct = reinterpret_cast<PCOPYDATASTRUCT>(lParam);

		if (receiveCopyDataStruct->dwData == 4)
		{
			DestroyWindow(hWnd);
		}
		else if (receiveCopyDataStruct->dwData == 5)
		{
			transparent = false;

			if (strcmp(reinterpret_cast<const char*>(receiveCopyDataStruct->lpData), "0") == 0)
			{
				draw = false;
			}
			else if (strcmp(reinterpret_cast<const char*>(receiveCopyDataStruct->lpData), "1") == 0)
			{
				draw = true;
			}
			else if (strcmp(reinterpret_cast<const char*>(receiveCopyDataStruct->lpData), "Transparent") == 0)
			{
				transparent = true;
			}
			else
			{
				char* context = nullptr;
				char* ptr = strtok_s(reinterpret_cast<char*>(receiveCopyDataStruct->lpData), ":", &context);

				if (strcmp(ptr, "img") == 0)
				{
					std::wstring path(ASSET_PATH);

					ptr = strtok_s(nullptr, ":", &context);

					if (atoi(ptr) == 0)
					{
						path += L"White";
					}
					else if (atoi(ptr) == 1)
					{
						path += L"Black";
					}

					ptr = strtok_s(nullptr, ":", &context);

					switch (atoi(ptr))
					{
					case 1:
						path += L"King";

						break;
					case 2:
						path += L"Queen";

						break;
					case 3:
						path += L"Rock";

						break;
					case 4:
						path += L"Bishop";

						break;
					case 5:
						path += L"Knight";

						break;
					case 6:
						path += L"Pawn";

						break;
					}

					path += L".png";

					hdc = GetDC(hWnd);

					DrawImage(hInstance, hWnd, hdc, path.c_str(), 0, 0, WINDOW_SIZE, WINDOW_SIZE);
					ReleaseDC(hWnd, hdc);
				}

				break;
			}

			InvalidateRect(hWnd, nullptr, false);
		}

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
		if (transparent)
		{
			pen = CreatePen(PS_SOLID, 1, TRANSPARENT_COLOR);
			oldPen = (HPEN)SelectObject(hdc, pen);
			brush = CreateSolidBrush(TRANSPARENT_COLOR);
			oldBrush = (HBRUSH)SelectObject(hdc, brush);
		}
		else
		{
			pen = CreatePen(PS_SOLID, 1, draw ? ON_COLOR : OFF_COLOR);
			oldPen = (HPEN)SelectObject(hdc, pen);
			brush = CreateSolidBrush(draw ? ON_COLOR : OFF_COLOR);
			oldBrush = (HBRUSH)SelectObject(hdc, brush);
		}

		Rectangle(hdc, 0, 0, clientRect.right, clientRect.bottom);
		SelectObject(hdc, oldPen);
		SelectObject(hdc, oldBrush);
		DeleteObject(pen);
		DeleteObject(brush);

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
		// Gdi+ shutdown
		Gdiplus::GdiplusShutdown(token);

		PostQuitMessage(0);

		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

void DrawImage(HINSTANCE hInstance, HWND hWnd, HDC hdc, const wchar_t* imagePath, unsigned int x, unsigned int y, int width, int height)
{
	Gdiplus::Image image(imagePath);
	Gdiplus::Rect rect(x, y, width, height);

	// Check error
	if (image.GetLastStatus() != Gdiplus::Ok)
	{
		return;
	}

	Gdiplus::Graphics graphics(hdc);

	graphics.DrawImage(&image, rect);
}