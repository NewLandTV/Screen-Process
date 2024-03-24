#include <iostream>
#include <string>
#include <Windows.h>

#define BIT_COUNT 4

// Enums
enum class Bit
{
	OFF,
	ON
};

// Global variables
Bit* bits[BIT_COUNT];

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		std::cout << "Usage : " << argv[0] << " [width] [height]";

		return 0;
	}

	// Get width and height
	unsigned short width = atoi(argv[1]);
	unsigned short height = atoi(argv[2]);

	// Displays a width x height screen
	for (unsigned short x = 0; x < width; x++)
	{
		for (unsigned short y = 0; y < height; y++)
		{
			std::string command("start E:\\Programming\\Languages\\Programming\\C++\\CPP_Lecture\\MultiScreen\\x64\\Debug\\ScreenProcess.exe ");

			command.append(std::to_string(x));
			command.append(" ");
			command.append(std::to_string(y));

			system(command.c_str());
		}
	}

	// Setup datas
	for (unsigned char i = 0; i < BIT_COUNT; i++)
	{
		bits[i] = new Bit[width * height];
	}

	for (unsigned int i = 0; i < width * height; i++)
	{
		bits[0][i] = Bit::ON;
		bits[1][i] = Bit::OFF;
		bits[2][i] = i & 1 ? Bit::ON : Bit::OFF;
		bits[3][i] = i & 1 ? Bit::OFF : Bit::ON;
	}

	// Control screen and draw
	COPYDATASTRUCT copyDataStruct;

	while (true)
	{
		if (GetAsyncKeyState(0x1b) & 0x8000)
		{
			break;
		}
		for (unsigned char i = 0; i < BIT_COUNT; i++)
		{
			if (GetAsyncKeyState(0x31 + i) & 0x8000)
			{
				for (unsigned short x = 0; x < width; x++)
				{
					for (unsigned short y = 0; y < height; y++)
					{
						// Set data
						copyDataStruct.dwData = 5;
						copyDataStruct.cbData = 2;
						copyDataStruct.lpData = (void*)(bits[i][x * height + y] == Bit::ON ? "1" : "0");

						// Find window by class name
						std::wstring className(L"ScreenProcessWindowApp_");

						className.append(std::to_wstring(x));
						className.append(L"x");
						className.append(std::to_wstring(y));

						SendMessage(FindWindow(className.c_str(), nullptr), WM_COPYDATA, NULL, (LPARAM)&copyDataStruct);
					}
				}
			}
		}
	}

	// Release datas
	for (unsigned char i = 0; i < BIT_COUNT; i++)
	{
		delete[] bits[i];
	}

	// Set data
	copyDataStruct.dwData = 4;

	// Destroy all windows
	for (unsigned short x = 0; x < width; x++)
	{
		for (unsigned short y = 0; y < height; y++)
		{
			// Find window by class name
			std::wstring className(L"ScreenProcessWindowApp_");

			className.append(std::to_wstring(x));
			className.append(L"x");
			className.append(std::to_wstring(y));

			SendMessage(FindWindow(className.c_str(), nullptr), WM_COPYDATA, NULL, (LPARAM)&copyDataStruct);
		}
	}

	return 0;
}