#pragma once

#include "General.h"
#include "Hex.h"




namespace game_module
{


	class Map
	{

	private:

		Hex *** Root; // указатель на двумерный массив гексов
		size_type Dimension; // размерность карты
		std::string MapType; // тип карты

	public:

		~Map() = default; // вызывает метод clear

		Map() = default;

		Map(size_type dimension = 10,
			size_type player_number = 6,
			const std::string & map_type = "classic");

		//Map(const Map & map);

		//Map & operator = (const Map & map);


		// базовые конструкторы, операторы
		Map(Map && map) = delete;
		Map & operator = (Map && map) = delete;
		//

		size_type dimension() const;

		std::string map_type() const;

		Hex & operator () (const Pair & pair);

		Hex operator () (const Pair & pair) const;

		Hex & operator () (size_type coord1, size_type coord2);

		Hex operator () (size_type coord1, size_type coord2) const;


		//void clear(); // очищает карту, в том числе удаляет всех юнитов на ней

		
		std::vector<Pair> get_neighbours(size_type coord1, size_type coord2) const;

		std::vector<Pair> get_neighbours(Pair hex) const;

		std::vector<Pair> get_exist_neighbours(size_type coord1, size_type coord2) const;

		std::vector<Pair> get_exist_neighbours(Pair hex) const;
		

		std::pair<size_type, std::vector<std::pair<size_type, std::vector<Pair>>>>
			solve_maze(const Pair & hex,
				const hex_color & basic_color,
				const std::vector<std::pair<unit_type, bool>> * units_list = nullptr,
				const hex_color & new_color = extra
			);

	private:

		void generate_map( // вспомогательный метод, генерирующий карту, вызывается в конструкторе
			size_type dimension = 10, size_type player_number = 6,
			const std::string & map_type = "classic"
		);


	};


	void print_map(const Map & map);

}
