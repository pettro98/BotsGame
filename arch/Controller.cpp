#include "Controller.h"
#include <map>


namespace game_module
{

	Controller::Controller(IFullAccess * my_access)
		: MyAccess(my_access)
	{ }

	hex_color Controller::color(const Pair & hex) const
	{
		if ((*MyAccess)(hex) != nullptr)
		{
			return (*MyAccess)(hex)->color();
		}
		return game_module::hex_color::black;
	}

	Pair Controller::capital(const Pair & hex) const
	{
		if ((*MyAccess)(hex)->get_capital() != nullptr)
		{
			return (*MyAccess)(hex)->get_capital()->coordinates();
		}
		return Pair(0, 0);
	}

	size_type Controller::distance(const Pair & hex1, const Pair & hex2) const
	{
		return get_distance(hex1, hex2);
	}
	
	unit_type Controller::get_type(const Pair & hex) const
	{
		if ((*MyAccess)(hex) != nullptr)
		{
			return (*MyAccess)(hex)->get_unit_type();
		}
		return game_module::unit_type::none;
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
			for (auto & i : get_neighbours(hex,
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
		if (is_army(get_type(hex)))
		{
			return static_cast<Army *>(get_unit(hex))->moved();
		}
		return true;
	}

	size_type Controller::get_district_money(const Pair & hex) const
	{
		if (get_capital(hex) != nullptr)
		{
			return get_capital(hex)->district_money();
		}
		return 0;
	}

	size_type Controller::get_district_income(const Pair & hex) const
	{
		if (get_capital(hex) != nullptr)
		{
			return get_capital(hex)->district_income();
		}
		return 0;
	}

	size_type Controller::get_farms_number(const Pair & hex) const
	{
		if (get_capital(hex) != nullptr)
		{
			return get_capital(hex)->farms_number();
		}
		return 0;
	}

	std::string Controller::get_map_type() const
	{
		return MyAccess->get_game_map().map_type();
	} 

	size_type Controller::get_map_dimension_x() const
	{
		return MyAccess->get_game_map().dimension_x();
	}

	size_type Controller::get_map_dimension_y() const
	{
		return MyAccess->get_game_map().dimension_y();
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

	std::vector<hex_color> Controller::get_players_colors() const
	{
		std::vector<hex_color> result;
		for (auto & i : MyAccess->get_players())
		{
			if (MyAccess->player_in_game(i->color()))
			{
				result.push_back(i->color());
			}
		}
		return result;
	}

	std::list<Pair> Controller::get_player_capitals(hex_color color) const
	{
		return MyAccess->get_player(color)->get_capitals();
	}

	size_type Controller::get_current_turn() const
	{
		return MyAccess->current_turn();
	}

	size_type Controller::get_max_turns() const
	{
		return MyAccess->max_turns();
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
				|| !is_army(get_type(hex1))
				|| distance(hex1, hex2) > 6
				|| ((color(hex1) == color(hex2) && !is_ready_to_take(get_type(hex2)))
					|| (color(hex1) != color(hex2) && get_unit_strength(hex1) != 4
						&& get_hex_strength(hex2) >= get_unit_strength(hex1))))
			{
				return false;
			}
		}
		if (distance(hex1, hex2) == 1 && move_points >= 1)
		{
			return true;
		}
		hex_color basic_color = color(hex1);
		for (auto & i : get_neighbours(hex2,
			[basic_color](hex_color color) { return color == basic_color; }))
		{
			if (can_move(hex1, i, move_points - 1))
			{
				return true;
			}
		}
		return false;
	}

	bool Controller::can_place_tower(const Pair & hex) const
	{
		if (MyAccess->get_current_player() == color(hex)
			&& !(*MyAccess)(hex)->occupied())
		{
			return true;
		}
		return false;
	}

	bool Controller::can_place_farm(const Pair & hex) const
	{
		hex_color basic_color = MyAccess->get_current_player();
		if (MyAccess->get_current_player() == color(hex)
			&& !(*MyAccess)(hex)->occupied()
			&& MyAccess->get_game_map().get_neighbours_exist(hex,
				[basic_color](hex_color color) { return color == basic_color; },
				[](unit_type type) { return (type == game_module::unit_type::farm
					|| type == game_module::unit_type::capital); }))
		{
			return true;
		}
		return false;
	}

	bool Controller::can_place_army(const Pair & hex, size_type strength) const
	{
		if (strength > 0 && strength < 5 && !is_black(color(hex)))
		{
			if (color(hex) == MyAccess->get_current_player())
			{
				if (is_ready_to_take(get_type(hex)) 
					|| (is_army(get_type(hex)) && get_unit_strength(hex) + strength < 5))
				{
					return true;
				}
			}
			else
			{
				hex_color basic_color = MyAccess->get_current_player();
				if ((strength == 4 || strength > get_hex_strength(hex))
					&& get_neighbours_exist(hex,
						[basic_color](hex_color color) {return color == basic_color; }))
				{
					return true;
				}
			}
		}
		return false;
	}

	std::vector<Pair> Controller::hexs_to_place_farm(const Pair & hex) const
	{
		std::vector<Pair> result;
		hex_color basic_color = color(hex);
		for (auto & i : easy_solve_maze(hex, is_none))
		{
			if (get_neighbours_exist(i,
				[basic_color](hex_color color) { return color == basic_color;  },
				[](unit_type type) { return (type == game_module::unit_type::farm 
					|| type == game_module::unit_type::capital); }))
			{
				result.push_back(i);
			}
		}
		return result;
	}
	
	std::vector<Pair> Controller::hexs_to_move_army(const Pair & hex) const
	{		
		std::vector<Pair> result;
		std::map<Pair, size_t> buf;
		buf.insert(std::make_pair(hex, 0));
		for (auto & i : get_neighbours(hex))
		{
			if (color(i) != color(hex))
			{
				buf.insert(std::make_pair(i, 6));
			}
			else
			{
				buf.insert(std::make_pair(i, 1));
			}
		}
		bool find = true;
		while (find)
		{
			find = false;
			for (auto & i : buf)
			{
				if (i.second >= 6 || i.second == 0)
				{
					continue;
				}
				for (auto & j : get_neighbours(i.first))
				{
					auto iter = buf.find(j);
					if (iter != buf.end())
					{
						if (color(iter->first) == color(hex)
							&& i.second + 1 < iter->second)
						{
							iter->second = i.second + 1;
						}
					}
					else
					{
						find = true;
						if (color(j) != color(hex))
						{
							buf.insert(std::make_pair(j, 6));
						}
						else
						{
							buf.insert(std::make_pair(j, i.second + 1));
						}
					}
				}
			}
		}
		buf.erase(hex);
		for (auto & i : buf)
		{
			if (i.second < 7)
			{
				if (color(i.first) == color(hex))
				{
					if (is_ready_to_take(get_type(i.first)))
					{
						result.push_back(i.first);
					}
				}
				else
				{
					if (get_unit_strength(hex) == 4 || get_unit_strength(hex) > get_hex_strength(i.first))
					{
						result.push_back(i.first);
					}
				}
			}
		}
		return result;
	}

	std::vector<Pair> Controller::get_district_units(const Pair & hex, unit_type seek_type) const
	{
		return easy_solve_maze(hex,
			[seek_type](unit_type type) { return type == seek_type; } );
	}

	std::vector<Pair> Controller::get_district_static(const Pair & hex) const
	{
		return easy_solve_maze(hex, is_static);
	}

	std::vector<Pair> Controller::get_army_list(const Pair & hex) const
	{
		std::vector<Pair> result;
		for (auto & i : easy_solve_maze(hex, is_army))
		{
			if (!static_cast<Army *>(get_unit(i))->moved())
			{
				result.push_back(i);
			}
		}
		return result;
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
		return MyAccess->get_game_map().get_hex_row(hex, radius, compare1, compare2);
	}

	bool Controller::get_hex_row_exist(const Pair & hex, size_type radius,
		std::function <bool(hex_color)> compare1,
		std::function <bool(unit_type)> compare2) const
	{
		return MyAccess->get_game_map().get_hex_row_exist(hex, radius, compare1, compare2);
	}

	std::vector<Pair> Controller::get_internal_border(const Pair & hex,
		std::function <bool(unit_type)> compare) const
	{
		return MyAccess->get_game_map().get_internal_border(hex, compare);
	}

	std::vector<Pair> Controller::get_external_border(const Pair & hex,
		std::function <bool(hex_color)> compare1,
		std::function <bool(unit_type)> compare2) const
	{
		return MyAccess->get_game_map().get_external_border(hex, compare1, compare2);
	}

	std::vector<Pair> Controller::easy_solve_maze(const Pair & hex,
		std::function <bool(unit_type)> compare) const
	{
		return MyAccess->get_game_map().easy_solve_maze(hex, compare);
	}

	size_type Controller::easy_solve_maze_count(const Pair & hex,
		std::function <bool(unit_type)> compare) const
	{
		return MyAccess->get_game_map().easy_solve_maze_count(hex, compare);
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
		if (strength == 2)
		{
			return 15;
		}
		else if (strength == 3)
		{
			return 35;
		}
		return 0;
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
				if (is_static(get_type(end)))
				{
					get_capital(start)->change_district_income(-Tree::income());
				}
			}
			else
			{
				get_capital(start)->change_district_income(1);
				if (get_capital(end))
				{
					get_capital(end)->change_district_income((*MyAccess)(end));
				}
				if (is_farm(get_type(end)))
				{
					get_capital(end)->change_farms_number(-1);
				}
			}
			(*MyAccess)(end)->set_capital((*MyAccess)(start));
			if (color(end) != color(start))
			{
				bool need_correct = false;
				hex_color basic_color = color(end);
				(*MyAccess)(end)->set_color(color(start));
				if (get_type(end) == game_module::unit_type::capital)
				{
					MyAccess->get_player(basic_color)->remove_capital(end);
					need_correct = true;
				}
				(*MyAccess)(end)->set_unit((*MyAccess)(start)->get_unit());
				static_cast<Army *>((*MyAccess)(end)->get_unit())->set_moved(true);
				(*MyAccess)(start)->remove_unit();
				hex_color new_color = color(start);
				std::vector<std::set<Pair>> to_correct = hexs_to_correct(end, new_color);
				if (!to_correct[1].empty())
				{
					correct_capital(end);
				}
				if (!is_blank(basic_color))
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
			MyAccess->add_to_moves(MyAccess->get_current_player());
			return true;
		}
	}

