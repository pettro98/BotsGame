#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <map>


namespace game_module
{
	using size_type = int;

	struct Pair
	{
		size_type First;
		size_type Second;
		~Pair() = default;
		Pair() = default;
		Pair(const Pair & pair);
		Pair(size_type first, size_type second);
		bool operator == (const Pair & pair) const;
		bool operator < (const Pair & pair) const;
	};

	bool operator != (const Pair & pair1, const Pair & pair2);

	std::ostream & operator << (std::ostream & out, const Pair & pair);

	enum unit_type
	{
		none = 0,
		army,
		tower,
		capital,
		farm,
		palm,
		pine,
		grave
	};
	
	bool is_none(unit_type type);
	bool is_army(unit_type type);
	bool is_tower(unit_type type);
	bool is_capital(unit_type type);
	bool is_farm(unit_type type);
	bool is_palm(unit_type type);
	bool is_pine(unit_type type);
	bool is_grave(unit_type type);
	bool is_tree(unit_type type);
	bool is_static(unit_type type);
	bool is_player_unit(unit_type type);
	bool is_ready_to_take(unit_type type);

	enum hex_color {
		black = -1,
		blank,
		red,
		green,
		orange,
		purple,
		blue,
		yellow,
		extra
	};

	bool is_color(hex_color color);
	bool is_black(hex_color color);
	bool is_blank(hex_color color);
	bool is_extra(hex_color color);
	bool is_player_color(hex_color color);

	struct Result
	{
	public:
		size_type winner;
		std::vector<size_type> points;
		std::vector<size_type> last_turn;
		std::vector<size_type> built_armies;
		std::vector<size_type> built_farms;
		std::vector<size_type> built_towers;
		std::vector<size_type> moves;
		Result();
	};
}
