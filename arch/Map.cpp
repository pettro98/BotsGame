#pragma once
#include "Map.h"
#include <windows.h>
#include <functional>


namespace game_module
{

	Map::~Map()
	{
		clear();
	}

	Map::Map(size_type dimension,
			size_type player_number,
			const std::string & map_type)
			: Dimension(dimension)
			, MapType(map_type)
	{
		generate_map(player_number);
	}

	size_type Map::dimension() const
	{
		return Dimension;
	}

	std::string Map::map_type() const
	{
		return MapType;
	}

	Hex * Map::operator () (const Pair & hex) const
	{
		return Root[hex.First][hex.Second];
	}

	Hex * Map::operator () (size_type coord1, size_type coord2) const
	{
		return Root[coord1][coord2];
	}

	hex_color Map::color(const Pair & hex) const
	{
		return (*this)(hex)->color();
	}

	unit_type Map::type(const Pair & hex) const
	{
		return (*this)(hex)->get_hex_unit_type();
	}

	bool Map::hex_exist(const Pair & hex) const
	{
		return (hex.First > 0 && hex.Second > 0 && hex.First < Dimension - 1 
			&& hex.Second < Dimension - 1);
	}

	void Map::clear()
	{
		for (size_type i = 0; i < Dimension; ++i)
		{
			for (size_type j = 0; j < Dimension; ++j)
			{
				delete Root[i][j];
			}
			delete[] Root[i];
		}
		delete[] Root;
	}

	bool Map::get_neighbours_exist(size_type coord1, size_type coord2,
		std::function <bool(hex_color)> compare1,
		std::function <bool(unit_type)> compare2) const
	{
		if (coord1 % 2)
		{
			if (hex_acceptable(Pair(coord1 - 1, coord2), compare1, compare2)
			|| hex_acceptable(Pair(coord1 - 1, coord2 + 1), compare1, compare2)
			|| hex_acceptable(Pair(coord1, coord2 + 1), compare1, compare2)
			|| hex_acceptable(Pair(coord1 + 1, coord2 + 1), compare1, compare2)
			|| hex_acceptable(Pair(coord1 + 1, coord2), compare1, compare2)
			|| hex_acceptable(Pair(coord1, coord2 - 1), compare1, compare2))
			{
				return true;
			}
		}
		else
		{
			if (hex_acceptable(Pair(coord1 - 1, coord2 - 1), compare1, compare2)
			|| hex_acceptable(Pair(coord1 - 1, coord2), compare1, compare2)
			|| hex_acceptable(Pair(coord1, coord2 + 1), compare1, compare2)
			|| hex_acceptable(Pair(coord1 + 1, coord2), compare1, compare2)
			|| hex_acceptable(Pair(coord1 + 1, coord2 - 1), compare1, compare2)
			|| hex_acceptable(Pair(coord1, coord2 - 1), compare1, compare2))
			{
				return true;
			}
		}
		return false;
	}

	bool Map::get_neighbours_exist(const Pair & hex,
		std::function <bool(hex_color)> compare1,
		std::function <bool(unit_type)> compare2) const
	{
		return get_neighbours_exist(hex.First, hex.Second, compare1, compare2);
	}

