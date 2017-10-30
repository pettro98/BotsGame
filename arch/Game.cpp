#include "Game.h"



namespace game_module
{
	Game::~Game()
	{
		delete GameMap;
	}


	Game::Game(size_type max_trs, size_type map_dimension, std::string map_type,
		std::vector<Player> * players)
		: CurrentTurn(0)
		, MaxTurns(max_trs)
	{
		Map * GameMap = new Map(map_dimension, 6, map_type);
		Results.points.reserve(6);
		Results.last_turn.reserve(6);
		Results.built_armies.reserve(6);
		Results.built_farms.reserve(6);
		Results.built_towers.reserve(6);
		Results.moves.reserve(6);
	}




	bool Game::check_end_game() const
	{
		if (CurrentTurn > MaxTurns)
			return true;

		size_type players_in_game = 0;

		for (auto & i : Players)
			if (i.get_capitals_number())
				++players_in_game;

		if (players_in_game > 1)
			return false;
		return true;
				
	}


	Hex * Game::get_hex(Pair hex)
	{
		return &(*GameMap)(hex);
	}

	Hex * Game::get_hex(size_type coord1, size_type coord2)
	{
		return &(*GameMap)(Pair(coord1, coord2));
	}

	Hex * Game::operator () (Pair hex)
	{ 
		return &(*GameMap)(hex);
	}

	Hex * Game::operator () (size_type coord1, size_type coord2)
	{
		return &(*GameMap)(Pair(coord1, coord2));
	}

	Unit * Game::get_unit(Pair hex)
	{
		return (*GameMap)(hex).get_hex_unit();
	}
	/*
	std::vector<Player> & Game::get_players()
	{
		return Players;
	}*/

	Map & Game::get_game_map()
	{
		return *GameMap;
	}

	size_type Game::current_turn() const
	{
		return CurrentTurn;
	}

	size_type Game::max_turns() const
	{
		return MaxTurns;
	}

	bool Game::player_in_game(size_type player_index) const
	{
		for (auto & i : Players)
			if (i.index() == player_index)
				return i.get_capitals_number();
		return false;
	}
		
	void Game::set_unit(Pair hex, Unit * unit)
	{
		(*GameMap)(hex).set_hex_unit(unit);
		unit->set_hex(&(*GameMap)(hex));
	}

	void Game::set_color(Pair hex, hex_color new_color)
	{
		(*GameMap)(hex).set_index(new_color);
	}

	void Game::turn_passed()
	{
		++CurrentTurn;
	}

	void Game::add_points(size_type player_index, size_type points)
	{
		Results.points[player_index - 1] += points;
	}

	void Game::set_winner(size_type players_index)
	{
		Results.winner = players_index;
	}

	void Game::set_last_turn(size_type player_index, size_type turn)
	{
		Results.last_turn[player_index - 1] = turn;
	}

	void Game::add_to_built_armies(size_type player_index, size_type str)
	{
		Results.built_armies[player_index - 1] += str;
	}

	void Game::add_to_built_farms(size_type player_index)
	{
		++Results.built_farms[player_index - 1];
	}

	void Game::add_to_built_towers(size_type player_index, size_type str)
	{
		Results.built_towers[player_index - 1] += str;
	}

	void Game::add_to_moves(size_type player_index)
	{
		++Results.moves[player_index - 1];
	}


		//bool Game::make_players();

}
