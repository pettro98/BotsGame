#include "Controller.h"


namespace game_module
{

	Controller::Controller(IFullAccess * my_access)
		: MyAccess(my_access)
	{ }

	hex_color Controller::color(const Pair & hex) const
	{
		return (*MyAccess)(hex)->color();
	}

	Pair Controller::capital(const Pair & hex) const
	{
		if ((*MyAccess)(hex)->get_hex_capital() != nullptr)
		{
			return (*MyAccess)(hex)->get_hex_capital()->coordinates();
		}
		return Pair(0, 0);
	}

	bool Controller::get_neighbours_exist(const Pair & hex,
		std::function <bool(hex_color)> compare1,
		std::function <bool(unit_type)> compare2) const
	{
		return MyAccess->get_game_map().get_neighbours_exist(hex,
			compare1, compare2);
	}

	std::vector<Pair> Controller::get_neighbours(const Pair & hex,
		std::function <bool(hex_color)> compare1,
		std::function <bool(unit_type)> compare2) const
	{
		return MyAccess->get_game_map().get_neighbours(hex, compare1, compare2);
	}

	std::vector<Pair> Controller::get_hex_row(const Pair & hex, size_type radius,
		std::function <bool(hex_color)> compare1,
		std::function <bool(unit_type)> compare2) const
	{
		return MyAccess->get_game_map().get_hex_row(hex, radius,
			compare1, compare2);
	}

	bool Controller::get_hex_row_exist(const Pair & hex, size_type radius,
		std::function <bool(hex_color)> compare1,
		std::function <bool(unit_type)> compare2) const
	{
		return MyAccess->get_game_map().get_hex_row_exist(hex, radius,
			compare1, compare2);
	}

	size_type Controller::distance(const Pair & hex1, const Pair & hex2) const
	{
		return get_distance(hex1, hex2);
	}
	
	bool Controller::can_move(const Pair & hex1, const Pair & hex2, size_type move_points) const
	{
		if (move_points == 0)
		{
			return false;
		}
		if (move_points == Army::move_points())
		{
			if (is_black(color(hex1))
				|| is_black(color(hex2))
				|| !is_army(get_unit_type(hex1))
				|| get_hex_strength(hex2) >= get_unit_strength(hex1)
				|| distance(hex1, hex2) > 6)
			{
				return false;
			}
		}
		if (distance(hex1, hex2) == 1 && move_points >= 1)
		{
			return true;
		}
		hex_color basic_color = color(hex1);
		for (auto & i : MyAccess->get_game_map().get_neighbours(hex2,
			[basic_color](hex_color color) { return color == basic_color; }))
		{
			if (can_move(hex1, i, move_points - 1))
			{
				return true;
			}	
		}
		return false;
	}
	
	unit_type Controller::get_unit_type(const Pair & hex) const
	{
		return (*MyAccess)(hex)->get_hex_unit_type();
	}

	size_type Controller::get_unit_strength(const Pair & hex) const
	{
		if ((*MyAccess)(hex)->occupied())
		{
			return get_unit(hex)->strength();
		}
		return 0;
	}

	size_type Controller::get_hex_strength(const Pair & hex) const
	{	
		if (is_player_color(color(hex)))
		{
			size_type result(get_unit_strength(hex));
			hex_color basic_color = color(hex);
			for (auto & i : MyAccess->get_game_map().get_neighbours(hex,
				[basic_color](hex_color color) { return color == basic_color; },
				is_player_unit))
			{
				if (get_unit_strength(i) > result)
				{
					result = get_unit_strength(i);
				}
			}
			return result;
		}
		return 0;
		
	}

	bool Controller::get_moved(const Pair & hex) const
	{
		if (get_unit_type(hex) == game_module::unit_type::army)
		{
			return static_cast<Army *>(get_unit(hex))->moved();
		}
		return true;
	}

	size_type Controller::get_district_money(const Pair & hex) const
	{
		if (get_capital(hex))
		{
			return get_capital(hex)->district_money();
		}
		return 0;
	}

	std::string Controller::get_map_type() const
	{
		return MyAccess->get_game_map().map_type();
	} 

	size_type Controller::get_map_dimension() const
	{
		return MyAccess->get_game_map().dimension();
	}
	
	size_type Controller::get_players_number() const
	{
		size_type result(0);
		for (auto & i : MyAccess->get_players())
		{
			if (MyAccess->player_in_game(i->color()))
			{
				++result;
			}
		}
		return result;
	}

