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
					auto row = get_hex_row(j, k, get_map_dimension());
					std::random_shuffle(row.begin(), row.end());
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
			std::random_shuffle(hex_to_capture.begin(), hex_to_capture.end());
			for (auto & j : hex_to_capture)
			{
				if (get_district_income(i) < 3)
				{
					break;
				}
				auto neighbours = get_neighbours(j);
				std::random_shuffle(neighbours.begin(), neighbours.end());
				for (auto & k : get_neighbours(j))
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
}