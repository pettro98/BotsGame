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
		return (First < pair.First) || (First == pair.First && Second < pair.Second);
	}

	bool operator != (const Pair & pair1, const Pair & pair2)
	{
		return !(pair1 == pair2);
	}

	bool is_type(unit_type type)
	{
		return true;
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

	std::string get_color_string(hex_color color)
	{
		switch (color)
		{
		case game_module::hex_color::black:
			return "black";
		case game_module::hex_color::blank:
			return "blank";
		case game_module::hex_color::red:
			return "red";
		case game_module::hex_color::green:
			return "green";
		case game_module::hex_color::cyan:
			return "cyan";
		case game_module::hex_color::purple:
			return "purple";
		case game_module::hex_color::blue:
			return "blue";
		case game_module::hex_color::yellow:
			return "yellow";
		case game_module::hex_color::extra:
			return "extra";
		default:
			return "error_color";
		}
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
		: winner(game_module::hex_color::black)
	{
		last_turn.resize(6);
		built_armies.resize(6);
		built_farms.resize(6);
		built_towers.resize(6);
		moves.resize(6);
	}
}