	bool Controller::buy_tower(const Pair & hex, size_type strength)
	{
		if (!can_place_tower(hex)
			|| strength < 2
			|| strength > 3
			|| get_district_money(hex) - get_tower_cost(strength) < 0)
		{
			return false;
		}
		else
		{
			(*MyAccess)(hex)->set_unit(unit_factory(tower, strength));
			get_capital(hex)->change_district_money(-get_tower_cost(strength));
			get_capital(hex)->change_district_income(Tower::income(strength));
			MyAccess->add_to_built_towers(MyAccess->get_current_player(), strength);
			return true;
		}
	}

	bool Controller::buy_farm(const Pair & hex)
	{
		if (!can_place_farm(hex)
			|| get_district_money(hex) - get_farm_cost(hex) < 0)
		{
			return false;
		}
		else
		{
			(*MyAccess)(hex)->set_unit(unit_factory(game_module::unit_type::farm));
			get_capital(hex)->change_farms_number(1);
			(*MyAccess)(hex)->get_capital()->change_district_money(-get_farm_cost(hex));
			get_capital(hex)->change_district_income(Farm::income());
			MyAccess->add_to_built_farms(MyAccess->get_current_player());
			return true;
		}
	}

	bool Controller::buy_army(const Pair & hex, size_type strength)
	{
		if (!can_place_army(hex, strength))
		{
			return false;
		}
		else if (color(hex) == MyAccess->get_current_player())
		{
			if (get_district_money(hex) - get_army_cost(strength) < 0)
			{
				return false;
			}
			if (is_army(get_type(hex)))
			{
				static_cast<Army *>((*MyAccess)(hex)->get_unit())
					->set_strength(strength + get_unit_strength(hex));
				get_capital(hex)->change_district_income(-Army::income(get_unit_strength(hex)));
				get_capital(hex)->change_district_income(Army::income(strength + get_unit_strength(hex)));
			}
			else
			{
				bool change_moved = false;
				if (is_static(get_type(hex)))
				{
					get_capital(hex)->change_district_income(-Tree::income());
					change_moved = true;
				}
				(*MyAccess)(hex)->set_unit(unit_factory(game_module::unit_type::army, strength));
				if (change_moved)
				{
					static_cast<Army *>((*MyAccess)(hex)->get_unit())->set_moved(true);
				}
				get_capital(hex)->change_district_income(Army::income(strength));
			}
			(*MyAccess)(hex)->get_capital()->change_district_money(-get_army_cost(strength));
			MyAccess->add_to_built_armies(MyAccess->get_current_player(), strength);
		}
		else
		{
			Pair basic_district = hex;
			{
				hex_color basic_color = MyAccess->get_current_player();
				for (auto & i : get_neighbours(hex,
					[basic_color](hex_color color) {return color == basic_color; }))
				{
					if (get_district_money(i) - get_army_cost(strength) > 0)
					{
						basic_district = i;
						break;
					}
				}
				if (basic_district == hex)
				{
					return false;
				}
			}
			if (get_capital(hex) != nullptr)
			{
				get_capital(hex)->change_district_income((*MyAccess)(hex));
			}
			(*MyAccess)(hex)->set_capital((*MyAccess)(basic_district));
			bool need_correct = false;
			hex_color basic_color = color(hex);
			(*MyAccess)(hex)->set_color(MyAccess->get_current_player());
			if (is_capital(get_type(hex)))
			{
				MyAccess->get_player(basic_color)->remove_capital(hex);
				need_correct = true;
			}
			(*MyAccess)(hex)->set_unit(unit_factory(game_module::unit_type::army, strength));
			static_cast<Army *>((*MyAccess)(hex)->get_unit())->set_moved(true);
			std::vector<std::set<Pair>> to_correct = hexs_to_correct(hex, MyAccess->get_current_player());
			if (!to_correct[1].empty())
			{
				correct_capital(hex);
			}
			if (!is_blank(basic_color))
			{
				to_correct = hexs_to_correct(hex, basic_color);
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
			get_capital(hex)->change_district_income(Army::income(strength) + 1);
			get_capital(hex)->change_district_money(-get_army_cost(strength));
			MyAccess->add_to_built_armies(MyAccess->get_current_player(), strength);
			MyAccess->add_to_moves(MyAccess->get_current_player());
		}
		return true;
	}

	Unit * Controller::get_unit(const Pair & hex) const
	{
		if ((*MyAccess)(hex) != nullptr && (*MyAccess)(hex)->occupied())
		{
			return (*MyAccess)(hex)->get_unit();
		}
		return nullptr;
	}

	Capital * Controller::get_capital(const Pair & hex) const
	{
		if ((*MyAccess)(hex) != nullptr)
		{
			return (*MyAccess)(hex)->get_capital();
		}
		return nullptr;
	}

	void Controller::correct_capital(const Pair & hex)
	{		
		std::vector<Pair> district(MyAccess->get_game_map().solve_maze(hex));			
		if (district.size() == 1)
		{
			if (is_player_unit(get_type(hex)))
			{
				if (get_type(hex) == game_module::unit_type::capital)
				{
					MyAccess->get_player(color(hex))->remove_capital(hex);
				}
				(*MyAccess)(hex)->remove_unit();
			}
			(*MyAccess)(hex)->set_capital(nullptr);
			return;
		}
		std::vector<Pair> district_capitals;
		for (auto & i : district)
		{
			if (get_type(i) == game_module::unit_type::capital)
			{
				district_capitals.push_back(i);
			}
		}
		Pair new_capital(hex);
		if (district_capitals.size() == 0)
		{		
			new_capital = hex;	
			if (is_player_unit(get_type(new_capital)))
			{
				for (auto & i : district)
				{
					if (is_ready_to_take(get_type(i)))
					{
						new_capital = i;
						break;
					}
					if (is_player_unit(get_type(new_capital))
						&& get_unit(new_capital)->cost()
						> get_unit(i)->cost())
					{
						new_capital = i;
					}
				}
			}			
			(*MyAccess)(new_capital)->set_unit(unit_factory(game_module::unit_type::capital));
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
				static_cast<Capital *>((*MyAccess)(new_capital)->get_unit())
					->change_district_money(get_district_money(i));
				(*MyAccess)(i)->delete_unit();
			}
		}	
		for (auto & i : district)
		{
			(*MyAccess)(i)->set_capital((*MyAccess)(new_capital));
		}
		if (district.size() > 1)
		{
			calculate_income(district);
		}	
	}

	std::vector<std::set<Pair>> Controller::hexs_to_correct(const Pair & hex, hex_color color) const
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
			if (is_static(get_type(i)))
			{
				result += Tree::income();
			}
			else if (is_player_unit(get_type(i)))
			{
				if (is_farm(get_type(i)))
				{
					result += Farm::income();
					++farms_number;
				}
				else if (is_army(get_type(i)) || is_tower(get_type(i)))
				{
					result -= get_unit(i)->cost();
				}
			}
		}
		get_capital(district[0])->change_district_income(
			-get_capital(district[0])->district_income());
		get_capital(district[0])->change_district_income(result);
		get_capital(district[0])->change_farms_number(
			-get_capital(district[0])->farms_number());
		get_capital(district[0])->change_farms_number(farms_number);
	}
}