	std::vector<Pair> Map::get_neighbours(size_type coord1, size_type coord2,
		std::function <bool(hex_color)> compare1,
		std::function <bool(unit_type)> compare2) const
	{
		std::vector<Pair> result;
		if (coord1 % 2)
		{
			if (hex_acceptable(Pair(coord1 - 1, coord2), compare1, compare2))
			{
				result.push_back(Pair(coord1 - 1, coord2));
			}
			if (hex_acceptable(Pair(coord1 - 1, coord2 + 1), compare1, compare2))
			{
				result.push_back(Pair(coord1 - 1, coord2 + 1));
			}
			if (hex_acceptable(Pair(coord1, coord2 + 1), compare1, compare2))
			{
				result.push_back(Pair(coord1, coord2 + 1));
			}
			if (hex_acceptable(Pair(coord1 + 1, coord2 + 1), compare1, compare2))
			{
				result.push_back(Pair(coord1 + 1, coord2 + 1));
			}
			if (hex_acceptable(Pair(coord1 + 1, coord2), compare1, compare2))
			{
				result.push_back(Pair(coord1 + 1, coord2));
			}
			if (hex_acceptable(Pair(coord1, coord2 - 1), compare1, compare2))
			{
				result.push_back(Pair(coord1, coord2 - 1));
			}
		}
		else
		{
			if (hex_acceptable(Pair(coord1 - 1, coord2 - 1), compare1, compare2))
			{
				result.push_back(Pair(coord1 - 1, coord2 - 1));
			}
			if (hex_acceptable(Pair(coord1 - 1, coord2), compare1, compare2))
			{
				result.push_back(Pair(coord1 - 1, coord2));
			}
			if (hex_acceptable(Pair(coord1, coord2 + 1), compare1, compare2))
			{
				result.push_back(Pair(coord1, coord2 + 1));
			}
			if (hex_acceptable(Pair(coord1 + 1, coord2), compare1, compare2))
			{
				result.push_back(Pair(coord1 + 1, coord2));
			}
			if (hex_acceptable(Pair(coord1 + 1, coord2 - 1), compare1, compare2))
			{
				result.push_back(Pair(coord1 + 1, coord2 - 1));
			}
			if (hex_acceptable(Pair(coord1, coord2 - 1), compare1, compare2))
			{
				result.push_back(Pair(coord1, coord2 - 1));
			}
		}
		return result;
	}

	std::vector<Pair> Map::get_neighbours(const Pair & hex,
		std::function <bool(hex_color)> compare1,
		std::function <bool(unit_type)> compare2) const
	{
		return get_neighbours(hex.First, hex.Second, compare1, compare2);
	}
	
	std::vector<Pair> Map::get_hex_row(const Pair & hex, size_type radius,
		std::function <bool(hex_color)> compare1,
		std::function <bool(unit_type)> compare2) const
	{
		std::vector<Pair> result;
		if (radius == 0)
		{
			if (hex_acceptable(hex, compare1, compare2))
			{
				result.push_back(hex);
			}
			return result;
		}
		if (hex.First % 2)
		{
			for (size_type i = hex.Second - (radius) / 2; i < hex.Second + (radius + 3) / 2; ++i)
			{
				if (hex_acceptable(Pair(hex.First - radius, i), compare1, compare2))
				{
					result.push_back(Pair(hex.First - radius, i));
				}
			}
			for (size_type i = hex.First - radius + 1; i < hex.First + radius; ++i)
			{
				if (hex_acceptable(Pair(i, hex.Second - abs(hex.First - i) / 2 + radius),
					compare1, compare2))
				{
					result.push_back(Pair(i, hex.Second - abs(hex.First - i) / 2 + radius));
				}
			}
			for (size_type i = hex.Second + (radius + 1) / 2; i > hex.Second - (radius + 2) / 2; --i)
			{
				if (hex_acceptable(Pair(hex.First + radius, i), compare1, compare2))
				{
					result.push_back(Pair(hex.First + radius, i));
				}
			}
			for (size_type i = hex.First + radius - 1; i > hex.First - radius; --i)
			{
				if (hex_acceptable(Pair(i, hex.Second + (abs(hex.First - i) + 1) / 2 - radius),
					compare1, compare2))
				{
					result.push_back(Pair(i, hex.Second + (abs(hex.First - i) + 1) / 2 - radius));
				}
			}
		}
		else
		{
			for (size_type i = hex.Second - (radius + 1) / 2; i < hex.Second + (radius + 2) / 2; ++i)
			{
				if (hex_acceptable(Pair(hex.First - radius, i), compare1, compare2))
				{
					result.push_back(Pair(hex.First - radius, i));
				}
			}
			for (size_type i = hex.First - radius + 1; i < hex.First + radius; ++i)
			{
				if (hex_acceptable(Pair(i, hex.Second - (abs(hex.First - i) + 1) / 2 + radius),
					compare1, compare2))
				{
					result.push_back(Pair(i, hex.Second - (abs(hex.First - i) + 1) / 2 + radius));
				}
			}
			for (size_type i = hex.Second + (radius) / 2; i > hex.Second - (radius + 3) / 2; --i)
			{
				if (hex_acceptable(Pair(hex.First + radius, i), compare1, compare2))
				{
					result.push_back(Pair(hex.First + radius, i));
				}
			}
			for (size_type i = hex.First + radius - 1; i > hex.First - radius; --i)
			{
				if (hex_acceptable(Pair(i, hex.Second + abs(hex.First - i) / 2 - radius),
					compare1, compare2))
				{
					result.push_back(Pair(i, hex.Second + abs(hex.First - i) / 2 - radius));
				}
			}
		}
		return result;
	}

