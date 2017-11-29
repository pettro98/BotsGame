#pragma once
#include "Player.h"

namespace game_module
{

	class Bot
		: public Player
	{
	public:
		~Bot() = default;
		Bot(hex_color color, const std::string player_name);
		void turn();
		void custom_sort(std::vector<Pair> & row) const;
		void custom_farm_sort(std::vector<Pair> & row) const;
		std::vector<size_type> custom_tower_sort(std::vector<Pair> & row) const;
		size_type profit_to_move(const Pair & hex, hex_color basic_color) const;
		size_type profit_to_place_farm(const Pair & hex) const;
		size_type profit_to_place_tower(const Pair & hex, size_type strength) const;
		bool move_to_enemy(const Pair & hex);
	};
}