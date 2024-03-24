#include <iostream>
#include <string>
#include <Windows.h>

#define CHESS_BOARD_SIZE 8

// Enums
enum class Piece
{
	Empty,
	King,
	Queen,
	Rock,
	Bishop,
	Knight,
	Pawn
};

// Global variables
Piece board[CHESS_BOARD_SIZE][CHESS_BOARD_SIZE];

int main()
{
	// Displays a width x height screen
	for (unsigned short x = 0; x < CHESS_BOARD_SIZE; x++)
	{
		for (unsigned short y = 0; y < CHESS_BOARD_SIZE; y++)
		{
			std::string command("start E:\\Programming\\Languages\\Programming\\C++\\CPP_Lecture\\MultiScreen\\x64\\Debug\\ScreenProcess.exe ");

			command.append(std::to_string(x));
			command.append(" ");
			command.append(std::to_string(y));

			system(command.c_str());
		}
	}

	// Setup datas
	ZeroMemory(&board, sizeof(board));

	board[0][0] = Piece::Rock;
	board[1][0] = Piece::Knight;
	board[2][0] = Piece::Bishop;
	board[3][0] = Piece::Queen;
	board[4][0] = Piece::King;
	board[5][0] = Piece::Bishop;
	board[6][0] = Piece::Knight;
	board[7][0] = Piece::Rock;
	board[0][1] = Piece::Pawn;
	board[1][1] = Piece::Pawn;
	board[2][1] = Piece::Pawn;
	board[3][1] = Piece::Pawn;
	board[4][1] = Piece::Pawn;
	board[5][1] = Piece::Pawn;
	board[6][1] = Piece::Pawn;
	board[7][1] = Piece::Pawn;
	board[0][6] = Piece::Pawn;
	board[1][6] = Piece::Pawn;
	board[2][6] = Piece::Pawn;
	board[3][6] = Piece::Pawn;
	board[4][6] = Piece::Pawn;
	board[5][6] = Piece::Pawn;
	board[6][6] = Piece::Pawn;
	board[7][6] = Piece::Pawn;
	board[0][7] = Piece::Rock;
	board[1][7] = Piece::Knight;
	board[2][7] = Piece::Bishop;
	board[3][7] = Piece::Queen;
	board[4][7] = Piece::King;
	board[5][7] = Piece::Bishop;
	board[6][7] = Piece::Knight;
	board[7][7] = Piece::Rock;

	// Control screen and draw
	COPYDATASTRUCT copyDataStruct;

	for (unsigned short x = 0; x < CHESS_BOARD_SIZE; x++)
	{
		for (unsigned short y = 0; y < CHESS_BOARD_SIZE; y++)
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
			copyDataStruct.lpData = (void*)(x + CHESS_BOARD_SIZE + y & 1 ? "0" : "1");

			SendMessage(FindWindow(className.c_str(), nullptr), WM_COPYDATA, NULL, reinterpret_cast<LPARAM>(&copyDataStruct));

			// Piece image
			copyDataStruct.dwData = 5;

			std::string data("img:");

			data += white ? "0" : "1";
			data += ":";
			data += std::to_string(static_cast<int>(board[x][y]));

			copyDataStruct.cbData = data.length() + 1;
			copyDataStruct.lpData = reinterpret_cast<void*>(const_cast<char*>(data.c_str()));

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
	for (unsigned short x = 0; x < CHESS_BOARD_SIZE; x++)
	{
		for (unsigned short y = 0; y < CHESS_BOARD_SIZE; y++)
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