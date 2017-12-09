#include "Bot.h"
#include <string>
#include <algorithm> 
#include <map>

namespace game_module
{
	Bot::Bot(hex_color color, const std::string pl_name)
		: Player(color, pl_name)
	{ }

	void Bot::turn()
	{ 
		for (auto & i : get_capitals())
		{
			// buy farms
			auto hexs_to_place = hexs_to_place_farm(i);
			std::random_shuffle(hexs_to_place.begin(), hexs_to_place.end());
			custom_farm_sort(hexs_to_place);
			for (auto & j : hexs_to_place)
			{
				if (buy_farm(j) && get_district_money(i) + 5 < get_farm_cost(i))
				{
					break;
				}
			}
			// move armies
			for (auto & j : get_army_list(i))
			{
				auto hexs_to_capture = hexs_to_move_army(j);
				std::random_shuffle(hexs_to_capture.begin(), hexs_to_capture.end());
				custom_sort(hexs_to_capture);
				for (auto & p : hexs_to_capture)
				{		
					if (profit_to_move(j, color()) >= profit_to_move(p, color()))
					{
						hex_color basic_color = color();
						if (!get_neighbours_exist(j,
							[basic_color](hex_color color) { return color != basic_color; }))
						{
							move_to_enemy(j);
						}
						break;
					}
					make_move(j, p);
					break;
				}
			}
			// buy towers
			size_type money_limit = 0.6 * get_district_money(i);
			auto district_hexs = get_internal_border(i, is_none);
			std::random_shuffle(district_hexs.begin(), district_hexs.end());
			auto rule = custom_tower_sort(district_hexs);
			for (size_type j = 0; j < rule.size(); ++j)
			{

				if (get_district_income(i) < 3 || get_district_money(i) < get_tower_cost(2)
					|| money_limit > get_district_money(i))
				{ 
					break;
				}
				buy_tower(district_hexs[j], rule[j]);
			}
			// buy armies on statics
			auto static_units = get_district_static(i);
			std::random_shuffle(static_units.begin(), static_units.end());
			for (auto & j : static_units)
			{
				if (!buy_army(j))
				{
					break;
				}
			}		
			// buy armies
			auto hex_to_capture = get_external_border(i);
			std::random_shuffle(hex_to_capture.begin(), hex_to_capture.end());
			custom_sort(hex_to_capture);
			for (auto & j : hex_to_capture)
			{
				if (get_district_income(i) < 3 ||
					get_district_money(i) < get_army_cost(2))
				{
					break;
				}	
				buy_army(j, get_hex_strength(j) + 1);
			}
		}
	}

	void Bot::custom_sort(std::vector<Pair> & row) const
	{
		std::multimap<size_type, Pair> new_order;
		for (auto & i : row) 
		{
			new_order.insert(std::pair<size_type, Pair>(
				profit_to_move(i, color()), i));
		}
		auto original = row.begin();
		for (auto it = new_order.rbegin(); it != new_order.rend(); ++it)
		{
			*original = (*it).second;
			++original;
		}
	}

	void Bot::custom_farm_sort(std::vector<Pair> & row) const
	{
		std::multimap<size_type, Pair> new_order;
		for (auto & i : row)
		{
			new_order.insert(std::pair<size_type, Pair>(
				profit_to_place_farm(i), i));
		}
		auto original = row.begin();
		for (auto it = new_order.rbegin(); it != new_order.rend(); ++it)
		{
			*original = (*it).second;
			++original;
		}
	}

	std::vector<size_type> Bot::custom_tower_sort(std::vector<Pair> & row) const
	{
		std::multimap<size_type, std::pair<Pair, size_type>> new_order;
		for (auto & i : row)
		{
			if (profit_to_place_tower(i, 2) >= profit_to_place_tower(i, 3)
				&& profit_to_place_tower(i, 2) >= 15)
			{
				new_order.insert(std::pair<size_type, std::pair<Pair, size_type>>(
					profit_to_place_tower(i, 2), std::make_pair(i, 2)));
			}
			else if (profit_to_place_tower(i, 3) >= 30)
			{
				new_order.insert(std::pair<size_type, std::pair<Pair, size_type>>(
					profit_to_place_tower(i, 3), std::make_pair(i, 3)));
			}
		}
		row.clear();
		std::vector<size_type> rule;
		for (auto it = new_order.rbegin(); it != new_order.rend(); ++it)
		{
			rule.push_back(it->second.second);
			row.push_back(it->second.first);
		}
		return rule;
	}

	size_type Bot::profit_to_move(const Pair & hex, hex_color basic_color) const
	{
		size_type result(4 * get_neighbours(hex,
			[basic_color](hex_color color) { return basic_color == color; } ).size());
		if (color(hex) == basic_color)
		{
			if (is_static(get_type(hex)))
			{
				result += 24;
			}
			else
			{
				result -= 20;
				size_t size = get_neighbours(hex,
					[basic_color](hex_color color) { return basic_color != color; }).size();
				if (size <= 3)
				{
					result += 8 * size;
				}
				else
				{
					result += 24 - 12 * (size - 3);
				}
			}
		}
		else
		{
			result += 24;
			result += 8 * get_neighbours(hex,
				[basic_color](hex_color color) { return basic_color == color; },
				is_farm).size();
			if (is_army(get_type(hex)))
			{
				result += 4 * get_unit_strength(hex);
			}
			else if (is_farm(get_type(hex)))
			{
				result += 14;
			}
		}
		return result;
	}

	size_type Bot::profit_to_place_farm(const Pair & hex) const
	{
		size_type result(0);
		hex_color basic_color = color(hex);
		for (auto & i : get_neighbours(hex,
			[basic_color](hex_color color) { return color == basic_color; }))
		{
			++result;
			if (is_farm(get_type(i)) || is_capital(get_type(i)))
			{
				result += 10;
			}
		}
		result -= 4 * get_neighbours(hex, is_player_color).size();
		return result;
	}

	size_type Bot::profit_to_place_tower(const Pair & hex, size_type strength) const
	{
		size_type result(0);
		if (strength > get_hex_strength(hex))
		{
			result += strength - get_hex_strength(hex);
		}
		hex_color basic_color = color(hex);
		for (auto & i : get_neighbours(hex,
			[basic_color](hex_color color) { return basic_color == color; }))
		{
			if (strength > get_hex_strength(hex))
			{
				if (is_none(get_type(i)) || is_army(get_type(i)))
				{
					result += strength - get_hex_strength(i);
				}
				else if (is_farm(get_type(i)))
				{
					result += 2 * (strength - get_hex_strength(i));
				}
			}
			result += 2 * get_neighbours(hex,
				[basic_color](hex_color color)
			{ return (color != basic_color && color != game_module::hex_color::blank); }).size();
			result += 2 * get_neighbours(hex,
				[basic_color](hex_color color)
			{ return (color != basic_color && color != game_module::hex_color::blank); },
				is_army).size();
		}
		return result;
	}

	bool Bot::move_to_enemy(const Pair & hex)
	{
		if (!is_army(get_type(hex)))
		{
			return false;
		}
		Pair capital(hex);
		for (auto & i : get_players_colors())
		{
			if (i == color())
			{
				continue;
			}
			for (auto & j : get_player_capitals(i))
			{
				if (capital == hex || distance(capital, hex) > distance(j, hex))
				{
					capital = j;
				}
			}
		}
		Pair result(hex);
		if (capital != hex)
		{
			for (auto & i : hexs_to_move_army(hex))
			{
				if (result == hex || distance(result, capital) > distance(i, capital))
				{
					result = i;
				}
			}
		}
		return make_move(hex, result);
	}
}