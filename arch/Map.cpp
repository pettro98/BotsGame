#pragma once
#include "Map.h"
#include <windows.h>
#include <functional>
#include <algorithm> 


namespace game_module
{

	Map::~Map()
	{
		clear();
	}

	Map::Map(size_type dimension_x,
		size_type dimension_y,
		size_type player_number,
		const std::string & map_type)
			: DimensionX(dimension_x)
			, DimensionY(dimension_y)
			, MapType(map_type)
	{
		generate_map(player_number);
	}

	size_type Map::dimension_x() const
	{
		return DimensionX;
	}

	size_type Map::dimension_y() const
	{
		return DimensionY;
	}

	std::string Map::map_type() const
	{
		return MapType;
	}

	Hex * Map::operator () (const Pair & hex) const
	{
		return (*this)(hex.First, hex.Second);
	}

	Hex * Map::operator () (size_type coord1, size_type coord2) const
	{
		if (coord1 < DimensionX && coord1 >= 0 && coord2 < DimensionY && coord2 >= 0)
		{
			return Root[coord1][coord2];
		}
		return nullptr;
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
		return (hex.First > 0 && hex.Second > 0 && hex.First < DimensionX - 1 
			&& hex.Second < DimensionY - 1);
	}

	void Map::clear()
	{
		for (size_type i = 0; i < DimensionX; ++i)
		{
			for (size_type j = 0; j < DimensionY; ++j)
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
		std::function <bool(unit_type)> compare2,
		bool return_black) const
	{
		std::vector<Pair> result;
		if (coord1 % 2)
		{
			if (hex_acceptable(Pair(coord1 - 1, coord2), compare1, compare2, return_black))
			{
				result.push_back(Pair(coord1 - 1, coord2));
			}
			if (hex_acceptable(Pair(coord1 - 1, coord2 + 1), compare1, compare2, return_black))
			{
				result.push_back(Pair(coord1 - 1, coord2 + 1));
			}
			if (hex_acceptable(Pair(coord1, coord2 + 1), compare1, compare2, return_black))
			{
				result.push_back(Pair(coord1, coord2 + 1));
			}
			if (hex_acceptable(Pair(coord1 + 1, coord2 + 1), compare1, compare2, return_black))
			{
				result.push_back(Pair(coord1 + 1, coord2 + 1));
			}
			if (hex_acceptable(Pair(coord1 + 1, coord2), compare1, compare2, return_black))
			{
				result.push_back(Pair(coord1 + 1, coord2));
			}
			if (hex_acceptable(Pair(coord1, coord2 - 1), compare1, compare2, return_black))
			{
				result.push_back(Pair(coord1, coord2 - 1));
			}
		}
		else
		{
			if (hex_acceptable(Pair(coord1 - 1, coord2 - 1), compare1, compare2, return_black))
			{
				result.push_back(Pair(coord1 - 1, coord2 - 1));
			}
			if (hex_acceptable(Pair(coord1 - 1, coord2), compare1, compare2, return_black))
			{
				result.push_back(Pair(coord1 - 1, coord2));
			}
			if (hex_acceptable(Pair(coord1, coord2 + 1), compare1, compare2, return_black))
			{
				result.push_back(Pair(coord1, coord2 + 1));
			}
			if (hex_acceptable(Pair(coord1 + 1, coord2), compare1, compare2, return_black))
			{
				result.push_back(Pair(coord1 + 1, coord2));
			}
			if (hex_acceptable(Pair(coord1 + 1, coord2 - 1), compare1, compare2, return_black))
			{
				result.push_back(Pair(coord1 + 1, coord2 - 1));
			}
			if (hex_acceptable(Pair(coord1, coord2 - 1), compare1, compare2, return_black))
			{
				result.push_back(Pair(coord1, coord2 - 1));
			}
		}
		return result;
	}

	std::vector<Pair> Map::get_neighbours(const Pair & hex,
		std::function <bool(hex_color)> compare1,
		std::function <bool(unit_type)> compare2,
		bool return_black) const
	{
		return get_neighbours(hex.First, hex.Second, compare1, compare2, return_black);
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

	std::vector<Pair> Map::get_internal_border(const Pair & hex,
		std::function <bool(unit_type)> compare) const
	{
		std::vector<Pair> result;
		hex_color basic_color = color(hex);
		for (auto & i : easy_solve_maze(hex))
		{
			if (get_neighbours_exist(i, 
				[basic_color](hex_color color) { return color != basic_color; },
				compare))
			{
				result.push_back(i);
			}
		}
		return result;
	}

	std::vector<Pair> Map::get_external_border(const Pair & hex,
		std::function <bool(hex_color)> compare1,
		std::function <bool(unit_type)> compare2) const
	{
		std::vector<Pair> result;
		bool incomplete = true;
		size_type radius = 1;
		hex_color basic_color = color(hex);
		if (compare1(color(hex)))
		{
			compare1 = [basic_color](hex_color color) { return color != basic_color; };
		}
		while (incomplete)
		{
			incomplete = false;
			for (auto & i : get_hex_row(hex, radius, compare1, compare2))
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
				[basic_color](hex_color color) { return color == basic_color; }))
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
				if (is_extra(map_impress(i).Color))
				{
					find = true;
					continue;
				}
				for (auto & j : get_neighbours(i,
					[basic_color](hex_color color) { return color == basic_color; }))
				{
					if (is_extra(map_impress(j).Color))
					{
						map_impress(i).Color = game_module::hex_color::extra;
						result.push_back(i);
						incomplete = true;
						break;
					}
				}
			}
			for (auto i = row.rbegin(); i != row.rend(); ++i)
			{
				if (is_extra(map_impress(*i).Color))
				{
					find = true;
					continue;
				}
				for (auto & j : get_neighbours(*i,
					[basic_color](hex_color color) { return color == basic_color; }))
				{
					if (is_extra(map_impress(j).Color))
					{
						map_impress(*i).Color = game_module::hex_color::extra;
						result.push_back(*i);
						incomplete = true;
						break;
					}
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
		if (MapType == "random")
		{
			if (DimensionX < 17)
			{
				DimensionX = 17;
			}
			else if (!(DimensionX % 2))
			{
				++DimensionX;
			}
			if (DimensionY < 17)
			{
				DimensionY = 17;
			}
			else if (!(DimensionY % 2))
			{
				++DimensionY;
			}
			Hex *** root = new Hex **[DimensionX];
			for (size_type i = 0; i < DimensionX; ++i)
			{
				root[i] = new Hex *[DimensionY];
				for (size_type j = 0; j < DimensionY; ++j)
				{
					root[i][j] = new Hex(i, j);
					root[i][j]->set_color(game_module::hex_color::black);
				}
			}
			Root = root;
			root[DimensionX / 2][DimensionY / 2]->set_color(game_module::hex_color::blank);

			for (size_type i = 0; i < 10; ++i)
			{
				Pair start_point(DimensionX / 2, DimensionY / 2);
				for (size_type j = 0; j < 0.4 * DimensionX * DimensionY / 10;)
				{
					auto black_neighbours = get_neighbours(start_point, is_black, is_none, true);
					std::random_shuffle(black_neighbours.begin(), black_neighbours.end());
					bool find = false;
					for (auto & k : black_neighbours)
					{
						if (hex_exist(k))
						{
							(*this)(k)->set_color(game_module::hex_color::blank);
							j++;
							start_point = k;
							find = true;
							break;
						}
					}
					if (!find)
					{
						black_neighbours = get_neighbours(start_point, is_blank, is_none, true);
						std::random_shuffle(black_neighbours.begin(), black_neighbours.end());
						start_point = black_neighbours[0];
					}
				}
			}
			auto all_map = solve_maze(Pair(DimensionX / 2, DimensionY / 2));
			std::vector<Pair> capitals;
			size_type iterations = 0;
			while (capitals.size() < player_number)
			{
				++iterations;
				Pair new_capital = all_map[std::rand() % all_map.size()];
				bool find = false;
				for (auto & i : capitals)
				{
					if (get_distance(new_capital, i) < 6)
					{
						find = true;
						break;
					}
				}
				if (!find)
				{
					capitals.push_back(new_capital);
					iterations = 0;
				}
				if (iterations > 100)
				{
					capitals.clear();
				}
			}
			for (auto & i : capitals)
			{
				(*this)(i)->set_hex_unit(unit_factory(game_module::unit_type::capital));
			}
			return;
		}
		else
		{
			if (DimensionX < 11)
			{
				DimensionX = 11;
			}
			else if (!(DimensionX % 2))
			{
				++DimensionX;
			}
			if (DimensionY < 11)
			{
				DimensionY = 11;
			}
			else if (!(DimensionY % 2))
			{
				++DimensionY;
			}
			Hex *** root = new Hex **[DimensionX];
			for (size_type i = 0; i < DimensionX; ++i)
			{
				root[i] = new Hex *[DimensionY];
				for (size_type j = 0; j < DimensionY; ++j)
				{
					root[i][j] = new Hex(i, j);
					if (!hex_exist(Pair(i, j)))
					{
						root[i][j]->set_color(game_module::hex_color::black);
					}
				}
			}
			Root = root;
			if (MapType == "classic")
			{
				(*this)(Pair(3, 3))->set_hex_unit(unit_factory(game_module::unit_type::capital));
				(*this)(Pair(3, DimensionY - 4))->set_hex_unit(unit_factory(game_module::unit_type::capital));
				(*this)(Pair(DimensionX - 4, 3))->set_hex_unit(unit_factory(game_module::unit_type::capital));
				(*this)(Pair(DimensionX - 4, DimensionY - 4))->set_hex_unit(unit_factory(game_module::unit_type::capital));
				for (size_type i = 0; i < 3; ++i)
				{
					for (auto & j : get_hex_row(Pair(DimensionX / 2, DimensionY / 2), i))
					{
						(*this)(j)->set_hex_unit(unit_factory(game_module::unit_type::pine));
					}
				}
			}
			else if (MapType == "duel")
			{
				(*this)(Pair(DimensionX / 2, 3))->set_hex_unit(unit_factory(game_module::unit_type::capital));
				(*this)(Pair(DimensionX / 2, DimensionY - 4))->set_hex_unit(unit_factory(game_module::unit_type::capital));
				for (size_type i = 0; i < 3; ++i)
				{
					for (auto & j : get_hex_row(Pair(DimensionX / 2, DimensionY / 2), i))
					{
						(*this)(j)->set_hex_unit(unit_factory(game_module::unit_type::pine));
					}
				}
			}
		}
	}

	bool Map::hex_acceptable(const Pair & hex,
		std::function <bool(hex_color)> compare1,
		std::function <bool(unit_type)> compare2,
		bool return_black) const
	{
		if (return_black)
		{
			return (hex_exist(hex) && compare1(color(hex)) && compare2(type(hex)));
		}
		return (hex_exist(hex) && color(hex) != game_module::hex_color::black
			&& compare1(color(hex)) && compare2(type(hex)));
	}

	void print_map(const Map & map)
	{
		HANDLE hSTDOut = GetStdHandle(STD_OUTPUT_HANDLE);
		int colors[8] = {0x0007, 0x0004, 0x0002, 0x0003, 0x0005, 0x0001, 0x0006, 0x0000 };
		char type[8] = {'0', '1', '2', 'C', 'f', '?', '?', 'G'};
		for (size_type i = 0; i < map.dimension_y(); ++i)
		{		
			for (size_type j = 0; j < map.dimension_x(); ++j)
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
						if (map(j, i)->get_hex_unit()->strength() == 2)
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
				if (j + 1 < map.dimension_x() && i - 1 >= 0)
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
							if (map(j + 1, i - 1)->get_hex_unit()->strength() == 2)
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
			for (size_type j = 0; j < map.dimension_x(); ++j)
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
						if (map(j, i)->get_hex_unit()->strength() == 2)
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
				if (j + 1 < map.dimension_x())
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
							if (map(j + 1, i)->get_hex_unit()->strength() == 2)
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
		for (size_type i = 0; i < DimensionX; ++i)
		{
			for (size_type j = 0; j < DimensionY; ++j)
			{
				delete Root[i][j];
			}
			delete[] Root[i];
		}
		delete[] Root;
	}

	MapImpress::MapImpress(const Map & map)
		: DimensionX(map.dimension_x())
		, DimensionY(map.dimension_y())
	{
		HexImpress *** root = new HexImpress **[DimensionX];
		for (size_type i = 0; i < DimensionX; ++i)
		{
			root[i] = new HexImpress *[DimensionY];
			for (size_type j = 0; j < DimensionY; ++j)
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
