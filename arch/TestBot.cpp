#include "TestBot.h"
#include <string>
#include <algorithm> 

namespace game_module
{
	TestBot::TestBot(hex_color color, const std::string pl_name)
		: Player(color, pl_name)
	{ }

	void TestBot::turn()
	{ 
		for (auto & i : get_capitals())
		{
			while (get_district_income(i) + 5 > get_farm_cost(i))
			{
				for (auto & j : get_district_units(i, farm))
				{
					bool bought = false;
					auto neigbours = get_neighbours(j);
					std::random_shuffle(neigbours.begin(), neigbours.end());
					for (auto & k : neigbours)
					{
						if (buy_farm(k))
						{
							if (get_district_income(i) + 5 < get_farm_cost(i))
							{
								bought = true;
							}
							break;
						}
					}
					if (bought)
					{
						break;
					}
				}
				for (auto & j : get_district_units(i, game_module::unit_type::capital))
				{
					bool bought = false;
					auto neigbours = get_neighbours(j);
					std::random_shuffle(neigbours.begin(), neigbours.end());
					for (auto & k : neigbours)
					{
						if (buy_farm(k))
						{
							if (get_district_income(i) + 5 < get_farm_cost(i))
							{
								bought = true;
							}
							break;
						}
					}
					if (bought)
					{
						break;
					}
				}
				break;
			}
			for (auto & j : get_army_list(i))
			{
				for (size_type k = 1; k < 6; ++k)
				{
					auto row = get_hex_row(j, k);
					custom_sort(row);
					for (auto & p : row)
					{
						if ((get_unit_type(p) == palm
							|| get_unit_type(p) == pine
							|| get_unit_type(p) == grave
							|| color(p) != color())
								&& can_move(j, p))
						{
							make_move(j, p);
							k = 6;
							break;
						}
					}
				}
			}
			auto static_units = get_district_static(i);
			std::random_shuffle(static_units.begin(), static_units.end());
			for (auto & j : static_units)
			{
				if (!buy_army(j))
					break;
			}		
			auto hex_to_capture = get_hex_to_capture(i);
			custom_sort(hex_to_capture);
			for (auto & j : hex_to_capture)
			{
				if (get_district_income(i) < 3)
				{
					break;
				}
				auto neighbours = get_neighbours(j);
				custom_sort(neighbours);
				for (auto & k : neighbours)
				{
					if (capital(k) == capital(i)
						&& is_none(get_unit_type(k))
						&& buy_army(k, get_hex_strength(j) + 1))
					{
						make_move(k, j);
						break;
					}
				}
			}			
		}
	}

	void TestBot::custom_sort(std::vector<Pair> & row)
	{
		std::multimap<size_type, Pair> new_order;
		for (auto & i : row) 
		{
			new_order.insert(std::pair<size_type, Pair>(
				profit_points(i, color()), i));
		}
		auto original = row.begin();
		for (auto it = new_order.rbegin(); it != new_order.rend(); ++it)
		{
			*original = (*it).second;
			++original;
		}
	}

	size_type TestBot::profit_points(const Pair & hex, hex_color basic_color) const
	{
		size_type result(2 * get_neighbours(hex,
			[basic_color](hex_color color) { return basic_color == color; } ).size());
		if (color(hex) == basic_color)
		{
			if (is_static(get_unit_type(hex)))
			{
				result += 2;
			}
			else
			{
				result -= 4;
			}
		}
		else if (is_army(get_unit_type(hex)))
		{
			result += get_unit_strength(hex);
		}
		else if (is_farm(get_unit_type(hex)))
		{
			result += 4;
		}
		result += 4 * get_neighbours(hex,
			[basic_color](hex_color color) { return basic_color == color; },
			is_farm).size();
		return result;
	}
}