	size_type Controller::get_current_turn() const
	{
		return MyAccess->current_turn();
	}

	size_type Controller::get_max_turns() const
	{
		return MyAccess->max_turns();
	}

	bool Controller::can_place_static(const Pair & hex) const
	{
		if (MyAccess->get_current_player() == color(hex)
			&& !(*MyAccess)(hex)->occupied())
		{
			return true;
		}
		return false;
	}

	bool Controller::can_place_army(const Pair & hex, size_type strength) const
	{
		if (strength < 1 || strength > 4 
			|| MyAccess->get_current_player() != color(hex)
			|| !(is_ready_to_take(get_unit_type(hex))
				|| (is_army(get_unit_type(hex)) && get_unit_strength(hex) + strength < 5)))
		{
			return false;
		}
		return true;
	}

	std::vector<Pair> Controller::get_hex_to_capture(const Pair & hex) const
	{
		return MyAccess->get_game_map().get_district_border_hexs(hex);
	}

	std::vector<Pair> Controller::get_district_units(const Pair & hex, unit_type seek_type) const
	{
		return MyAccess->get_game_map().easy_solve_maze(hex,
			[seek_type](unit_type type) { return type == seek_type; } );
	}

	std::vector<Pair> Controller::get_br_unit(const Pair & hex, unit_type type) const
	{
		std::vector<Pair> result;
		for (auto & i : MyAccess->get_game_map().get_district_border_hexs(hex))
		{
			if (get_unit_type(i) == type)
			{
				result.push_back(i);
			}
		}
		return result;
	}

	std::vector<Pair> Controller::get_enemy_br_units(const Pair & hex) const
	{
		std::vector<Pair> result;
		for (auto & i : MyAccess->get_game_map().get_district_border_hexs(hex))
		{
			if (is_player_unit(get_unit_type(i)))
			{
				result.push_back(i);
			}
		}
		return result;
	}

	std::vector<Pair> Controller::get_district_static(const Pair & hex) const
	{
		return MyAccess->get_game_map().easy_solve_maze(hex, is_static);
	}

	std::vector<Pair> Controller::get_br_static(const Pair & hex) const
	{
		std::vector<Pair> result;
		for (auto & i : MyAccess->get_game_map().get_district_border_hexs(hex))
		{
			if (is_static(get_unit_type(i)))
			{
				result.push_back(i);
			}
		}
		return result;
	}

	bool Controller::make_move(const Pair & start, const Pair & end)
	{
		if (color(start) != MyAccess->get_current_player()
			|| !can_move(start, end))
		{
			return false;
		}
		else
		{
			if (color(start) == color(end))
			{
				if (is_static(get_unit_type(end)))
				{
					get_capital(start)->change_district_income(2);
				}
				(*MyAccess)(end)->set_hex_unit(get_unit(start));
				(*MyAccess)(start)->remove_hex_unit();
			}
			else
			{
				get_capital(start)->change_district_income(1);
				if (get_capital(end))
				{
					get_capital(end)->change_district_income((*MyAccess)(end));
				}
				if (is_farm(get_unit_type(end)))
				{
					get_capital(end)->change_farms_number(-1);
				}
			}
			(*MyAccess)(end)->set_hex_capital((*MyAccess)(start));
			if (color(end) != color(start))
			{
				bool need_correct = false;
				hex_color basic_color = color(end);
				(*MyAccess)(end)->set_color(color(start));
				if (get_unit_type(end) == game_module::unit_type::capital)
				{
					MyAccess->get_player(basic_color)->remove_capital(end);
					need_correct = true;
				}
				(*MyAccess)(end)->set_hex_unit((*MyAccess)(start)->get_hex_unit());
				static_cast<Army *>((*MyAccess)(end)->get_hex_unit())->set_moved(true);
				(*MyAccess)(start)->remove_hex_unit();
				hex_color new_color = color(start);
				std::vector<std::set<Pair>> to_correct = hexs_to_correct(end, new_color);
				if (!to_correct[1].empty())
				{
					correct_capital(end);
				}
				if (basic_color != blank)
				{
					to_correct = hexs_to_correct(end, basic_color);	
					if (need_correct || !to_correct[1].empty() || to_correct[0].size() == 1)
					{
						for (size_type i = 0; i < 3; ++i)
						{
							if (!to_correct[i].empty())
							{
								correct_capital(*to_correct[i].begin());
							}
						}
					}
				}
 			}
			return true;
		}
	}

