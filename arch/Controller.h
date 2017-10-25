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
		size_type get_index(pair hex) const;
		size_type distance(pair hex1, pair hex2) const;
		bool can_move(const Hex & hex1, const Hex & hex2) const;
		unit_type get_unit_type(pair hex) const;
		size_type get_strength(pair hex) const;
		size_type get_capital_index(pair hex) const;
		bool get_moved(pair hex) const;
		size_type get_district_money(pair hex) const;
		size_type get_district_index(pair hex) const;
		std::string get_map_type() const;
		size_type get_map_dimension() const;
		size_type get_players_number() const;
		bool can_place_static(pair hex) const;
		bool can_place_army(pair hex, size_type str = 1) const;
		std::vector<pair> get_neighbours(pair hex) const;
		std::vector<pair> get_hex_to_capture(pair hex) const;
		std::vector<pair> get_district_units(pair hex, unit_type type) const;
		std::vector<pair> get_enemy_br_units(size_type district_index) const;
		std::vector<pair> get_enemy_inv_armies(size_type district_index) const;
		std::vector<pair> get_enemy_br_towers(size_type district_index) const;
		std::vector<pair> get_enemy_br_farms(size_type district_index) const;
		std::vector<pair> get_district_static(size_type district_index) const;
		std::vector<pair> get_br_static(size_type district_index) const;
		std::vector<pair> get_district_palms(size_type district_index) const;
		std::vector<pair> get_br_palms(size_type district_index) const;

		bool make_move(pair start, pair end);
		bool buy_tower(pair hex, size_type strength = 1);
		bool buy_farm(pair hex);
		bool buy_army(pair hex, size_type strength = 1);

		size_type get_district_income(pair hex) const;
		size_type get_district_costs(pair hex) const;
		size_type get_farm_cost(size_type district_index) const;
		size_type get_army_cost(size_type strength = 1) const;
		size_type get_tower_cost(size_type strength = 1) const;
		std::vector<pair> get_army_list() const;
	};

}
