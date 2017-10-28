#pragma once

#include "General.h"
#include "Hex.h"


namespace game_module
{
	template<size_t N>
	class Map
	{
	private:
		using mas = std::array<std::array<Hex, N>, N>;

		mas Root;		     // двумерный массив гексов
		std::string MapType; // тип карты

	public:
		~Map() // вызывает метод clear
		{
			clear();
		}

		// базовые конструкторы, операторы
		Map(size_type player_number = 6, const std::string & map_type = "classic")
		{
			generate_map(player_number, map_type)
		}

		Map(const Map & map) = delete;
		Map & operator = (const Map & map) = delete;		//зачем?

		Map(Map && map) = delete;
		Map & operator = (Map && map) = delete;
		//

		size_t dimension() const
		{
			return N;
		}
		std::string map_type() const
		{
			return MapType;
		}

		Hex & operator () (const pair & coord)		   // выдаёт гекс
		{
			return Root[coord.first][coord.second];
		}
		Hex & operator () (size_type coord_1, size_type coord_2) // выдаёт гекс
		{
			return Root[coord_1][coord_2];
		}

		void clear() // очищает карту, в том числе удаляет всех юнитов на ней
		{
			for (int i = 0; i < N; ++i)
				for (int j = 0; j < N; ++j)
				{
					Root[i][j].delete_hex_unit();
				}
		}

	private:
		void generate_map( // вспомогательный метод, генерирующий карту, вызывается в конструкторе
						  size_type player_number = 6,
						  const std::string & map_type = "classic");

	};

}
