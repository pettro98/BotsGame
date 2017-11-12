#include "General.h"
#include "Game.h"
#include "Controller.h"
#include "View.h"
#include "TestBot.h"
#include <windows.h>
#include <algorithm> 
#include <ctime> 
#include <string>

using namespace game_module;

int main() 
{
	std::srand(time(0));
	HANDLE out_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD maxWindow = GetLargestConsoleWindowSize(out_handle);
	SMALL_RECT srctWindow = { 0, 0, maxWindow.X - 1, maxWindow.Y - 1 };
	SMALL_RECT minWindow = { 0, 0, 0, 0 };
	SetConsoleWindowInfo(out_handle, true, &minWindow);
	SetConsoleScreenBufferSize(out_handle, maxWindow);
	SetConsoleWindowInfo(out_handle, true, &srctWindow);

	Game game(30, 19, "classic");
	Controller controller(&game);
	TestBot bot1(game_module::hex_color::red, "red");
	TestBot bot2(game_module::hex_color::green, "green");
	TestBot bot3(game_module::hex_color::orange, "orange");
	TestBot bot4(game_module::hex_color::purple, "purple");
	std::vector<Player *> players = { &bot1, &bot2, &bot3, &bot4 };
	game.get_players() = players;
	game.make_players(&controller);
	bool end = false;	
	print_map(game.get_game_map());
	while (game.current_turn() <= game.max_turns() && !end)
	{
		for (size_type i = 0; i < game.get_players().size(); ++i)
		{
			game.set_current_player(game.get_players()[i]->color());
			game.prepare_player(&controller);
			if (game.check_end_game())
			{
				end = true;
				break;
			}
			if (game.player_in_game(game.get_current_player()))
			{
				game.get_players()[i]->turn();
				std::cout << game.current_turn() << " " 
					<< game.get_player(game.get_current_player())->name() << std::endl;
				print_map(game.get_game_map());
			}
			else
			{
				if (game.results().last_turn[game.get_current_player() - 1] == 0)
				{
					game.set_last_turn(game.get_current_player(), game.current_turn());
				}
			}
		}
		game.double_trees(&controller);
		game.turn_passed();
	}
	for (size_type i = 0; i < game.get_players().size(); ++i)
	{
		if (game.results().last_turn[game.get_current_player() - 1] == 0)
		{
			game.set_last_turn(game.get_current_player(), game.max_turns());
		}
	}
	std::cout << std::endl;	
	system("pause");
}