	bool Map::get_hex_row_exist(const Pair & hex, size_type radius,
		std::function <bool(hex_color)> compare1,
		std::function <bool(unit_type)> compare2) const
	{
		if (radius == 0)
		{
			if (hex_acceptable(hex, compare1, compare2))
			{
				return true;
			}
		}
		if (hex.First % 2)
		{
			for (size_type i = hex.Second - (radius) / 2; i < hex.Second + (radius + 3) / 2; ++i)
			{
				if (hex_acceptable(Pair(hex.First - radius, i), compare1, compare2))
				{
					return true;
				}
			}
			for (size_type i = hex.First - radius + 1; i < hex.First + radius; ++i)
			{
				if (hex_acceptable(Pair(i, hex.Second - abs(hex.First - i) / 2 + radius),
					compare1, compare2))
				{
					return true;
				}
			}
			for (size_type i = hex.Second + (radius + 1) / 2; i > hex.Second - (radius + 2) / 2; --i)
			{
				if (hex_acceptable(Pair(hex.First + radius, i), compare1, compare2))
				{
					return true;
				}
			}
			for (size_type i = hex.First + radius - 1; i > hex.First - radius; --i)
			{
				if (hex_acceptable(Pair(i, hex.Second + (abs(hex.First - i) + 1) / 2 - radius),
					compare1, compare2))
				{
					return true;
				}
			}
		}
		else
		{
			for (size_type i = hex.Second - (radius + 1) / 2; i < hex.Second + (radius + 2) / 2; ++i)
			{
				if (hex_acceptable(Pair(hex.First - radius, i), compare1, compare2))
				{
					return true;
				}
			}
			for (size_type i = hex.First - radius + 1; i < hex.First + radius; ++i)
			{
				if (hex_acceptable(Pair(i, hex.Second - (abs(hex.First - i) + 1) / 2 + radius),
					compare1, compare2))
				{
					return true;
				}
			}
			for (size_type i = hex.Second + (radius) / 2; i > hex.Second - (radius + 3) / 2; --i)
			{
				if (hex_acceptable(Pair(hex.First + radius, i), compare1, compare2))
				{
					return true;
				}
			}
			for (size_type i = hex.First + radius - 1; i > hex.First - radius; --i)
			{
				if (hex_acceptable(Pair(i, hex.Second + abs(hex.First - i) / 2 - radius),
					compare1, compare2))
				{
					return true;
				}
			}
		}
		return false;
	}

