#pragma once
#include "Map.h"
#include <windows.h>


namespace game_module
{

	Map::Map(size_type dimension,
			size_type player_number,
			const std::string & map_type)
			: Dimension(dimension)
			, MapType(map_type)
	{
		generate_map(Dimension, player_number, MapType);
	}

	size_type Map::dimension() const
	{
		return Dimension;
	}

	std::string Map::map_type() const
	{
		return MapType;
	}

	Hex & Map::operator () (const Pair & pair)
	{
		return *Root[pair.First][pair.Second];
	}

	Hex Map::operator () (const Pair & pair) const
	{
		return *Root[pair.First][pair.Second];
	}

	Hex & Map::operator () (size_type coord1, size_type coord2)
	{
		return *Root[coord1][coord2];
	}

	Hex Map::operator () (size_type coord1, size_type coord2) const
	{
		return *Root[coord1][coord2];
	}


	std::vector<Pair> Map::get_neighbours(size_type coord1, size_type coord2) const
	{
		std::vector<Pair> result;
		if (coord1 % 2)
		{
			if (coord1 - 1 >= 0)
				result.push_back(Pair(coord1 - 1, coord2));
			if (coord1 - 1 >= 0, coord2 + 1 < Dimension)
				result.push_back(Pair(coord1 - 1, coord2 + 1));
			if (coord2 + 1 < Dimension)
				result.push_back(Pair(coord1, coord2 + 1));
			if (coord1 + 1 < Dimension, coord2 + 1 < Dimension)
				result.push_back(Pair(coord1 + 1, coord2 + 1));
			if (coord1 + 1 < Dimension)
				result.push_back(Pair(coord1 + 1, coord2));
			if (coord2 - 1 >= 0)
				result.push_back(Pair(coord1, coord2 - 1));
		}
		else
		{
			if (coord1 - 1 >= 0, coord2 - 1 >= 0)
				result.push_back(Pair(coord1 - 1, coord2 - 1));
			if (coord1 - 1 >= 0)
				result.push_back(Pair(coord1 - 1, coord2));
			if (coord2 + 1 < Dimension)
				result.push_back(Pair(coord1, coord2 + 1));
			if (coord1 + 1 < Dimension)
				result.push_back(Pair(coord1 + 1, coord2));
			if (coord1 + 1 < Dimension, coord2 - 1 >= 0)
				result.push_back(Pair(coord1 + 1, coord2 - 1));
			if (coord2 - 1 >= 0)
				result.push_back(Pair(coord1, coord2 - 1));
		}

		return result;

	}

	std::vector<Pair> Map::get_neighbours(Pair hex) const
	{
		return get_neighbours(hex.First, hex.Second);
	}

	std::vector<Pair> Map::get_exist_neighbours(size_type coord1, size_type coord2) const
	{
		std::vector<Pair> result;

		for (Pair & i : get_neighbours(coord1, coord2))
		{
			if (i.First >= 0 && i.First < Dimension
			&& i.Second >= 0 && i.Second < Dimension
			&& (*this)(i).index() != -1)
				result.push_back(i);
		}

		return result;
	}

	std::vector<Pair> Map::get_exist_neighbours(Pair hex) const
	{
		return get_exist_neighbours(hex.First, hex.Second);
	}


	std::vector<Pair> Map::get_district_border_hexs(const Pair & hex)
	{

		std::vector<Pair> result;

		bool incomplete = true;
		size_type radius = 1;

		while (incomplete)
		{
			incomplete = false;
			for (auto & i : get_hex_row(hex, radius, Dimension))
				if ((*this)(i).index() != black 
					&& (*this)(i).index() != (*this)(hex).index())
				{
					for (auto & j : get_exist_neighbours(i))
						if ((*this)(j).index() == (*this)(hex).index())
						{
							result.push_back(i);
							break;
						}
				}
				else if ((*this)(i).index() == (*this)(hex).index())
					incomplete = true;


			++radius;
		}

		return result;

	}

