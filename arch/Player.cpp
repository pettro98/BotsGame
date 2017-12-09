#include "Player.h"



namespace game_module
{

	Player::Player(hex_color color, const std::string & player_name)
		: Color(color)
		, Name(player_name)
	{ }

	hex_color Player::color() const
	{
		return Color;
	}

	std::string Player::name() const
	{
		return Name;
	}

	size_type Player::get_capitals_number() const
	{
		return Capitals.size();
	}

	std::list<Pair> Player::get_capitals() const
	{
		return Capitals;
	}

	bool Player::operator == (const Player & player) const
	{
		return (Color == player.color());
	}

	hex_color Player::color(const Pair & hex) const
	{
		return GameController->color(hex);
	}

	Pair Player::capital(const Pair & hex) const
	{
		return GameController->capital(hex);
	}

	size_type Player::distance(const Pair & hex1, const Pair & hex2) const
	{
		return GameController->distance(hex1, hex2);
	}

	unit_type Player::get_type(const Pair & hex) const
	{
		return GameController->get_type(hex);
	}

	size_type Player::get_unit_strength(const Pair & hex) const
	{
		return GameController->get_unit_strength(hex);
	}

	size_type Player::get_hex_strength(const Pair & hex) const
	{
		return GameController->get_hex_strength(hex);
	}

	bool Player::get_moved(const Pair & hex) const
	{
		return GameController->get_moved(hex);
	}

	size_type Player::get_district_money(const Pair & hex) const
	{
		return GameController->get_district_money(hex);
	}

	size_type Player::get_district_income(const Pair & hex) const
	{
		return GameController->get_district_income(hex);
	}

	size_type Player::get_farms_number(const Pair & hex) const
	{
		return GameController->get_farms_number(hex);
	}

	std::string Player::get_map_type() const
	{
		return GameController->get_map_type();
	}

	size_type Player::get_map_dimension_x() const
	{
		return GameController->get_map_dimension_x();
	}

	size_type Player::get_map_dimension_y() const
	{
		return GameController->get_map_dimension_y();
	}

	size_type Player::get_players_number() const
	{
		return GameController->get_players_number();
	}

	std::vector<hex_color> Player::get_players_colors() const
	{
		return GameController->get_players_colors();
	}

	std::list<Pair> Player::get_player_capitals(hex_color color) const
	{
		return GameController->get_player_capitals(color);
	}

	size_type Player::get_current_turn() const
	{
		return GameController->get_current_turn();
	}
	size_type Player::get_max_turns() const
	{
		return GameController->get_max_turns();
	}

	bool Player::can_move(const Pair & hex1, const Pair & hex2) const
	{
		return GameController->can_move(hex1, hex2, Army::move_points());
	}

	bool Player::can_place_tower(const Pair & hex) const
	{
		return GameController->can_place_tower(hex);
	}

	bool Player::can_place_farm(const Pair & hex) const
	{
		return GameController->can_place_farm(hex);
	}

	bool Player::can_place_army(const Pair & hex, size_type strength) const
	{
		return GameController->can_place_army(hex, strength);
	}

	std::vector<Pair> Player::hexs_to_place_farm(const Pair & hex) const
	{
		return GameController->hexs_to_place_farm(hex);
	}

	std::vector<Pair> Player::hexs_to_move_army(const Pair & hex) const
	{
		return GameController->hexs_to_move_army(hex);
	}

	std::vector<Pair> Player::get_district_units(const Pair & hex, unit_type type) const
	{
		return GameController->get_district_units(hex, type);
	}

	std::vector<Pair> Player::get_district_static(const Pair & hex) const
	{
		return GameController->get_district_static(hex);
	}

	std::vector<Pair> Player::get_army_list(const Pair & hex) const
	{
		return GameController->get_army_list(hex);
	}

	bool Player::get_neighbours_exist(const Pair & hex,
		std::function <bool(hex_color)> compare1,
		std::function <bool(unit_type)> compare2) const
	{
		return GameController->get_neighbours_exist(hex, compare1, compare2);
	}

	std::vector<Pair> Player::get_neighbours(const Pair & hex,
		std::function <bool(hex_color)> compare1,
		std::function <bool(unit_type)> compare2) const
	{
		return GameController->get_neighbours(hex, compare1, compare2);
	}

	std::vector<Pair> Player::get_hex_row(const Pair & hex, size_type radius,
		std::function <bool(hex_color)> compare1,
		std::function <bool(unit_type)> compare2) const
	{
		return GameController->get_hex_row(hex, radius, compare1, compare2);
	}

	bool Player::get_hex_row_exist(const Pair & hex, size_type radius,
		std::function <bool(hex_color)> compare1,
		std::function <bool(unit_type)> compare2) const
	{
		return GameController->get_hex_row_exist(hex, radius, compare1, compare2);
	}

	std::vector<Pair> Player::get_internal_border(const Pair & hex,
		std::function <bool(unit_type)> compare) const
	{
		return GameController->get_internal_border(hex, compare);
	}

	std::vector<Pair> Player::get_external_border(const Pair & hex,
		std::function <bool(hex_color)> compare1,
		std::function <bool(unit_type)> compare2) const
	{
		return GameController->get_external_border(hex, compare1, compare2);
	}

	std::vector<Pair> Player::easy_solve_maze(const Pair & hex,
		std::function <bool(unit_type)> compare) const
	{
		return GameController->easy_solve_maze(hex, compare);
	}

	size_type Player::easy_solve_maze_count(const Pair & hex,
		std::function <bool(unit_type)> compare) const
	{
		return GameController->easy_solve_maze_count(hex, compare);
	}

	size_type Player::get_farm_cost(const Pair & hex) const
	{
		return GameController->get_farm_cost(hex);
	}

	size_type Player::get_army_cost(size_type strength) const
	{
		return GameController->get_army_cost(strength);
	}

	size_type Player::get_tower_cost(size_type strength) const
	{
		return GameController->get_tower_cost(strength);
	}

	bool Player::make_move(const Pair & start, const Pair & end)
	{
		return GameController->make_move(start, end);
	}

	bool Player::buy_tower(const Pair & hex, size_type strength)
	{
		return GameController->buy_tower(hex, strength);
	}

	bool Player::buy_farm(const Pair & hex)
	{
		return GameController->buy_farm(hex);
	}

	bool Player::buy_army(const Pair & hex, size_type strength)
	{
		return GameController->buy_army(hex, strength);
	}

	void Player::add_capital(const Pair & capital)
	{
		Capitals.push_back(capital);
	}

	bool Player::remove_capital(const Pair & capital)
	{
		for (auto i = Capitals.begin(); i != Capitals.end(); ++i)
		{
			if (*i == capital) {
				Capitals.erase(i);
				return true;
			}
		}
		return false;
	}

	void Player::set_controller(Controller * controller)
	{
		GameController = controller;
	}

	bool operator != (const Player & player1, const Player & player2)
	{
		return !(player1 == player2);
	}
}
