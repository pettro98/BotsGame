




namespace game_module
{

	class IGetData
	{

	public:

		virtual Hex * get_hex(pair hex) = 0;
		virtual Hex * get_hex(size_type coord1, size_type coord2) = 0;
		virtual Hex * operator () (pair hex) = 0;
		virtual Hex * operator () (size_type coord1, size_type coord2) = 0;
		virtual Unit * get_unit(pair hex) = 0;
		virtual Army * get_army(pair hex) = 0;
		virtual std::vector<Player> & get_players() = 0;
		virtual Map & get_game_map() = 0;
		virtual size_type current_turn() const = 0;
		virtual size_type max_turns() const = 0;
		virtual bool player_in_game(size_type player_index) const = 0;

	};

}
