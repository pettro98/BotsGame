#include "Game.h"
#include <algorithm> 


namespace game_module
{

	Game::~Game()
	{
		delete GameMap;
	}

	Game::Game(size_type max_turns, size_type map_dimension, std::string map_type)
		: CurrentPlayer(blank)
		, CurrentTurn(0)
		, MaxTurns(max_turns)
	{
		GameMap = new Map(map_dimension, 4, map_type);
	}

	bool Game::check_end_game() const
	{
		if (CurrentTurn > MaxTurns)
		{
			return true;
		}
		size_type players_in_game = 0;
		for (auto & i : Players)
		{
			if (i->get_capitals_number())
			{
				++players_in_game;
			}
		}
		if (players_in_game > 1)
		{
			return false;
		}
		return true;			
	}

	Hex * Game::operator () (const Pair & hex)
	{ 
		return (*GameMap)(hex);
	}

	Hex * Game::operator () (size_type coord1, size_type coord2)
	{
		return (*GameMap)(Pair(coord1, coord2));
	}

	Unit * Game::get_unit(const Pair & hex)
	{
		return (*GameMap)(hex)->get_hex_unit();
	}
	
	std::vector<Player *> & Game::get_players()
	{
		return Players;
	}

	Player * Game::get_player(hex_color seek_color)
	{
		for (auto & i : get_players())
		{
			if (i->color() == seek_color)
			{
				return i;
			}
		}
		return nullptr;
	}

	hex_color Game::get_current_player() const
	{
		return CurrentPlayer;
	}

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

	Result & Game::results()
	{
		return Results;
	}

	bool Game::player_in_game(hex_color color) const
	{
		for (auto & i : Players)
		{
			if (i->color() == color)
			{
				return (i->get_capitals_number() != 0);
			}
		}
		return false;
	}
		
	void Game::set_unit(const Pair & hex, Unit * unit)
	{
		(*GameMap)(hex)->set_hex_unit(unit);
	}

	void Game::set_color(const Pair & hex, hex_color new_color)
	{
		(*GameMap)(hex)->set_color(new_color);
	}

	void Game::turn_passed()
	{
		++CurrentTurn;
	}

	void Game::set_current_player(hex_color new_index)
	{
		CurrentPlayer = new_index;
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

	bool Game::make_players(Controller * controller)
	{
		Players[0]->add_capital(Pair(3, 3));
		static_cast<Capital *>(get_unit(Pair(3, 3)))->change_district_income(7);
		Players[0]->set_controller(controller);
		Players[1]->add_capital(Pair(3, 15));
		static_cast<Capital *>(get_unit(Pair(3, 15)))->change_district_income(7);
		Players[1]->set_controller(controller);
		Players[2]->add_capital(Pair(15, 3));
		static_cast<Capital *>(get_unit(Pair(15, 3)))->change_district_income(7);
		Players[2]->set_controller(controller);
		Players[3]->add_capital(Pair(15, 15));
		static_cast<Capital *>(get_unit(Pair(15, 15)))->change_district_income(7);
		Players[3]->set_controller(controller);
		return true;
	}

	void Game::prepare_player(Controller * controller)
	{
		for (auto & i : get_player(get_current_player())->get_capitals())
		{
			for (auto & j : controller->get_district_units(i, game_module::unit_type::grave))
			{
				(*this)(j)->set_hex_unit(unit_factory(game_module::unit_type::palm));
			}
			if (!static_cast<Capital *>((*this)(i)->get_hex_capital())
				->change_district_money(controller->get_district_income(i)))
			{
				for (auto & j : controller->get_district_units(i, game_module::unit_type::army))
				{
					static_cast<Army *>((*this)(j)->get_hex_unit())->die();
				}
				controller->calculate_income(this->get_game_map().easy_solve_maze(i));
			}
			else
			{
				for (auto & j : controller->get_district_units(i, game_module::unit_type::army))
				{
					static_cast<Army *>((*this)(j)->get_hex_unit())->set_moved(false);
				}
			}
		}
	}

	void Game::double_trees(Controller * controller)
	{
		for (size_type i = 0; i < get_game_map().dimension(); ++i)
		{
			for (size_type j = 0; j < get_game_map().dimension(); ++j)
			{
				if ((*this)(Pair(i, j))->occupied()
					&& is_tree((*this)(Pair(i, j))->get_hex_unit_type()))
				{
					if (static_cast<Tree *>((*this)(Pair(i, j))->get_hex_unit())->ready_to_double())
					{
						auto neighbours = get_game_map().get_neighbours(Pair(i, j));
						std::random_shuffle(neighbours.begin(), neighbours.end());
						for (auto & k : neighbours)
						{
							if (!(*this)(k)->occupied())
							{
								(*this)(k)->set_hex_unit(unit_factory((*this)(Pair(i, j))
									->get_hex_unit()->type()));
								static_cast<Tree *>((*this)(Pair(i, j))->get_hex_unit())->has_doubled();
								if ((*this)(k)->get_hex_capital())
								{
									(*this)(k)->get_hex_capital()->change_district_income(-2);
								}
								break;
							}
						}						
					}
					else
					{
						++(*static_cast<Tree *>((*this)(Pair(i, j))->get_hex_unit()));
					}
				}
			}
		}
	}
}
