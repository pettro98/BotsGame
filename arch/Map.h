#pragma once

#include "General.h"
#include "Hex.h"


namespace game_module
{

	class Map
	{
	private:
		Hex ** Root; // указатель на двумерный массив гексов
		size_type Dimension; // размерность карты
		std::string MapType; // тип карты

	public:
		~Map(); // вызывает метод clear
		Map() = delete;
		Map(size_type dimension = 10,
		    size_type player_number = 6,
		    const std::string & map_type = "classic");
		Map(const Map & map);
		Map & operator = (const Map & map);

		// базовые конструкторы, операторы
		Map(Map && map) = delete;
		Map & operator = (Map && map) = delete;
		//

		size_type dimension() const;
		size_type map_type() const;

		Hex & operator () (const pair & coord); // выдаёт гекс
		Hex & operator () (size_type coord_1, size_type coord_2); // выдаёт гекс
	
		void clear(); // очищает карту, в том числе удаляет всех юнитов на ней
	
	private:
		void generate_map( // вспомогательный метод, генерирующий карту, вызывается в конструкторе
			size_type dimension = 10, size_type player_number = 6,
			const std::string & map_type = "classic");

	};

}
