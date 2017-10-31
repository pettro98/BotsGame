#include "Controller.h"


namespace game_module
{

	Controller::Controller(IFullAccess * my_access)
		: MyAccess(my_access)
	{ }


	size_type Controller::get_index(const Pair & hex) const
	{
		return (*MyAccess)(hex)->index();
	}

	size_type Controller::distance(const Pair & hex1, const Pair & hex2) const
	{
		return get_distance(hex1, hex2);
	}
	
	
	bool Controller::can_move(const Pair & hex1, const Pair & hex2, size_type move_points) const
	{
		if (move_points == Army::move_points())
		{
			if (!(*MyAccess)(hex1)->occupied()
				|| (*MyAccess)(hex1)->get_hex_unit()->type() != army
				|| (*MyAccess)(hex2)->index() == black
				|| (*MyAccess)(hex2)->get_hex_unit()->strength() >= (*MyAccess)(hex1)->get_hex_unit()->strength()
				|| get_distance(hex1, hex2) > 6)
				return false;
		}

		if (get_distance(hex1, hex2) == 1 && move_points >= 1)
			return true;

		for (auto & i : MyAccess->get_game_map().get_exist_neighbours(hex2))
		{
			if ((*MyAccess)(i)->index() == (*MyAccess)(hex1)->index())
			{
				if (can_move(hex1, i, --move_points))
					return true;
			}
			else if (!(*MyAccess)(i)->occupied()
				|| ((*MyAccess)(i)->occupied()
					&& (*MyAccess)(i)->get_hex_unit()->strength() < (*MyAccess)(hex1)->get_hex_unit()->strength()))
				if (can_move(hex1, i, 0))
					return true;
		}

		return false;

	}
	
	unit_type Controller::get_unit_type(Pair hex) const
	{
		if ((*MyAccess)(hex)->occupied())
			return (*MyAccess)(hex)->get_hex_unit()->type();
		return none;
	}

	size_type Controller::get_strength(Pair hex) const
	{
		if ((*MyAccess)(hex)->occupied())
			return (*MyAccess)(hex)->get_hex_unit()->strength();
		return 0;
	}

	bool Controller::get_moved(Pair hex) const
	{
		if ((*MyAccess)(hex)->occupied() && (*MyAccess)(hex)->get_hex_unit()->type() == army)
			return static_cast<Army *>((*MyAccess)(hex)->get_hex_unit())->moved();
		return true;
	}

	size_type Controller::get_district_money(Pair hex) const
	{
		if ((*MyAccess)(hex)->occupied() && (*MyAccess)(hex)->get_hex_unit()->type() == capital)
			return static_cast<Capital *>((*MyAccess)(hex)->get_hex_unit())->district_money();
		return 0;
	}

	size_type Controller::get_district_index(Pair hex) const
	{
		if ((*MyAccess)(hex)->occupied() && (*MyAccess)(hex)->get_hex_unit()->type() == capital)
			return static_cast<Capital *>((*MyAccess)(hex)->get_hex_unit())->district_index();
		return -1;
	}

	std::string Controller::get_map_type() const
	{
		return MyAccess->get_game_map().map_type();
	} 

	size_type Controller::get_map_dimension() const
	{
		return MyAccess->get_game_map().dimension();
	}

	//
	//size_type Controller::get_players_number() const
	//{
	//	return MyAccess->get_players().size();
	//	return 0;
	//}

	bool Controller::can_place_static(Pair hex) const
	{
		if (!(*MyAccess)(hex)->occupied())
			return true;
		if ((*MyAccess)(hex)->occupied()
			&& ((*MyAccess)(hex)->get_hex_unit()->type() == pine
				|| (*MyAccess)(hex)->get_hex_unit()->type() == palm
				|| (*MyAccess)(hex)->get_hex_unit()->type() == grave))
			return true;
		return false;
	}

	bool Controller::can_place_army(Pair hex, size_type str) const
	{
		if (str < 1 || str > 4)
			return false;

		if (!(*MyAccess)(hex)->occupied())
			return true;

		if ((*MyAccess)(hex)->occupied()
			&& ((*MyAccess)(hex)->get_hex_unit()->type() == pine
				|| (*MyAccess)(hex)->get_hex_unit()->type() == palm
				|| (*MyAccess)(hex)->get_hex_unit()->type() == grave))
			return true;

		if ((*MyAccess)(hex)->occupied()
			&& (*MyAccess)(hex)->get_hex_unit()->type() == army
			&& (*MyAccess)(hex)->get_hex_unit()->strength() + str < 5)
			return true;

		return false;

	}

	std::vector<Pair> Controller::get_neighbours(Pair hex) const
	{
		return MyAccess->get_game_map().get_exist_neighbours(hex);
	}

	
	std::vector<Pair> Controller::get_hex_to_capture(Pair hex) const
	{
		return MyAccess->get_game_map().get_district_border_hexs(hex);
	}

	std::vector<Pair> Controller::get_district_units(Pair hex, unit_type type) const
	{
		std::vector<Pair> result;

		for (auto & i : MyAccess->get_game_map().easy_solve_maze(hex))
			if ((*MyAccess)(hex)->occupied() && (*MyAccess)(hex)->get_hex_unit_type() == type)
				result.push_back(i);

		return result;

	}

