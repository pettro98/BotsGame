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

		size_type get_index(const Pair & hex) const;
		size_type distance(const Pair & hex1, const Pair & hex2) const;
		bool can_move(const Pair & hex1, const Pair & hex2, size_type move_points = Army::move_points()) const;
		unit_type get_unit_type(Pair hex) const;
		size_type get_strength(Pair hex) const;
		bool get_moved(Pair hex) const;
		size_type get_district_money(Pair hex) const;
		size_type get_district_index(Pair hex) const;
		std::string get_map_type() const;
		size_type get_map_dimension() const;
		size_type get_players_number() const;
		bool can_place_static(Pair hex) const;
		bool can_place_army(Pair hex, size_type str = 1) const;
		std::vector<Pair> get_neighbours(Pair hex) const;
		std::vector<Pair> get_hex_to_capture(Pair hex) const;
		std::vector<Pair> get_district_units(Pair hex, unit_type type) const;
		std::vector<Pair> get_br_unit(Pair hex, unit_type type) const;
		std::vector<Pair> get_enemy_br_units(Pair hex) const;
		std::vector<Pair> get_enemy_inv_armies(Pair hex) const;
		std::vector<Pair> get_district_static(Pair hex) const;
		std::vector<Pair> get_br_static(Pair hex) const;

		bool make_move(Pair start, Pair end);
		bool buy_tower(Pair hex, size_type strength = 1);
		bool buy_farm(Pair hex);
		bool buy_army(Pair hex, size_type strength = 1);

		size_type get_district_income(Pair hex) const;
		size_type get_farm_cost(Pair hex) const;
		size_type get_army_cost(size_type strength = 1) const;
		size_type get_tower_cost(size_type strength = 1) const;
		std::vector<Pair> get_army_list(Pair hex) const;
	};

}
