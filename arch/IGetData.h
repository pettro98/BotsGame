#pragma once

#include "General.h"
#include "Hex.h"
#include "Map.h"
#include "Unit.h"
#include "Player.h"


namespace game_module
{

	class Player;

	class IGetData
	{
	public:
		virtual Hex * operator () (const Pair & hex) = 0;
		virtual Hex * operator () (size_type coord1, size_type coord2) = 0;
		virtual Unit * get_unit(const Pair & hex) = 0;
		virtual std::vector<Player *> & get_players() = 0;
		virtual Player * get_player(hex_color seek_color) = 0;
		virtual hex_color get_current_player() const = 0;
		virtual Map & get_game_map() = 0;
		virtual size_type current_turn() const = 0;
		virtual size_type max_turns() const = 0;
		virtual Result & results() = 0;
		virtual bool player_in_game(hex_color color) const = 0;
	};
}