	bool Controller::buy_tower(const Pair & hex, size_type strength)
	{
		if (!can_place_static(hex)
			|| strength < 1
			|| strength > 2
			|| get_district_money(hex) - get_tower_cost(strength) < 0)
		{
			return false;
		}
		else
		{
			(*MyAccess)(hex)->set_hex_unit(unit_factory(tower, strength));
			(*MyAccess)(hex)->get_hex_capital()->change_district_money(-get_tower_cost(strength));
			get_capital(hex)->change_district_income(-get_tower_cost(strength));
			return true;
		}
	}

	bool Controller::buy_farm(const Pair & hex)
	{
		hex_color basic_color = color(hex);
		if (!can_place_static(hex)
			|| get_district_money(hex) - get_farm_cost(hex) < 0
			|| !MyAccess->get_game_map().get_neighbours_exist(hex,
				[basic_color](hex_color color) { return color == basic_color; },
				[](unit_type type) { return (type == game_module::unit_type::farm
					|| type == game_module::unit_type::capital); }))
		{
			return false;
		}
		else
		{
			(*MyAccess)(hex)->set_hex_unit(unit_factory(farm));
			get_capital(hex)->change_farms_number(1);
			(*MyAccess)(hex)->get_hex_capital()->change_district_money(-get_farm_cost(hex));
			get_capital(hex)->change_district_income(Farm::income());
			return true;
		}
	}

	bool Controller::buy_army(const Pair & hex, size_type strength)
	{
		if (!can_place_army(hex, strength)
			|| get_district_money(hex) - get_army_cost(strength) < 0)
		{
			return false;
		}
		else
		{
			if (get_unit_type(hex) == army)
			{
				static_cast<Army *>((*MyAccess)(hex)->get_hex_unit())
					->set_strength(strength + get_unit_strength(hex));
				get_capital(hex)->change_district_income(
					2 * pow(3, get_unit_strength(hex) - 1));
				get_capital(hex)->change_district_income(
					- 2 * pow(3, strength + get_unit_strength(hex) - 1));
			}
			else
			{
				bool change_moved = false;
				if (is_static(get_unit_type(hex)))
				{
					get_capital(hex)->change_district_income(2);
					change_moved = true;
				}
				(*MyAccess)(hex)->set_hex_unit(unit_factory(army, strength));
				if (change_moved)
				{
					static_cast<Army *>((*MyAccess)(hex)->get_hex_unit())->set_moved(true);
				}
				get_capital(hex)->change_district_income(- 2 * pow(3, strength - 1));
			}		
			(*MyAccess)(hex)->get_hex_capital()->change_district_money(-get_army_cost(strength));
			return true;	
		}
	}

	size_type Controller::get_district_income(const Pair & hex) const
	{
		return get_capital(hex)->district_income();
	}

	size_type Controller::get_farm_cost(const Pair & hex) const
	{
		return 12 + 2 * get_capital(hex)->farms_number();
	}

	size_type Controller::get_army_cost(size_type strength) const
	{
		if (strength > 0 && strength < 5)
		{
			return 10 * strength;
		}
		return 0;
	}

	size_type Controller::get_tower_cost(size_type strength) const
	{
		if (strength == 1)
		{
			return 15;
		}
		if (strength == 2)
		{
			return 35;
		}
		return 0;
	}

	std::vector<Pair> Controller::get_army_list(const Pair & hex) const
	{
		std::vector<Pair> result;
		for (auto & i : get_district_units(hex, game_module::unit_type::army))
		{
			if (!static_cast<Army *>(get_unit(i))->moved())
			{
				result.push_back(i);
			}
		}
		return result;
	}

	Unit * Controller::get_unit(const Pair & hex) const
	{
		if ((*MyAccess)(hex)->occupied())
		{
			return (*MyAccess)(hex)->get_hex_unit();
		}
		return nullptr;
	}

	Capital * Controller::get_capital(const Pair & hex) const
	{
		return (*MyAccess)(hex)->get_hex_capital();
	}

