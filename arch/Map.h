#pragma once

#include "General.h"
#include "Hex.h"
#include <set>
#include <functional>


namespace game_module
{
	class Map
	{
	private:
		Hex *** Root;
		size_type DimensionX;
		size_type DimensionY;
		std::string MapType;
	public:
		~Map();
		Map(size_type dimension_x = 19,
			size_type dimension_y = 19,
			size_type player_number = 4,
			const std::string & map_type = "classic");
		size_type dimension_x() const;
		size_type dimension_y() const;
		std::string map_type() const;
		Hex * operator () (const Pair & hex) const;
		Hex * operator () (size_type coord1, size_type coord2) const;
		hex_color color(const Pair & hex) const;
		unit_type type(const Pair & hex) const;
		bool hex_exist(const Pair & hex) const;
		void clear();
		bool get_neighbours_exist(size_type coord1, size_type coord2,
			std::function <bool(hex_color)> compare1 = [](hex_color color) { return true; },
			std::function <bool(unit_type)> compare2 = [](unit_type type) { return true; }) const;
		bool get_neighbours_exist(const Pair & hex,
			std::function <bool(hex_color)> compare1 = [](hex_color color) { return true; },
			std::function <bool(unit_type)> compare2 = [](unit_type type) { return true; }) const;
		std::vector<Pair> get_neighbours(size_type coord1, size_type coord2,
			std::function <bool(hex_color)> compare1 = [](hex_color color) { return true; },
			std::function <bool(unit_type)> compare2 = [](unit_type type) { return true; },
			bool return_black = false) const;
		std::vector<Pair> get_neighbours(const Pair & hex,
			std::function <bool(hex_color)> compare1 = [](hex_color color) { return true; },
			std::function <bool(unit_type)> compare2 = [](unit_type type) { return true; },
			bool return_black = false) const;
		std::vector<Pair> get_hex_row(const Pair & hex, size_type radius,
			std::function <bool(hex_color)> compare1 = [](hex_color color) { return true; },
			std::function <bool(unit_type)> compare2 = [](unit_type type) { return true; }) const;
		bool get_hex_row_exist(const Pair & hex, size_type radius,
			std::function <bool(hex_color)> compare1 = [](hex_color color) { return true; },
			std::function <bool(unit_type)> compare2 = [](unit_type type) { return true; }) const;
		std::vector<Pair> get_internal_border(const Pair & hex,
			std::function <bool(unit_type)> compare = [](unit_type type) { return true; }) const;
		std::vector<Pair> get_external_border(const Pair & hex,
			std::function <bool(hex_color)> compare1 = [](hex_color color) { return true; },
			std::function <bool(unit_type)> compare2 = [](unit_type type) { return true; }) const;
		std::vector<Pair> easy_solve_maze(const Pair & hex,
			std::function <bool(unit_type)> compare = [](unit_type type) { return true; }) const;
		size_type easy_solve_maze_count(const Pair & hex,
			std::function <bool(unit_type)> compare = [](unit_type type) { return true; }) const;
		std::vector<Pair> solve_maze(const Pair & hex);
	private:
		void generate_map(size_type player_number = 4);
		bool hex_acceptable(const Pair & hex,
		std::function <bool(hex_color)> compare1 = [](hex_color color) { return true; },
		std::function <bool(unit_type)> compare2 = [](unit_type type) { return true; },
		bool return_black = false) const;
	};

	void print_map(const Map & map);

	struct HexImpress
	{
		Pair Coordinates;
		hex_color Color;
		unit_type UnitType;
		HexImpress(const Hex & hex);
	};

	struct MapImpress
	{
		HexImpress *** Root;
		size_type DimensionX;
		size_type DimensionY;
		~MapImpress();
		MapImpress(const Map & map);
		HexImpress & operator () (const Pair & pair);
	};
}