	std::vector<Pair> Map::get_district_border_hexs(const Pair & hex)
	{
		std::vector<Pair> result;
		bool incomplete = true;
		size_type radius = 1;
		hex_color basic_color = color(hex);
		while (incomplete)
		{
			incomplete = false;
			for (auto & i : get_hex_row(hex, radius,
				[basic_color](hex_color color) { return color != basic_color; }))
			{
				for (auto & j : get_neighbours(i,
					[basic_color](hex_color color) { return color == basic_color; }))
				{
					if ((*this)(j)->get_hex_capital() == (*this)(hex)->get_hex_capital())
					{
						result.push_back(i);
						break;
					}
				}
			}
			for (auto & i : get_hex_row(hex, radius,
				[basic_color](hex_color color) { return color == basic_color; }))
			{
				if ((*this)(i)->get_hex_capital() == (*this)(hex)->get_hex_capital())
				{
					incomplete = true;
					break;
				}
			}
			++radius;
		}
		return result;
	}
	
	std::vector<Pair> Map::district_units(const Pair & hex, std::function<bool(unit_type)> compare) const
	{
		std::vector<Pair> result;
		if ((*this)(hex)->get_hex_capital() == nullptr)
		{
			return result;
		}
		bool incomplete = true;
		size_type radius = 0;
		while (incomplete)
		{
			incomplete = false;
			for (auto & i : get_hex_row(hex, radius))
			{
				if ((*this)(hex)->get_hex_capital() == (*this)(i)->get_hex_capital())
				{
					if (compare(type(i)))
					{
						result.push_back(i);
					}
					incomplete = true;
				}
			}
			++radius;
		}
		return result;
	}

	std::vector<Pair> Map::easy_solve_maze(const Pair & hex,
		std::function <bool(unit_type)> compare) const
	{
		std::vector<Pair> result;
		if ((*this)(hex)->get_hex_capital() == nullptr)
		{
			return result;
		}
		bool incomplete = true;
		size_type radius = 0;
		hex_color basic_color = color(hex);
		while (incomplete)
		{
			incomplete = false;
			for (auto & i : get_hex_row(hex, radius,
				[basic_color](hex_color color) { return color == basic_color; }, compare))
			{
				if ((*this)(hex)->get_hex_capital() == (*this)(i)->get_hex_capital())
				{
					incomplete = true;
					result.push_back(i);
				}
			}
			++radius;
		}
		return result;
	}

	size_type Map::easy_solve_maze_count(const Pair & hex,
		std::function <bool(unit_type)> compare) const
	{
		size_type result(0);
		if ((*this)(hex)->get_hex_capital() == nullptr)
		{
			return result;
		}
		bool incomplete = true;
		size_type radius = 0;
		hex_color basic_color = color(hex);
		while (incomplete)
		{
			incomplete = false;
			for (auto & i : get_hex_row(hex, radius,
				[basic_color](hex_color color) { return color == basic_color; }, compare))
			{
				if ((*this)(hex)->get_hex_capital() == (*this)(i)->get_hex_capital())
				{
					++result;
				}
			}
			if (get_hex_row_exist(hex, radius,
				[basic_color](hex_color color) { return color == basic_color; }))
			{
				incomplete = true;
			}
			++radius;
		}
		return result;
	}
	
	std::vector<Pair> Map::solve_maze(const Pair & hex)
	{	
		std::vector<Pair> result;
		MapImpress map_impress(*this);
		hex_color basic_color = color(hex);
		result.push_back(hex);
		map_impress(hex).Color = game_module::hex_color::extra;
		size_type radius = 1;
		bool incomplete(true);
		bool find;
		while (incomplete)
		{
			find = false;
			incomplete = false;
			std::vector<Pair> row(get_hex_row(hex, radius,
				[basic_color](hex_color color) { return color == basic_color; }));
			for (auto & i : row)
			{
				if (map_impress(i).Color != game_module::hex_color::extra)
				{
					for (auto & j : get_neighbours(i, is_player_color))
					{
						if (map_impress(j).Color == game_module::hex_color::extra)
						{
							map_impress(i).Color = game_module::hex_color::extra;
							result.push_back(i);
							incomplete = true;
							break;
						}
					}
				}	
			}
			for (auto i = row.rbegin(); i != row.rend(); ++i)
			{
				if (map_impress(*i).Color != game_module::hex_color::extra)
				{
					for (auto & j : get_neighbours(*i, is_player_color))
					{
						if (map_impress(j).Color == game_module::hex_color::extra)
						{
							map_impress(*i).Color = game_module::hex_color::extra;
							result.push_back(*i);
							incomplete = true;
							break;
						}
					}
				}
			}	
			for (auto & i : row)
			{
				if (is_extra(map_impress(i).Color))
				{
					find = true;
					break;
				}
			}
			if (incomplete)
			{
				--radius;
			}
			else if (find) {
				radius += 1;
				incomplete = true;
			}
		}
		return result;			
	}
	
