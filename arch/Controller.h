#pragma once

#include "General.h"
#include "Hex.h"
#include "IFullAccess.h"


namespace game_module
{

	class Controller
	{
	private:
		IFullAccess * MyAccess;
	public:
		~Controller() = default;
		Controller(IFullAccess * my_access);
		hex_color color(const Pair & hex) const;
		Pair capital(const Pair & hex) const;
		std::vector<Pair> get_neighbours(const Pair & hex) const;
		size_type distance(const Pair & hex1, const Pair & hex2) const;
		bool can_move(const Pair & hex1, const Pair & hex2, size_type move_points = Army::move_points()) const;
		unit_type get_unit_type(const Pair & hex) const;
		size_type get_unit_strength(const Pair & hex) const;
		size_type get_hex_strength(const Pair & hex) const;
		bool get_moved(const Pair & hex) const;
		size_type get_district_money(const Pair & hex) const;
		std::string get_map_type() const;
		size_type get_map_dimension() const;
		size_type get_players_number() const;
		size_type get_current_turn() const;
		size_type get_max_turns() const;
		bool can_place_static(const Pair & hex) const;
		bool can_place_army(const Pair & hex, size_type str = 1) const;
		std::vector<Pair> get_hex_to_capture(const Pair & hex) const;
		std::vector<Pair> get_district_units(const Pair & hex, unit_type type) const;
		std::vector<Pair> get_br_unit(const Pair & hex, unit_type type) const;
		std::vector<Pair> get_enemy_br_units(const Pair & hex) const;
		std::vector<Pair> get_district_static(const Pair & hex) const;
		std::vector<Pair> get_br_static(const Pair & hex) const;
		bool make_move(const Pair & start, const Pair & end);
		bool buy_tower(const Pair & hex, size_type strength = 1);
		bool buy_farm(const Pair & hex);
		bool buy_army(const Pair & hex, size_type strength = 1);
		size_type get_district_income(const Pair & hex) const;
		size_type get_farm_cost(const Pair & hex) const;
		size_type get_army_cost(size_type strength = 1) const;
		size_type get_tower_cost(size_type strength = 1) const;
		std::vector<Pair> get_army_list(const Pair & hex) const;
	private:
		Unit * get_unit(const Pair & hex) const;
		Capital * get_capital(const Pair & hex) const;
		void correct_capital(const Pair & hex);
		std::vector<std::set<Pair>> hexs_to_correct(const Pair & hex,
			hex_color color) const;
		void calculate_income(const std::vector<Pair> & district);
		friend class Game;
	};
}