	std::vector<Pair> Controller::get_br_unit(Pair hex, unit_type type) const
	{
		std::vector<Pair> result;

		for (auto & i : MyAccess->get_game_map().get_district_border_hexs(hex))
			if ((*MyAccess)(hex)->occupied() && (*MyAccess)(hex)->get_hex_unit_type() == type)
				result.push_back(i);

		return result;
	}

	std::vector<Pair> Controller::get_enemy_br_units(Pair hex) const
	{

		std::vector<Pair> result;

		for (auto & i : MyAccess->get_game_map().get_district_border_hexs(hex))
			if ((*MyAccess)(hex)->occupied() && (*MyAccess)(hex)->get_hex_unit_type() != palm
				&& (*MyAccess)(hex)->get_hex_unit_type() != pine
				&& (*MyAccess)(hex)->get_hex_unit_type() != grave)
				result.push_back(i);

		return result;

	}

	//std::vector<Pair> Controller::get_enemy_inv_armies(Pair hex) const;


	std::vector<Pair> Controller::get_district_static(Pair hex) const
	{
		std::vector<Pair> result;

		for (auto & i : MyAccess->get_game_map().easy_solve_maze(hex))
			if ((*MyAccess)(hex)->occupied() && ( (*MyAccess)(hex)->get_hex_unit_type() == palm
				|| (*MyAccess)(hex)->get_hex_unit_type() == pine
				|| (*MyAccess)(hex)->get_hex_unit_type() == grave))
				result.push_back(i);

		return result;
	}

	std::vector<Pair> Controller::get_br_static(Pair hex) const
	{
		std::vector<Pair> result;

		for (auto & i : MyAccess->get_game_map().get_district_border_hexs(hex))
			if ((*MyAccess)(hex)->occupied() && ((*MyAccess)(hex)->get_hex_unit_type() == palm
				|| (*MyAccess)(hex)->get_hex_unit_type() == pine
				|| (*MyAccess)(hex)->get_hex_unit_type() == grave))
				result.push_back(i);

		return result;
	}

	

	bool Controller::make_move(Pair start, Pair end)
	{
		if (can_move(start, end))
		{
			(*MyAccess)(end)->set_hex_unit((*MyAccess)(start)->get_hex_unit());
			(*MyAccess)(start)->remove_hex_unit();
			return true;
		}
		return false;
	}

	bool Controller::buy_tower(Pair hex, size_type strength)
	{
		if (strength < 1 || strength > 2)
			return false;
		if ((*MyAccess)(hex)->get_hex_capital()->district_money() - get_tower_cost(strength) >= 0)
		{
			(*MyAccess)(hex)->remove_hex_unit(); //!
			Unit * new_tower = new Tower((*MyAccess)(hex), strength);
			(*MyAccess)(hex)->get_hex_capital()->change_district_money(-get_tower_cost(strength));
			return true;
		}
		return false;
	}

	bool Controller::buy_farm(Pair hex)
	{
		if ((*MyAccess)(hex)->get_hex_capital()->district_money() - get_farm_cost(hex) >= 0)
		{
			(*MyAccess)(hex)->remove_hex_unit(); //!
			Unit * new_tower = new Farm((*MyAccess)(hex));
			(*MyAccess)(hex)->get_hex_capital()->change_district_money(-get_farm_cost(hex));
			return true;
		}
		return false;
	}

	bool Controller::buy_army(Pair hex, size_type strength)
	{
		if (strength < 1 || strength > 4)
			return false;
		if ((*MyAccess)(hex)->get_hex_capital()->district_money() - get_army_cost(strength) >= 0)
		{
			(*MyAccess)(hex)->remove_hex_unit(); //!
			Unit * new_tower = new Army((*MyAccess)(hex), strength);
			(*MyAccess)(hex)->get_hex_capital()->change_district_money(-get_army_cost(strength));
			return true;
		}
		return false;

	}

	size_type Controller::get_district_income(Pair hex) const
	{
		size_type result = 0;

		result += MyAccess->get_game_map().easy_solve_maze(hex).size();
		result += 4 * (get_district_units(hex, farm).size());
		result -= 2 * (get_district_static(hex).size());

		for (auto & i : get_district_units(hex, army))
		{
			result -= (static_cast<Army *>((*MyAccess)(hex)->get_hex_unit()))->cost();
		}

		for (auto & i : get_district_units(hex, tower))
		{
			result -= (static_cast<Tower *>((*MyAccess)(hex)->get_hex_unit()))->cost();
		}

		return result;

	}

	size_type Controller::get_farm_cost(Pair hex) const
	{
		return 12 + 2 * get_district_units(hex, farm).size();
	}

	size_type Controller::get_army_cost(size_type strength) const
	{
		if (strength > 0, strength < 5)
			return 10 * strength;
		return 0;
	}

	size_type Controller::get_tower_cost(size_type strength) const
	{
		if (strength == 1)
			return 15;
		if (strength == 2)
			return 35;
		return 0;
	}

	std::vector<Pair> Controller::get_army_list(Pair hex) const
	{

		std::vector<Pair> result;

		for (auto & i : get_district_units(hex, army))
		{
			if (!(static_cast<Army *>((*MyAccess)(i)->get_hex_unit()))->moved())
				result.push_back(i);
		}

		return result;

	}

	
}
