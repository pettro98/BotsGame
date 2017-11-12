#pragma once

#include "General.h"


namespace game_module
{

	class ISetData
	{
	public:
		virtual void set_unit(pair hex, Unit * unit) = 0;
		virtual void set_color(pair hex, size_t  index) = 0;
		virtual void turn_passed() = 0;
		virtual void add_points(size_type player_index, size_type points) = 0;
		virtual void set_winner(size_type players_index) = 0;
		virtual void set_last_turn(size_type player_index, size_type turn) = 0;
		virtual void add_to_built_armies(size_type player_index, size_type str = 1) = 0;
		virtual void add_to_built_farms(size_type players_index) = 0;
		virtual void add_to_built_towers(size_type players_index, size_type str = 1) = 0;
		virtual void add_to_moves(size_type players_index) = 0;
	};

}
