#include <iostream>
#include <string>
#include <Windows.h>

#define SCREEN_PROCESS_WIDTH 10
#define SCREEN_PROCESS_HEIGHT 5

int main()
{
	// Displays a width x height screen
	for (unsigned short x = 0; x < SCREEN_PROCESS_WIDTH; x++)
	{
		for (unsigned short y = 0; y < SCREEN_PROCESS_HEIGHT; y++)
		{
			std::string command("start E:\\Programming\\Languages\\Programming\\C++\\CPP_Lecture\\MultiScreen\\x64\\Debug\\ScreenProcess.exe ");

			command.append(std::to_string(x));
			command.append(" ");
			command.append(std::to_string(y));

			system(command.c_str());
		}
	}

	// Control screen and draw
	COPYDATASTRUCT copyDataStruct;

	for (unsigned short x = 0; x < SCREEN_PROCESS_WIDTH; x++)
	{
		for (unsigned short y = 0; y < SCREEN_PROCESS_HEIGHT; y++)
		{
			bool white = y >= 6;

			// Find window by class name
			std::wstring className(L"ScreenProcessWindowApp_");

			className.append(std::to_wstring(x));
			className.append(L"x");
			className.append(std::to_wstring(y));

			// Background color
			copyDataStruct.dwData = 5;
			copyDataStruct.cbData = 2;
			copyDataStruct.lpData = (void*)"0";

			SendMessage(FindWindow(className.c_str(), nullptr), WM_COPYDATA, NULL, reinterpret_cast<LPARAM>(&copyDataStruct));
		}
	}

	while (true)
	{
		if (GetAsyncKeyState(0x1b) & 0x8000)
		{
			break;
		}
	}

	// Set data
	copyDataStruct.dwData = 4;

	// Destroy all windows
	for (unsigned short x = 0; x < SCREEN_PROCESS_WIDTH; x++)
	{
		for (unsigned short y = 0; y < SCREEN_PROCESS_HEIGHT; y++)
		{
			// Find window by class name
			std::wstring className(L"ScreenProcessWindowApp_");

			className.append(std::to_wstring(x));
			className.append(L"x");
			className.append(std::to_wstring(y));

			SendMessage(FindWindow(className.c_str(), nullptr), WM_COPYDATA, NULL, reinterpret_cast<LPARAM>(&copyDataStruct));
		}
	}

	return 0;
}