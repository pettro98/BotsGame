#include "General.h"


namespace game_module
{
	Pair::Pair(const Pair & pair)
		: First(pair.First)
		, Second(pair.Second)
	{ }

	Pair::Pair(size_type first, size_type second)
		: First(first)
		, Second(second)
	{ }

	bool Pair::operator == (const Pair & pair) const
	{
		return (First == pair.First && Second == pair.Second);
	}

	bool Pair::operator < (const Pair & pair) const
	{
		if (First < pair.First)
		{
			return true;
		}
		else if (First == pair.First)
		{
			return Second < pair.Second;
		}
		return false;
	}

	bool operator != (const Pair & pair1, const Pair & pair2)
	{
		return !(pair1 == pair2);
	}

	std::ostream & operator << (std::ostream & out, const Pair & pair)
	{
		out << "(" << pair.First << ", " << pair.Second << ")";
		return out;
	}

	bool is_none(unit_type type)
	{
		return type == game_module::unit_type::none;
	}

	bool is_army(unit_type type)
	{
		return type == game_module::unit_type::army;
	}

	bool is_tower(unit_type type)
	{
		return type == game_module::unit_type::tower;
	}

	bool is_capital(unit_type type)
	{
		return type == game_module::unit_type::capital;
	}

	bool is_farm(unit_type type)
	{
		return type == game_module::unit_type::farm;
	}

	bool is_palm(unit_type type)
	{
		return type == game_module::unit_type::palm;
	}

	bool is_pine(unit_type type)
	{
		return type == game_module::unit_type::pine;
	}

	bool is_grave(unit_type type)
	{
		return type == game_module::unit_type::grave;
	}

	bool is_tree(unit_type type)
	{
		return (type == game_module::unit_type::palm
			|| type == game_module::unit_type::pine);
	}

	bool is_static(unit_type type)
	{
		return (is_tree(type) || type == game_module::unit_type::grave);
	}

	bool is_player_unit(unit_type type)
	{
		return (type == game_module::unit_type::army
			|| type == game_module::unit_type::capital
			|| type == game_module::unit_type::tower
			|| type == game_module::unit_type::farm);
	}

	bool is_ready_to_take(unit_type type)
	{
		return (is_static(type) || type == game_module::unit_type::none);
	}

	bool is_color(hex_color color)
	{
		return true;
	}

	bool is_black(hex_color color)
	{
		return color == game_module::hex_color::black;
	}

	bool is_blank(hex_color color)
	{
		return color == game_module::hex_color::blank;
	}

	bool is_red(hex_color color)
	{
		return color == game_module::hex_color::red;
	}

	bool is_green(hex_color color)
	{
		return color == game_module::hex_color::green;
	}

	bool is_orange(hex_color color)
	{
		return color == game_module::hex_color::orange;
	}

	bool is_purple(hex_color color)
	{
		return color == game_module::hex_color::purple;
	}

	bool is_blue(hex_color color)
	{
		return color == game_module::hex_color::blue;
	}

	bool is_yellow(hex_color color)
	{
		return color == game_module::hex_color::yellow;
	}

	bool is_extra(hex_color color)
	{
		return color == game_module::hex_color::extra;
	}

	bool is_player_color(hex_color color)
	{
		return (color != game_module::hex_color::black
			&& color != game_module::hex_color::blank
			&& color != game_module::hex_color::extra);
	}

	Result::Result()
		: winner(-1)
	{
		points.resize(6);
		last_turn.resize(6);
		built_armies.resize(6);
		built_farms.resize(6);
		built_towers.resize(6);
		moves.resize(6);
	}
}
