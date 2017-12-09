#include "General.h"
#include "Game.h"
#include "Controller.h"
#include "View.h"
#include "Bot.h"
#include <windows.h>
#include <ctime> 

using namespace game_module;

int main() 
{
	HANDLE out_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD maxWindow = GetLargestConsoleWindowSize(out_handle);
	SMALL_RECT srctWindow = { 0, 0, maxWindow.X - 1, maxWindow.Y - 1 };
	SMALL_RECT minWindow = { 0, 0, 0, 0 };
	SetConsoleWindowInfo(out_handle, true, &minWindow);
	SetConsoleScreenBufferSize(out_handle, maxWindow);
	SetConsoleWindowInfo(out_handle, true, &srctWindow);

	std::srand(time(0));
	Game game(500, 29, 18, "classic");
	game.start_game();
	game.show_results();
	system("pause");
	return 0;
}
