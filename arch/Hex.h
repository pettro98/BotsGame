#pragma once

#include "General.h"
#include "Unit.h"



namespace game_module
{
	class Unit;
	class Capital;

	class Hex
	{
	private:
		Pair Coordinates; // координаты гекса
		hex_color Color; // индекс владельца
		Unit * HexUnit; // указатель на юнит в гексе 
		Capital * HexCapital; // указатель на столицу области
	public:
		~Hex();
		Hex(size_type coord_1, size_type coord_2);
		Pair coordinates() const;
		hex_color color() const;
		unit_type get_hex_unit_type() const;
		Unit * get_hex_unit();
		Capital * get_hex_capital();
		bool occupied() const;
		bool operator == (const Hex & hex) const;		
		void set_color(hex_color new_color);	
		void set_hex_capital(Hex * new_capital);
		void remove_hex_unit();
		void delete_hex_unit();
		void set_hex_unit(Unit * unit);		
	};

	bool operator != (const Hex & hex1, const Hex & hex2);
	size_type get_distance(const Pair & hex1, const Pair & hex2);
	size_type get_distance(const Hex & hex1, const Hex & hex2);
	std::vector<Pair> get_hex_row(const Pair & hex, size_type radius, size_type dimension);
	bool hex_exist(const Pair & hex, size_type dimension);
}
