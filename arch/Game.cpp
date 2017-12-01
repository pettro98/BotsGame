#include "Game.h"
#include "Bot.h"
#include <algorithm> 
#include <string>

namespace game_module
{

	Game::~Game()
	{
		for (auto & i : Players)
		{
			delete i;
		}
		delete GameMap;
	}

	Game::Game(size_type max_turns, size_type dimension_x, size_type dimension_y
		, std::string map_type, size_type players_number)
		: CurrentPlayer(game_module::hex_color::blank)
		, CurrentTurn(0)
		, MaxTurns(max_turns)
	{
		Players = get_bots();
		if (map_type == "duel")
		{
			while (Players.size() > 2)
			{
				Players.pop_back();
			}
			while (Players.size() < 2)
			{
				add_bot();
			}
			GameMap = new Map(dimension_x, dimension_y, 2, map_type);
		}
		else if (map_type == "classic")
		{
			while (Players.size() > 4)
			{
				Players.pop_back();
			}
			while (Players.size() < 4)
			{
				add_bot();
			}
			GameMap = new Map(dimension_x, dimension_y, 4, map_type);
		}
		else if (map_type == "random")
		{
			if (players_number < 2 || players_number > 6)
			{
				players_number = 6;
			}
			while (Players.size() > players_number)
			{
				Players.pop_back();
			}
			while (Players.size() < players_number)
			{
				add_bot();
			}
			GameMap = new Map(dimension_x, dimension_y, players_number, map_type);
		}
		else
		{
			throw("incorect_map_type");
		}
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
		if ((*GameMap)(hex) != nullptr)
		{
			return (*GameMap)(hex)->get_hex_unit();
		}
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
		if ((*GameMap)(hex) != nullptr)
		{
			(*GameMap)(hex)->set_hex_unit(unit);
		}
	}

	void Game::set_color(const Pair & hex, hex_color new_color)
	{
		if ((*GameMap)(hex) != nullptr)
		{
			(*GameMap)(hex)->set_color(new_color);
		}
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

	bool Game::place_players(Controller * controller)
	{
		std::vector<Pair> capitals;
		for (size_type i = 1; i < GameMap->dimension_x() - 1; ++i)
		{
			for (size_type j = 1; j < GameMap->dimension_y() - 1; ++j)
			{
				if (get_unit(Pair(i, j)) != nullptr
					&& is_capital(get_unit(Pair(i, j))->type()))
				{
					capitals.push_back(Pair(i, j));
				}
			}
		}
		for (size_type i = 0; i < capitals.size(); ++i)
		{
			Players[i]->add_capital(capitals[i]);
			(*this)(capitals[i])->set_color(Players[i]->color());
			(*this)(capitals[i])->set_hex_capital((*this)(capitals[i]));
			static_cast<Capital *>(get_unit(capitals[i]))->change_district_income(1);
			for (auto & j : GameMap->get_neighbours(capitals[i]))
			{
				(*this)(j)->set_color(Players[i]->color());
				(*this)(j)->set_hex_capital((*this)(capitals[i]));
				static_cast<Capital *>(get_unit(capitals[i]))->change_district_income(1);
			}
			Players[i]->set_controller(controller);
		}
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
		for (size_type i = 0; i < get_game_map().dimension_x(); ++i)
		{
			for (size_type j = 0; j < get_game_map().dimension_y(); ++j)
			{
				if (is_tree((*this)(Pair(i, j))->get_hex_unit_type()))
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
									(*this)(k)->get_hex_capital()->change_district_income(Tree::income());
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

	hex_color Game::get_winner(Controller * controller)
	{
		if (!check_end_game())
		{
			return game_module::hex_color::blank;
		}
		std::vector<hex_color> players(controller->get_players_colors());
		if (players.size() == 1)
		{
			return players[0];
		}
		std::vector<size_type> players_scores;
		players_scores.resize(players.size());
		std::multimap<size_type, hex_color> result;
		for (size_t i = 0; i < players.size(); ++i)
		{
			for (auto & j : controller->get_player_capitals(players[i]))
			{
				players_scores[i] += 10 * controller->easy_solve_maze_count(j);
				players_scores[i] += 40 * controller->get_farms_number(j);
				players_scores[i] += 20 * controller->easy_solve_maze_count(j, is_army);
			}
			result.insert(std::make_pair(players_scores[i], players[i]));
		}
		return result.rbegin()->second;
	}

	std::vector<Player *> Game::get_bots()
	{
		std::vector<Player *> result;
		return result;
	}

	void Game::add_bot()
	{
		std::vector<hex_color> players_colors = {
			red,
			green,
			cyan,
			purple,
			blue,
			yellow,
		};
		hex_color bot_color(game_module::hex_color::extra);
		for (auto & i : players_colors)
		{
			bool correct_color = true;
			for (auto & j : Players)
			{
				if (j->color() == i)
				{
					correct_color = false;
					break;
				}
			}
			if (correct_color)
			{
				bot_color = i;
				break;
			}
		}
		Bot * new_bot = new Bot(bot_color, get_color_string(bot_color));
		Players.push_back(new_bot);
	}
}
