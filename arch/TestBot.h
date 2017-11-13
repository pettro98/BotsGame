#pragma once
#include "Player.h"

namespace game_module
{

	class TestBot
		: public Player
	{
	public:
		~TestBot() = default;
		TestBot(hex_color color, const std::string pl_name);
		void turn();
		void custom_sort(std::vector<Pair> & row);
		size_type profit_points(const Pair & hex, hex_color basic_color) const;
	};
}