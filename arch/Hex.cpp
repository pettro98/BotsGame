#pragma once
#include "Hex.h"



namespace game_module
{
	Hex::~Hex()
	{
		delete HexUnit;
	}

	Hex::Hex(size_type coord_1, size_type coord_2)
		: Coordinates(coord_1, coord_2)
		, Color(game_module::hex_color::blank)
		, HexUnit(nullptr)
		, HexCapital(nullptr)
	{ }

	Pair Hex::coordinates() const
	{
		return Coordinates;
	}

	hex_color Hex::color() const
	{
		return Color;
	}

	unit_type Hex::get_unit_type() const
	{
		if (HexUnit == nullptr)
		{
			return game_module::unit_type::none;
		}
		return HexUnit->type();
	}

	Unit * Hex::get_unit()
	{
		return HexUnit;
	}

	Capital * Hex::get_capital()
	{
		return HexCapital;
	}

	bool Hex::occupied() const
	{
		return HexUnit != nullptr;
	}

	bool Hex::operator == (const Hex & hex) const
	{
		return Coordinates == hex.coordinates();
	}

	void Hex::set_color(hex_color new_color)
	{
		Color = new_color;
	}

	void Hex::set_unit(Unit * unit)
	{
		delete HexUnit;
		HexUnit = unit;
		HexUnit->set_hex(this);
	}

	void Hex::set_capital(Hex * new_capital)
	{
		if (new_capital == nullptr)
		{
			HexCapital = nullptr;
		}
		else if (new_capital->get_unit_type() == capital)
		{
			HexCapital = static_cast<Capital *>(new_capital->get_unit());
		}
		else
		{
			HexCapital = static_cast<Capital *>(new_capital->get_capital());
		}
	}

	void Hex::remove_unit()
	{
		HexUnit = nullptr;
	}

	void Hex::delete_unit()
	{
		delete HexUnit;
		HexUnit = nullptr;
	}

	bool operator != (const Hex & hex1, const Hex & hex2)
	{
		return !(hex1 == hex2);
	}

	size_type get_distance(const Pair & hex1, const Pair & hex2)
	{
		size_type result(abs(hex1.First - hex2.First));
		if (hex1.First % 2)
		{
			if (hex2.Second >= hex1.Second - result / 2 && hex2.Second <= hex1.Second + (result + 1) / 2)
			{
				return result;
			}
			else
			{
				if (hex2.Second < hex1.Second - result / 2)
				{
					return result + abs(hex2.Second - hex1.Second + result / 2);
				}
				else
				{
					return result + abs(hex2.Second - hex1.Second - (result + 1) / 2);
				}
			}
		}
		else
		{
			if (hex2.Second >= hex1.Second - (result + 1) / 2 && hex2.Second <= hex1.Second + result / 2)
			{
				return result;
			}
			else
			{
				if (hex2.Second < hex1.Second - (result + 1) / 2)
				{
					return result + abs(hex2.Second - hex1.Second + (result + 1) / 2);
				}
				else
				{
					return result + abs(hex2.Second - hex1.Second - result / 2);
				}
			}
		}
	}
}
