#pragma once

#include "General.h"
#include "Controller.h"
#include "Unit.h"


namespace game_module
{

	class Controller;

	class Player
	{
	private:
		hex_color Color;
		std::string Name;
		std::list<Pair> Capitals;
	protected:
		Controller * GameController;
	public:
		Player() = delete;
		virtual ~Player() = default;
		Player(hex_color color, const std::string & player_name);
		hex_color color() const;
		std::string name() const;	
		size_type get_capitals_number() const;
		std::list<Pair> get_capitals() const;
		bool operator == (const Player & player) const;
		virtual void turn() = 0;
		hex_color color(const Pair & hex) const;
		Pair capital(const Pair & hex) const;
		size_type distance(const Pair & hex1, const Pair & hex2) const;
		unit_type get_type(const Pair & hex) const;
		size_type get_unit_strength(const Pair & hex) const;
		size_type get_hex_strength(const Pair & hex) const;
		bool get_moved(const Pair & hex) const;
		size_type get_district_money(const Pair & hex) const;
		size_type get_district_income(const Pair & hex) const;
		size_type get_farms_number(const Pair & hex) const;
		std::string get_map_type() const;
		size_type get_map_dimension_x() const;
		size_type get_map_dimension_y() const;
		size_type get_players_number() const;
		std::vector<hex_color> get_players_colors() const;
		std::list<Pair> get_player_capitals(hex_color color) const;
		size_type get_current_turn() const;
		size_type get_max_turns() const;
		bool can_move(const Pair & hex1, const Pair & hex2) const;
		bool can_place_tower(const Pair & hex) const;
		bool can_place_farm(const Pair & hex) const;
		bool can_place_army(const Pair & hex, size_type strength = 1) const;
		std::vector<Pair> hexs_to_place_farm(const Pair & hex) const;
		std::vector<Pair> hexs_to_move_army(const Pair & hex) const;
		std::vector<Pair> get_district_units(const Pair & hex, unit_type type) const;
		std::vector<Pair> get_district_static(const Pair & hex) const;
		std::vector<Pair> get_army_list(const Pair & hex) const;
		bool get_neighbours_exist(const Pair & hex,
			std::function <bool(hex_color)> compare1 = [](hex_color color) { return true; },
			std::function <bool(unit_type)> compare2 = [](unit_type type) { return true; }) const;
		std::vector<Pair> get_neighbours(const Pair & hex,
			std::function <bool(hex_color)> compare1 = [](hex_color color) { return true; },
			std::function <bool(unit_type)> compare2 = [](unit_type type) { return true; }) const;
		std::vector<Pair> get_hex_row(const Pair & hex, size_type radius,
			std::function <bool(hex_color)> compare1 = [](hex_color color) { return true; },
			std::function <bool(unit_type)> compare2 = [](unit_type type) { return true; }) const;
		bool get_hex_row_exist(const Pair & hex, size_type radius,
			std::function <bool(hex_color)> compare1 = [](hex_color color) { return true; },
			std::function <bool(unit_type)> compare2 = [](unit_type type) { return true; }) const;
		std::vector<Pair> easy_solve_maze(const Pair & hex,
			std::function <bool(unit_type)> compare = [](unit_type type) { return true; }) const;
		size_type easy_solve_maze_count(const Pair & hex,
			std::function <bool(unit_type)> compare = [](unit_type type) { return true; }) const;
		std::vector<Pair> get_internal_border(const Pair & hex,
			std::function <bool(unit_type)> compare = [](unit_type type) { return true; }) const;
		std::vector<Pair> get_external_border(const Pair & hex,
			std::function <bool(hex_color)> compare1 = [](hex_color color) { return true; },
			std::function <bool(unit_type)> compare2 = [](unit_type type) { return true; }) const;
		size_type get_farm_cost(const Pair & hex) const;
		size_type get_army_cost(size_type strength = 1) const;
		size_type get_tower_cost(size_type strength = 1) const;
		bool make_move(const Pair & start, const Pair & end);
		bool buy_tower(const Pair & hex, size_type strength = 1);
		bool buy_farm(const Pair & hex);
		bool buy_army(const Pair & hex, size_type strength = 1);
	private:
		void add_capital(const Pair & capital);
		bool remove_capital(const Pair & capital);
		void set_controller(Controller * controller);
		friend class Game;
		friend class Controller;
	};

	bool operator != (const Player & player1, const Player & player2);
}