	void Map::generate_map(size_type player_number)
	{
		Hex *** root = new Hex **[Dimension];
		for (size_type i = 0; i < Dimension; ++i)
		{
			root[i] = new Hex *[Dimension];
			for (size_type j = 0; j < Dimension; ++j)
			{
				root[i][j] = new Hex(i, j);
				if (i * j == 0 || i >= Dimension - 1 || j >= Dimension - 1)
				{
					root[i][j]->set_color(game_module::hex_color::black);
				}
			}
		}
		Root = root;
		if (Dimension == 19 && MapType == "classic")
		{
			(*this)(Pair(3, 3))->set_hex_unit(unit_factory(game_module::unit_type::capital, 1));
			(*this)(Pair(3, 3))->set_color(red);
			(*this)(Pair(3, 3))->set_hex_capital((*this)(Pair(3, 3)));
			for (auto & i : get_neighbours(Pair(3, 3)))
			{
				(*this)(i)->set_color(red);
				(*this)(i)->set_hex_capital((*this)(Pair(3, 3)));
			}
			
			(*this)(Pair(3, 15))->set_hex_unit(unit_factory(game_module::unit_type::capital, 1));
			(*this)(Pair(3, 15))->set_color(game_module::hex_color::green);
			(*this)(Pair(3, 15))->set_hex_capital((*this)(Pair(3, 15)));
			for (auto & i : get_neighbours(Pair(3, 15)))
			{
				(*this)(i)->set_color(game_module::hex_color::green);
				(*this)(i)->set_hex_capital((*this)(Pair(3, 15)));
			}
			(*this)(Pair(15, 3))->set_hex_unit(unit_factory(game_module::unit_type::capital, 1));
			(*this)(Pair(15, 3))->set_color(game_module::hex_color::orange);
			(*this)(Pair(15, 3))->set_hex_capital((*this)(Pair(15, 3)));
			for (auto & i : get_neighbours(Pair(15, 3)))
			{
				(*this)(i)->set_color(game_module::hex_color::orange);
				(*this)(i)->set_hex_capital((*this)(Pair(15, 3)));

			}
			(*this)(Pair(15, 15))->set_hex_unit(unit_factory(game_module::unit_type::capital, 1));
			(*this)(Pair(15, 15))->set_color(game_module::hex_color::purple);
			(*this)(Pair(15, 15))->set_hex_capital((*this)(Pair(15, 15)));
			for (auto & i : get_neighbours(Pair(15, 15)))
			{
				(*this)(i)->set_color(game_module::hex_color::purple);
				(*this)(i)->set_hex_capital((*this)(Pair(15, 15)));
			}		
			for (size_type i = 0; i < 3; ++i)
			{
				for (auto & j : get_hex_row(Pair(9, 9), i))
				{
					(*this)(j)->set_hex_unit(unit_factory(game_module::unit_type::pine));
				}
			}			
			/*for (size_type i = 0; i < 1; ++i)
			{
				for (auto & j : get_hex_row(Pair(3, 9), i))
				{
					(*this)(j)->set_hex_unit(unit_factory(game_module::unit_type::palm));
				}
			}
			for (size_type i = 0; i < 1; ++i)
			{
				for (auto & j : get_hex_row(Pair(9, 3), i))
				{
					(*this)(j)->set_hex_unit(unit_factory(game_module::unit_type::palm));
				}
			}
			for (size_type i = 0; i < 1; ++i)
			{
				for (auto & j : get_hex_row(Pair(9, 15), i))
				{
					(*this)(j)->set_hex_unit(unit_factory(game_module::unit_type::palm));
				}
			}
			for (size_type i = 0; i < 1; ++i)
			{
				for (auto & j : get_hex_row(Pair(15, 9), i))
				{
					(*this)(j)->set_hex_unit(unit_factory(game_module::unit_type::palm));
				}
			}	*/	
		}
	}

