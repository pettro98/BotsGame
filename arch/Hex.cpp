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
		, Index(blank)
		, HexUnit(nullptr)
		, HexCapital(nullptr)
	{ }


	Pair Hex::coordinates() const
	{
		return Coordinates;
	}

	hex_color Hex::index() const
	{
		return Index;
	}

	unit_type Hex::get_hex_unit_type() const
	{
		if (HexUnit == nullptr)
			return none;
		else
			return HexUnit->type();
	}

	Unit * Hex::get_hex_unit()
	{
		return HexUnit;
	}

	Capital * Hex::get_hex_capital()
	{
		return HexCapital;
	}

	bool Hex::occupied() const
	{
		return (HexUnit);
	}

	bool Hex::operator == (const Hex & hex) const
	{
		return (Coordinates == hex.coordinates());
	}

	void Hex::set_index(hex_color new_index)
	{
		Index = new_index;
	}

	void Hex::set_hex_capital(Capital * new_capital)
	{
		HexCapital = new_capital;
	}

	void Hex::remove_hex_unit()
	{
		HexUnit = nullptr;
	}

	void Hex::delete_hex_unit()
	{
		delete HexUnit;
		HexUnit = nullptr;
	}

	void Hex::set_hex_unit(Unit * unit)
	{
		delete HexUnit;
		HexUnit = unit;
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
				return result;
			else
			{
				if (hex2.Second < hex1.Second - result / 2)
					return result + abs(hex2.Second - hex1.Second + result / 2);
				else
					return result + abs(hex2.Second - hex1.Second - (result + 1) / 2);
			}

		}
		else
		{

			if (hex2.Second >= hex1.Second - (result + 1) / 2 && hex2.Second <= hex1.Second + result / 2)
				return result;
			else
			{
				if (hex2.Second < hex1.Second - (result + 1) / 2)
					return result + abs(hex2.Second - hex1.Second + (result + 1) / 2);
				else
					return result + abs(hex2.Second - hex1.Second - result / 2);
			}

		}
	}

	size_type get_distance(const Hex & hex1, const Hex & hex2)
	{
		return get_distance(hex1.coordinates(), hex2.coordinates());
	}


	std::vector<Pair> get_hex_row(const Pair & hex, size_type radius, size_type max)
	{

		std::vector<Pair> result;

		if (radius == 0)
		{
			result.push_back(hex);
			return result;
		}

		if (hex.First % 2)
		{

			for (size_type i = hex.Second - (radius) / 2; i < hex.Second + (radius + 3) / 2; ++i)
				if (hex_exist(Pair(hex.First - radius, i), max))
					result.push_back(Pair(hex.First - radius, i));

			for (size_type i = hex.First - radius + 1; i < hex.First + radius; ++i)
				if (hex_exist(Pair(i, hex.Second - abs(hex.First - i) / 2 + radius), max))
					result.push_back(Pair(i, hex.Second - abs(hex.First - i) / 2 + radius));

			for (size_type i = hex.Second + (radius + 1) / 2; i > hex.Second - (radius + 2) / 2; --i)
				if (hex_exist(Pair(hex.First + radius, i), max))
					result.push_back(Pair(hex.First + radius, i));

			for (size_type i = hex.First + radius - 1; i > hex.First - radius; --i)
				if (hex_exist(Pair(i, hex.Second + (abs(hex.First - i) + 1) / 2 - radius), max))
					result.push_back(Pair(i, hex.Second + (abs(hex.First - i) + 1) / 2 - radius));

		}
		else
		{

			for (size_type i = hex.Second - (radius + 1) / 2; i < hex.Second + (radius + 2) / 2; ++i)
				if (hex_exist(Pair(hex.First - radius, i), max))
					result.push_back(Pair(hex.First - radius, i));

			for (size_type i = hex.First - radius + 1; i < hex.First + radius; ++i)
				if (hex_exist(Pair(i, hex.Second - (abs(hex.First - i) + 1) / 2 + radius), max))
					result.push_back(Pair(i, hex.Second - (abs(hex.First - i) + 1) / 2 + radius));

			for (size_type i = hex.Second + (radius) / 2; i > hex.Second - (radius + 3) / 2; --i)
				if (hex_exist(Pair(hex.First + radius, i), max))
					result.push_back(Pair(hex.First + radius, i));

			for (size_type i = hex.First + radius - 1; i > hex.First - radius; --i)
				if (hex_exist(Pair(i, hex.Second + abs(hex.First - i) / 2 - radius), max))
					result.push_back(Pair(i, hex.Second + abs(hex.First - i) / 2 - radius));

		}

		return result;

	}

	bool hex_exist(const Pair & hex, size_type max)
	{
		return (hex.First > 0 && hex.Second > 0 && hex.First < max && hex.Second < max);
	}

}
