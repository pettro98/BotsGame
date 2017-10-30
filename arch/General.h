#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <map>


namespace game_module
{

	using size_type = short;

	struct Pair
	{

		size_type First;
		size_type Second;

		~Pair() = default;
		Pair() = default;

		Pair(const Pair & pair);

		Pair(size_type first, size_type second);

		bool operator == (const Pair & pair) const;

	};

	bool operator != (const Pair & pair1, const Pair & pair2);

	std::ostream & operator << (std::ostream & out, const Pair & pair);


	enum unit_type
	{
		none = 0,
		army,
		tower,
		capital,
		farm,
		palm,
		pine,
		grave
	};

	enum hex_color {
		black = -1,
		blank,
		green,
		red,
		blue,
		orange,
		purple,
		yellow,
		extra
	};


	struct Result // структура результатов игры
	{
	public:
		size_type winner = 0; // победитель игры, по умолчанию 0
		std::vector<size_type> points; // очки каждого игрока
		std::vector<size_type> last_turn; // ход выхода из игры каждого игрока
		std::vector<size_type> built_armies; // суммарная сила армий, 
											 // построенных каждым игроком за время партии
		std::vector<size_type> built_farms; // кол - во всех ферм,
											// построенных каждым игроком за время партии
		std::vector<size_type> built_towers; // суммарная сила башен, 
											 // построенных каждым игроком за время партии
		std::vector<size_type> moves; // кол - во перемещений 
									  // юнитов каждого игрока за время партии
	};

}