	std::vector<Pair> Map::easy_solve_maze(const Pair & hex) const
	{

		std::vector<Pair> result;

		if ((*this)(hex).get_hex_capital() == nullptr)
			return result;

		result.push_back(hex);

		bool incomplete = true;
		size_type radius = 1;

		while (incomplete)
		{
			incomplete = false;
			for (auto & i : get_hex_row(hex, radius, Dimension))
				if ((*this)(hex).get_hex_capital() == (*this)(i).get_hex_capital())
				{
					incomplete = true;
					result.push_back(i);
				}

			++radius;
		}

		return result;

	}


	std::vector<Pair> Map::solve_maze(const Pair & hex)
	{
		
		std::vector<Pair> result;

		MapImpress map_impress(*this);

		hex_color basic_color = map_impress(hex).Color;

		map_impress(hex).Color = extra;
		result.push_back(hex);

		size_type radius = 1;
		bool incomplete = true;
		bool find = false;

		while (incomplete)
		{

			find = false;
			incomplete = false;

			std::vector<Pair> row(get_hex_row(hex, radius, Dimension));

			for (auto & i : row)
			{
				if (map_impress(i).Color == basic_color)
				{
					for (auto & j : get_exist_neighbours(i))
						if (map_impress(j).Color == extra)
						{
							map_impress(i).Color = extra;
							result.push_back(i);
							incomplete = true;
							break;
						}
				}
			}

			for (auto i = row.rbegin(); i != row.rend(); ++i)
			{
				if (map_impress(*i).Color == basic_color)
				{
					for (auto & j : get_exist_neighbours(*i))
						if (map_impress(j).Color == extra)
						{
							map_impress(*i).Color = extra;
							result.push_back(*i);
							incomplete = true;
							break;
						}
				}
			}
			
			for (auto & i : row)
			{
				if (map_impress(i).Color == extra)
				{
					find = true;
					break;
				}
			}

			if (incomplete)
				--radius;
			else if (find) {
				++radius;
				incomplete = true;
			}
		

		}

		return result;
			
	}

	

	void Map::generate_map( // вспомогательный метод, генерирующий карту, вызывается в конструкторе
		size_type dimension, size_type player_number,
		const std::string & map_type
	)
	{

		size_t x_dimension = dimension; // > 2
		size_t y_dimension = dimension; // > 2 
		
		size_t x_max = x_dimension;
		size_t y_max = y_dimension;

		Hex *** root = new Hex **[y_max];

		for (size_t i = 0; i < y_max; ++i)
		{
			root[i] = new Hex *[x_max];
			for (size_t j = 0; j < x_max; ++j)
			{
				root[i][j] = new Hex(i, j);
				if (i * j == 0
					|| i >= y_max - 1
					|| j >= x_max - 1
					)
				{
					root[i][j]->set_index(black);
				}
			}
		}

		Root = root;

	}


	void print_map(const Map & map)
	{

		for (size_type i = 0; i < map.dimension(); ++i)
		{
			for (size_type j = 0; j < map.dimension(); ++j)
			{
				if (map(i, j).index() != black)
					std::cout << map(i, j).index();
				else
					std::cout << "*";
			}
			std::cout << std::endl;
		}

	}


	MapImpress::~MapImpress()
	{
		for (size_type i = 0; i < Dimension; ++i)
		{
			for (size_type j = 0; j < Dimension; ++j)
				delete Root[i][j];
			delete[] Root[i];
		}
		delete[] Root;
	}

	MapImpress::MapImpress(const Map & map)
		: Dimension(map.dimension())
	{
		HexImpress *** root = new HexImpress **[Dimension];

		for (size_t i = 0; i < Dimension; ++i)
		{
			root[i] = new HexImpress *[Dimension];
			for (size_t j = 0; j < Dimension; ++j)
				root[i][j] = new HexImpress(map(Pair(i, j)));
		}

		Root = root;
	}

	HexImpress & MapImpress::operator () (const Pair & pair)
	{
		return *Root[pair.First][pair.Second];
	}

	HexImpress::HexImpress(const Hex & hex)
		: Coordinates(hex.coordinates())
		, Color(hex.index())
	{
		if (hex.occupied())
			UnitType = hex.get_hex_unit_type();
		else
			UnitType = none;
	}


}