	bool Map::hex_acceptable(const Pair & hex,
		std::function <bool(hex_color)> compare1,
		std::function <bool(unit_type)> compare2) const
	{
		return (hex_exist(hex) && color(hex) != game_module::hex_color::black
			&& compare1(color(hex)) && compare2(type(hex)));
	}

	void print_map(const Map & map)
	{
		HANDLE hSTDOut = GetStdHandle(STD_OUTPUT_HANDLE);
		int colors[8] = {0x0007, 0x0004, 0x0002, 0x0005, 0x0003, 0x0001, 0x0006, 0x0000 };
		char type[8] = {'0', '1', '2', 'C', 'f', '?', '?', 'G'};
		for (size_type i = 0; i < map.dimension(); ++i)
		{		
			for (size_type j = 0; j < map.dimension(); ++j)
			{
				if (j % 2)
				{
					continue;
				}
				if (map(j, i)->color() == black)
				{
					SetConsoleTextAttribute(hSTDOut, 0x0007 | FOREGROUND_INTENSITY);
					std::cout << "/";
					SetConsoleTextAttribute(hSTDOut, 0x0000 | FOREGROUND_INTENSITY);
					std::cout << "*";
					SetConsoleTextAttribute(hSTDOut, 0x0007 | FOREGROUND_INTENSITY);
					std::cout << char(0x5c);
				}
				else
				{
					SetConsoleTextAttribute(hSTDOut, 0x0007 | FOREGROUND_INTENSITY);
					std::cout << "/";
					SetConsoleTextAttribute(hSTDOut, colors[map(j, i)->color()] | FOREGROUND_INTENSITY);
					if (is_army(map(j, i)->get_hex_unit_type()))
					{
						std::cout << map(j, i)->get_hex_unit()->strength();
					}
					else if (is_tower(map(j, i)->get_hex_unit_type()))
					{
						if (map(j, i)->get_hex_unit()->strength() == 1)
						{
							std::cout << 't';
						}
						else
						{
							std::cout << 'T';
						}
					}
					else
					{
						std::cout << type[map(j, i)->get_hex_unit_type()];
					}
					SetConsoleTextAttribute(hSTDOut, 0x0007 | FOREGROUND_INTENSITY);
					std::cout << char(0x5c);
				}
				if (j + 1 < map.dimension() && i - 1 >= 0)
				{
					if (map(j + 1, i - 1)->color() == black)
					{
						SetConsoleTextAttribute(hSTDOut, 0x0000 | FOREGROUND_INTENSITY);
						std::cout << "*";
					}
					else
					{
						SetConsoleTextAttribute(hSTDOut, colors[map(j + 1, i - 1)->color()] | FOREGROUND_INTENSITY);
						if (is_army(map(j + 1, i - 1)->get_hex_unit_type()))
						{
							std::cout << map(j + 1, i - 1)->get_hex_unit()->strength();

						}
						else if (is_tower(map(j + 1, i - 1)->get_hex_unit_type()))
						{
							if (map(j + 1, i - 1)->get_hex_unit()->strength() == 1)
							{
								std::cout << 't';
							}
							else
							{
								std::cout << 'T';
							}
						}
						else
						{
							std::cout << type[map(j + 1, i - 1)->get_hex_unit_type()];
						}
					}
				}
				else
				{
					std::cout << " ";
				}
			}
			std::cout << std::endl;
			for (size_type j = 0; j < map.dimension(); ++j)
			{
				if (j % 2)
				{
					continue;
				}
				if (map(j, i)->color() == black)
				{
					SetConsoleTextAttribute(hSTDOut, 0x0007 | FOREGROUND_INTENSITY);
					std::cout << char(0x5c);
					SetConsoleTextAttribute(hSTDOut, 0x0000 | FOREGROUND_INTENSITY);
					std::cout << "*";
					SetConsoleTextAttribute(hSTDOut, 0x0007 | FOREGROUND_INTENSITY);
					std::cout << "/";
				}
				else
				{
					SetConsoleTextAttribute(hSTDOut, 0x0007 | FOREGROUND_INTENSITY);
					std::cout << char(0x5c);
					SetConsoleTextAttribute(hSTDOut, colors[map(j, i)->color()] | FOREGROUND_INTENSITY);
					if (is_army(map(j, i)->get_hex_unit_type()))
					{
						std::cout << map(j, i)->get_hex_unit()->strength();

					}
					else if (is_tower(map(j, i)->get_hex_unit_type()))
					{
						if (map(j, i)->get_hex_unit()->strength() == 1)
						{
							std::cout << 't';
						}
						else
						{
							std::cout << 'T';
						}
					}
					else
					{
						std::cout << type[map(j, i)->get_hex_unit_type()];
					}
					SetConsoleTextAttribute(hSTDOut, 0x0007 | FOREGROUND_INTENSITY);
					std::cout << "/";
				}
				if (j + 1 < map.dimension())
				{
					if (map(j + 1, i)->color() == black)
					{
						SetConsoleTextAttribute(hSTDOut, 0x0000 | FOREGROUND_INTENSITY);
						std::cout << "*";
					}
					else
					{
						SetConsoleTextAttribute(hSTDOut, colors[map(j + 1, i)->color()] | FOREGROUND_INTENSITY);
						if (is_army(map(j + 1, i)->get_hex_unit_type()))
						{
							std::cout << map(j + 1, i)->get_hex_unit()->strength();

						}
						else if (is_tower(map(j + 1, i)->get_hex_unit_type()))
						{
							if (map(j + 1, i)->get_hex_unit()->strength() == 1)
							{
								std::cout << 't';
							}
							else
							{
								std::cout << 'T';
							}
						}
						else
						{
							std::cout << type[map(j + 1, i)->get_hex_unit_type()];
						}
					}
				}
				else
				{
					std::cout << " ";
				}
			}
			std::cout << std::endl;		
		}	
	}

	MapImpress::~MapImpress()
	{
		for (size_type i = 0; i < Dimension; ++i)
		{
			for (size_type j = 0; j < Dimension; ++j)
			{
				delete Root[i][j];
			}
			delete[] Root[i];
		}
		delete[] Root;
	}

	MapImpress::MapImpress(const Map & map)
		: Dimension(map.dimension())
	{
		HexImpress *** root = new HexImpress **[Dimension];
		for (size_type i = 0; i < Dimension; ++i)
		{
			root[i] = new HexImpress *[Dimension];
			for (size_type j = 0; j < Dimension; ++j)
			{
				root[i][j] = new HexImpress(*map(Pair(i, j)));
			}
		}
		Root = root;
	}

	HexImpress & MapImpress::operator () (const Pair & pair)
	{
		return *Root[pair.First][pair.Second];
	}

	HexImpress::HexImpress(const Hex & hex)
		: Coordinates(hex.coordinates())
		, Color(hex.color())
	{
		UnitType = hex.get_hex_unit_type();
	}
}
