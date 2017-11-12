#pragma once

#include "General.h"
#include "Map.h"
#include "Player.h"
#include "Hex.h"
#include "Unit.h"


namespace game_module
{

	class Game
		: public IFullAccess
	{
	private:
		std::vector<Player * > Players; // игроки, учавствующие в игре и их индексы
		hex_color CurrentPlayer;
		Map * GameMap; // указатель на карту
		size_type CurrentTurn; // текущий ход
		size_type MaxTurns; // количество ходов, через которое игра прекратится автоматически
		Result Results; // результаты соревнования, обновляются по ходу игры
	public:
		~Game();
		Game(size_type max_turns, size_type map_dimension, std::string map_type);
		bool check_end_game() const;
		Hex * get_hex(const Pair & hex);
		Hex * get_hex(size_type coord1, size_type coord2);
		Hex * operator () (const Pair & hex);
		Hex * operator () (size_type coord1, size_type coord2);
		Unit * get_unit(const Pair & hex);
		std::vector<Player *> & get_players();
		Player * get_player(hex_color seek_color);
		hex_color get_current_player() const;
		Map & get_game_map();
		size_type current_turn() const;
		size_type max_turns() const;
		Result & results();
		bool player_in_game(hex_color color) const;
		void set_unit(const Pair & hex, Unit * unit);
		void set_color(const Pair & hex, hex_color new_color);
		void turn_passed();
		void set_current_player(hex_color new_index);
		void add_points(size_type player_index, size_type points);
		void set_winner(size_type players_index);
		void set_last_turn(size_type player_index, size_type turn);
		void add_to_built_armies(size_type player_index, size_type str = 1);
		void add_to_built_farms(size_type player_index);
		void add_to_built_towers(size_type player_index, size_type str = 1);
		void add_to_moves(size_type player_index);
		bool make_players(Controller * controller);
		void prepare_player(Controller * controller);
		void double_trees(Controller * controller);
	};
}