	void Controller::correct_capital(const Pair & hex)
	{		
		std::vector<Pair> district(MyAccess->get_game_map().solve_maze(hex));			
		if (district.size() == 1)
		{
			if (is_player_unit(get_unit_type(hex)))
			{
				if (get_unit_type(hex) == game_module::unit_type::capital)
				{
					MyAccess->get_player(color(hex))->remove_capital(hex);
				}
				(*MyAccess)(hex)->remove_hex_unit();
			}
			(*MyAccess)(hex)->set_hex_capital(nullptr);
			return;
		}
		std::vector<Pair> district_capitals;
		for (auto & i : district)
		{
			if (get_unit_type(i) == game_module::unit_type::capital)
			{
				district_capitals.push_back(i);
			}
		}
		Pair new_capital(hex);
		if (district_capitals.size() == 0)
		{		
			new_capital = hex;	
			if (is_player_unit(get_unit_type(new_capital)))
			{
				for (auto & i : district)
				{
					if (is_ready_to_take(get_unit_type(i)))
					{
						new_capital = i;
						break;
					}
					if (is_player_unit(get_unit_type(new_capital))
						&& get_unit(new_capital)->cost()
						> get_unit(i)->cost())
					{
						new_capital = i;
					}
				}
			}			
			(*MyAccess)(new_capital)->set_hex_unit(unit_factory(game_module::unit_type::capital, 1));
			MyAccess->get_player(color(hex))->add_capital(new_capital);
		}
		else if (district_capitals.size() > 1)
		{		
			new_capital = district_capitals[0];
			for (auto & i : district_capitals)
			{
				if (i == new_capital)
				{
					continue;
				}
				MyAccess->get_player(color(hex))->remove_capital(i);
				static_cast<Capital *>((*MyAccess)(new_capital)->get_hex_unit())
					->change_district_money(get_district_money(i));
				(*MyAccess)(i)->delete_hex_unit();
			}
		}	
		for (auto & i : district)
		{
			(*MyAccess)(i)->set_hex_capital((*MyAccess)(new_capital));
		}
		if (district.size() > 1)
		{
			calculate_income(district);
		}	
	}

	std::vector<std::set<Pair>> Controller::hexs_to_correct(const Pair & hex,
		hex_color color) const
	{
		std::vector<std::set<Pair>> result;
		result.resize(3);
		std::vector<Pair> neighbours_vec = MyAccess->get_game_map().get_neighbours(hex, 
			[color](hex_color check_color) { return check_color == color; });
		std::set<Pair> neighbours;
		for (auto & i : neighbours_vec)
		{
			neighbours.insert(i);
		}
		if (neighbours.size() > 0)
		{
			for (size_type i = 0; i < 3; ++i)
			{
				for (auto j = neighbours.begin(); j != neighbours.end();)
				{
					bool stop = false;
					if (result[i].empty())
					{
						result[i].insert(*j);
						neighbours.erase(j++);
						--i;
						break;
					}
					else
					{
						for (auto & k : result[i])
						{
							if (get_distance(*j, k) == 1
								&& (result[i].find(*j) == result[i].end()))
							{
								result[i].insert(*j);
								neighbours.erase(j++);
								stop = true;
								--i;
								break;
							}
							if (stop)
							{
								break;
							}
						}
					}
					if (!stop)
					{
						++j;
					}
					else
					{
						break;
					}
				}
			}
		}
		return result;
	}

	void Controller::calculate_income(const std::vector<Pair> & district)
	{
		size_type result = 0;
		size_type farms_number = 0;
		for (auto & i : district)
		{
			++result;
			if (is_static(get_unit_type(i)))
			{
				result -= 2;
			}
			else if (is_player_unit(get_unit_type(i)))
			{
				if (is_farm(get_unit_type(i)))
				{
					result += Farm::income();
					++farms_number;
				}
				else if (is_army(get_unit_type(i)) || is_tower(get_unit_type(i)))
				{
					result -= get_unit(i)->cost();
				}
			}
		}
		/*
		size_type result = 0;
		if (!district.empty())
		{
			Pair hex = district[0];
			result += MyAccess->get_game_map().easy_solve_maze_count(hex);
			result += 4 * MyAccess->get_game_map().easy_solve_maze_count(hex, is_farm);
			result -= 2 * MyAccess->get_game_map().easy_solve_maze_count(hex, is_static);
			for (auto & i : get_district_units(hex, army))
			{
				result -= get_unit(i)->cost();
			}
			for (auto & i : get_district_units(hex, tower))
			{
				result -= get_unit(i)->cost();
			}
		}
		*/
		get_capital(district[0])->change_district_income(
			-get_capital(district[0])->district_income());
		get_capital(district[0])->change_district_income(result);
		get_capital(district[0])->change_farms_number(
			-get_capital(district[0])->farms_number());
		get_capital(district[0])->change_farms_number(farms_number);
	}
}
