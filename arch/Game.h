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
		std::vector<Player> Players; // игроки, учавствующие в игре и их индексы
		Map * GameMap; // указатель на карту
		size_type CurrentTurn; // текущий ход
		size_type MaxTurns; // количество ходов, через которое игра прекратится автоматически
		Result * Results; // результаты соревнования, обновляются по ходу игры

	public:
		~Game();
		Game(size_type max_trs, size_type map_dimension, std::string map_type,
			std::map<size_type, Player> * players = nullptr);

		// базовые конструкторы, операторы 
		Game();
		Game(Game && game);
		Game & operator = (Game && game);
		Game(const Game & game);
		Game & operator = (const Game & game);
		//

		// получение полей класса 
		size_type current_turn() const;
		size_type max_turns() const;
		//

		bool check_end_game(); // проверяет не закончилась ли игра

		// IFullAccess

		//IGetData
		Hex * get_hex(pair hex);
		Hex * get_hex(size_type coord1, size_type coord2);
		Hex * operator () (pair hex);
		Hex * operator () (size_type coord1, size_type coord2);
		Unit * get_unit(pair hex);
		Army * get_army(pair hex);
		std::vector<Player> & get_players();
		Map & get_game_map();
		size_type current_turn() const;
		size_type max_turns() const;
		bool player_in_game(size_type player_index) const;
		//

		// ISetData
		void set_unit(pair hex, Unit * unit);
		void set_color(pair hex, size_t  index);
		void turn_passed();
		void add_points(size_type player_index, size_type points);
		void set_winner(size_type players_index);
		void set_last_turn(size_type player_index, size_type turn);
		void add_to_built_armies(size_type player_index, size_type str = 1);
		void add_to_built_farms(size_type players_index);
		void add_to_built_towers(size_type players_index, size_type str = 1);
		void add_to_moves(size_type players_index);
		//

		//

	private:
		bool make_players(); // создает объекты TPlayer в случае если игроков не передали в конструктор
		// вызывается в кострукторе
	};

}
