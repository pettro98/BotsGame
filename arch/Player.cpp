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

	std::vector<Pair> Player::get_neighbours(const Pair & hex) const
	{
		return GameController->get_neighbours(hex);
	}

	size_type Player::distance(const Pair & hex1, const Pair & hex2) const
	{
		return GameController->distance(hex1, hex2);
	}

	bool Player::can_move(const Pair & hex1, const Pair & hex2, size_type move_points) const
	{
		return GameController->can_move(hex1, hex2, move_points);
	}

	unit_type Player::get_unit_type(const Pair & hex) const
	{
		return GameController->get_unit_type(hex);
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

	std::string Player::get_map_type() const
	{
		return GameController->get_map_type();
	}

	size_type Player::get_map_dimension() const
	{
		return GameController->get_map_dimension();
	}

	size_type Player::get_players_number() const
	{
		return GameController->get_players_number();
	}

	size_type Player::get_current_turn() const
	{
		return GameController->get_current_turn();
	}
	size_type Player::get_max_turns() const
	{
		return GameController->get_max_turns();
	}

	bool Player::can_place_static(const Pair & hex) const
	{
		return GameController->can_place_static(hex);
	}

	bool Player::can_place_army(const Pair & hex, size_type strength) const
	{
		return GameController->can_place_army(hex, strength);
	}

	std::vector<Pair> Player::get_hex_to_capture(const Pair & hex) const
	{
		return GameController->get_hex_to_capture(hex);
	}

	std::vector<Pair> Player::get_district_units(const Pair & hex, unit_type type) const
	{
		return GameController->get_district_units(hex, type);
	}

	std::vector<Pair> Player::get_br_unit(const Pair & hex, unit_type type) const
	{
		return GameController->get_br_unit(hex, type);
	}

	std::vector<Pair> Player::get_enemy_br_units(const Pair & hex) const
	{
		return GameController->get_enemy_br_units(hex);
	}

	std::vector<Pair> Player::get_district_static(const Pair & hex) const
	{
		return GameController->get_district_static(hex);
	}

	std::vector<Pair> Player::get_br_static(const Pair & hex) const
	{
		return GameController->get_br_static(hex);
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

	size_type Player::get_district_income(const Pair & hex) const
	{
		return GameController->get_district_income(hex);
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

	std::vector<Pair> Player::get_army_list(const Pair & hex) const
	{
		return GameController->get_army_list(hex);
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
