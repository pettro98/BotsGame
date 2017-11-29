#include "General.h"
#include "Game.h"
#include "Controller.h"
#include "View.h"
#include "Bot.h"
#include <windows.h>
#include <algorithm> 
#include <ctime> 
#include <string>
#include <iostream>
#include <fstream>


using namespace game_module;

void start_game(Game & game, Controller & controller, bool show_map = false)
{
	HANDLE hSTDOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (show_map)
	{
		print_map(game.get_game_map());
	}
	while (game.current_turn() <= game.max_turns())
	{
		for (size_type i = 0; i < game.get_players().size(); ++i)
		{
			game.set_current_player(game.get_players()[i]->color());
			game.prepare_player(&controller);
			if (game.player_in_game(game.get_current_player()))
			{
				game.get_players()[i]->turn();
				SetConsoleTextAttribute(hSTDOut, 0x0007 | FOREGROUND_INTENSITY);
				std::cout << game.current_turn() << " "
					<< game.get_player(game.get_current_player())->name() << std::endl;
			}
		}
		for (size_type i = 0; i < game.get_players().size(); ++i)
		{
			if (!game.player_in_game(game.get_players()[i]->color())
				&& game.results().last_turn[game.get_players()[i]->color() - 1] == 0)
			{
				game.set_last_turn(game.get_players()[i]->color(), game.current_turn());
			}
		}
		if (game.check_end_game())
		{
			break;
		}
		if (show_map)
		{
			print_map(game.get_game_map());
		}
		game.double_trees(&controller);
		game.turn_passed();
	}
	if (show_map)
	{
		print_map(game.get_game_map());
	}
}

void show_results(Game & game)
{
	for (size_type i = 0; i < game.get_players().size(); ++i)
	{
		if (game.results().last_turn[game.get_players()[i]->color() - 1] == 0)
		{
			game.set_last_turn(game.get_players()[i]->color(), game.current_turn());
		}
	}
	for (size_type i = 0; i < game.get_players().size(); ++i)
	{
		std::cout << "color = " << game.get_players()[i]->name()
			<< ", last turn = " << game.results().last_turn[i]
			<< ", built farms = " << game.results().built_farms[i]
			<< ", built towers = " << game.results().built_towers[i]
			<< ", built armies = " << game.results().built_armies[i]
			<< ", moves = " << game.results().moves[i]
			<< std::endl;
	}
}

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

	Game game(500, 33, 27, "classic");
	Controller controller(&game);
	Bot bot1(game_module::hex_color::red, "red");
	Bot bot2(game_module::hex_color::green, "green");
	Bot bot3(game_module::hex_color::orange, "orange");
	Bot bot4(game_module::hex_color::purple, "purple");
	std::vector<Player *> players = { &bot1, &bot2, &bot3, &bot4 };
	game.get_players() = players;
	game.make_players(&controller);

	start_game(game, controller, true);
	show_results(game);
	system("pause");
	return 0;
}
