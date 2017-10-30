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
			if ((*this)(i).index() != -1)
				result.push_back(i);
		}

		return result;
	}

	std::vector<Pair> Map::get_exist_neighbours(Pair hex) const
	{
		return get_exist_neighbours(hex.First, hex.Second);
	}


	std::pair<size_type, std::vector<std::pair<size_type, std::vector<Pair>>>>
		Map::solve_maze(const Pair & hex,
			const hex_color & basic_color,
			const std::vector<std::pair<unit_type, bool>> * units_list,
			const hex_color & new_color
		)
	{
		
		std::pair<size_type, std::vector<std::pair<size_type, std::vector<Pair>>>> result;
		if (units_list)
			result.second.reserve(units_list->size());

		(*this)(hex).set_index(new_color);

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
				if ((*this)(i).index() == basic_color)
				{
					if (units_list)
					{
						++result.first;
						for (size_type j = 0; j < units_list->size(); ++j)
							if ((*units_list)[j].first == (*this)(i).get_hex_unit_type())
							{
								++result.second[j].first;
								if ((*units_list)[j].second)
									result.second[j].second.push_back(i);
							}
					}
					(*this)(i).set_index(new_color);
					incomplete = true;
				}
			}

			for (auto i = row.rbegin(); i != row.rend(); ++i)
			{
				if ((*this)(*i).index() == basic_color)
				{
					if (units_list)
					{
						++result.first;
						for (size_type j = 0; j < units_list->size(); ++j)
							if ((*units_list)[j].first == (*this)(*i).get_hex_unit_type())
							{
								++result.second[j].first;
								if ((*units_list)[j].second)
									result.second[j].second.push_back(*i);
							}
					}
					(*this)(*i).set_index(new_color);
					incomplete = true;
				}
			}
			
			for (auto & i : row)
			{
				if ((*this)(i).index() == new_color)
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
		size_t useless_hexs = 0;

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
					++useless_hexs;
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


}
