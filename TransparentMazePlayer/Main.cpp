#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <Windows.h>

#define SCREEN_PROCESS_WIDTH 13
#define SCREEN_PROCESS_HEIGHT 7

// Macros
#define CheckMazeRange(x, y) (x > 0 && x < SCREEN_PROCESS_WIDTH - 1 && y > 0 && y < SCREEN_PROCESS_HEIGHT - 1)

// Enums
enum class Direction
{
	Left,
	Up,
	Right,
	Down
};

enum class MazeData
{
	Wall,
	Empty,
	Visited,
	Finish
};

// Global variables
const int DIRECTION[4][2] =
{
	{ 0, -2 },
	{ 0, 2 },
	{ -2, 0 },
	{ 2, 0 }
};

MazeData maze[SCREEN_PROCESS_HEIGHT][SCREEN_PROCESS_WIDTH];

unsigned short playerXPosition = 1;
unsigned short playerYPosition = 1;

// Functions
void Init();

void ShuffleArray(Direction array[], int size);
void GenerateMaze(int x, int y);
COORD GetRandomCoord();

void ClearPlayer();
void DrawPlayer();

int main()
{
	Init();

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
			// Find window by class name
			std::wstring className(L"ScreenProcessWindowApp_");

			className.append(std::to_wstring(x));
			className.append(L"x");
			className.append(std::to_wstring(y));

			// Background color
			copyDataStruct.dwData = 5;
			copyDataStruct.cbData = 12;
			copyDataStruct.lpData = (void*)"Transparent";

			SendMessage(FindWindow(className.c_str(), nullptr), WM_COPYDATA, NULL, reinterpret_cast<LPARAM>(&copyDataStruct));
		}
	}

	DrawPlayer();

	float inputTimer = 0.0f;
	bool visibleMaze = false;

	while (true)
	{
		if (maze[playerYPosition][playerXPosition] == MazeData::Finish)
		{
			MessageBeep(MB_ICONINFORMATION);

			MessageBox(GetConsoleWindow(), L"GAME CLEAR!", L"Information", MB_OK | MB_ICONINFORMATION);

			std::cout << "GAME CLEAR!" << std::endl;

			visibleMaze = true;
			inputTimer = -0.5f;
		}

		for (unsigned short x = 0; x < SCREEN_PROCESS_WIDTH; x++)
		{
			for (unsigned short y = 0; y < SCREEN_PROCESS_HEIGHT; y++)
			{
				if (maze[y][x] != MazeData::Wall)
				{
					continue;
				}

				// Find window by class name
				std::wstring className(L"ScreenProcessWindowApp_");

				className.append(std::to_wstring(x));
				className.append(L"x");
				className.append(std::to_wstring(y));

				// Background color
				copyDataStruct.dwData = 5;
				copyDataStruct.cbData = 12;
				copyDataStruct.lpData = (void*)(visibleMaze ? "0" : "Transparent");

				if (maze[y][x] == MazeData::Finish)
				{
					copyDataStruct.lpData = (void*)"1";
				}

				SendMessage(FindWindow(className.c_str(), nullptr), WM_COPYDATA, NULL, reinterpret_cast<LPARAM>(&copyDataStruct));
			}
		}

		if ((GetAsyncKeyState('W') & 0x8000 || GetAsyncKeyState('w') & 0x8000) && playerYPosition > 0 && maze[playerYPosition - 1][playerXPosition] != MazeData::Wall)
		{
			ClearPlayer();

			playerYPosition--;

			DrawPlayer();
		}
		if ((GetAsyncKeyState('A') & 0x8000 || GetAsyncKeyState('a') & 0x8000) && playerXPosition > 0 && maze[playerYPosition][playerXPosition - 1] != MazeData::Wall)
		{
			ClearPlayer();

			playerXPosition--;

			DrawPlayer();
		}
		if ((GetAsyncKeyState('S') & 0x8000 || GetAsyncKeyState('s') & 0x8000) && playerYPosition + 1 < SCREEN_PROCESS_HEIGHT && maze[playerYPosition + 1][playerXPosition] != MazeData::Wall)
		{
			ClearPlayer();

			playerYPosition++;

			DrawPlayer();
		}
		if ((GetAsyncKeyState('D') & 0x8000 || GetAsyncKeyState('d') & 0x8000) && playerXPosition + 1 < SCREEN_PROCESS_WIDTH && maze[playerYPosition][playerXPosition + 1] != MazeData::Wall)
		{
			ClearPlayer();

			playerXPosition++;

			DrawPlayer();
		}

		if (GetAsyncKeyState(0x20) & 0x8000 && inputTimer >= 1.0f)
		{
			inputTimer = 0.0f;
			visibleMaze = !visibleMaze;
		}

		if (inputTimer < -0.1f)
		{
			Sleep(5000);

			break;
		}

		inputTimer += 0.05f;

		if (GetAsyncKeyState(0x1b) & 0x8000)
		{
			break;
		}

		Sleep(50);
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

void Init()
{
	// Set random seed
	srand(static_cast<unsigned int>(time(nullptr)));

	COORD startPoint = GetRandomCoord();

	memset(maze, static_cast<int>(MazeData::Wall), sizeof(maze));
	GenerateMaze(startPoint.X, startPoint.Y);

	maze[SCREEN_PROCESS_HEIGHT - 2][SCREEN_PROCESS_WIDTH - 2] = MazeData::Finish;
}

void ShuffleArray(Direction array[], int size)
{
	for (int i = 0; i < size; i++)
	{
		int randomIndex = rand() % (size - i) + i;

		Direction temp = array[i];

		array[i] = array[randomIndex];
		array[randomIndex] = temp;
	}
}

void GenerateMaze(int x, int y)
{
	Direction directions[4]
	{
		Direction::Up,
		Direction::Right,
		Direction::Down,
		Direction::Left
	};

	maze[y][x] = MazeData::Visited;

	ShuffleArray(directions, 4);

	for (int i = 0; i < 4; i++)
	{
		int nx = DIRECTION[static_cast<int>(directions[i])][0] + x;
		int ny = DIRECTION[static_cast<int>(directions[i])][1] + y;

		if (CheckMazeRange(nx, ny) && maze[ny][nx] == MazeData::Wall)
		{
			GenerateMaze(nx, ny);

			if (y != ny)
			{
				maze[(y + ny) >> 1][x] = MazeData::Empty;
			}
			else
			{
				maze[y][(x + nx) >> 1] = MazeData::Empty;
			}

			maze[ny][nx] = MazeData::Empty;
		}
	}
}

COORD GetRandomCoord()
{
	unsigned short x = rand() % (SCREEN_PROCESS_WIDTH - 1) + 1;
	unsigned short y = rand() % (SCREEN_PROCESS_HEIGHT - 1) + 1;

	if (!(x & 1))
	{
		x--;
	}
	if (!(y & 1))
	{
		y--;
	}

	COORD result;

	result.X = x;
	result.Y = y;

	return result;
}

void ClearPlayer()
{
	// Find window by class name
	std::wstring className(L"ScreenProcessWindowApp_");

	className.append(std::to_wstring(playerXPosition));
	className.append(L"x");
	className.append(std::to_wstring(playerYPosition));

	// Background color
	COPYDATASTRUCT copyDataStruct;

	copyDataStruct.dwData = 5;
	copyDataStruct.cbData = 12;
	copyDataStruct.lpData = (void*)"Transparent";

	SendMessage(FindWindow(className.c_str(), nullptr), WM_COPYDATA, NULL, reinterpret_cast<LPARAM>(&copyDataStruct));
}

void DrawPlayer()
{
	// Find window by class name
	std::wstring className(L"ScreenProcessWindowApp_");

	className.append(std::to_wstring(playerXPosition));
	className.append(L"x");
	className.append(std::to_wstring(playerYPosition));

	// Background color
	COPYDATASTRUCT copyDataStruct;

	copyDataStruct.dwData = 5;
	copyDataStruct.cbData = 2;
	copyDataStruct.lpData = (void*)"1";

	SendMessage(FindWindow(className.c_str(), nullptr), WM_COPYDATA, NULL, reinterpret_cast<LPARAM>(&copyDataStruct));
}